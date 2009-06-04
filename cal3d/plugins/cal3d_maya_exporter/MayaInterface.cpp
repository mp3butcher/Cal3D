//----------------------------------------------------------------------------//
// MayaInterface.cpp                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "StdAfx.h"
#include "MayaInterface.h"
#include "Exporter.h"
#include "MayaNode.h"
#include "MayaMesh.h"
#include "MayaMaterial.h"

#include <windows.h>

#include <maya/MItSelectionList.h>
#include <maya/MItDag.h>
#include <maya/MFnLight.h>
#include <maya/MFnMesh.h>
#include <maya/MFnIkJoint.h>
#include <maya/MEulerRotation.h>

#define PI 3.1415926535897f

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//
CMayaInterface::CMayaInterface()
{
} 

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//
CMayaInterface::~CMayaInterface()
{
  
}

//----------------------------------------------------------------------------//
// Helper functions                                                           //
//----------------------------------------------------------------------------//
BOOL CALLBACK WndEnumProc (HWND hwnd, LPARAM lParam)
{
  char buffer[2048];
  GetWindowText (hwnd, buffer, 2048);

  if (strstr (buffer, "Maya") != NULL)
  {
    *((HWND*)lParam) = hwnd;
    return FALSE;
  }

  return TRUE;
}

float MayaUnitToFPS (MTime::Unit unit)
{
  switch (unit)
  {
  case MTime::kHours:         return 1.0f/3600.0f;  
  case MTime::kMinutes:       return 1.0f/60.0f;
  case MTime::kSeconds:       return 1.0f;
  case MTime::kMilliseconds:  return 1000.0f;
  case MTime::kGames:         return 15;
  case MTime::kFilm:          return 24;
  case MTime::kPALFrame:      return 25;
  case MTime::kNTSCFrame:     return 30;
  case MTime::kShowScan:      return 48;
  case MTime::kPALField:      return 50;
  case MTime::kNTSCField:     return 60;
  case MTime::k2FPS:          return 2;
  case MTime::k3FPS:          return 3;
  case MTime::k4FPS:          return 4;
  case MTime::k5FPS:          return 5;
  case MTime::k6FPS:          return 6;
  case MTime::k8FPS:          return 8;
  case MTime::k10FPS:         return 10;
  case MTime::k12FPS:         return 12;
  case MTime::k16FPS:         return 16;
  case MTime::k20FPS:         return 20;
  case MTime::k40FPS:         return 40;
  case MTime::k75FPS:         return 75;
  case MTime::k80FPS:         return 80;
  case MTime::k100FPS:        return 100;
  case MTime::k120FPS:        return 120;
  case MTime::k125FPS:        return 125;
  case MTime::k150FPS:        return 150;
  case MTime::k200FPS:        return 200;
  case MTime::k240FPS:        return 240;
  case MTime::k250FPS:        return 250;
  case MTime::k300FPS:        return 300;
  case MTime::k375FPS:        return 375;
  case MTime::k400FPS:        return 400;
  case MTime::k500FPS:        return 500;
  case MTime::k600FPS:        return 600;
  case MTime::k750FPS:        return 750;
  case MTime::k1200FPS:       return 1200;
  case MTime::k1500FPS:       return 1500;
  case MTime::k2000FPS:       return 2000;
  case MTime::k3000FPS:       return 3000;
  case MTime::k6000FPS:       return 6000;
  case MTime::kInvalid:
  default: return 25;
  }
}

int MayaTimeToFrame (MTime &time)
{
  double val = time.value ();

  /*switch (MTime::uiUnit())
  {
  case MTime::kHours: return 1.0f/3600.0f;  
  case MTime::kMinutes: return 1.0f/60.0f;
  case MTime::kSeconds: return 1.0f;
  case MTime::kMilliseconds : return 1000.0f;
  case MTime::kGames: return (int)(val * 15.0f);
  case MTime::kPALFrame: return 25;
  case MTime::kNTSCFrame: return 30;
  case MTime::kShowScan: return 48;
  case MTime::kPALField: return 50;
  case MTime::kNTSCField: return 60;
  case MTime::kInvalid:
  default: return 25;
  }*/

  return 0;
}

////////////////////////////////////////////////////////////////////
// Check if an object is selected
////////////////////////////////////////////////////////////////////
bool isObjectSelected (const MDagPath& path)
{
   MDagPath sDagPath;
   
   MSelectionList activeList;
   MGlobal::getActiveSelectionList (activeList);
   
   MItSelectionList iter (activeList, MFn::kDagNode);
   
   while (!iter.isDone())
   {
      if (iter.getDagPath (sDagPath))
      {
         if (sDagPath == path)
            return true;
      }
      iter.next();
   }
   return false;
} // isObjectSelected

////////////////////////////////////////////////////////////////////
// Check if this object and all of its parents are selected.
////////////////////////////////////////////////////////////////////
bool isObjectOrParentSelected (const MDagPath & path)
{   
   bool result = false;
   MDagPath searchPath (path);
   while (true) 
   {
      if (isObjectSelected (searchPath))
      {
         result = true;
         break;
      }    
      if (searchPath.length () <= 1)
         break;
      searchPath.pop ();
   }   
   return result;
} //isObjectOrParentSelected

//----------------------------------------------------------------------------//
// Implementation                                                             //
//----------------------------------------------------------------------------//

/*

  Setup the interface

  */
bool CMayaInterface::Create (bool bSelectedOnly)
{
  //
  // fill in the selection list
  //

  // Create the DAG iterator.
  MStatus stat = MS::kSuccess;
  MItDag dagIterator (MItDag::kDepthFirst, MFn::kInvalid, &stat);
  if (MS::kSuccess != stat)
  {
    return false;
  }
  
  // Walk the DAG
  for (; !dagIterator.isDone (); dagIterator.next ())
  {
    // Retrieve the the current item pointed to by the iterator.
          MDagPath currentPath;
                stat = dagIterator.getPath (currentPath);
    if (!stat)
    {
      stat.perror ("MItDag::getPath");
      continue;
    }

                if (m_entireList.add (currentPath, MObject::kNullObj, true) != MS::kSuccess)
                        return false;

    // Only process objects we have selected if we did export selection
    if (bSelectedOnly && !isObjectSelected (currentPath))
      continue;
  
    // Create the function set interface to the current DAG node
    // to access DAG node methods.
    MFnDagNode fnDagNode (currentPath, &stat);
    if (!stat)
      continue;
    
    // We only want the final object.
    if (fnDagNode.isIntermediateObject ())
      continue;

    // Only add joints and meshes
    MObject theObj = currentPath.node ();
    const char *szType = currentPath.node ().apiTypeStr();
    
    if (currentPath.hasFn (MFn::kMesh)) 
    {
      if (m_selList.add (currentPath, MObject::kNullObj, true) != MS::kSuccess)
        return false;

      // Get the shaders connected to this mesh
      MObjectArray  shaderArray;
      MIntArray   faceToShaderArray;

      MFnMesh fnMesh (currentPath);

      if (fnMesh.getConnectedShaders (0, shaderArray, faceToShaderArray) == MS::kSuccess)
      {
        // Go through each one and create a material out of it
        for (unsigned int shi = 0; shi < shaderArray.length (); shi++)
        {
          // Make sure we don't already have this shader
          if (GetMaterialIDFromShader(shaderArray[shi]) != -1)
            continue;

          // Create material 
          CMayaMaterial *pMaterial = new CMayaMaterial; 
          if (!pMaterial->Create (shaderArray[shi]))
          {
            delete pMaterial;
            continue;
          } 

          // Add it to our material list
          m_materials.push_back(pMaterial);
        }
      }
    }

    if (currentPath.hasFn (MFn::kJoint))
    {
      if (m_selList.add (currentPath, MObject::kNullObj, true) != MS::kSuccess)
        return false;
    }
  }

  return true;
}

/*

  Clean up the interface

  */
bool CMayaInterface::Destroy ()
{
  for (unsigned int i = 0; i < m_materials.size(); i++)
    delete m_materials[i];

  m_materials.clear();

  return true;
}

int CMayaInterface::GetCurrentFrame()
{
  return (int)MAnimControl::currentTime ().value ();
}

float CMayaInterface::GetCurrentTime()
{
  return (float)MAnimControl::currentTime ().as (MTime::kSeconds);
}

int CMayaInterface::GetStartFrame()
{
  return (int)MAnimControl::minTime ().value ();
}

int CMayaInterface::GetEndFrame()
{
  return (int)MAnimControl::maxTime ().value ();
}

int CMayaInterface::GetFps()
{
  return (int)MayaUnitToFPS (MTime::uiUnit());
}

CWnd *CMayaInterface::GetMainWnd()
{
  HWND    hwnd = 0;

  if (!EnumWindows (WndEnumProc, (LPARAM)&hwnd))
    return 0;

  return CWnd::FromHandle(hwnd);
}

CBaseMaterial *CMayaInterface::GetMaterial(int materialId)
{
  CMayaMaterial *pMat = new CMayaMaterial;
  *pMat = *m_materials[materialId];

  return pMat;
}

int CMayaInterface::GetMaterialCount()
{
  return m_materials.size();
}

int CMayaInterface::GetMaterialIDFromShader (const MObject &shaderObj)
{
  for (unsigned int i = 0; i < m_materials.size(); i++)
  {
    CMayaMaterial *pMaterial = m_materials[i];
    if (pMaterial->GetMayaObject() == shaderObj)
      return i;
  }

  return -1;
}

CBaseMesh *CMayaInterface::GetMesh(CBaseNode *pNode, float)
{
  CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
  if (!pMayaNode) return 0;

  CMayaMesh *pMesh = new CMayaMesh;
  
  if (!pMesh->Create (pMayaNode->GetMayaDagPath (), this))
  {
    delete pMesh;
    return 0;
  }

  return pMesh;
}

bool CheckObjectNameAndChildren (const std::string& strName, MDagPath &dagPath, MDagPath &foundDagPath, bool bJointsOnly)
{
  MStatus       status;
  MFnDagNode      dagNode (dagPath, &status);
  if (status != MS::kSuccess)
    return false;

  if (bJointsOnly)
  {
    if (!dagNode.hasObj (MFn::kJoint))
      return false;
  }

  // is it this one?
  MString strThisName = dagNode.name();
  const char *szThisName = strThisName.asChar();
  if (stricmp (szThisName, strName.c_str()) == 0)
  {
    foundDagPath = dagPath;
    return true;
  }

  // check the children
  for (unsigned int i = 0; i < dagNode.childCount (); i++)
  {
    MObject objChild = dagNode.child (i, &status);
    if (status != MS::kSuccess)
      continue;

    MFnDagNode  childDagNode (objChild, &status); 
    if (status != MS::kSuccess)
      continue;
    
    MDagPath childPath; 
    if (childDagNode.getPath (childPath) != MS::kSuccess)
      continue;   

    if (CheckObjectNameAndChildren(strName, childPath, foundDagPath, bJointsOnly))
      return true;
  }

  return false;
}

CBaseNode *CMayaInterface::GetNode(const std::string& strName)
{
  MStatus       status;
  
  for (unsigned int i = 0; i < m_entireList.length(); i++)
  {
    bool     bFoundObj = false;
    MDagPath  dagPath, foundDagPath;
    if (m_entireList.getDagPath (i, dagPath) != MS::kSuccess)
      continue;;

    if (dagPath.hasFn (MFn::kJoint))
    {
      bFoundObj = CheckObjectNameAndChildren (strName, dagPath, foundDagPath, true);
    }
    else
    {
      MFnDagNode      dagNode (dagPath, &status);
      if (status != MS::kSuccess)
        continue;

      MString strThisName = dagNode.name();
      if (stricmp (strThisName.asChar(), strName.c_str()) == 0)
      {
        foundDagPath = dagPath;
        bFoundObj = true;       
      }
    }


    // did we find one?
    if (bFoundObj)
    {
      CMayaNode *pNode = new CMayaNode;
      if(pNode == 0)
      {
        theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
        return 0;
      }

      if (!pNode->Create (foundDagPath))
      {
        delete pNode;
        return 0;
      }

      return pNode;
    }
  }

  return 0;
}

int CMayaInterface::GetSelectedNodeCount()
{
  return m_selList.length();
}

CBaseNode *CMayaInterface::GetSelectedNode(int nodeId)
{
  if (nodeId >= (int)m_selList.length())
    return 0;

  CMayaNode *pNode = new CMayaNode;
  if(pNode == 0)
  {
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }
  
  MDagPath  path;
  m_selList.getDagPath (nodeId, path);
  
  if (!pNode->Create (path))
  {
    delete pNode;
    return 0;
  }

  return pNode;
}

//----------------------------------------------------------------------------------------------
// Gets the relative transformation of the given node
//----------------------------------------------------------------------------------------------
bool BoneGotParent (const MDagPath &bone)
{
  MDagPath parent = bone;
  MStatus status = parent.pop ();        
  return (status == MS::kSuccess && parent.hasFn (MFn::kTransform));
}

void CMayaInterface::GetTranslationAndRotationBoneSpace(CBaseNode *pNode, float time, CalVector& translation, CalQuaternion& rotation)
{
  // downcast the node to a Maya node
  CMayaNode *pMayaNode = dynamic_cast<CMayaNode *>(pNode);
  assert(pMayaNode);

  // Initial pose is at time 0.
  if (time < 0) time = 0;

  CPushCurrentTime _;

  MTime here(time, MTime::kSeconds);
  MAnimControl::setCurrentTime(here);

  MStatus status;
  MFnIkJoint joint(pMayaNode->GetDagPath(), &status);
  if (status != MS::kSuccess) {
    translation.clear();
    rotation.clear();
    return;
  }

  MQuaternion rotation_axis;
  status = joint.getScaleOrientation(rotation_axis);
  assert(status == MS::kSuccess);
  MVector translation_absolute = joint.translation(MSpace::kWorld, &status);
  assert(status == MS::kSuccess);
  MQuaternion rotation_absolute;
  status = joint.getRotation(rotation_absolute, MSpace::kWorld);
  assert(status == MS::kSuccess);
  MQuaternion rotation_bone_space = (rotation_axis * rotation_absolute).conjugate();
  MVector translation_bone_space = translation_absolute.rotateBy(rotation_bone_space);

  translation.x = float(-translation_bone_space.x);
  translation.y = float(-translation_bone_space.y);
  translation.z = float(-translation_bone_space.z);

  rotation.w = float(-rotation_bone_space.w);
  rotation.x = float( rotation_bone_space.x);
  rotation.y = float( rotation_bone_space.y);
  rotation.z = float( rotation_bone_space.z);
}


//----------------------------------------------------------------------------------------------
// Gets the absolute transformation of the given node
//----------------------------------------------------------------------------------------------
void CMayaInterface::GetTranslationAndRotation(CBaseNode *pNode, CBaseNode *pParentNode, float time, CalVector& translation, CalQuaternion& rotation)
{
  CMayaNode* pMayaNode = dynamic_cast<CMayaNode*>(pNode);
  assert(pMayaNode);
  // Ignore parent node.  We don't need it.

  // Initial pose is at time 0.
  if (time < 0) time = 0;

  CPushCurrentTime _;

  MTime here(time, MTime::kSeconds);
  MAnimControl::setCurrentTime(here);

  MStatus status;
  MFnIkJoint joint(pMayaNode->GetDagPath(), &status);
  if (status != MS::kSuccess) {
    translation.clear();
    rotation.clear();
    return;
  }


  // get translation
  MVector j_translation = joint.translation(MSpace::kTransform, &status);
  assert(status == MS::kSuccess);

  // get rotation
  MQuaternion j_rotation;
  status = joint.getRotation(j_rotation, MSpace::kTransform);
  assert(status == MS::kSuccess);

  // get orientation
  MQuaternion j_orientation;
  status = joint.getOrientation(j_orientation);
  assert(status == MS::kSuccess);

  // get rotate axis
  MQuaternion j_rotation_axis;
  status = joint.getScaleOrientation(j_rotation_axis);
  assert(status == MS::kSuccess);

  translation.x = float(j_translation.x);
  translation.y = float(j_translation.y);
  translation.z = float(j_translation.z);

  MQuaternion rot = j_rotation_axis * j_rotation * j_orientation;
  rotation.w = float(-rot.w);
  rotation.x = float( rot.x);
  rotation.y = float( rot.y);
  rotation.z = float( rot.z);


}


bool CMayaInterface::IsBone(CBaseNode *pNode)
{
  if (!pNode) return false;
  
  CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
  if (!pMayaNode) return false;

  return pMayaNode->GetMayaDagPath().hasFn (MFn::kJoint);
}

bool CMayaInterface::IsDummy(CBaseNode *pNode)
{

  return false;
}

bool CMayaInterface::IsMesh(CBaseNode *pNode)
{
  if (!pNode) return false;
  
  CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
  if (!pMayaNode) return false;

  MDagPath path = pMayaNode->GetMayaDagPath();
  
  // Is this a mesh node?
  if (path.hasFn (MFn::kMesh))
  {
    // See if we can make a MFnMesh out of this
    MStatus   status = MS::kSuccess;
    MFnMesh   fnMesh (path, &status);
    if (status != MS::kSuccess)
      return false;
    
    return true;
  }
  
  return false;
}

bool CMayaInterface::IsLight(CBaseNode *pNode)
{
  if (!pNode) return false;

  CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
  if (!pMayaNode) return false;

  MDagPath path = pMayaNode->GetMayaDagPath();

  // Is this a light node?
  if (path.hasFn (MFn::kLight))
  {
    // See if we can make a MFnLight out of this
    MStatus   status = MS::kSuccess;
    MFnLight   fnLight (path, &status);
    if (status != MS::kSuccess)
      return false;

    return true;
  }

  return false;
}

void CMayaInterface::SetProgressInfo(int percentage)
{

}

void CMayaInterface::StartProgressInfo(const std::string& strText)
{

}

void CMayaInterface::StopProgressInfo()
{

}

void CMayaInterface::GetAmbientLight( CalVector & )
{
  // TODO
}

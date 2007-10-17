//----------------------------------------------------------------------------//
// MaxInterface.cpp                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Exporter.h"
#include "MaxInterface.h"
#include "MaxNode.h"
#include "MaxMesh.h"
#include "MaxMaterial.h"
#include "MaxNullView.h"
#include "max2ogl.h"


CMaxInterface::CMaxInterface()
{
  m_pExpInterface = 0;
  m_pInterface  = 0;

  m_TabNodeFromMaxScript.ZeroCount();
  m_MatFromMaxScript = 0;
}


CMaxInterface::~CMaxInterface()
{
}

//----------------------------------------------------------------------------//
// Create a max interface instance                                            //
//----------------------------------------------------------------------------//
bool CMaxInterface::Create(ExpInterface *pExpInterface, Interface *pInterface)
{
  m_pExpInterface = pExpInterface;
  m_pInterface = pInterface;

  return true;
}

//----------------------------------------------------------------------------//
// Create a max interface instance and set the Tab node from Maxscript        //
//----------------------------------------------------------------------------//
bool CMaxInterface::Create(ExpInterface *pExpInterface, Interface *pInterface, INodeTab& _TabNodeFromMaxScript)
{
  int i = 0;
  m_pExpInterface = pExpInterface;
  m_pInterface  = pInterface;

  const int tabsize = _TabNodeFromMaxScript.Count();
  for (i=0;i<tabsize;i++)
  {
    INode* node = _TabNodeFromMaxScript[i];
    if (!node)return false;

    m_TabNodeFromMaxScript.Append(1,&node);
  }

  return true;
}

//----------------------------------------------------------------------------//
// Create a max interface instance and set the Tab materials from Maxscript   //
//----------------------------------------------------------------------------//
bool CMaxInterface::Create(ExpInterface *pExpInterface, Interface *pInterface, StdMat* _MatFromMaxScript)
{
  int i = 0;
  m_pExpInterface = pExpInterface;
  m_pInterface  = pInterface;
  
  if (! _MatFromMaxScript)return false;

  m_MatFromMaxScript = _MatFromMaxScript;

  return true;
}

//----------------------------------------------------------------------------//
// Get the current frame of the animation                                     //
//----------------------------------------------------------------------------//

int CMaxInterface::GetCurrentFrame()
{
  return m_pInterface->GetTime() / GetTicksPerFrame();
}

//----------------------------------------------------------------------------//
// Get the current time in seconds of the animation                           //
//----------------------------------------------------------------------------//

float CMaxInterface::GetCurrentTime()
{
  return TicksToSec(m_pInterface->GetTime());
}

//----------------------------------------------------------------------------//
// Get the end frame of the animation                                         //
//----------------------------------------------------------------------------//

int CMaxInterface::GetEndFrame()
{
  return m_pInterface->GetAnimRange().End() / GetTicksPerFrame();
}

//----------------------------------------------------------------------------//
// Get the frames per second (Fps) of the animation                           //
//----------------------------------------------------------------------------//

int CMaxInterface::GetFps()
{
  return GetFrameRate();
}

//----------------------------------------------------------------------------//
// Get the main window of the host application                                //
//----------------------------------------------------------------------------//

CWnd *CMaxInterface::GetMainWnd()
{
  return CWnd::FromHandle(m_pInterface->GetMAXHWnd());
}

//----------------------------------------------------------------------------//
// Get the material for a given id                                            //
//----------------------------------------------------------------------------//

CBaseMaterial *CMaxInterface::GetSubMaterial(int materialId, Mtl *pMtl,int & materialCount)
{
  
  // check if we have a standard material
  if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    // check if we reached the wanted material
    if(materialId == materialCount)
    {
      // allocate a new max material instance
      CMaxMaterial *pMaxMaterial;
      pMaxMaterial = new CMaxMaterial();
      if(pMaxMaterial == 0)
      {
        theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
        return 0;
      }
      
      // create the max material
      if(!pMaxMaterial->Create((StdMat *)pMtl))
      {
        delete pMaxMaterial;
        return 0;
      }
      
      return pMaxMaterial;
    }
    
    materialCount++;
  }

  if (pMtl->IsMultiMtl())
  {
    int subMatId;
    for(subMatId = 0; subMatId < pMtl->NumSubMtls();subMatId++)
    {
      CBaseMaterial *pMaxMaterial = GetSubMaterial(materialId, pMtl->GetSubMtl(subMatId),materialCount);
      if(pMaxMaterial != NULL)
        return pMaxMaterial;      
    }
  }
  return NULL;
  
}

CBaseMaterial *CMaxInterface::GetMaterial(int materialId)
{
  int materialCount;
  materialCount = 0;

  int materialBaseId;

/*//////////////////////////////////////////////////////////////////////////////
// The following code includes all materials found in the material library.   //                                                               //
////////////////////////////////////////////////////////////////////////////////

  // get the material library
  MtlBaseLib& mtlBaseLib = m_pInterface->GetMaterialLibrary();

  // loop through all material bases
  for(materialBaseId = 0; materialBaseId < mtlBaseLib.Count(); materialBaseId++)
  {
    // check if it is a material
    if(mtlBaseLib[materialBaseId]->SuperClassID()==MATERIAL_CLASS_ID)
    {
      // get the material
      Mtl *pMtl;
      pMtl = (Mtl *)mtlBaseLib[materialBaseId];

      // check if we have a standard material
      if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
      {
        // check if we reached the wanted material
        if(materialId == materialCount)
        {
          // allocate a new max material instance
          CMaxMaterial *pMaxMaterial;
          pMaxMaterial = new CMaxMaterial();
          if(pMaxMaterial == 0)
          {
            theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
            return 0;
          }

          // create the max material
          if(!pMaxMaterial->Create((StdMat *)pMtl))
          {
            delete pMaxMaterial;
            return 0;
          }

          return pMaxMaterial;
        }

        materialCount++;
      }
    }
  }

//////////////////////////////////////////////////////////////////////////////*/

  // get the material bases from the material editor slots
  for(materialBaseId = 0; materialBaseId < 24; materialBaseId++)
  {
    // get material base
    MtlBase *pMtlBase;
    pMtlBase = m_pInterface->GetMtlSlot(materialBaseId);

    // check if it is a material
    if(pMtlBase->SuperClassID()==MATERIAL_CLASS_ID)
    {
      // get the material
      Mtl *pMtl;
      pMtl = (Mtl *)pMtlBase;

      CBaseMaterial *pMaxMaterial = GetSubMaterial( materialId, pMtl, materialCount);
      if(pMaxMaterial != NULL)
        return pMaxMaterial;      
    }
  }

  theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
  return 0;
}

//----------------------------------------------------------------------------//
// Get the number of materials in the scene                                   //
//----------------------------------------------------------------------------//
int CMaxInterface::GetSubMaterialCount(Mtl *pMtl)
{
  int materialCount=0;

  if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) 
    return 1;

  if (pMtl->IsMultiMtl())
  {
    int subMatId;
    for(subMatId = 0; subMatId < pMtl->NumSubMtls();subMatId++)
    {
      materialCount+=GetSubMaterialCount(pMtl->GetSubMtl(subMatId));
    }
  }
  return materialCount;
}

int CMaxInterface::GetMaterialCount()
{
  int materialCount;
  materialCount = 0;

  int materialBaseId;

/*//////////////////////////////////////////////////////////////////////////////
// The following code includes all materials found in the material library.   //                                                               //
////////////////////////////////////////////////////////////////////////////////

  // get the material library
  MtlBaseLib& mtlBaseLib = m_pInterface->GetMaterialLibrary();

  // loop through all material bases in the material library
  for(materialBaseId = 0; materialBaseId < mtlBaseLib.Count(); materialBaseId++)
  {
    // check if it is a material
    if(mtlBaseLib[materialBaseId]->SuperClassID()==MATERIAL_CLASS_ID)
    {
      // get the material
      Mtl *pMtl;
      pMtl = (Mtl *)mtlBaseLib[materialBaseId];

      // check if we have a standard material
      if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) materialCount++;
    }
  }

//////////////////////////////////////////////////////////////////////////////*/

  // get the material bases from the material editor slots
  for(materialBaseId = 0; materialBaseId < 24; materialBaseId++)
  {
    // get material base
    MtlBase *pMtlBase;
    pMtlBase = m_pInterface->GetMtlSlot(materialBaseId);

    // check if it is a material
    if(pMtlBase->SuperClassID()==MATERIAL_CLASS_ID)
    {
      // get the material
      Mtl *pMtl;
      pMtl = (Mtl *)pMtlBase;

      materialCount+=GetSubMaterialCount(pMtl);

    }
  }

  return materialCount;
}

//----------------------------------------------------------------------------//
// Get the mesh of a given node                                               //
//----------------------------------------------------------------------------//

CBaseMesh *CMaxInterface::GetMesh(CBaseNode *pNode)
{
  // check for invalid nodes
  if(pNode == 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return 0;
  }
  
  // downcast the node to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // get the current time
  TimeValue time;
  time = m_pInterface->GetTime();

  // get max mesh instance
  ObjectState os;
  os = pMaxNode->GetINode()->EvalWorldState(time);

  TriObject *pTriObject = NULL;

  if(!os.obj->CanConvertToType(triObjectClassID))
  {
    theExporter.SetLastError("Invalid Mesh Object.", __FILE__, __LINE__);
    return 0;   
  }
  
  pTriObject=(TriObject*)os.obj->ConvertToType(time, triObjectClassID);
  if(pTriObject==NULL)
  {
    theExporter.SetLastError("Invalid Mesh Object.", __FILE__, __LINE__);
    return 0;
  }

  Mesh *pMesh = &pTriObject->mesh;
  
  bool bDelete= false;

  if(os.obj != pTriObject)
    bDelete = true;

  // allocate a new max mesh instance
  CMaxMesh *pMaxMesh;
  pMaxMesh = new CMaxMesh();
  if(pMaxMesh == 0)
  {
    if(bDelete) pTriObject->DeleteMe();
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }

  // create the max mesh
  if(!pMaxMesh->Create(pMaxNode->GetINode(), pMesh, pTriObject, (bDelete == true)))
  {
    if(bDelete) pTriObject->DeleteMe();
    delete pMaxMesh;
    return 0;
  }

  return pMaxMesh;
}

//----------------------------------------------------------------------------//
// Get the node with given name                                               //
//----------------------------------------------------------------------------//

CBaseNode *CMaxInterface::GetNode(const std::string& strName)
{
  // allocate a new max node instance
  CMaxNode *pNode;
  pNode = new CMaxNode();
  if(pNode == 0)
  {
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }

  // create the max node
  if(!pNode->Create(m_pInterface->GetINodeByName(strName.c_str())))
  {
    delete pNode;
    return 0;
  }

  return pNode;
}

//----------------------------------------------------------------------------//
// Get the transformation matrix of a given max node                          //
//----------------------------------------------------------------------------//

Matrix3 CMaxInterface::GetNodeTM(CMaxNode *pNode, float time)
{
  // initialize matrix with the identity
  Matrix3 tm, tmParent;
  tm.IdentityMatrix();

  // only do this for valid nodes
  if(pNode != 0)
  {
    bool isMirrored = false;
    // get the node transformation
    tm = pNode->GetINode()->GetNodeTM(SecToTicks(time));
    
	// This code seems to corrupt some models

    /*if(!pNode->GetINode()->IsRootNode())
    {
      tmParent = pNode->GetINode()->GetParentTM(SecToTicks(time));
      
      
      // check if the matrix is right handed
      
      if(DotProd( CrossProd( tmParent.GetRow(0).Normalize(), 
        tmParent.GetRow(1).Normalize() ).Normalize(), 
        tmParent.GetRow(2).Normalize() ) < -0.1)
      {
        isMirrored = true;
      }
    }*/

    // make the transformation uniform
    tm.NoScale();
    
    AffineParts parts;
    decomp_affine(tm, &parts);
    parts.q.MakeMatrix(tm);

    if(!isMirrored)
      tm.SetRow(3, parts.t);
    else
      tm.SetRow(3, -parts.t);
    
  }

  return tm;
}

//----------------------------------------------------------------------------//
// Get the number of selected nodes in the scene                              //
//----------------------------------------------------------------------------//

int CMaxInterface::GetSelectedNodeCount()
{
  // get the number of selected nodes
  int nodeCount;
  nodeCount = m_pInterface->GetSelNodeCount();

  // if nothing is selected, there is always the scene root node
  if(nodeCount == 0) return 1;

  return nodeCount;
}

//----------------------------------------------------------------------------//
// Get the node for a given node id                                           //
//----------------------------------------------------------------------------//

CBaseNode *CMaxInterface::GetSelectedNode(int nodeId)
{
  // get the number of selected nodes
  int nodeCount;
  nodeCount = m_pInterface->GetSelNodeCount();

  // if nothing is selected, we go with the scene root node
  if(nodeCount == 0)
  {
    // check if the given node id is valid
    if(nodeId == 0)
    {
      // allocate a new max node instance
      CMaxNode *pNode;
      pNode = new CMaxNode();
      if(pNode == 0)
      {
        theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
        return 0;
      }

      // create the max node
      if(!pNode->Create(m_pInterface->GetRootNode()))
      {
        delete pNode;
        return 0;
      }

      return pNode;
    }

    // invalid node id requested!
    return 0;
  }

  // check if the given node id is valid
  if((nodeId < 0) || (nodeId >= m_pInterface->GetSelNodeCount()))
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return 0;
  }

  // allocate a new max node instance
  CMaxNode *pNode;
  pNode = new CMaxNode();
  if(pNode == 0)
  {
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }

  // create the max node
  if(!pNode->Create(m_pInterface->GetSelNode(nodeId)))
  {
    delete pNode;
    return 0;
  }

  return pNode;
}

//----------------------------------------------------------------------------//
// Get the node for a given node id                                           //
//----------------------------------------------------------------------------//
CBaseNode *CMaxInterface::GetNodeFromMaxscriptArray(int nodeId)
{
  // Get the number of nodes in array
  int nodeCount;
  nodeCount = m_TabNodeFromMaxScript.Count();

  // if nothing is selected, we go with the scene root node
  if(nodeCount == 0)
  {
    // check if the given node id is valid
    if(nodeId == 0)
    {
      // allocate a new max node instance
      CMaxNode *pNode;
      pNode = new CMaxNode();
      if(pNode == 0)
      {
        theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
        return 0;
      }

      // create the max node
      if(!pNode->Create(m_pInterface->GetRootNode()))
      {
        delete pNode;
        return 0;
      }

      return pNode;
    }

    // invalid node id requested!
    return 0;
  }

  // check if the given node id is valid
  if((nodeId < 0) || (nodeId >= nodeCount))
  {
    theExporter.SetLastError("Invalid index.", __FILE__, __LINE__);
    return 0;
  }

  // allocate a new max node instance
  CMaxNode *pNode;
  pNode = new CMaxNode();
  if(pNode == 0)
  {
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }

  // create the max node from index in array
  if(!pNode->Create( m_TabNodeFromMaxScript[nodeId] ))
  {
    delete pNode;
    return 0;
  }

  return pNode;
}

CBaseMaterial* CMaxInterface::GetBaseMatFromMaxscript()
{
  // check if it is a material
  if(m_MatFromMaxScript->SuperClassID()==MATERIAL_CLASS_ID)
  {
    // get the material
    Mtl *pMtl;
    pMtl = (Mtl *)m_MatFromMaxScript;

    // check if we have a standard material
    if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
    {
      // allocate a new max material instance
      CMaxMaterial *pMaxMaterial;
      pMaxMaterial = new CMaxMaterial();
      if(pMaxMaterial == 0)
      {
        theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
        return 0;
      }

      // create the max material
      if(! pMaxMaterial->Create((StdMat *)pMtl))
      {
        delete pMaxMaterial;
        return 0;
      }

      return pMaxMaterial;
    }
  }

  theExporter.SetLastError("Invalid material index.", __FILE__, __LINE__);
  return 0;
}

//----------------------------------------------------------------------------//
// Get the start frame of the animation                                       //
//----------------------------------------------------------------------------//

int CMaxInterface::GetStartFrame()
{
  return m_pInterface->GetAnimRange().Start() / GetTicksPerFrame();
}

//----------------------------------------------------------------------------//
// Get the relative translation and rotation of a given node at a given time  //
//----------------------------------------------------------------------------//

void CMaxInterface::GetTranslationAndRotation(CBaseNode *pNode, CBaseNode *pParentNode, float time, CalVector& translation, CalQuaternion& rotation)
{
  // check for invalid nodes
  if(pNode == 0) return;

  // the initial node pose (time == -1.0) is at (time == 0.0) for 3ds max
  if(time < 0.0f) time = 0.0f;

  // downcast the node to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // downcast the parent node to a max node
  CMaxNode *pMaxParentNode;
  if(pParentNode == 0)
  {
    pMaxParentNode = 0;
  }
  else
  {
    pMaxParentNode = dynamic_cast<CMaxNode *>(pParentNode);
  }

  // calculate the relative transformation
  Matrix3 tm;
  tm = GetNodeTM(pMaxNode, time) * Inverse(GetNodeTM(pMaxParentNode, time));

  // cpinson here i should make the transform max2gl
  if (theExporter.GetAxisGL())
    tm=ConvertMax2Ogl(tm);

  // calculate the translation component
  Point3 p;
  p = tm.GetTrans();

  translation[0] = p[0];
  translation[1] = p[1];
  translation[2] = p[2];

  // calculate the rotation component
  Quat q(tm);

  rotation[0] = q[0];
  rotation[1] = q[1];
  rotation[2] = q[2];
  rotation[3] = q[3];
}

//----------------------------------------------------------------------------//
// Get the bone space transformation of a given node at a given time          //
//----------------------------------------------------------------------------//

void CMaxInterface::GetTranslationAndRotationBoneSpace(CBaseNode *pNode, float time, CalVector& translation, CalQuaternion& rotation)
{
  // check for invalid nodes
  if(pNode == 0) return;

  // the initial node pose (time == -1.0) is at (time == 0.0) for 3ds max
  if(time < 0.0f) time = 0.0f;

  // downcast the node to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // calculate the inverse transformation
  Matrix3 tm;
  tm = Inverse(GetNodeTM(pMaxNode, time));

  // cpinson here i should make the transform max2gl
  if (theExporter.GetAxisGL())
    tm=ConvertMax2Ogl(tm);


  // calculate the translation component
  Point3 p;
  p = tm.GetTrans();

  translation[0] = p[0];
  translation[1] = p[1];
  translation[2] = p[2];

  // calculate the rotation component
  Quat q(tm);

  rotation[0] = q[0];
  rotation[1] = q[1];
  rotation[2] = q[2];
  rotation[3] = q[3];
}

//----------------------------------------------------------------------------//
// Check if the given node is a biped bone                                    //
//----------------------------------------------------------------------------//

bool CMaxInterface::IsBipedBone(INode *pNode)
{
  // check for invalid nodes
  if(pNode == 0) return false;

  // check for root node
  if(pNode->IsRootNode()) return false;

  // check for bone node
  ObjectState os;
  os = pNode->EvalWorldState(0);
  if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return false;

  // check for biped node
  Control *pControl;
  pControl = pNode->GetTMController();
  if((pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Check if the given node is a bone                                          //
//----------------------------------------------------------------------------//

bool CMaxInterface::IsBone(CBaseNode *pNode)
{
  // check for invalid nodes
  if(pNode == 0) return false;

  // downcast to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // get internal max node
  INode *pINode;
  pINode = pMaxNode->GetINode();

  // check for root node
  if(pINode->IsRootNode()) return false;

  // check for bone node
  ObjectState os;
  os = pINode->EvalWorldState(0);
  if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0)) return true;

#if MAX_RELEASE >= 4000
  if(os.obj->ClassID() == BONE_OBJ_CLASSID) return true;
#endif

  if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return false;

  // check for biped node
  Control *pControl;
  pControl = pINode->GetTMController();
  if((pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Check if the given node is a dummy                                         //
//----------------------------------------------------------------------------//

bool CMaxInterface::IsDummy(CBaseNode *pNode)
{
  // check for invalid nodes
  if(pNode == 0) return false;

  // downcast to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // get internal max node
  INode *pINode;
  pINode = pMaxNode->GetINode();

  // check for root node
  if(pINode->IsRootNode()) return false;

  // check for dummy node
  ObjectState os;
  os = pINode->EvalWorldState(0);
  if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Check if the given node is a mesh                                          //
//----------------------------------------------------------------------------//

bool CMaxInterface::IsMesh(CBaseNode *pNode)
{
  // check for invalid nodes
  if(pNode == 0) return false;

  // downcast to a max node
  CMaxNode *pMaxNode;
  pMaxNode = dynamic_cast<CMaxNode *>(pNode);

  // get internal max node
  INode *pINode;
  pINode = pMaxNode->GetINode();

  // check for root node
  if(pINode->IsRootNode()) return false;

  // check for mesh node
  ObjectState os;
  os = pINode->EvalWorldState(0);
  if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Progress callback function                                                 //
//----------------------------------------------------------------------------//

DWORD WINAPI CMaxInterface::ProgressFunction(LPVOID arg)
{
  return 0;
}

//----------------------------------------------------------------------------//
// Set the progress information to a given text and percentage                //
//----------------------------------------------------------------------------//

void CMaxInterface::SetProgressInfo(int percentage)
{
  m_pInterface->ProgressUpdate(percentage);
}

//----------------------------------------------------------------------------//
// Start the progress information process                                     //
//----------------------------------------------------------------------------//

void CMaxInterface::StartProgressInfo(const std::string& strText)
{
  m_pInterface->ProgressStart(const_cast<char *>(strText.c_str()), true, ProgressFunction, 0);
}

//----------------------------------------------------------------------------//
// Stop the progress information process                                      //
//----------------------------------------------------------------------------//

void CMaxInterface::StopProgressInfo()
{
  m_pInterface->ProgressEnd();
}

//----------------------------------------------------------------------------//
// Get the number of nodes in the tab that has been filled by Maxscript       //
//----------------------------------------------------------------------------//
int  CMaxInterface::GetNumNodesInTabNodeFromMaxscript(void)
{
  return m_TabNodeFromMaxScript.Count();
}

//----------------------------------------------------------------------------//
// Get node in the tab that has been filled by Maxscript from its index  //
//----------------------------------------------------------------------------//
INode* CMaxInterface::GetNodeInTabNodeFromMaxscript(int idx)
{
  const int num = m_TabNodeFromMaxScript.Count();
  if (idx <0 || idx >= num) return NULL;
  
  return m_TabNodeFromMaxScript[idx];
}

//----------------------------------------------------------------------------//
// Get material in the tab that has been filled by Maxscript from its index  //
//----------------------------------------------------------------------------//
StdMat* CMaxInterface::GetStdMatFromMaxscript()
{
  return m_MatFromMaxScript;
}
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
	case MTime::kHours: return 1.0f/3600.0f;	
	case MTime::kMinutes: return 1.0f/60.0f;
	case MTime::kSeconds: return 1.0f;
	case MTime::kMilliseconds : return 1000.0f;
	case MTime::kGames: return 15;
	case MTime::kPALFrame: return 25;
	case MTime::kNTSCFrame: return 30;
	case MTime::kShowScan: return 48;
	case MTime::kPALField: return 50;
	case MTime::kNTSCField: return 60;
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
	MDagPath dagPath;
	MDagPath currentPath;
	for (; !dagIterator.isDone (); dagIterator.next ())
	{
		// Retrieve the the current item pointed to by the iterator.
		MObject currentNode = dagIterator.item (&stat);
		stat = dagIterator.getPath (currentPath);
		if (!stat)
		{
			stat.perror ("MItDag::getPath");
			continue;
		}

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
			MObjectArray	shaderArray;
			MIntArray		faceToShaderArray;

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
	HWND		hwnd = 0;

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

CBaseMesh *CMayaInterface::GetMesh(CBaseNode *pNode)
{
	CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
	if (!pMayaNode) return 0;

	CMayaMesh *pMesh = new CMayaMesh;
	
	if (!pMesh->Create (pMayaNode->GetMayaDagPath (), this))
	{
		delete pMesh;
		return 0;
	}

	return dynamic_cast<CBaseMesh*>(pMesh);
}

bool CheckObjectNameAndChildren (const std::string& strName, MDagPath &dagPath, MDagPath &foundDagPath, bool bJointsOnly)
{
	MStatus			  status;
	MFnDagNode		  dagNode (dagPath, &status);
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

		MFnDagNode	childDagNode (objChild, &status);	
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
	MStatus			  status;
	
	for (unsigned int i = 0; i < m_selList.length(); i++)
	{
		bool		 bFoundObj = false;
		MDagPath	dagPath, foundDagPath;
		if (m_selList.getDagPath (i, dagPath) != MS::kSuccess)
			continue;;

		if (dagPath.hasFn (MFn::kJoint))
		{
			bFoundObj = CheckObjectNameAndChildren (strName, dagPath, foundDagPath, true);
		}
		else
		{
			MFnDagNode		  dagNode (dagPath, &status);
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
	
	MDagPath	path;
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
	// check for invalid nodes
	if(pNode == 0) return;

    MTime here(time, MTime::kSeconds);
    MAnimControl::setCurrentTime(here);

	// downcast the node to a Maya node
	CMayaNode *pMayaNode;
//	pMayaNode = dynamic_cast<CMayaNode *>(pNode);
	pMayaNode = (CMayaNode *)(pNode);

	MFnTransform fnTransform(pMayaNode->GetDagPath());
	MStatus status;
	MVector trans=fnTransform.translation( MSpace::kWorld, &status);
	double qx,qy,qz,qw;
	status=fnTransform.getRotationQuaternion(qx,qy,qz,qw, MSpace::kWorld);

	char str[100];
	std::string name=pMayaNode->GetName();
	sprintf(str,"%s / OTrans: %f %f %f, Rot: %f %f %f %f",name.c_str(),trans.x,trans.y,trans.z,qx,qy,qz,qw);
	MGlobal::displayInfo(str);

	MTransformationMatrix TM1=fnTransform.transformation(&status);
	MMatrix M1=TM1.asMatrixInverse();
	MTransformationMatrix TM(M1);
	trans=TM.translation(MSpace::kWorld,&status);
	status=TM.getRotationQuaternion(qx,qy,qz,qw);

	translation[0] = float(trans.x);
	translation[1] = float(trans.y);
	translation[2] = float(trans.z);
	rotation[0] = float(qx);
	rotation[1] = float(qy);
	rotation[2] = float(qz);
	rotation[3] = float(qw);


	// calculate the relative transformation
/*ø	Matrix3 tm;
	tm = Inverse(GetNodeTM(pMayaNode, time));

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
	*/
}


void GetWorldTransform (const MDagPath &transformPath, MTransformationMatrix &outMatrix)
{
	MStatus status;

	MFnTransform topTransform (transformPath, &status);
	
	outMatrix = topTransform.transformation ();
	MDagPath currPath = transformPath;

	status = currPath.pop (); // Get the parent
	while (status == MS::kSuccess && currPath.hasFn (MFn::kTransform))
	{
		MFnTransform thisTransform (transformPath, &status);
		MTransformationMatrix thisMatrix = thisTransform.transformation ();

		MMatrix matParent = outMatrix.asMatrix ();
		MMatrix matChild = thisMatrix.asMatrix ();

		outMatrix = matParent * matChild;

		status = currPath.pop (); // Get the parent
	}
}
//----------------------------------------------------------------------------------------------
// Gets the absolute transformation of the given node
//----------------------------------------------------------------------------------------------
void CMayaInterface::GetTranslationAndRotation(CBaseNode *pNode, CBaseNode *pParentNode, float time, CalVector& translation, CalQuaternion& rotation)
{
	CMayaNode* pMayaNode = dynamic_cast<CMayaNode*>(pNode);
	if (!pMayaNode) return;

        CMayaNode* pMayaParentNode = dynamic_cast<CMayaNode*>(pParentNode);

    MTime here(time, MTime::kSeconds);
    MAnimControl::setCurrentTime(here);


	MFnTransform fnTransform(pMayaNode->GetDagPath());

	MFnIkJoint fnIkJoint(pMayaNode->GetDagPath());

	char str[200];
	std::string name=pMayaNode->GetName();

	MStatus status;
	MVector trans=fnTransform.translation( MSpace::kObject , &status);
	sprintf(str,"%s / Translation: kObject(%f %f %f)",name.c_str(),trans.x,trans.y,trans.z);
	MGlobal::displayInfo(str);

	trans=fnTransform.translation( MSpace::kWorld, &status);
	sprintf(str,"%s / Translation: kWorld(%f %f %f)",name.c_str(),trans.x,trans.y,trans.z);
	MGlobal::displayInfo(str);

	trans=fnTransform.translation( MSpace::kTransform , &status);
	sprintf(str,"%s / Translation: kTransform(%f %f %f)",name.c_str(),trans.x,trans.y,trans.z);
	MGlobal::displayInfo(str);

	trans=fnTransform.translation( MSpace::kTransform , &status);
	
	MQuaternion quat;

	status=fnIkJoint.getRotation(quat,MSpace::kObject);
	sprintf(str,"%s / getRotation(kObject): %f %f %f %f",name.c_str(),quat[0],quat[1],quat[2],quat[3]);
	MGlobal::displayInfo(str);

	status=fnIkJoint.getRotation(quat,MSpace::kWorld);
	sprintf(str,"%s / getRotation(kWorld): %f %f %f %f",name.c_str(),quat[0],quat[1],quat[2],quat[3]);
	MGlobal::displayInfo(str);

	status=fnIkJoint.getRotation(quat,MSpace::kTransform);
	sprintf(str,"%s / getRotation(kTransform): %f %f %f %f",name.c_str(),quat[0],quat[1],quat[2],quat[3]);
	MGlobal::displayInfo(str);

	status=fnIkJoint.getRotation(quat,MSpace::kWorld);

	double qx,qy,qz,qw;
//	status=fnTransform.getRotationQuaternion(qx,qy,qz,qw, MSpace::kObject );

//	status=fnIkJoint.getOrientation(quat);
	qx=quat[0]; qy=quat[1]; qz=quat[2]; qw=quat[3];

//	std::string str;

	sprintf(str,"%s / WTrans: %f %f %f, Rot: %f %f %f %f",name.c_str(),trans.x,trans.y,trans.z,qx,qy,qz,qw);
	MGlobal::displayInfo(str);

	// calculate the relative transformation
//	Matrix3 tm;
//	tm = GetNodeTM(pMayaNode, time) * Inverse(GetNodeTM(pMayaParentNode, time));

	// calculate the translation component
//	Point3 p;
//	p = tm.GetTrans();

	MTransformationMatrix TM1=fnTransform.transformation(&status);
	MMatrix M1=TM1.asMatrix();
	MMatrix M;
	if (pMayaParentNode) {
		MFnTransform fnTransformParent(pMayaParentNode->GetDagPath());
		MTransformationMatrix TMP=fnTransformParent.transformation(&status);
		MMatrix MP=TMP.asMatrixInverse();
		MGlobal::displayInfo("MAXhasparent\n");

		//M=M1*MP;
		M=M1;
	} else
		M=M1;

	MTransformationMatrix TM(M);
	trans=TM.translation(MSpace::kWorld,&status);
	status=TM.getRotationQuaternion(qx,qy,qz,qw);
	qx=-qx; qy=-qy; qz=-qz; // qw=qw;

	sprintf(str,"%s / MAXTranslation: kObject(%f %f %f)",name.c_str(),trans.x,trans.y,trans.z);
	MGlobal::displayInfo(str);
	sprintf(str,"%s / MAXRotation(kObject): %f %f %f %f",name.c_str(),quat[0],quat[1],quat[2],quat[3]);
	MGlobal::displayInfo(str);

	translation[0] = float(trans.x);
	translation[1] = float(trans.y);
	translation[2] = float(trans.z);

	// calculate the rotation component
//	Quat q(tm);

	rotation[0] = float(qx);
	rotation[1] = float(qy);
	rotation[2] = float(qz);
	rotation[3] = float(qw);
	
}

//void CMayaInterface::GetTranslationAndRotationBoneSpace(CBaseNode *pNode, float time, CalVector& translation, CalQuaternion& rotation)
//{
//	if (!pNode)
//		return;
//
//	CMayaNode *pMayaNode = dynamic_cast<CMayaNode*>(pNode);
//	if (!pMayaNode) return;
//
//	MDagPath path = pMayaNode->GetMayaDagPath ();
//	if (!path.hasFn (MFn::kTransform))
//		return;
//
//	MStatus			status;
//	/*MFnTransform	fnTransform (path, &status);
//	if (status != MS::kSuccess) return;
//
//	MFnDependencyNode depNode (path.node(), &status);
//	if (status != MS::kSuccess) return;
//
//	//MFnTransform	fnTransform (path, &status);
//	//if (status != MS::kSuccess) return;
//
//	MTransformationMatrix	matrix = fnTransform.transformation();
//
//	MVector MTrans = matrix.translation (MSpace::kWorld);
//	translation.set (MTrans.x, MTrans.y, MTrans.z);
//
//	matrix.getRotationQuaternion (rotation.x, rotation.y, rotation.z, rotation.w);*/
//
//	/*MString strCommand;
//
//	// Get relative translation
//	MDoubleArray	trans, angles;
//
//	strCommand = "joint -q -a -p " + depNode.name();
//	if (MGlobal::executeCommand (strCommand, trans) != MS::kSuccess) return;
//
//	translation.set (trans[0], trans[1], trans[2]);
//
//	// Get euler angles
//	double	angleX=0, angleY=0, angleZ=0;
//
//	strCommand = "joint -q -angleX " + depNode.name();
//	if (MGlobal::executeCommand (strCommand, angles) != MS::kSuccess) return;
//	angleX = angles[0] * (PI/180.0f);
//		
//	strCommand = "joint -q -angleY " + depNode.name();
//	if (MGlobal::executeCommand (strCommand, angles) != MS::kSuccess) return;
//	angleY = angles[0] * (PI/180.0f);
//
//	strCommand = "joint -q -angleZ " + depNode.name();
//	if (MGlobal::executeCommand (strCommand, angles) != MS::kSuccess) return;
//	angleZ = angles[0] * (PI/180.0f);
//
//	MString strRotOrder;
//	strCommand = "joint -q -roo " + depNode.name();
//	if (MGlobal::executeCommand (strCommand, strRotOrder) != MS::kSuccess) return;
//
//	MEulerRotation::RotationOrder	rotOrder;
//
//	if (strRotOrder == "xyz")
//		rotOrder = MEulerRotation::kXYZ;
//	else
//	if (strRotOrder == "yzx")
//		rotOrder = MEulerRotation::kYZX;
//	else
//	if (strRotOrder == "zxy")
//		rotOrder = MEulerRotation::kZXY;
//	else
//	if (strRotOrder == "xzy")
//		rotOrder = MEulerRotation::kXZY;
//	else
//	if (strRotOrder == "yxz")
//		rotOrder = MEulerRotation::kYXZ;
//	else
//	if (strRotOrder == "zyx")
//		rotOrder = MEulerRotation::kZYX;
//	
//	// Get quaternion
//	MEulerRotation	eulerRot (angleX, angleY, angleZ, rotOrder);
//	MQuaternion		quatRot = eulerRot.asQuaternion();
//	
//	rotation.x = quatRot.x; 
//	rotation.y = quatRot.y; 
//	rotation.z = quatRot.z; 
//	rotation.w = quatRot.w;*/
//
//	// TODO: TAKE INTO ACCOUNT TIME!~
//
//	// Get the transformation node
//	MFnTransform	fnTransform (path, &status);
//	if (status != MS::kSuccess) return;
//
//	// Get translation
//	MVector vTrans = fnTransform.translation (MSpace::kWorld);
//	translation.set ((float)vTrans.x, (float)vTrans.y, (float)vTrans.z);
//
//	// Get quaternion rotation
//	MQuaternion	quaternion;
//	if (fnTransform.getRotation (quaternion, MSpace::kWorld) != MS::kSuccess)
//		return;
//
//	rotation.x = (float)quaternion.x;		
//	rotation.y = (float)quaternion.y;
//	rotation.z = (float)quaternion.z;
//	rotation.w = (float)quaternion.w;
//}

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
		MStatus		status = MS::kSuccess;
		MFnMesh		fnMesh (path, &status);
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

//----------------------------------------------------------------------------//
// MayaInterface.cpp                                                          //
// Copyright (C) 2001-2004 Bruno 'Beosil' Heidelberger, Thomas Cowell         //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//
#ifndef MAYA_INTERFACE_H
#define MAYA_INTERFACE_H

#include "BaseInterface.h"

#include <maya/MGlobal.h>
#include <maya/MComputation.h>
#include <maya/MSelectionList.h>
#include <maya/MTime.h>
#include <maya/MDagPath.h>
#include <maya/MAnimControl.h>
#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

class CMayaMaterial;

class CMayaInterface : public CBaseInterface
{
public:
	CMayaInterface();
	virtual ~CMayaInterface();

	bool Create (bool bSelectedOnly);
	bool Destroy ();
	
	int GetCurrentFrame();
	float GetCurrentTime();
	int GetEndFrame();
	int GetFps();
	CWnd *GetMainWnd();
	CBaseMaterial *GetMaterial(int materialId);
	int GetMaterialCount();
	CBaseMesh *GetMesh(CBaseNode *pNode);
	CBaseNode *GetNode(const std::string& strName);
	int GetSelectedNodeCount();
	CBaseNode *GetSelectedNode(int nodeId);
	int GetStartFrame();
	void GetTranslationAndRotation(CBaseNode *pNode, CBaseNode *pParentNode, float time, CalVector& translation, CalQuaternion& rotation);
	void GetTranslationAndRotationBoneSpace(CBaseNode *pNode, float time, CalVector& translation, CalQuaternion& rotation);
	bool IsBone(CBaseNode *pNode);
	bool IsDummy(CBaseNode *pNode);
	bool IsMesh(CBaseNode *pNode);
	void SetProgressInfo(int percentage);
	void StartProgressInfo(const std::string& strText);
	void StopProgressInfo();

	int GetMaterialIDFromShader (const MObject &shaderObj);

private:
	bool EnumMaterials ();

	MSelectionList m_selList;
        MSelectionList m_entireList;
	
	std::vector<CMayaMaterial*>	m_materials;
};


class CPushCurrentTime
{
public:
  CPushCurrentTime() {
    time = MAnimControl::currentTime();
  }

  ~CPushCurrentTime() {
    MAnimControl::setCurrentTime(time);
  }

private:
  MTime time;
};


#endif

//----------------------------------------------------------------------------//
// MilkInterface.h                                                            //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_INTERFACE_H
#define MILK_INTERFACE_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseInterface.h"

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseNode;
class CBaseMaterial;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkInterface : public CBaseInterface
{
// misc
protected:
	static DWORD WINAPI ProgressFunction(LPVOID arg);

// member variables
protected:
  msModel *m_pIModel;
  std::vector<int> m_vectorRootBoneId;
  std::vector<int> m_vectorSelectedMeshId;

// constructors/destructor
public:
	CMilkInterface();
	virtual ~CMilkInterface();

// member functions
public:
	bool Create(msModel *pIModel);
	int GetCurrentFrame();
	float GetCurrentTime();
	int GetEndFrame();
	int GetFps();
  msModel *GetIModel();
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
};

#endif

//----------------------------------------------------------------------------//

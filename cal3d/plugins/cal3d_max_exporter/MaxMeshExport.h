//----------------------------------------------------------------------------//
// MaxMeshExport.h                                                            //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_MESH_EXPORT_H
#define MAX_MESH_EXPORT_H

class MeshMaxscriptExportParams
{
protected:
	MeshMaxscriptExportParams();

public:
//constructor
	MeshMaxscriptExportParams(INode* _MeshNode, const char* _SkeletonFilename, int _MaxNumBonesPerVertex, float _WeightThreshold, int _LODCreation, int _springsystem);
	virtual ~MeshMaxscriptExportParams();

//Members variables
	INode*	m_MeshNode;
	char*	m_SkeletonFilename;
	int		m_MaxNumBonesPerVertex;
	float	m_WeightThreshold;
	int		m_LODCreation;
	int		m_springsystem;
};



//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxMeshExport : public SceneExport
{
// constructors/destructor
public:
	CMaxMeshExport();
	virtual ~CMaxMeshExport();

	static int ExportMeshFromMaxscriptCall(const TCHAR *name, const MeshMaxscriptExportParams& _param);


// interface functions	
public:
	const TCHAR *AuthorName();
	const TCHAR *CopyrightMessage();
	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options);
	const TCHAR *Ext(int n);
	int ExtCount();
	const TCHAR *LongDesc();
	const TCHAR *OtherMessage1();
	const TCHAR *OtherMessage2();
	const TCHAR *ShortDesc();
	void ShowAbout(HWND hWnd);
	unsigned int Version();
};

#endif

//----------------------------------------------------------------------------//

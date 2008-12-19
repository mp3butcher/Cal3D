//----------------------------------------------------------------------------//
// Exporter.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef EXPORTER_H
#define EXPORTER_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include <string>
#include <vector>
#include <list>

#include <MeshCandidate.h>

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseInterface;
class CSkeletonCandidate;


//----------------------------------------------------------------------------//
// Export options                                                             //
//----------------------------------------------------------------------------//
#define CAL3D_EXPORT_OPTION_DESELECT_DUMMY_NODES ( 0 )
#define CAL3D_EXPORT_OPTION_DESELECT_EX_NODES ( 1 )
#define CAL3D_EXPORT_OPTION_DESELECT_EX_NODES_PREFIX_STRING "ex_"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CExporter
{
// member variables
protected:
	CBaseInterface *m_pInterface;
	std::string m_strLastError;

  bool m_useAxisGL;

// constructors/destructor
public:
	CExporter();
	virtual ~CExporter();

// member functions
public:
	bool Create(CBaseInterface *pInterface);
	bool ExportAnimation(const std::string& strFilename);
	bool ExportMorphAnimation(const std::string& strFilename);
	bool ExportMaterial(const std::string& strFilename);
	bool ExportMesh(const std::string& strFilename);
	bool ExportSkeleton(const std::string& strFilename);
	CBaseInterface *GetInterface();
	const std::string& GetLastError();
	void SetLastError(const std::string& strText, const std::string& strFilename, int line);
	void SetLastErrorFromCal(const std::string& strFilename, int line);

  void SetAxisGL(bool _useAxisGL) { m_useAxisGL=_useAxisGL;}
	bool ExportAnimationFromMaxscriptCall	(const std::string& strFilename, void* AnimExportParams);
  bool GetAxisGL() { return m_useAxisGL;}
	bool ExportMorphAnimationFromMaxscriptCall	(const std::string& strFilename, void* AnimExportParams);
	bool ExportMaterialFromMaxscriptCall	(const std::string& strFilename);
	bool ExportMeshFromMaxscriptCall		(const std::string& strFilename, void* MeshExportParams);
	bool ExportSkeletonFromMaxscriptCall	(const std::string& strFilename, bool bShowUI);

 protected:
        // internal functions
        bool meshCandidateToCoreMesh(CMeshCandidate const & meshCandidate, CalCoreMesh & coreMesh,
          CalVector const & positionOffset);

};

extern CExporter theExporter;

#endif

//----------------------------------------------------------------------------//

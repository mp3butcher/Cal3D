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

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseInterface;

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
	bool ExportMaterial(const std::string& strFilename);
	bool ExportMesh(const std::string& strFilename);
	bool ExportSkeleton(const std::string& strFilename);
	CBaseInterface *GetInterface();
	const std::string& GetLastError();
	void SetLastError(const std::string& strText, const std::string& strFilename, int line);
	void SetLastErrorFromCal(const std::string& strFilename, int line);

  void SetAxisGL(bool _useAxisGL) { m_useAxisGL=_useAxisGL;}
  bool GetAxisGL() { return m_useAxisGL;}
};

extern CExporter theExporter;

#endif

//----------------------------------------------------------------------------//

//****************************************************************************//
// error.h                                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_ERROR_H
#define CAL_ERROR_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The error class.
  *****************************************************************************/

class CAL3D_API CalError
{
// misc
public:
  static const unsigned int OK;
  static const unsigned int INTERNAL;
  static const unsigned int INVALID_HANDLE;
  static const unsigned int MEMORY_ALLOCATION_FAILED;
  static const unsigned int FILE_NOT_FOUND;
  static const unsigned int INVALID_FILE_FORMAT;
  static const unsigned int FILE_PARSER_FAILED;
  static const unsigned int INDEX_BUILD_FAILED;
  static const unsigned int NO_PARSER_DOCUMENT;
  static const unsigned int INVALID_ANIMATION_DURATION;
  static const unsigned int BONE_NOT_FOUND;
  static const unsigned int INVALID_ATTRIBUTE_VALUE;
  static const unsigned int INVALID_KEYFRAME_COUNT;
  static const unsigned int INVALID_ANIMATION_TYPE;
  static const unsigned int FILE_CREATION_FAILED;
  static const unsigned int FILE_WRITING_FAILED;
  static const unsigned int INCOMPATIBLE_FILE_VERSION;
  static const unsigned int NO_MESH_IN_MODEL;
  static const unsigned int MAX_ERROR_CODE;

// member variables
protected:
  static unsigned int m_lastErrorCode;
  static std::string m_strLastErrorFile;
  static int m_lastErrorLine;
  static std::string m_strLastErrorText;

// constructors/destructor
protected:
	CalError();
	virtual ~CalError();

// member functions	
public:
  static unsigned int getLastErrorCode();
	static std::string getLastErrorDescription();
  static const std::string& getLastErrorFile();
  static int getLastErrorLine();
  static const std::string& getLastErrorText();
  static void printLastError();
	static void setLastError(unsigned int code, const std::string& strFile, int line, const std::string& strText = "");
};

#endif

//****************************************************************************//

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

#include "cal3d/global.h"

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
  enum Code
  {
    OK = 0,
    INTERNAL,
    INVALID_HANDLE,
    MEMORY_ALLOCATION_FAILED,
    FILE_NOT_FOUND,
    INVALID_FILE_FORMAT,
    FILE_PARSER_FAILED,
    INDEX_BUILD_FAILED,
    NO_PARSER_DOCUMENT,
    INVALID_ANIMATION_DURATION,
    BONE_NOT_FOUND,
    INVALID_ATTRIBUTE_VALUE,
    INVALID_KEYFRAME_COUNT,
    INVALID_ANIMATION_TYPE,
    FILE_CREATION_FAILED,
    FILE_WRITING_FAILED,
    INCOMPATIBLE_FILE_VERSION,
    NO_MESH_IN_MODEL,
    MAX_ERROR_CODE	
  };

// member variables
protected:
  static Code m_lastErrorCode;
  static std::string m_strLastErrorFile;
  static int m_lastErrorLine;
  static std::string m_strLastErrorText;

// constructors/destructor
protected:
  CalError();
  virtual ~CalError();

// member functions	
public:
  static Code getLastErrorCode();
  static std::string getLastErrorDescription();
  static const std::string& getLastErrorFile();
  static int getLastErrorLine();
  static const std::string& getLastErrorText();
  static void printLastError();
  static void setLastError(Code code, const std::string& strFile, int line, const std::string& strText = "");
};

#endif

//****************************************************************************//

//****************************************************************************//
// error.cpp                                                                  //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "error.h"

//****************************************************************************//
// Static member variables initialization                                     //
//****************************************************************************//

const unsigned int CalError::OK = 0;
const unsigned int CalError::INTERNAL = 1;
const unsigned int CalError::INVALID_HANDLE = 2;
const unsigned int CalError::MEMORY_ALLOCATION_FAILED = 3;
const unsigned int CalError::FILE_NOT_FOUND = 4;
const unsigned int CalError::INVALID_FILE_FORMAT = 5;
const unsigned int CalError::FILE_PARSER_FAILED = 6;
const unsigned int CalError::INDEX_BUILD_FAILED = 7;
const unsigned int CalError::NO_PARSER_DOCUMENT = 8;
const unsigned int CalError::INVALID_ANIMATION_DURATION = 9;
const unsigned int CalError::BONE_NOT_FOUND = 10;
const unsigned int CalError::INVALID_ATTRIBUTE_VALUE = 11;
const unsigned int CalError::INVALID_KEYFRAME_COUNT = 12;
const unsigned int CalError::INVALID_ANIMATION_TYPE = 13;
const unsigned int CalError::FILE_CREATION_FAILED = 14;
const unsigned int CalError::FILE_WRITING_FAILED = 15;
const unsigned int CalError::INCOMPATIBLE_FILE_VERSION = 16;
const unsigned int CalError::NO_MESH_IN_MODEL = 17;
const unsigned int CalError::MAX_ERROR_CODE = 18;

unsigned int CalError::m_lastErrorCode = CalError::OK;
std::string CalError::m_strLastErrorFile;
int CalError::m_lastErrorLine = -1;
std::string CalError::m_strLastErrorText;

 /*****************************************************************************/
/** Constructs the error instance.
  *
  * This function is the default constructor of the error instance.
  *****************************************************************************/

CalError::CalError()
{
}

 /*****************************************************************************/
/** Destructs the error instance.
  *
  * This function is the destructor of the error instance.
  *****************************************************************************/

CalError::~CalError()
{
}

 /*****************************************************************************/
/** Returns the code of the last error.
  *
  * This function returns the code of the last error that occured inside the
  * library.
  *
  * @return The code of the last error.
  *****************************************************************************/

unsigned int CalError::getLastErrorCode()
{
  return m_lastErrorCode;
}

 /*****************************************************************************/
/** Returns a description of the last error.
  *
  * This function returns a short description of the last error that occured
  * inside the library.
  *
  * @return The description of the last error.
  *****************************************************************************/

std::string CalError::getLastErrorDescription()
{
  switch(m_lastErrorCode)
  {
    case OK:
      return "No error found";
      break;
    case INTERNAL:
      return "Internal error";
      break;
    case INVALID_HANDLE:
      return "Invalid handle as argument";
      break;
    case MEMORY_ALLOCATION_FAILED:
      return "Memory allocation failed";
      break;
    case FILE_NOT_FOUND:
      return "File not found";
      break;
    case INVALID_FILE_FORMAT:
      return "Invalid file format";
      break;
    case FILE_PARSER_FAILED:
      return "Parser failed to process file";
      break;
    case INDEX_BUILD_FAILED:
      return "Building of the index failed";
      break;
    case NO_PARSER_DOCUMENT:
      return "There is no document to parse";
      break;
    case INVALID_ANIMATION_DURATION:
      return "The duration of the animation is invalid";
      break;
    case BONE_NOT_FOUND:
      return "Bone not found";
      break;
    case INVALID_ATTRIBUTE_VALUE:
      return "Invalid attribute value";
      break;
    case INVALID_KEYFRAME_COUNT:
      return "Invalid number of keyframes";
      break;
    case INVALID_ANIMATION_TYPE:
      return "Invalid animation type";
      break;
    case FILE_CREATION_FAILED:
      return "Failed to create file";
      break;
    case FILE_WRITING_FAILED:
      return "Failed to write to file";
      break;
    case INCOMPATIBLE_FILE_VERSION:
      return "Incompatible file version";
      break;
    case NO_MESH_IN_MODEL:
      return "No mesh attached to the model";
      break;
    default:
      break;
  }

  return "Unknown error";
}

 /*****************************************************************************/
/** Returns the name of the file where the last error occured.
  *
  * This function returns the name of the file where the last error occured.
  *
  * @return The name of the file where the last error occured.
  *****************************************************************************/

const std::string& CalError::getLastErrorFile()
{
  return m_strLastErrorFile;
}

 /*****************************************************************************/
/** Returns the line number where the last error occured.
  *
  * This function returns the line number where the last error occured.
  *
  * @return The line number where the last error occured.
  *****************************************************************************/

int CalError::getLastErrorLine()
{
  return m_lastErrorLine;
}

 /*****************************************************************************/
/** Returns the supplementary text of the last error.
  *
  * This function returns the suppementary text of the last error occured
  * inside the library.
  *
  * @return The supplementary text of the last error.
  *****************************************************************************/

const std::string& CalError::getLastErrorText()
{
  return m_strLastErrorText;
}

 /*****************************************************************************/
/** Dumps all information about the last error to the standard output.
  *
  * This function dumps all the information about the last error that occured
  * inside the library to the standard output.
  *****************************************************************************/

void CalError::printLastError()
{
  std::cout << "cal3d : " << getLastErrorDescription();

  // only print supplementary information if there is some
  if(m_strLastErrorText.size() > 0)
  {
    std::cout << " '" << m_strLastErrorText << "'";
  }

  std::cout << " in " << m_strLastErrorFile << "(" << m_lastErrorLine << ")" << std::endl;
}

 /*****************************************************************************/
/** Sets all the information about the last error.
  *
  * This function sets all the information about the last error that occured
  * inside the library.
  *
  * @param code  The code of the last error.
  * @param strFile  The file where the last error occured.
  * @param line  The line number where the last error occured.
  * @param strText  The supplementary text of the last error.
  *****************************************************************************/

void CalError::setLastError(unsigned int code, const std::string& strFile, int line, const std::string& strText)
{
  if(code >= MAX_ERROR_CODE) code = INTERNAL;

  m_lastErrorCode = code;
  m_strLastErrorFile = strFile;
  m_lastErrorLine = line;
  m_strLastErrorText = strText;
}

//****************************************************************************//

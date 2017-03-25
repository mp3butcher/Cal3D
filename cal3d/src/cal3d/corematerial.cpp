//****************************************************************************//
// corematerial.cpp                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "cal3d/error.h"
#include "cal3d/corematerial.h"

using namespace cal3d;

CalCoreMaterial::CalCoreMaterial()
  : m_userData(0)
{
} 

CalCoreMaterial::CalCoreMaterial( const CalCoreMaterial& inOther )
  : m_ambientColor( inOther.m_ambientColor )
  , m_diffuseColor( inOther.m_diffuseColor )
  , m_specularColor( inOther.m_specularColor )
  , m_shininess( inOther.m_shininess )
  , m_vectorMap( inOther.m_vectorMap )
  , m_userData( inOther.m_userData )
  , m_name( inOther.m_name )
  , m_filename( inOther.m_filename )
{
}


 /*****************************************************************************/
/** Returns the number of maps.
  *
  * This function returns the number of mapss in the core material instance.
  *
  * @return The number of maps.
  *****************************************************************************/

int CalCoreMaterial::getMapCount() const
{
  return m_vectorMap.size();
}

 /*****************************************************************************/
/** Returns a specified map texture filename.
  *
  * This function returns the texture filename for a specified map ID of the
  * core material instance.
  *
  * @param mapId The ID of the map.
  *
  * @return One of the following values:
  *         \li the filename of the map texture
  *         \li an empty string if an error happened
  *****************************************************************************/

const std::string& CalCoreMaterial::getMapFilename(int mapId) const
{
  // check if the map id is valid
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    static const std::string strNull;
    return strNull;
  }

  return m_vectorMap[mapId].strFilename;
}

 /*****************************************************************************/
/** Returns a specified map type
  *
  * This function returns the map type for a specified map ID of the
  * core material instance. The type will be an exporter-specific string which
  * explains what the Map is meant to be used for, such as "Opacity"
  *
  * @param mapId The ID of the map.
  *
  * @return One of the following values:
  *         \li the type of the map 
  *         \li an empty string if an error happend
  *****************************************************************************/

const std::string& CalCoreMaterial::getMapType(int mapId)
{
  // check if the map id is valid
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    static std::string null;
    return null;
  }

  return m_vectorMap[mapId].mapType;
}

 /*****************************************************************************/
/** Provides access to a specified map user data.
  *
  * This function returns the user data stored in the specified map of the core
  * material instance.
  *
  * @param mapId The ID of the map.
  *
  * @return One of the following values:
  *         \li the user data stored in the specified map
  *         \li \b 0 if an error happened
  *****************************************************************************/

Cal::UserData CalCoreMaterial::getMapUserData(int mapId)
{
  // check if the map id is valid
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorMap[mapId].userData;
}

 /*****************************************************************************/
/** Provides access to a specified map user data.
  *
  * This function returns the user data stored in the specified map of the core
  * material instance.
  *
  * @param mapId The ID of the map.
  *
  * @return One of the following values:
  *         \li the user data stored in the specified map
  *         \li \b 0 if an error happened
  *****************************************************************************/

const Cal::UserData CalCoreMaterial::getMapUserData(int mapId) const
{
  // check if the map id is valid
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorMap[mapId].userData;
}



 /*****************************************************************************/
/** Reserves memory for the maps.
  *
  * This function reserves memory for the maps of the core material instance.
  *
  * @param mapCount The number of maps that this core material instance should
  *                 be able to hold.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreMaterial::reserve(int mapCount)
{
	bool	success = true;
	try
	{
		// reserve the space needed in all the vectors
		m_vectorMap.reserve(mapCount);
		m_vectorMap.resize(mapCount);
	}
	catch (...)
	{
		success = false;
	}

	return success;
}


 /*****************************************************************************/
/** Sets a specified map.
  *
  * This function sets a specified map in the core material instance.
  *
  * @param mapId  The ID of the map.
  * @param map The map that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreMaterial::setMap(int mapId, const Map& map)
{
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size())) return false;

  m_vectorMap[mapId] = map;

  return true;
}

 /*****************************************************************************/
/** Stores specified map user data.
  *
  * This function stores user data in a specified map of the core material
  * instance.
  *
  * @param mapId  The ID of the map.
  * @param userData The user data that should be stored.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreMaterial::setMapUserData(int mapId, Cal::UserData userData)
{
  if((mapId < 0) || (mapId >= (int)m_vectorMap.size())) return false;

  m_vectorMap[mapId].userData = userData;

  return true;
}


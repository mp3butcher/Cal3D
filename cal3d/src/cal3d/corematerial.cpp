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
/** Returns the ambient color.
  *
  * This function returns the ambient color of the core material instance.
  *
  * @return A reference to the ambient color.
  *****************************************************************************/

const CalCoreMaterial::Color& CalCoreMaterial::getAmbientColor() const
{
  return m_ambientColor;
}

 /*****************************************************************************/
/** Returns the diffuse color.
  *
  * This function returns the diffuse color of the core material instance.
  *
  * @return A reference to the diffuse color.
  *****************************************************************************/

const CalCoreMaterial::Color& CalCoreMaterial::getDiffuseColor() const
{
  return m_diffuseColor;
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
/** Returns the shininess factor.
  *
  * This function returns the shininess factor of the core material instance.
  *
  * @return The shininess factor.
  *****************************************************************************/

float CalCoreMaterial::getShininess() const
{
  return m_shininess;
}

 /*****************************************************************************/
/** Returns the specular color.
  *
  * This function returns the specular color of the core material instance.
  *
  * @return A reference to the specular color.
  *****************************************************************************/

const CalCoreMaterial::Color& CalCoreMaterial::getSpecularColor() const
{
  return m_specularColor;
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the core material instance.
  *
  * @return The user data stored in the core material instance.
  *****************************************************************************/

Cal::UserData CalCoreMaterial::getUserData()
{
  return m_userData;
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the core material instance.
  *
  * @return The user data stored in the core material instance.
  *****************************************************************************/

const Cal::UserData CalCoreMaterial::getUserData() const
{
  return m_userData;
}

 /*****************************************************************************/
/** Returns the map vector.
  *
  * This function returns the vector that contains all maps of the core material
  * instance.
  *
  * @return A reference to the map vector.
  *****************************************************************************/

std::vector<CalCoreMaterial::Map>& CalCoreMaterial::getVectorMap()
{
  return m_vectorMap;
}

 /*****************************************************************************/
/** Returns the map vector.
  *
  * This function returns the vector that contains all maps of the core material
  * instance.
  *
  * @return A reference to the map vector.
  *****************************************************************************/

const std::vector<CalCoreMaterial::Map>& CalCoreMaterial::getVectorMap() const
{
  return m_vectorMap;
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
/** Sets the ambient color.
  *
  * This function sets the ambient color of the core material instance.
  *
  * @param ambientColor The ambient color that should be set.
  *****************************************************************************/

void CalCoreMaterial::setAmbientColor(const CalCoreMaterial::Color& ambientColor)
{
  m_ambientColor = ambientColor;
}

 /*****************************************************************************/
/** Sets the diffuse color.
  *
  * This function sets the diffuse color of the core material instance.
  *
  * @param ambientColor The diffuse color that should be set.
  *****************************************************************************/

void CalCoreMaterial::setDiffuseColor(const CalCoreMaterial::Color& diffuseColor)
{
  m_diffuseColor = diffuseColor;
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

 /*****************************************************************************/
/** Sets the shininess factor.
  *
  * This function sets the shininess factor of the core material instance.
  *
  * @param shininess The shininess factor that should be set.
  *****************************************************************************/

void CalCoreMaterial::setShininess(float shininess)
{
  m_shininess = shininess;
}

 /*****************************************************************************/
/** Sets the specular color.
  *
  * This function sets the specular color of the core material instance.
  *
  * @param ambientColor The specular color that should be set.
  *****************************************************************************/

void CalCoreMaterial::setSpecularColor(const CalCoreMaterial::Color& specularColor)
{
  m_specularColor = specularColor;
}


 /*****************************************************************************/
/** 
  * Set the name of the file in which the core material is stored, if any.
  *
  * @param filename The path of the file.
  *****************************************************************************/

void CalCoreMaterial::setFilename(const std::string& filename)
{
  m_filename = filename;
}

 /*****************************************************************************/
/** 
  * Get the name of the file in which the core material is stored, if any.
  *
  * @return One of the following values:
  *         \li \b empty string if the material was not stored in a file
  *         \li \b the path of the file
  *
  *****************************************************************************/

const std::string& CalCoreMaterial::getFilename(void) const
{
  return m_filename;
}

 /*****************************************************************************/
/** 
  * Set the symbolic name of the core material.
  *
  * @param name A symbolic name.
  *****************************************************************************/

void CalCoreMaterial::setName(const std::string& name)
{
  m_name = name;
}

 /*****************************************************************************/
/** 
  * Get the symbolic name the core material.
  *
  * @return One of the following values:
  *         \li \b empty string if the material was no associated to a symbolic name
  *         \li \b the symbolic name
  *
  *****************************************************************************/

const std::string& CalCoreMaterial::getName(void) const
{
  return m_name;
}

 /*****************************************************************************/
/** Stores user data.
  *
  * This function stores user data in the core material instance.
  *
  * @param userData The user data that should be stored.
  *****************************************************************************/

void CalCoreMaterial::setUserData(Cal::UserData userData)
{
  m_userData = userData;
}

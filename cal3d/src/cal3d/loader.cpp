//****************************************************************************//
// loader.cpp                                                                 //
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

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "loader.h"
#include "error.h"
#include "matrix.h"
#include "vector.h"
#include "quaternion.h"
#include "coremodel.h"
#include "coreskeleton.h"
#include "corebone.h"
#include "coreanimation.h"
#include "coretrack.h"
#include "corekeyframe.h"
#include "coremesh.h"
#include "coresubmesh.h"
#include "corematerial.h"

#ifdef CAL_USE_XML
#include "tinyxml.h"
#endif

 /*****************************************************************************/
/** Constructs the loader instance.
  *
  * This function is the default constructor of the loader instance.
  *****************************************************************************/

CalLoader::CalLoader()
{
}

 /*****************************************************************************/
/** Destructs the loader instance.
  *
  * This function is the destructor of the loader instance.
  *****************************************************************************/

CalLoader::~CalLoader()
{
}

 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreAnimation *CalLoader::loadCoreAnimation(const std::string& strFilename)
{
#ifdef CAL_USE_XML
  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC)==0)
	 return loadXmlCoreAnimation(strFilename);
#endif

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  char magic[4];
  if(!CalPlatform::readBytes(file, &magic[0], 4) || (memcmp(&magic[0], Cal::ANIMATION_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!CalPlatform::readInteger(file, version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core animation instance
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = new CalCoreAnimation();
  if(pCoreAnimation == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core animation instance
  if(!pCoreAnimation->create())
  {
    delete pCoreAnimation;
    return 0;
  }

  // get the duration of the core animation
  float duration;
  if(!CalPlatform::readFloat(file, duration))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreAnimation->destroy();
    delete pCoreAnimation;
    return 0;
  }

  // check for a valid duration
  if(duration <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__, strFilename);
    pCoreAnimation->destroy();
    delete pCoreAnimation;
    return 0;
  }

  // set the duration in the core animation instance
  pCoreAnimation->setDuration(duration);

  // read the number of tracks
  int trackCount;
  if(!CalPlatform::readInteger(file, trackCount) || (trackCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // load all core bones
  int trackId;
  for(trackId = 0; trackId < trackCount; trackId++)
  {
    // load the core track
    CalCoreTrack *pCoreTrack;
    pCoreTrack = loadCoreTrack(file, strFilename);
    if(pCoreTrack == 0)
    {
      pCoreAnimation->destroy();
      delete pCoreAnimation;
      return 0;
    }

    // add the core track to the core animation instance
    pCoreAnimation->addCoreTrack(pCoreTrack);
  }

  // explicitly close the file
  file.close();

  return pCoreAnimation;
}

 /*****************************************************************************/
/** Loads a core bone instance.
  *
  * This function loads a core bone instance from a file stream.
  *
  * @param file The file stream to load the core bone instance from.
  * @param strFilename The name of the file stream.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreBone *CalLoader::loadCoreBones(std::ifstream& file, const std::string& strFilename)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // read the name of the bone
  std::string strName;
  CalPlatform::readString(file, strName);

  // get the translation of the bone
  float tx, ty, tz;
  CalPlatform::readFloat(file, tx);
  CalPlatform::readFloat(file, ty);
  CalPlatform::readFloat(file, tz);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  CalPlatform::readFloat(file, rx);
  CalPlatform::readFloat(file, ry);
  CalPlatform::readFloat(file, rz);
  CalPlatform::readFloat(file, rw);

  // get the bone space translation of the bone
  float txBoneSpace, tyBoneSpace, tzBoneSpace;
  CalPlatform::readFloat(file, txBoneSpace);
  CalPlatform::readFloat(file, tyBoneSpace);
  CalPlatform::readFloat(file, tzBoneSpace);

  // get the bone space rotation of the bone
  float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
  CalPlatform::readFloat(file, rxBoneSpace);
  CalPlatform::readFloat(file, ryBoneSpace);
  CalPlatform::readFloat(file, rzBoneSpace);
  CalPlatform::readFloat(file, rwBoneSpace);

  // get the parent bone id
  int parentId;
  CalPlatform::readInteger(file, parentId);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core bone instance
  CalCoreBone *pCoreBone;
  pCoreBone = new CalCoreBone();
  if(pCoreBone == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core bone instance
  if(!pCoreBone->create(strName))
  {
    delete pCoreBone;
    return 0;
  }

  // set the parent of the bone
  pCoreBone->setParentId(parentId);

  // set all attributes of the bone
  pCoreBone->setTranslation(CalVector(tx, ty, tz));
  pCoreBone->setRotation(CalQuaternion(rx, ry, rz, rw));
  pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
  pCoreBone->setRotationBoneSpace(CalQuaternion(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace));

  // read the number of children
  int childCount;
  if(!CalPlatform::readInteger(file, childCount) || (childCount < 0))
  {
    pCoreBone->destroy();
    delete pCoreBone;
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // load all children ids
  for(; childCount > 0; childCount--)
  {
    int childId;
    if(!CalPlatform::readInteger(file, childId) || (childId < 0))
    {
      pCoreBone->destroy();
      delete pCoreBone;
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
    }

    pCoreBone->addChildId(childId);
  }

  return pCoreBone;
}

 /*****************************************************************************/
/** Loads a core keyframe instance.
  *
  * This function loads a core keyframe instance from a file stream.
  *
  * @param file The file stream to load the core keyframe instance from.
  * @param strFilename The name of the file stream.
  *
  * @return One of the following values:
  *         \li a pointer to the core keyframe
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreKeyframe *CalLoader::loadCoreKeyframe(std::ifstream& file, const std::string& strFilename)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // get the time of the keyframe
  float time;
  CalPlatform::readFloat(file, time);

  // get the translation of the bone
  float tx, ty, tz;
  CalPlatform::readFloat(file, tx);
  CalPlatform::readFloat(file, ty);
  CalPlatform::readFloat(file, tz);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  CalPlatform::readFloat(file, rx);
  CalPlatform::readFloat(file, ry);
  CalPlatform::readFloat(file, rz);
  CalPlatform::readFloat(file, rw);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core keyframe instance
  CalCoreKeyframe *pCoreKeyframe;
  pCoreKeyframe = new CalCoreKeyframe();
  if(pCoreKeyframe == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core keyframe instance
  if(!pCoreKeyframe->create())
  {
    delete pCoreKeyframe;
    return 0;
  }

  // set all attributes of the keyframe
  pCoreKeyframe->setTime(time);
  pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
  pCoreKeyframe->setRotation(CalQuaternion(rx, ry, rz, rw));

  return pCoreKeyframe;
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a file.
  *
  * @param strFilename The name of the file to load the core material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMaterial *CalLoader::loadCoreMaterial(const std::string& strFilename)
{
#ifdef CAL_USE_XML
	if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC)==0)
	 return loadXmlCoreMaterial(strFilename);
#endif

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  char magic[4];
  if(!CalPlatform::readBytes(file, &magic[0], 4) || (memcmp(&magic[0], Cal::MATERIAL_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!CalPlatform::readInteger(file, version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core material instance
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = new CalCoreMaterial();
  if(pCoreMaterial == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core material instance
  if(!pCoreMaterial->create())
  {
    delete pCoreMaterial;
    return 0;
  }

  // get the ambient color of the core material
  CalCoreMaterial::Color ambientColor;
  CalPlatform::readBytes(file, &ambientColor, sizeof(ambientColor));

  // get the diffuse color of the core material
  CalCoreMaterial::Color diffuseColor;
  CalPlatform::readBytes(file, &diffuseColor, sizeof(diffuseColor));

  // get the specular color of the core material
  CalCoreMaterial::Color specularColor;
  CalPlatform::readBytes(file, &specularColor, sizeof(specularColor));

  // get the shininess factor of the core material
  float shininess;
  CalPlatform::readFloat(file, shininess);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
    delete pCoreMaterial;
    return 0;
  }

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(shininess);

  // read the number of maps
  int mapCount;
  if(!CalPlatform::readInteger(file, mapCount) || (mapCount < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // reserve memory for all the material data
  if(!pCoreMaterial->reserve(mapCount))
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
    delete pCoreMaterial;
    return 0;
  }

  // load all maps
  int mapId;
  for(mapId = 0; mapId < mapCount; mapId++)
  {
    CalCoreMaterial::Map map;

    // read the filename of the map
    std::string strName;
    CalPlatform::readString(file, map.strFilename);

    // initialize the user data
    map.userData = 0;

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreMaterial->destroy();
      delete pCoreMaterial;
      return 0;
    }

    // set map in the core material instance
    pCoreMaterial->setMap(mapId, map);
  }

  // explicitly close the file
  file.close();

  return pCoreMaterial;
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a file.
  *
  * @param strFilename The name of the file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMesh *CalLoader::loadCoreMesh(const std::string& strFilename)
{
#ifdef CAL_USE_XML
   if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC)==0)
	 return loadXmlCoreMesh(strFilename);
#endif
  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  char magic[4];
  if(!CalPlatform::readBytes(file, &magic[0], 4) || (memcmp(&magic[0], Cal::MESH_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!CalPlatform::readInteger(file, version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // get the number of submeshes
  int submeshCount;
  if(!CalPlatform::readInteger(file, submeshCount))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core mesh instance
  CalCoreMesh *pCoreMesh;
  pCoreMesh = new CalCoreMesh();
  if(pCoreMesh == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core mesh instance
  if(!pCoreMesh->create())
  {
    delete pCoreMesh;
    return 0;
  }

  // load all core submeshes
  int submeshId;
  for(submeshId = 0; submeshId < submeshCount; submeshId++)
  {
    // load the core submesh
    CalCoreSubmesh *pCoreSubmesh;
    pCoreSubmesh = loadCoreSubmesh(file, strFilename);
    if(pCoreSubmesh == 0)
    {
      pCoreMesh->destroy();
      delete pCoreMesh;
      return 0;
    }

    // add the core submesh to the core mesh instance
    pCoreMesh->addCoreSubmesh(pCoreSubmesh);
  }

  // explicitly close the file
  file.close();

  return pCoreMesh;
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSkeleton *CalLoader::loadCoreSkeleton(const std::string& strFilename)
{
#ifdef CAL_USE_XML
  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC)==0)
	 return loadXmlCoreSkeleton(strFilename);
#endif

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  char magic[4];
  if(!CalPlatform::readBytes(file, &magic[0], 4) || (memcmp(&magic[0], Cal::SKELETON_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!CalPlatform::readInteger(file, version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // read the number of bones
  int boneCount;
  if(!CalPlatform::readInteger(file, boneCount) || (boneCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core skeleton instance
  CalCoreSkeleton *pCoreSkeleton;
  pCoreSkeleton = new CalCoreSkeleton();
  if(pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core skeleton instance
  if(!pCoreSkeleton->create())
  {
    delete pCoreSkeleton;
    return 0;
  }

  // load all core bones
  int boneId;
  for(boneId = 0; boneId < boneCount; boneId++)
  {
    // load the core bone
    CalCoreBone *pCoreBone;
    pCoreBone = loadCoreBones(file, strFilename);
    if(pCoreBone == 0)
    {
      pCoreSkeleton->destroy();
      delete pCoreSkeleton;
      return 0;
    }

    // set the core skeleton of the core bone instance
    pCoreBone->setCoreSkeleton(pCoreSkeleton);

    // add the core bone to the core skeleton instance
    pCoreSkeleton->addCoreBone(pCoreBone);
  }

  // explicitly close the file
  file.close();

  // calculate state of the core skeleton
  pCoreSkeleton->calculateState();

  return pCoreSkeleton;
}

 /*****************************************************************************/
/** Loads a core submesh instance.
  *
  * This function loads a core submesh instance from a file stream.
  *
  * @param file The file stream to load the core submesh instance from.
  * @param strFilename The name of the file stream.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSubmesh *CalLoader::loadCoreSubmesh(std::ifstream& file, const std::string& strFilename)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // get the material thread id of the submesh
  int coreMaterialThreadId;
  CalPlatform::readInteger(file,coreMaterialThreadId );

  // get the number of vertices, faces, level-of-details and springs
  int vertexCount;
  CalPlatform::readInteger(file,vertexCount);

  int faceCount;
  CalPlatform::readInteger(file, faceCount);

  int lodCount;
  CalPlatform::readInteger(file, lodCount);

  int springCount;
  CalPlatform::readInteger(file, springCount);

  // get the number of texture coordinates per vertex
  int textureCoordinateCount;
  CalPlatform::readInteger(file, textureCoordinateCount);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core submesh instance
  CalCoreSubmesh *pCoreSubmesh;
  pCoreSubmesh = new CalCoreSubmesh();
  if(pCoreSubmesh == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core submesh instance
  if(!pCoreSubmesh->create())
  {
    delete pCoreSubmesh;
    return 0;
  }

  // set the LOD step count
  pCoreSubmesh->setLodCount(lodCount);

  // set the core material id
  pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

  // reserve memory for all the submesh data
  if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__, strFilename);
    pCoreSubmesh->destroy();
    delete pCoreSubmesh;
    return 0;
  }

  // load all vertices and their influences
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    CalCoreSubmesh::Vertex vertex;

    // load data of the vertex
    CalPlatform::readFloat(file, vertex.position.x);
    CalPlatform::readFloat(file, vertex.position.y);
    CalPlatform::readFloat(file, vertex.position.z);
    CalPlatform::readFloat(file, vertex.normal.x);
    CalPlatform::readFloat(file, vertex.normal.y);
    CalPlatform::readFloat(file, vertex.normal.z);
    CalPlatform::readInteger(file, vertex.collapseId);
    CalPlatform::readInteger(file, vertex.faceCollapseCount);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
      return 0;
    }

    // load all texture coordinates of the vertex
    int textureCoordinateId;
    for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; textureCoordinateId++)
    {
      CalCoreSubmesh::TextureCoordinate textureCoordinate;

      // load data of the influence
      CalPlatform::readFloat(file, textureCoordinate.u);
      CalPlatform::readFloat(file, textureCoordinate.v);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSubmesh->destroy();
        delete pCoreSubmesh;
        return 0;
      }

      // set texture coordinate in the core submesh instance
      pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
    }

    // get the number of influences
    int influenceCount;
    if(!CalPlatform::readInteger(file, influenceCount) || (influenceCount < 0))
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
      return 0;
    }

    // reserve memory for the influences in the vertex
    vertex.vectorInfluence.reserve(influenceCount);
    vertex.vectorInfluence.resize(influenceCount);

    // load all influences of the vertex
    int influenceId;
    for(influenceId = 0; influenceId < influenceCount; influenceId++)
    {
      // load data of the influence
      CalPlatform::readInteger(file, vertex.vectorInfluence[influenceId].boneId),
      CalPlatform::readFloat(file, vertex.vectorInfluence[influenceId].weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSubmesh->destroy();
        delete pCoreSubmesh;
        return 0;
      }
    }

    // set vertex in the core submesh instance
    pCoreSubmesh->setVertex(vertexId, vertex);

    // load the physical property of the vertex if there are springs in the core submesh
    if(springCount > 0)
    {
      CalCoreSubmesh::PhysicalProperty physicalProperty;

      // load data of the physical property
      CalPlatform::readFloat(file, physicalProperty.weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSubmesh->destroy();
        delete pCoreSubmesh;
        return 0;
      }

      // set the physical property in the core submesh instance
      pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);
    }
  }

  // load all springs
  int springId;
  for(springId = 0; springId < springCount; springId++)
  {
    CalCoreSubmesh::Spring spring;

    // load data of the spring
    CalPlatform::readInteger(file, spring.vertexId[0]);
    CalPlatform::readInteger(file, spring.vertexId[1]);
    CalPlatform::readFloat(file, spring.springCoefficient);
    CalPlatform::readFloat(file, spring.idleLength);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
      return 0;
    }

    // set spring in the core submesh instance
    pCoreSubmesh->setSpring(springId, spring);
  }

  // load all faces
  int faceId;
  for(faceId = 0; faceId < faceCount; faceId++)
  {
    CalCoreSubmesh::Face face;

    // load data of the face
	
	int tmp[3];
	CalPlatform::readInteger(file, tmp[0]);
	CalPlatform::readInteger(file, tmp[1]);
	CalPlatform::readInteger(file, tmp[2]);

	if(sizeof(CalIndex)==2)
	{
		if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
		{			
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			pCoreSubmesh->destroy();
			delete pCoreSubmesh;
			return 0;
		}
	}
	face.vertexId[0]=tmp[0];
	face.vertexId[1]=tmp[1];
	face.vertexId[2]=tmp[2];
	
    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
      return 0;
    }

    // set face in the core submesh instance
    pCoreSubmesh->setFace(faceId, face);
  }

  return pCoreSubmesh;
}

 /*****************************************************************************/
/** Loads a core track instance.
  *
  * This function loads a core track instance from a file stream.
  *
  * @param file The file stream to load the core track instance from.
  * @param strFilename The name of the file stream.
  *
  * @return One of the following values:
  *         \li a pointer to the core track
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreTrack *CalLoader::loadCoreTrack(std::ifstream& file, const std::string& strFilename)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // read the bone id
  int coreBoneId;
  if(!CalPlatform::readInteger(file, coreBoneId) || (coreBoneId < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core track instance
  CalCoreTrack *pCoreTrack;
  pCoreTrack = new CalCoreTrack();
  if(pCoreTrack == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core track instance
  if(!pCoreTrack->create())
  {
    delete pCoreTrack;
    return 0;
  }

  // link the core track to the appropriate core bone instance
  pCoreTrack->setCoreBoneId(coreBoneId);

  // read the number of keyframes
  int keyframeCount;
  if(!CalPlatform::readInteger(file, keyframeCount) || (keyframeCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // load all core keyframes
  int keyframeId;
  for(keyframeId = 0; keyframeId < keyframeCount; keyframeId++)
  {
    // load the core keyframe
    CalCoreKeyframe *pCoreKeyframe;
    pCoreKeyframe = loadCoreKeyframe(file, strFilename);
    if(pCoreKeyframe == 0)
    {
      pCoreTrack->destroy();
      delete pCoreTrack;
      return 0;
    }

    // add the core keyframe to the core track instance
    pCoreTrack->addCoreKeyframe(pCoreKeyframe);
  }

  return pCoreTrack;
}

#ifdef CAL_USE_XML
 /*****************************************************************************/
/** Loads a core skeleton instance from a XML file.
  *
  * This function loads a core skeleton instance from a XML file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSkeleton *CalLoader::loadXmlCoreSkeleton(const std::string& strFilename)
{	
  
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  TiXmlNode* node;
  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
  }  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_MAGIC)!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return false;
  }

  TiXmlElement*skeleton = header->NextSiblingElement();
  if(!skeleton || stricmp(skeleton->Value(),"SKELETON")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return false;
  }  
  
  // allocate a new core skeleton instance
  CalCoreSkeleton *pCoreSkeleton;
  pCoreSkeleton = new CalCoreSkeleton();
  if(pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core skeleton instance
  if(!pCoreSkeleton->create())
  {
    delete pCoreSkeleton;
    return 0;
  }

  TiXmlElement* bone;
  for( bone = skeleton->FirstChildElement();bone;bone = bone->NextSiblingElement() )
  {
	  if(stricmp(bone->Value(),"BONE")!=0)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }	  
	  
	  std::string strName=bone->Attribute("NAME");
		  

	  // get the translation of the bone

	  TiXmlElement* translation = bone->FirstChildElement();
	  if(!translation || stricmp( translation->Value(),"TRANSLATION")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
		  return false;
	  }

	  float tx, ty, tz;
	  
	  node = translation->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }	  
	  TiXmlText* translationdata = node->ToText();
	  if(!translationdata)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }	  
	  str.clear();
	  str << translationdata->Value();
	  str >> tx >> ty >> tz;

      // get the rotation of the bone

	  TiXmlElement* rotation = translation->NextSiblingElement();
	  if(!rotation || stricmp(rotation->Value(),"ROTATION")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
		  return false;
	  }

	  float rx, ry, rz, rw;
	  
	  node = rotation->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  TiXmlText* rotationdata = node->ToText();
	  if(!rotationdata)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  str.clear();
	  str << rotationdata->Value();
	  str >> rx >> ry >> rz >> rw;	  
	  
	  // get the bone space translation of the bone
      
	  
	  TiXmlElement* translationBoneSpace = rotation->NextSiblingElement();
	  if(!rotation || stricmp(translationBoneSpace->Value(),"LOCALTRANSLATION")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;     
		  return false;
	  }

	  float txBoneSpace, tyBoneSpace, tzBoneSpace;
	  
	  node = translationBoneSpace->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  TiXmlText* translationBoneSpacedata = node->ToText();
	  if(!translationBoneSpacedata)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  str.clear();
	  str << translationBoneSpacedata->Value();
	  str >> txBoneSpace >> tyBoneSpace >> tzBoneSpace;

      // get the bone space rotation of the bone

	  TiXmlElement* rotationBoneSpace = translationBoneSpace->NextSiblingElement();
	  if(!rotationBoneSpace || stricmp(rotationBoneSpace->Value(),"LOCALROTATION")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;      
		  return false;
	  }

	  float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
	  
	  node = rotationBoneSpace->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  TiXmlText* rotationBoneSpacedata = node->ToText();
	  if(!rotationBoneSpacedata)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  str.clear();
	  str << rotationBoneSpacedata->Value();
	  str >> rxBoneSpace >> ryBoneSpace >> rzBoneSpace >> rwBoneSpace;
	  
	  // get the parent bone id

	  TiXmlElement* parent = rotationBoneSpace->NextSiblingElement();
	  if(!parent ||stricmp(parent->Value(),"PARENTID")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;       
		  return false;
	  }
	  

	  int parentId;
	  
	  node = parent->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  TiXmlText* parentid = node->ToText();
  	  if(!parentid)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
	  }
	  parentId = atoi(parentid->Value());

	  // allocate a new core bone instance
	  CalCoreBone *pCoreBone;
	  pCoreBone = new CalCoreBone();
	  if(pCoreBone == 0)
	  {
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
		  CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		  return 0;
	  }

	  // create the core bone instance
	  if(!pCoreBone->create(strName))
	  {
		  pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
		  delete pCoreBone;
		  return 0;
	  }

	  // set the parent of the bone
	  pCoreBone->setParentId(parentId);

	  // set all attributes of the bone
	  pCoreBone->setTranslation(CalVector(tx, ty, tz));
	  pCoreBone->setRotation(CalQuaternion(rx, ry, rz, rw));
	  pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
	  pCoreBone->setRotationBoneSpace(CalQuaternion(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace));


	  TiXmlElement* child;
	  for( child = parent->NextSiblingElement();child;child = child->NextSiblingElement() )
	  {
		  if(stricmp(child->Value(),"CHILDID")!=0)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreSkeleton->destroy();
			  delete pCoreSkeleton;
			  pCoreBone->destroy();
			  delete pCoreBone;
			  return false;
		  }

		  TiXmlNode *node= child->FirstChild();
		  if(!node)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreSkeleton->destroy();
			  delete pCoreSkeleton;
			  pCoreBone->destroy();
			  delete pCoreBone;
			  return false;
		  }
		  TiXmlText* childid = node->ToText();
		  if(!childid)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreSkeleton->destroy();
			  delete pCoreSkeleton;
			  pCoreBone->destroy();
			  delete pCoreBone;
			  return false;
		  }
		  
		  int childId = atoi(childid->Value());
		  
		  pCoreBone->addChildId(childId);
	  }

	  // set the core skeleton of the core bone instance
    pCoreBone->setCoreSkeleton(pCoreSkeleton);

    // add the core bone to the core skeleton instance
    pCoreSkeleton->addCoreBone(pCoreBone);

	}

	doc.Clear();

	pCoreSkeleton->calculateState();

    return pCoreSkeleton;
}

 /*****************************************************************************/
/** Loads a core animation instance from a XML file.
  *
  * This function loads a core animation instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreAnimation *CalLoader::loadXmlCoreAnimation(const std::string& strFilename)
{
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }
  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_MAGIC)!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return false;
  }

  TiXmlElement*animation = header->NextSiblingElement();
  if(!animation || stricmp(animation->Value(),"ANIMATION")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return false;
  }  

  int trackCount= atoi(animation->Attribute("NUMTRACKS"));
  float duration=atof(animation->Attribute("DURATION"));
    
  // allocate a new core animation instance
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = new CalCoreAnimation();
  if(pCoreAnimation == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core animation instance
  if(!pCoreAnimation->create())
  {
    delete pCoreAnimation;
    return 0;
  }


  // check for a valid duration
  if(duration <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__, strFilename);
    pCoreAnimation->destroy();
    delete pCoreAnimation;
    return 0;
  }

  // set the duration in the core animation instance
  pCoreAnimation->setDuration(duration);
  TiXmlElement* track=animation->FirstChildElement();

  // load all core bones
  int trackId;
  for(trackId = 0; trackId < trackCount; trackId++)
  {
	  if(!track || stricmp(track->Value(),"TRACK")!=0)
	  {
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  pCoreAnimation->destroy();
		  delete pCoreAnimation;
		  return false;
	  }

	  CalCoreTrack *pCoreTrack;

	  pCoreTrack = new CalCoreTrack();
	  if(pCoreTrack == 0)
	  {
		  pCoreAnimation->destroy();
          delete pCoreAnimation;
		  CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		  return 0;
	  }

	  // create the core track instance
	  if(!pCoreTrack->create())
	  {
		  pCoreAnimation->destroy();
          delete pCoreAnimation;
		  delete pCoreTrack;
		  return 0;
	  }

	  int coreBoneId = atoi(track->Attribute("BONEID"));

	  // link the core track to the appropriate core bone instance
	  pCoreTrack->setCoreBoneId(coreBoneId);

	  // read the number of keyframes
	  int keyframeCount= atoi(track->Attribute("NUMKEYFRAMES"));

	  if(keyframeCount <= 0)
	  {
		  pCoreAnimation->destroy();
		  delete pCoreAnimation;
		  pCoreTrack->destroy();
		  delete pCoreTrack;
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  return 0;
	  }

	  TiXmlElement* keyframe= track->FirstChildElement();

	  // load all core keyframes
	  int keyframeId;
	  for(keyframeId = 0; keyframeId < keyframeCount; keyframeId++)
	  {
		  // load the core keyframe
		  if(!keyframe|| stricmp(keyframe->Value(),"KEYFRAME")!=0)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }

		  float time= atof(keyframe->Attribute("TIME"));

		  TiXmlElement* translation = keyframe->FirstChildElement();
		  if(!translation || stricmp(translation->Value(),"TRANSLATION")!=0)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }

		  float tx, ty, tz;

		  node = translation->FirstChild();
		  if(!node)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }

		  TiXmlText* translationdata = node->ToText();
		  if(!translationdata)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }
		  str.clear();
		  str << translationdata->Value();
		  str >> tx >> ty >> tz;  

		  TiXmlElement* rotation = translation->NextSiblingElement();
		  if(!rotation || stricmp(rotation->Value(),"ROTATION")!=0)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }

		  float rx, ry, rz, rw;

		  node = rotation->FirstChild();
		  if(!node)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }
		  TiXmlText* rotationdata = node->ToText();
		  if(!rotationdata)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  return false;
		  }
		  str.clear();
		  str << rotationdata->Value();
		  str >> rx >> ry >> rz >> rw;  
		  
		  // allocate a new core keyframe instance

		  CalCoreKeyframe *pCoreKeyframe;
		  pCoreKeyframe = new CalCoreKeyframe();
		  if(pCoreKeyframe == 0)
		  {
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;			  
			  CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
			  return 0;
		  }

		  // create the core keyframe instance
		  if(!pCoreKeyframe->create())
		  {
			  pCoreAnimation->destroy();
		      delete pCoreAnimation;
		      pCoreTrack->destroy();
		      delete pCoreTrack;
			  delete pCoreKeyframe;
			  return 0;			  
		  }
		  // set all attributes of the keyframe
		  pCoreKeyframe->setTime(time);
		  pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
		  pCoreKeyframe->setRotation(CalQuaternion(rx, ry, rz, rw));

		  // add the core keyframe to the core track instance
         pCoreTrack->addCoreKeyframe(pCoreKeyframe);

		 keyframe = keyframe->NextSiblingElement();

	  }
      
	  pCoreAnimation->addCoreTrack(pCoreTrack);	  
	  track=track->NextSiblingElement();
  }

  // explicitly close the file
  doc.Clear();

  return pCoreAnimation;
}

 /*****************************************************************************/
/** Loads a core mesh instance from a Xml file.
  *
  * This function loads a core mesh instance from a Xml file.
  *
  * @param strFilename The name of the file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMesh *CalLoader::loadXmlCoreMesh(const std::string& strFilename)
{

  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }
  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::MESH_XMLFILE_MAGIC)!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return false;
  }

  TiXmlElement*mesh = header->NextSiblingElement();
  if(!mesh || stricmp(mesh->Value(),"MESH")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return false;
  }

  // get the number of submeshes
  int submeshCount = atoi(mesh->Attribute("NUMSUBMESH"));
  
  // allocate a new core mesh instance
  CalCoreMesh *pCoreMesh;
  pCoreMesh = new CalCoreMesh();
  if(pCoreMesh == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core mesh instance
  if(!pCoreMesh->create())
  {
    delete pCoreMesh;
    return 0;
  }

  TiXmlElement*submesh = mesh->FirstChildElement();

  // load all core submeshes
  int submeshId;
  for(submeshId = 0; submeshId < submeshCount; submeshId++)
  {
	  if(!submesh || stricmp(submesh->Value(),"SUBMESH")!=0)
	  {
		  pCoreMesh->destroy();
		  delete pCoreMesh;
		  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		  return false;
	  }

	  // get the material thread id of the submesh
	  int coreMaterialThreadId = atoi(submesh->Attribute("MATERIAL"));

	  // get the number of vertices, faces, level-of-details and springs
	  int vertexCount = atoi(submesh->Attribute("NUMVERTICES"));

	  int faceCount = atoi(submesh->Attribute("NUMFACES"));

	  int lodCount = atoi(submesh->Attribute("NUMLODSTEPS"));

	  int springCount = atoi(submesh->Attribute("NUMSPRINGS"));

	  int textureCoordinateCount = atoi(submesh->Attribute("NUMTEXCOORDS"));

	  // allocate a new core submesh instance
	  CalCoreSubmesh *pCoreSubmesh;
	  pCoreSubmesh = new CalCoreSubmesh();
	  if(pCoreSubmesh == 0)
	  {
		  CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		  pCoreMesh->destroy();
		  delete pCoreMesh;
		  return 0;
	  }

	  // create the core submesh instance
	  if(!pCoreSubmesh->create())
	  {
		  pCoreMesh->destroy();
		  delete pCoreMesh;
		  delete pCoreSubmesh;
		  return 0;
	  }

	  // set the LOD step count
	  pCoreSubmesh->setLodCount(lodCount);

	  // set the core material id
	  pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

	  // reserve memory for all the submesh data
	  if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
	  {
		  CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__, strFilename);
		  pCoreMesh->destroy();
		  delete pCoreMesh;
		  pCoreSubmesh->destroy();
		  delete pCoreSubmesh;
		  return 0;
	  }

	  TiXmlElement *vertex = submesh->FirstChildElement();
	  
	  // load all vertices and their influences
	  int vertexId;
	  for(vertexId = 0; vertexId < vertexCount; vertexId++)
	  {
		  if(!vertex || stricmp(vertex->Value(),"VERTEX")!=0)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }		  

		  CalCoreSubmesh::Vertex Vertex;

		  TiXmlElement *pos= vertex->FirstChildElement();
		  if(!pos || stricmp(pos->Value(),"POS")!=0)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  
		  node = pos->FirstChild();
		  if(!node)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }		  
		  TiXmlText* posdata = node->ToText();
		  if(!posdata)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  str.clear();
		  str << posdata->Value();
		  str >> Vertex.position.x >> Vertex.position.y >> Vertex.position.z;  

		  TiXmlElement *norm= pos->NextSiblingElement();
		  if(!norm||stricmp(norm->Value(),"NORM")!=0)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  
		  node = norm->FirstChild();
		  if(!norm)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  TiXmlText* normdata = node->ToText();
		  if(!normdata)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  str.clear();
		  str << normdata->Value();
		  str >> Vertex.normal.x >> Vertex.normal.y >> Vertex.normal.z;

		  TiXmlElement *collapse= norm->NextSiblingElement();
		  if(!collapse)
		  {
		      pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
		      delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  if(stricmp(collapse->Value(),"COLLAPSEID")==0)
		  {
			  node = collapse->FirstChild();
			  if(!node)
			  {
		          pCoreMesh->destroy();
			      delete pCoreMesh;
			      pCoreSubmesh->destroy();
		          delete pCoreSubmesh;
			      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			      return false;
			  }
			  TiXmlText* collapseid = node->ToText();
			  if(!collapseid)
			  {
		          pCoreMesh->destroy();
			      delete pCoreMesh;
			      pCoreSubmesh->destroy();
		          delete pCoreSubmesh;
			      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			      return false;
			  }
			  Vertex.collapseId = atoi(normdata->Value());

			  TiXmlElement *collapseCount= collapse->NextSiblingElement();
			  if(!collapseCount|| stricmp(collapseCount->Value(),"COLLAPSECOUNT")!=0)
			  {
				  pCoreMesh->destroy();
			      delete pCoreMesh;
				  pCoreSubmesh->destroy();
			      delete pCoreSubmesh;
			      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			      return false;
			  }

			  node = collapseCount->FirstChild();
			  if(!node)
			  {
		          pCoreMesh->destroy();
			      delete pCoreMesh;
			      pCoreSubmesh->destroy();
		          delete pCoreSubmesh;
			      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			      return false;
			  }
			  TiXmlText* collapseCountdata = node->ToText();
			  if(!collapseCountdata)
			  {
		          pCoreMesh->destroy();
			      delete pCoreMesh;
			      pCoreSubmesh->destroy();
		          delete pCoreSubmesh;
			      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			      return false;
			  }
			  Vertex.faceCollapseCount= atoi(collapseCountdata->Value());
			  collapse = collapseCount->NextSiblingElement();			  
		  }
		  else
		  {
			  Vertex.collapseId=-1;
			  Vertex.faceCollapseCount=0;
		  }


		  TiXmlElement *texcoord = collapse;

		  // load all texture coordinates of the vertex
		  int textureCoordinateId;
		  for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; textureCoordinateId++)
		  {
			  CalCoreSubmesh::TextureCoordinate textureCoordinate;
			  // load data of the influence
			  if(!texcoord || stricmp(texcoord->Value(),"TEXCOORD")!=0)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }

			  node = texcoord->FirstChild();
			  if(!node)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }
			  TiXmlText* texcoorddata = node->ToText();
			  if(!texcoorddata)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }
			  str.clear();
			  str << texcoorddata->Value();
			  str >> textureCoordinate.u >> textureCoordinate.v;

			  // set texture coordinate in the core submesh instance
			  pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
			  texcoord = texcoord->NextSiblingElement();
		  }

		  // get the number of influences
		  int influenceCount= atoi(vertex->Attribute("NUMINFLUENCES"));

		  if(influenceCount < 0)
		  {
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  pCoreSubmesh->destroy();
			  pCoreMesh->destroy();
			  delete pCoreMesh;
			  delete pCoreSubmesh;
			  return 0;
		  }

		  // reserve memory for the influences in the vertex
		  Vertex.vectorInfluence.reserve(influenceCount);
		  Vertex.vectorInfluence.resize(influenceCount);

		  TiXmlElement *influence = texcoord;
		  
		  // load all influences of the vertex
		  int influenceId;
		  for(influenceId = 0; influenceId < influenceCount; influenceId++)
		  {
			  if(!influence ||stricmp(influence->Value(),"INFLUENCE")!=0)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }

			  node = influence->FirstChild();
			  if(!node)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }
			  TiXmlText* influencedata = node->ToText();
			  if(!influencedata)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }

			  Vertex.vectorInfluence[influenceId].boneId = atoi(influence->Attribute("ID"));

			  Vertex.vectorInfluence[influenceId].weight = atof(influencedata->Value());

			  influence=influence->NextSiblingElement();	  
		  }

		  // set vertex in the core submesh instance
		  pCoreSubmesh->setVertex(vertexId, Vertex);

		  TiXmlElement *physique = influence;

			  

		  // load the physical property of the vertex if there are springs in the core submesh
		  if(springCount > 0)
		  {
			  CalCoreSubmesh::PhysicalProperty physicalProperty;
			  
			  if(!physique || stricmp(physique->Value(),"PHYSIQUE")!=0)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }
			  node = physique->FirstChild();
			  if(!node)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }
			  TiXmlText* physiquedata = node->ToText();
			  if(!physiquedata)
			  {
				  pCoreMesh->destroy();
				  delete pCoreMesh;
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  return false;
			  }

			  physicalProperty.weight = atof(physiquedata->Value());

			  // set the physical property in the core submesh instance
			  pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);				  
			  
		  }


		  vertex = vertex->NextSiblingElement();
	 }

	 TiXmlElement *spring= vertex;

	 // load all springs
	 int springId;
	 for(springId = 0; springId < springCount; springId++)
	 {
		  CalCoreSubmesh::Spring Spring;
		  if(!spring ||stricmp(spring->Value(),"SPRING")!=0)
		  {
			  pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
			  delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }
		  str.clear();
		  str << spring->Attribute("VERTEXID");
		  str >> Spring.vertexId[0] >> Spring.vertexId[1];
		  Spring.springCoefficient = atof(spring->Attribute("COEF"));
		  Spring.idleLength = atof(spring->Attribute("LENGTH"));

		  // set spring in the core submesh instance
		  pCoreSubmesh->setSpring(springId, Spring);
		  spring = spring->NextSiblingElement();
	 }

 	 TiXmlElement *face = spring;

	 // load all faces
	 int faceId;
	 for(faceId = 0; faceId < faceCount; faceId++)
	 {
		  CalCoreSubmesh::Face Face;

		  if(!face || stricmp(face->Value(),"FACE")!=0)
		  {
			  pCoreMesh->destroy();
			  delete pCoreMesh;
			  pCoreSubmesh->destroy();
			  delete pCoreSubmesh;
			  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			  return false;
		  }

		  int tmp[3];

		  // load data of the face

		  str.clear();
		  str << face->Attribute("VERTEXID");
		  str >> tmp[0] >> tmp [1] >> tmp[2];

		  if(sizeof(CalIndex)==2)
		  {
			  if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
			  {
				  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				  pCoreSubmesh->destroy();
				  delete pCoreSubmesh;
				  return 0;
			  }
		  }
		  Face.vertexId[0]=tmp[0];
		  Face.vertexId[1]=tmp[1];
		  Face.vertexId[2]=tmp[2];

		  pCoreSubmesh->setFace(faceId, Face);

		  face=face->NextSiblingElement();
	  }
	  submesh=submesh->NextSiblingElement();
	
	  // add the core submesh to the core mesh instance
      pCoreMesh->addCoreSubmesh(pCoreSubmesh);

  }


// explicitly close the file
doc.Clear();
return pCoreMesh;
}



 /*****************************************************************************/
/** Loads a core material instance from a XML file.
  *
  * This function loads a core material instance from a XML file.
  *
  * @param strFilename The name of the file to load the core material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happend
  *****************************************************************************/


CalCoreMaterial *CalLoader::loadXmlCoreMaterial(const std::string& strFilename)
{
  std::stringstream str;
  int r,g,b,a;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }
  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_MAGIC)!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return false;
  }

  TiXmlElement*material = header->NextSiblingElement();
  if(!material||stricmp(material->Value(),"MATERIAL")!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return false;
  }
  

  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = new CalCoreMaterial();
  if(pCoreMaterial == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core material instance
  if(!pCoreMaterial->create())
  {
    delete pCoreMaterial;
    return 0;
  }

  TiXmlElement* ambient = material->FirstChildElement();
  if(!ambient ||stricmp(ambient->Value(),"AMBIENT")!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }

  CalCoreMaterial::Color ambientColor; 
  node = ambient->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  TiXmlText* ambientdata = node->ToText();
  if(!ambientdata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  str << ambientdata->Value();
  str >> r >> g >> b >> a;
  ambientColor.red = (unsigned char)r;
  ambientColor.green = (unsigned char)g;
  ambientColor.blue = (unsigned char)b;
  ambientColor.alpha = (unsigned char)a; 

  TiXmlElement* diffuse = ambient->NextSiblingElement();
  if(!diffuse || stricmp(diffuse->Value(),"DIFFUSE")!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }

  CalCoreMaterial::Color diffuseColor; 
  node = diffuse->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  TiXmlText* diffusedata = node->ToText();
  if(!diffusedata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  str.clear();
  str << diffusedata->Value();
  str >> r >> g >> b >> a;
  diffuseColor.red = (unsigned char)r;
  diffuseColor.green = (unsigned char)g;
  diffuseColor.blue = (unsigned char)b;
  diffuseColor.alpha = (unsigned char)a;
  

  TiXmlElement* specular = diffuse->NextSiblingElement();
  if(!specular||stricmp(specular->Value(),"SPECULAR")!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }

  CalCoreMaterial::Color specularColor; 
  node = specular->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  TiXmlText* speculardata = node->ToText();
  if(!speculardata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  str.clear();
  str << speculardata->Value();
  str >> r >> g >> b >> a;
  specularColor.red = (unsigned char)r;
  specularColor.green = (unsigned char)g;
  specularColor.blue = (unsigned char)b;
  specularColor.alpha = (unsigned char)a;


  TiXmlElement* shininess = specular->NextSiblingElement();
  if(!shininess||stricmp(shininess->Value(),"SHININESS")!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }

  float fshininess;
  node = shininess->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  TiXmlText* shininessdata = node->ToText();
  if(!shininessdata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
  }
  fshininess = atof(shininessdata->Value());

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(fshininess);
  
  std::vector<std::string> MatFileName;

  TiXmlElement* map;

  for( map = shininess->NextSiblingElement();map;map = map->NextSiblingElement() )
  {
	  if(!map||stricmp(map->Value(),"MAP")!=0)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
	  }
	  

	  node= map->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
	  }

	  TiXmlText* mapfile = node->ToText();
	  if(!mapfile)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return false;
	  }

	  MatFileName.push_back(mapfile->Value());
  }

  pCoreMaterial->reserve(MatFileName.size());



  for(unsigned int mapId=0;mapId<MatFileName.size();mapId++)
  {
	  CalCoreMaterial::Map Map;
	  // initialize the user data
	  Map.userData = 0;

	  Map.strFilename= MatFileName[mapId];	  

	  // set map in the core material instance
	  pCoreMaterial->setMap(mapId, Map);
  }

  doc.Clear();
  
  return pCoreMaterial;
}
#endif





//****************************************************************************//

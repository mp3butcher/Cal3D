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
  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  unsigned int magic;
  file.read((char *)&magic, sizeof(magic));
  if(!file || (memcmp(&magic, Cal::ANIMATION_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  unsigned int version;
  file.read((char *)&version, sizeof(version));
  if(!file || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
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
  file.read((char *)&duration, 4);
  if(!file)
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
  file.read((char *)&trackCount, 4);
  if(!file || (trackCount <= 0))
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

  // get the name length of the bone
  int len;
  file.read((char *)&len, 4);
  if(len < 1)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // read the name of the bone
  char *strBuffer;
  strBuffer = new char[len];
  file.read(strBuffer, len);
  std::string strName;
  strName = strBuffer;
  delete [] strBuffer;

  // get the translation of the bone
  float tx, ty, tz;
  file.read((char *)&tx, 4);
  file.read((char *)&ty, 4);
  file.read((char *)&tz, 4);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  file.read((char *)&rx, 4);
  file.read((char *)&ry, 4);
  file.read((char *)&rz, 4);
  file.read((char *)&rw, 4);

  // get the bone space translation of the bone
  float txBoneSpace, tyBoneSpace, tzBoneSpace;
  file.read((char *)&txBoneSpace, 4);
  file.read((char *)&tyBoneSpace, 4);
  file.read((char *)&tzBoneSpace, 4);

  // get the bone space rotation of the bone
  float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
  file.read((char *)&rxBoneSpace, 4);
  file.read((char *)&ryBoneSpace, 4);
  file.read((char *)&rzBoneSpace, 4);
  file.read((char *)&rwBoneSpace, 4);

  // get the parent bone id
  int parentId;
  file.read((char *)&parentId, 4);

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
  file.read((char *)&childCount, 4);
  if(!file || (childCount < 0))
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
    file.read((char *)&childId, 4);
    if(!file || (childId < 0))
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
  file.read((char *)&time, 4);

  // get the translation of the bone
  float tx, ty, tz;
  file.read((char *)&tx, 4);
  file.read((char *)&ty, 4);
  file.read((char *)&tz, 4);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  file.read((char *)&rx, 4);
  file.read((char *)&ry, 4);
  file.read((char *)&rz, 4);
  file.read((char *)&rw, 4);

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
  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  unsigned int magic;
  file.read((char *)&magic, sizeof(magic));
  if(!file || (memcmp(&magic, Cal::MATERIAL_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  unsigned int version;
  file.read((char *)&version, sizeof(version));
  if(!file || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
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
  file.read((char *)&ambientColor.red, 1);
  file.read((char *)&ambientColor.green, 1);
  file.read((char *)&ambientColor.blue, 1);
  file.read((char *)&ambientColor.alpha, 1);

  // get the diffuse color of the core material
  CalCoreMaterial::Color diffuseColor;
  file.read((char *)&diffuseColor.red, 1);
  file.read((char *)&diffuseColor.green, 1);
  file.read((char *)&diffuseColor.blue, 1);
  file.read((char *)&diffuseColor.alpha, 1);

  // get the specular color of the core material
  CalCoreMaterial::Color specularColor;
  file.read((char *)&specularColor.red, 1);
  file.read((char *)&specularColor.green, 1);
  file.read((char *)&specularColor.blue, 1);
  file.read((char *)&specularColor.alpha, 1);

  // get the shininess factor of the core material
  float shininess;
  file.read((char *)&shininess, 4);

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
  file.read((char *)&mapCount, 4);
  if(!file)
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

    // get the filename length of the map
    int len;
    file.read((char *)&len, 4);
    if(len < 1)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreMaterial->destroy();
      delete pCoreMaterial;
      return 0;
    }

    // read the filename of the map
    char *strBuffer;
    strBuffer = new char[len];
    file.read(strBuffer, len);
    map.strFilename = strBuffer;
    delete [] strBuffer;

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
  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  unsigned int magic;
  file.read((char *)&magic, sizeof(magic));
  if(!file || (memcmp(&magic, Cal::MESH_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  unsigned int version;
  file.read((char *)&version, sizeof(version));
  if(!file || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // get the number of submeshes
  int submeshCount;
  file.read((char *)&submeshCount, 4);

  // check if an error happend
  if(!file)
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
  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if this is a valid file
  unsigned int magic;
  file.read((char *)&magic, sizeof(magic));
  if(!file || (memcmp(&magic, Cal::SKELETON_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // check if the version is compatible with the library
  unsigned int version;
  file.read((char *)&version, sizeof(version));
  if(!file || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // read the number of bones
  int boneCount;
  file.read((char *)&boneCount, 4);
  if(!file || (boneCount <= 0))
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
  file.read((char *)&coreMaterialThreadId, 4);

  // get the number of vertices, faces, level-of-details and springs
  int vertexCount;
  file.read((char *)&vertexCount, 4);
  int faceCount;
  file.read((char *)&faceCount, 4);
  int lodCount;
  file.read((char *)&lodCount, 4);
  int springCount;
  file.read((char *)&springCount, 4);

  // get the number of texture coordinates per vertex
  int textureCoordinateCount;
  file.read((char *)&textureCoordinateCount, 4);

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
    file.read((char *)&vertex.position.x, 4);
    file.read((char *)&vertex.position.y, 4);
    file.read((char *)&vertex.position.z, 4);
    file.read((char *)&vertex.normal.x, 4);
    file.read((char *)&vertex.normal.y, 4);
    file.read((char *)&vertex.normal.z, 4);
    file.read((char *)&vertex.collapseId, 4);
    file.read((char *)&vertex.faceCollapseCount, 4);

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
      file.read((char *)&textureCoordinate.u, 4);
      file.read((char *)&textureCoordinate.v, 4);

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
    file.read((char *)&influenceCount, 4);

    // check if an error happend
    if(!file)
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
      file.read((char *)&vertex.vectorInfluence[influenceId].boneId, 4);
      file.read((char *)&vertex.vectorInfluence[influenceId].weight, 4);

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
      file.read((char *)&physicalProperty.weight, 4);

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
    file.read((char *)&spring.vertexId[0], 4);
    file.read((char *)&spring.vertexId[1], 4);
    file.read((char *)&spring.springCoefficient, 4);
    file.read((char *)&spring.idleLength, 4);

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
    file.read((char *)&face.vertexId[0], 4);
    file.read((char *)&face.vertexId[1], 4);
    file.read((char *)&face.vertexId[2], 4);

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
  file.read((char *)&coreBoneId, 4);
  if(!file || (coreBoneId < 0))
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
  file.read((char *)&keyframeCount, 4);
  if(!file || (keyframeCount <= 0))
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

//****************************************************************************//

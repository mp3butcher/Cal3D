//****************************************************************************//
// saver.cpp                                                                  //
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

#include "saver.h"
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
/** Constructs the saver instance.
  *
  * This function is the default constructor of the saver instance.
  *****************************************************************************/

CalSaver::CalSaver()
{
}

 /*****************************************************************************/
/** Destructs the saver instance.
  *
  * This function is the destructor of the saver instance.
  *****************************************************************************/

CalSaver::~CalSaver()
{
}

 /*****************************************************************************/
/** Saves a core animation instance.
  *
  * This function saves a core animation instance to a file.
  *
  * @param strFilename The name of the file to save the core animation instance
  *                    to.
  * @param pCoreAnimation A pointer to the core animation instance that should
  *                       be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreAnimation(const std::string& strFilename, CalCoreAnimation *pCoreAnimation)
{
  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::ANIMATION_FILE_MAGIC, sizeof(Cal::ANIMATION_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the duration of the core animation
  if(!CalPlatform::writeFloat(file, pCoreAnimation->getDuration()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get core track list
  std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();

  // write the number of tracks
  if(!CalPlatform::writeInteger(file, listCoreTrack.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // write all core bones
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // save core track
    if(!saveCoreTrack(file, strFilename, *iteratorCoreTrack))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  return true;
}

 /*****************************************************************************/
/** Saves a core bone instance.
  *
  * This function saves a core bone instance to a file stream.
  *
  * @param file The file stream to save the core bone instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreBone A pointer to the core bone instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreBones(std::ofstream& file, const std::string& strFilename, CalCoreBone *pCoreBone)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the name of the bone
  if(!CalPlatform::writeString(file, pCoreBone->getName()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the translation of the bone
  const CalVector& translation = pCoreBone->getTranslation();
  CalPlatform::writeFloat(file, translation[0]);
  CalPlatform::writeFloat(file, translation[1]);
  CalPlatform::writeFloat(file, translation[2]);

  // write the rotation of the bone
  const CalQuaternion& rotation = pCoreBone->getRotation();
  CalPlatform::writeFloat(file, rotation[0]);
  CalPlatform::writeFloat(file, rotation[1]);
  CalPlatform::writeFloat(file, rotation[2]);
  CalPlatform::writeFloat(file, rotation[3]);

  // write the translation of the bone
  const CalVector& translationBoneSpace = pCoreBone->getTranslationBoneSpace();
  CalPlatform::writeFloat(file, translationBoneSpace[0]);
  CalPlatform::writeFloat(file, translationBoneSpace[1]);
  CalPlatform::writeFloat(file, translationBoneSpace[2]);
  CalPlatform::writeFloat(file, translationBoneSpace[3]);

  // write the rotation of the bone
  const CalQuaternion& rotationBoneSpace = pCoreBone->getRotationBoneSpace();
  CalPlatform::writeFloat(file, rotationBoneSpace[0]);
  CalPlatform::writeFloat(file, rotationBoneSpace[1]);
  CalPlatform::writeFloat(file, rotationBoneSpace[2]);
  CalPlatform::writeFloat(file, rotationBoneSpace[3]);

  // write the parent bone id
  if(!CalPlatform::writeInteger(file, pCoreBone->getParentId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get children list
  std::list<int>& listChildId = pCoreBone->getListChildId();

  // write the number of children
  if(!CalPlatform::writeInteger(file, listChildId.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all children ids
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = listChildId.begin(); iteratorChildId != listChildId.end(); ++iteratorChildId)
  {
    // write the child id
    if(!CalPlatform::writeInteger(file, *iteratorChildId))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core keyframe instance.
  *
  * This function saves a core keyframe instance to a file stream.
  *
  * @param file The file stream to save the core keyframe instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreKeyframe A pointer to the core keyframe instance that should be
  *                      saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreKeyframe(std::ofstream& file, const std::string& strFilename, CalCoreKeyframe *pCoreKeyframe)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the time of the keyframe
  CalPlatform::writeFloat(file, pCoreKeyframe->getTime());

  // write the translation of the keyframe
  const CalVector& translation = pCoreKeyframe->getTranslation();
  CalPlatform::writeFloat(file, translation[0]);
  CalPlatform::writeFloat(file, translation[1]);
  CalPlatform::writeFloat(file, translation[2]);

  // write the rotation of the keyframe
  const CalQuaternion& rotation = pCoreKeyframe->getRotation();
  CalPlatform::writeFloat(file, rotation[0]);
  CalPlatform::writeFloat(file, rotation[1]);
  CalPlatform::writeFloat(file, rotation[2]);
  CalPlatform::writeFloat(file, rotation[3]);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core material instance.
  *
  * This function saves a core material instance to a file.
  *
  * @param strFilename The name of the file to save the core material instance
  *                    to.
  * @param pCoreMaterial A pointer to the core material instance that should
  *                      be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreMaterial(const std::string& strFilename, CalCoreMaterial *pCoreMaterial)
{
  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::MATERIAL_FILE_MAGIC, sizeof(Cal::MATERIAL_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the ambient color
  CalCoreMaterial::Color ambientColor;
  ambientColor = pCoreMaterial->getAmbientColor();
  CalPlatform::writeBytes(file, &ambientColor, sizeof(ambientColor));

  // write the diffuse color
  CalCoreMaterial::Color diffusetColor;
  diffusetColor = pCoreMaterial->getDiffuseColor();
  CalPlatform::writeBytes(file, &diffusetColor, sizeof(diffusetColor));

  // write the specular color
  CalCoreMaterial::Color specularColor;
  specularColor = pCoreMaterial->getSpecularColor();
  CalPlatform::writeBytes(file, &specularColor, sizeof(specularColor));

  // write the shininess factor
  CalPlatform::writeFloat(file, pCoreMaterial->getShininess());

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the map vector
  std::vector<CalCoreMaterial::Map>& vectorMap = pCoreMaterial->getVectorMap();

  // write the number of maps
  if(!CalPlatform::writeInteger(file, vectorMap.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all maps
  int mapId;
  for(mapId = 0; mapId < (int)vectorMap.size(); mapId++)
  {
    CalCoreMaterial::Map& map = vectorMap[mapId];

    // write the filename of the map
    if(!CalPlatform::writeString(file, map.strFilename))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  // explicitly close the file
  file.close();

  return true;
}

 /*****************************************************************************/
/** Saves a core mesh instance.
  *
  * This function saves a core mesh instance to a file.
  *
  * @param strFilename The name of the file to save the core mesh instance to.
  * @param pCoreMesh A pointer to the core mesh instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreMesh(const std::string& strFilename, CalCoreMesh *pCoreMesh)
{
  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::MESH_FILE_MAGIC, sizeof(Cal::MESH_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the submesh vector
  std::vector<CalCoreSubmesh *>& vectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();

  // write the number of submeshes
  if(!CalPlatform::writeInteger(file, vectorCoreSubmesh.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all core submeshes
  int submeshId;
  for(submeshId = 0; submeshId < (int)vectorCoreSubmesh.size(); submeshId++)
  {
    // write the core submesh
    if(!saveCoreSubmesh(file, strFilename, vectorCoreSubmesh[submeshId]))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  return true;
}

 /*****************************************************************************/
/** Saves a core skeleton instance.
  *
  * This function saves a core skeleton instance to a file.
  *
  * @param strFilename The name of the file to save the core skeleton instance
  *                    to.
  * @param pCoreSkeleton A pointer to the core skeleton instance that should be
  *                      saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreSkeleton(const std::string& strFilename, CalCoreSkeleton *pCoreSkeleton)
{
  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::SKELETON_FILE_MAGIC, sizeof(Cal::SKELETON_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the number of bones
  if(!CalPlatform::writeInteger(file, pCoreSkeleton->getVectorCoreBone().size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all core bones
  int boneId;
  for(boneId = 0; boneId < (int)pCoreSkeleton->getVectorCoreBone().size(); boneId++)
  {
    // write the core bone
    if(!saveCoreBones(file, strFilename, pCoreSkeleton->getCoreBone(boneId)))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  return true;
}

 /*****************************************************************************/
/** Saves a core submesh instance.
  *
  * This function saves a core submesh instance to a file stream.
  *
  * @param file The file stream to save the core submesh instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreSubmesh A pointer to the core submesh instance that should be
  *                     saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreSubmesh(std::ofstream& file, const std::string& strFilename, CalCoreSubmesh *pCoreSubmesh)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the core material thread id
  if(!CalPlatform::writeInteger(file, pCoreSubmesh->getCoreMaterialThreadId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the vertex, face, physical property and spring vector
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
  std::vector<CalCoreSubmesh::Face>& vectorFace = pCoreSubmesh->getVectorFace();
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pCoreSubmesh->getVectorPhysicalProperty();
  std::vector<CalCoreSubmesh::Spring>& vectorSpring = pCoreSubmesh->getVectorSpring();

  // write the number of vertices, faces, level-of-details and springs
  CalPlatform::writeInteger(file, vectorVertex.size());
  CalPlatform::writeInteger(file, vectorFace.size());
  CalPlatform::writeInteger(file, pCoreSubmesh->getLodCount());
  CalPlatform::writeInteger(file, pCoreSubmesh->getSpringCount());

  // get the texture coordinate vector vector
  std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorvectorTextureCoordinate = pCoreSubmesh->getVectorVectorTextureCoordinate();

  // write the number of texture coordinates per vertex
  CalPlatform::writeInteger(file, vectorvectorTextureCoordinate.size());

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all vertices
  int vertexId;
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); vertexId++)
  {
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // write the vertex data
    CalPlatform::writeFloat(file, vertex.position.x);
    CalPlatform::writeFloat(file, vertex.position.y);
    CalPlatform::writeFloat(file, vertex.position.z);
    CalPlatform::writeFloat(file, vertex.normal.x);
    CalPlatform::writeFloat(file, vertex.normal.y);
    CalPlatform::writeFloat(file, vertex.normal.z);
    CalPlatform::writeInteger(file, vertex.collapseId);
    CalPlatform::writeInteger(file, vertex.faceCollapseCount);

    // write all texture coordinates of this vertex
    int textureCoordinateId;
    for(textureCoordinateId = 0; textureCoordinateId < (int)vectorvectorTextureCoordinate.size(); textureCoordinateId++)
    {
      CalCoreSubmesh::TextureCoordinate& textureCoordinate = vectorvectorTextureCoordinate[textureCoordinateId][vertexId];

      // write the influence data
      CalPlatform::writeFloat(file, textureCoordinate.u);
      CalPlatform::writeFloat(file, textureCoordinate.v);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }

    // write the number of influences
    if(!CalPlatform::writeInteger(file, vertex.vectorInfluence.size()))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }

     // write all influences of this vertex
    int influenceId;
    for(influenceId = 0; influenceId < (int)vertex.vectorInfluence.size(); influenceId++)
    {
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // write the influence data
      CalPlatform::writeInteger(file, influence.boneId);
      CalPlatform::writeFloat(file, influence.weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }

    // save the physical property of the vertex if there are springs in the core submesh
    if(pCoreSubmesh->getSpringCount() > 0)
    {
      // write the physical property of this vertex if there are springs in the core submesh
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // write the physical property data
      CalPlatform::writeFloat(file, physicalProperty.weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }
  }

  // write all springs
  int springId;
  for(springId = 0; springId < (int)pCoreSubmesh->getSpringCount(); springId++)
  {
    CalCoreSubmesh::Spring& spring = vectorSpring[springId];

    // write the spring data
    CalPlatform::writeInteger(file, spring.vertexId[0]);
    CalPlatform::writeInteger(file, spring.vertexId[1]);
    CalPlatform::writeFloat(file, spring.springCoefficient);
    CalPlatform::writeFloat(file, spring.idleLength);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  // write all faces
  int faceId;
  for(faceId = 0; faceId < (int)vectorFace.size(); faceId++)
  {
    CalCoreSubmesh::Face& face = vectorFace[faceId];

    // write the face data
    CalPlatform::writeInteger(file, face.vertexId[0]);
    CalPlatform::writeInteger(file, face.vertexId[1]);
    CalPlatform::writeInteger(file, face.vertexId[2]);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core track instance.
  *
  * This function saves a core track instance to a file stream.
  *
  * @param file The file stream to save the core track instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreTrack A pointer to the core track instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreTrack(std::ofstream& file, const std::string& strFilename, CalCoreTrack *pCoreTrack)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the bone id
  if(!CalPlatform::writeInteger(file, pCoreTrack->getCoreBoneId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get core keyframe map
  std::map<float, CalCoreKeyframe *>& mapCoreKeyframe = pCoreTrack->getMapCoreKeyframe();

  // read the number of keyframes
  if(!CalPlatform::writeInteger(file, mapCoreKeyframe.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // save all core keyframes
  std::map<float, CalCoreKeyframe *>::iterator iteratorCoreKeyframe;
  for(iteratorCoreKeyframe = mapCoreKeyframe.begin(); iteratorCoreKeyframe != mapCoreKeyframe.end(); ++iteratorCoreKeyframe)
  {
    // save the core keyframe
    if(!saveCoreKeyframe(file, strFilename, iteratorCoreKeyframe->second))
    {
      return false;
    }
  }

  return true;
}

//****************************************************************************//

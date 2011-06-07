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

#include "cal3d/loader.h"
#include "cal3d/error.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"
#include "cal3d/corematerial.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/coretrack.h"
#include "cal3d/tinyxml.h"
#include "cal3d/streamsource.h"
#include "cal3d/buffersource.h"
#include "cal3d/xmlformat.h"
#include <memory>
using namespace cal3d;

#include "cal3d/calxmlbindings.h"

int CalLoader::loadingMode;
double CalLoader::translationTolerance = 0.25;
double CalLoader::rotationToleranceDegrees = 0.1;
bool CalLoader::loadingCompressionOn = false;
bool CalLoader::collapseSequencesOn = false;
int CalLoader::numEliminatedKeyframes = 0;
int CalLoader::numKeptKeyframes = 0;
int CalLoader::numCompressedAnimations = 0;
int CalLoader::numRoundedKeyframes = 0;


// Quat format: 
//
//  axis selection (2), 
//  asign (1), afixed (n), 
//  bsign (1), bfixed (n), 
//  csign (1), cfixed (n), 
//  time (in 1/30th second steps)
unsigned int const CalLoader::keyframeBitsPerOriComponent = 11;
unsigned int const CalLoader::keyframeBitsPerTime = 10;
//unsigned int const keyframeTimeMax = ( 1 << CalLoader::keyframeBitsPerTime );

// My resolution is in 1/4 units which is 1/2mm if 1 unit is 2mm.
// My large range is 2^23 units, or 2^24mm, which is 2^4km, or 16km.
// My small range is 2^7 units, or 2^8mm, which is 2.56m.
// The large pos has X, Y, Z, having 25 bits plus a sign bit, totaling 78 bits, leaving two bits padding.
// The small pos has X, Y, Z, having 9 bits plus a sign bit, totaling 30 bits, leaving two bits padding.
unsigned int const CalLoader::keyframeBitsPerUnsignedPosComponent = 25;
unsigned int const CalLoader::keyframeBitsPerPosPadding = 2;
unsigned int const CalLoader::keyframePosBytes = 10;
float const CalLoader::keyframePosRange = ( 1 << ( CalLoader::keyframeBitsPerUnsignedPosComponent - 2 ) ); // In units.

unsigned int const CalLoader::keyframeBitsPerUnsignedPosComponentSmall = 9;
unsigned int const CalLoader::keyframeBitsPerPosPaddingSmall = 2;
unsigned int const CalLoader::keyframePosBytesSmall = 4;
float const CalLoader::keyframePosRangeSmall = ( 1 << ( CalLoader::keyframeBitsPerUnsignedPosComponentSmall - 2 ) );




bool CAL3D_API CalVectorFromDataSrc( CalDataSource & dataSrc, CalVector * calVec )
{
   return dataSrc.readFloat(calVec->x) &&
      dataSrc.readFloat(calVec->y) &&
      dataSrc.readFloat(calVec->z);
}


bool
TranslationWritten( CalCoreKeyframe * lastCoreKeyframe, bool translationRequired, bool translationIsDynamic )
{
   return ( translationRequired && ( !lastCoreKeyframe || translationIsDynamic ) );
}

 /*****************************************************************************/
/** Sets optional flags which affect how the model is loaded into memory.
  *
  * This function sets the loading mode for all future loader calls.
  *
  * @param flags A boolean OR of any of the following flags
  *         \li LOADER_ROTATE_X_AXIS will rotate the mesh 90 degrees about the X axis,
  *             which has the effect of swapping Y/Z coordinates.
  *         \li LOADER_INVERT_V_COORD will substitute (1-v) for any v texture coordinate
  *             to eliminate the need for texture inversion after export.
  *
  *****************************************************************************/
void CalLoader::setLoadingMode(int flags)
{
    loadingMode = flags;
}

 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a file.
  *
  * @param strFilename The file to load the core animation instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel)
{
  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC)==0)
    return loadXmlCoreAnimation(strFilename, skel);

  // open the file
  std::ifstream file(strFilename.c_str(), std::ios::in | std::ios::binary);

  //make sure it was opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the animation
  CalStreamSource streamSrc( file );
  
  CalCoreAnimationPtr coreanim = loadCoreAnimation( streamSrc,skel );
  if(coreanim) coreanim->setFilename( strFilename );

  //close the file
  file.close();

  return coreanim;
}

/*****************************************************************************/
/** Loads a core animatedMorph instance.
*
* This function loads a core animatedMorph instance from a file.
*
* @param strFilename The file to load the core animatedMorph instance from.
*
* @return One of the following values:
*         \li a pointer to the core animatedMorph
*         \li \b 0 if an error happened
*****************************************************************************/

CalCoreAnimatedMorph *CalLoader::loadCoreAnimatedMorph(const std::string& strFilename)
{
   if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATEDMORPH_XMLFILE_EXTENSION)==0)
      return loadXmlCoreAnimatedMorph(strFilename);
   else
      return loadCoreAnimatedMorph(strFilename);
}


 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a file.
  *
  * @param strFilename The file to load the core material instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(const std::string& strFilename)
{

  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC)==0)
    return loadXmlCoreMaterial(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  // make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the material
  CalStreamSource streamSrc( file );
  
  CalCoreMaterialPtr coremat = loadCoreMaterial( streamSrc );

  if(coremat) coremat->setFilename( strFilename );

  //close the file
  file.close();

  return coremat;

}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a file.
  *
  * @param strFilename The file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(const std::string& strFilename)
{

  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC)==0)
    return loadXmlCoreMesh(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  // make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the mesh
  CalStreamSource streamSrc( file );
  
  CalCoreMeshPtr coremesh = loadCoreMesh( streamSrc );

  if(coremesh) coremesh->setFilename( strFilename );


  //close the file
  file.close();

  return coremesh;

}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a file.
  *
  * @param strFilename The file to load the core skeleton instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(const std::string& strFilename)
{

  if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC)==0)
    return loadXmlCoreSkeleton(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  //make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the skeleton
  CalStreamSource streamSrc( file );
  
  CalCoreSkeletonPtr coreskeleton = loadCoreSkeleton( streamSrc );

  //close the file
  file.close();

  return coreskeleton;

}


 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from an input stream.
  *
  * @param inputStream The stream to load the core animation instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(std::istream& inputStream, CalCoreSkeleton *skel )
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreAnimation(streamSrc, skel);
}

CalCoreAnimatedMorph *CalLoader::loadCoreAnimatedMorph(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreAnimatedMorph(streamSrc);
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from an input stream.
  *
  * @param inputStream The stream to load the core material instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreMaterial(streamSrc);
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from an input stream.
  *
  * @param inputStream The stream to load the core mesh instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreMesh(streamSrc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from an input stream.
  *
  * @param inputStream The stream to load the core skeleton instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreSkeleton(streamSrc);
}




 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core animation instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(void* inputBuffer, CalCoreSkeleton *skel)
{
  if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<ANIMATION", 10 ) == 0) )
  {
    TiXmlDocument doc;
    doc.Parse( static_cast<const char*>(inputBuffer) );
    if (doc.Error())
    {
        CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
       return 0;
    }
    return loadXmlCoreAnimation( doc, skel );
  }

   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreAnimation(bufferSrc,skel);
}

/*****************************************************************************/
/** Loads a core animatedMorph instance.
*
* This function loads a core animatedMorph instance from a memory buffer.
*
* @param inputBuffer The memory buffer to load the core animatedMorph instance 
*                    from. This buffer should be initialized and ready to 
*                    be read from.
*
* @return One of the following values:
*         \li a pointer to the core animatedMorph
*         \li \b 0 if an error happened
*****************************************************************************/

CalCoreAnimatedMorph * CalLoader::loadCoreAnimatedMorphFromBuffer(void* inputBuffer, unsigned int len)
{
   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   CalCoreAnimatedMorph * result = loadCoreAnimatedMorph(bufferSrc);
   if( result ) {
      return result;
   } else {
      return loadXmlCoreAnimatedMorph(inputBuffer);
   }

}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core material instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<MATERIAL", 9 ) == 0) )
	{
		cal3d::TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreMaterial( doc );
	}
	
   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreMaterial(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core mesh instance from. 
  *                    This buffer should be initialized and ready to be 
  *                    read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<MESH", 5 ) == 0) )
	{
		TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreMesh( doc );
	}

   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreMesh(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core skeleton instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<SKELETON", 9 ) == 0) )
	{
		TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreSkeleton( doc );
	}

   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreSkeleton(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a data source.
  *
  * @param dataSrc The data source to load the core animation instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(CalDataSource& dataSrc, CalCoreSkeleton *skel)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::ANIMATION_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  bool useAnimationCompression = CalLoader::usesAnimationCompression(version);
  if (Cal::versionHasCompressionFlag(version)) {
     int compressionFlag = 0;
     if (!dataSrc.readInteger(compressionFlag)) {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
        return 0;
     }
     // Only really need the first bit.
     useAnimationCompression = (compressionFlag != 0);
  }


  // allocate a new core animation instance
  CalCoreAnimationPtr pCoreAnimation(new(std::nothrow) CalCoreAnimation);
  if(!pCoreAnimation)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // get the duration of the core animation
  float duration;
  if(!dataSrc.readFloat(duration))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check for a valid duration
  if(duration <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__);
    return 0;
  }

  // set the duration in the core animation instance
  pCoreAnimation->setDuration(duration);

  // read the number of tracks
  int trackCount;
  if(!dataSrc.readInteger(trackCount) || (trackCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

	// read flags
	int flags = 0;
	if(version >= LIBRARY_VERSION) {
	  if(!dataSrc.readInteger(flags))
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
			return 0;
		}
	}

  // load all core bones
  int trackId;
  for(trackId = 0; trackId < trackCount; ++trackId)
  {
    // load the core track
    CalCoreTrack *pCoreTrack;
    pCoreTrack = loadCoreTrack(dataSrc,skel, version, useAnimationCompression);
    if(pCoreTrack == 0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    // add the core track to the core animation instance
    pCoreAnimation->addCoreTrack(pCoreTrack);
  }

  return pCoreAnimation;
}

void
CalLoader::compressCoreAnimation( CalCoreAnimation * anim, CalCoreSkeleton *skel )
{
   std::list<CalCoreTrack *>& listCoreTrack = anim->getListCoreTrack();
   std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
   for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
   {
      CalCoreTrack *pCoreTrack=*iteratorCoreTrack;
      pCoreTrack->compress( translationTolerance, rotationToleranceDegrees, skel );
   }
}


/*****************************************************************************/
/** Loads a core animatedMorph instance.
*
* This function loads a core animatedMorph instance from a data source.
*
* @param dataSrc The data source to load the core animatedMorph instance from.
*
* @return One of the following values:
*         \li a pointer to the core animatedMorph
*         \li \b 0 if an error happened
*****************************************************************************/
CalCoreAnimatedMorph *CalLoader::loadCoreAnimatedMorph(CalDataSource& dataSrc)
{

   // check if this is a valid file
   char magic[4];
   if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::ANIMATEDMORPH_FILE_MAGIC, 4) != 0))
   {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
   }

   // check if the version is compatible with the library
   int version;
   if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
   {
      CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
      return 0;
   }

   // allocate a new core animatedMorph instance
   CalCoreAnimatedMorph *pCoreAnimatedMorph;
   pCoreAnimatedMorph = new CalCoreAnimatedMorph();
   if(pCoreAnimatedMorph == 0)
   {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return 0;
   }

   // create the core animatedMorph instance
   if(!pCoreAnimatedMorph->create())
   {
      delete pCoreAnimatedMorph;
      return 0;
   }

   // get the duration of the core animatedMorph
   float duration;
   if(!dataSrc.readFloat(duration))
   {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      pCoreAnimatedMorph->destroy();
      delete pCoreAnimatedMorph;
      return 0;
   }

   // check for a valid duration
   if(duration <= 0.0f)
   {
      CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__);
      pCoreAnimatedMorph->destroy();
      delete pCoreAnimatedMorph;
      return 0;
   }

   // set the duration in the core animatedMorph instance
   pCoreAnimatedMorph->setDuration(duration);

   // read the number of tracks
   int trackCount;
   if(!dataSrc.readInteger(trackCount) || (trackCount <= 0))
   {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
   }

   // load all core bones
   int trackId;
   for(trackId = 0; trackId < trackCount; ++trackId)
   {
      // load the core track
      CalCoreMorphTrack *pCoreTrack;
      pCoreTrack = loadCoreMorphTrack(dataSrc);
      if(pCoreTrack == 0)
      {
         pCoreAnimatedMorph->destroy();
         delete pCoreAnimatedMorph;
         return 0;
      }

      // add the core track to the core animatedMorph instance
      pCoreAnimatedMorph->addCoreTrack(pCoreTrack);
   }

   return pCoreAnimatedMorph;
}


 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a data source.
  *
  * @param dataSrc The data source to load the core material instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::MATERIAL_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  bool hasMaterialTypes = (version >= Cal::FIRST_FILE_VERSION_WITH_MATERIAL_TYPES);

  // allocate a new core material instance
  CalCoreMaterialPtr pCoreMaterial = new(std::nothrow) CalCoreMaterial();
  if(!pCoreMaterial)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // get the ambient color of the core material
  CalCoreMaterial::Color ambientColor;
  if( !dataSrc.readBytes(&ambientColor, sizeof(ambientColor)) ) {
     CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
     return NULL;
  }

  // get the diffuse color of the core material
  CalCoreMaterial::Color diffuseColor;
  if( !dataSrc.readBytes(&diffuseColor, sizeof(diffuseColor)) ) {
     CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
     return NULL;
  }

  // get the specular color of the core material
  CalCoreMaterial::Color specularColor;
  if( !dataSrc.readBytes(&specularColor, sizeof(specularColor)) ) {
     CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
     return NULL;
  }

  // get the shininess factor of the core material
  float shininess;
  dataSrc.readFloat(shininess);

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(shininess);

  // read the number of maps
  int mapCount;
  if(!dataSrc.readInteger(mapCount) || (mapCount < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // reserve memory for all the material data
  if(!pCoreMaterial->reserve(mapCount))
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load all maps
  int mapId;
  for(mapId = 0; mapId < mapCount; ++mapId)
  {
    CalCoreMaterial::Map map;

    // read the filename of the map
    std::string strName;
    dataSrc.readString(map.strFilename);

    // if we support map types, read the type of the map
    if( hasMaterialTypes ) {
       dataSrc.readString(map.mapType);
    } else {
       map.mapType = "";
    }

    // initialize the user data
    map.userData = 0;

    // check if an error happened
    if(!dataSrc.ok())
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    // set map in the core material instance
    pCoreMaterial->setMap(mapId, map);
  }

  return pCoreMaterial;
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a data source.
  *
  * @param dataSrc The data source to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::MESH_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  bool hasVertexColors = (version >= Cal::FIRST_FILE_VERSION_WITH_VERTEX_COLORS);
  bool hasMorphTargetsInMorphFiles = (version >= Cal::FIRST_FILE_VERSION_WITH_MORPH_TARGETS_IN_MORPH_FILES);

  // get the number of submeshes
  int submeshCount;
  if(!dataSrc.readInteger(submeshCount))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core mesh instance
  CalCoreMeshPtr pCoreMesh = new(std::nothrow) CalCoreMesh();
  if(!pCoreMesh)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load all core submeshes
  for(int submeshId = 0; submeshId < submeshCount; ++submeshId)
  {
    // load the core submesh
    CalCoreSubmesh *pCoreSubmesh = loadCoreSubmesh(dataSrc, version);
    if(pCoreSubmesh == 0)
    {
      return 0;
    }

    // add the core submesh to the core mesh instance
    pCoreMesh->addCoreSubmesh(pCoreSubmesh);
  }

  return pCoreMesh;
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a data source.
  *
  * @param dataSrc The data source to load the core skeleton instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::SKELETON_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  bool hasNodeLights = (version >= Cal::FIRST_FILE_VERSION_WITH_NODE_LIGHTS);

  // read the number of bones
  int boneCount;
  if(!dataSrc.readInteger(boneCount) || (boneCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core skeleton instance
  CalCoreSkeletonPtr pCoreSkeleton = new(std::nothrow) CalCoreSkeleton();
  if(!pCoreSkeleton)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load the scene ambient
  if( hasNodeLights ) {
     CalVector sceneColor;
     CalVectorFromDataSrc( dataSrc, &sceneColor );
     pCoreSkeleton->setSceneAmbientColor(sceneColor);
  }

  // load all core bones
  for(int boneId = 0; boneId < boneCount; ++boneId)
  {
    // load the core bone
    CalCoreBone *pCoreBone = loadCoreBones(dataSrc, version);
    if(pCoreBone == 0)
    {
      return 0;
    }

    // set the core skeleton of the core bone instance
    pCoreBone->setCoreSkeleton(pCoreSkeleton.get());

    // add the core bone to the core skeleton instance
    pCoreSkeleton->addCoreBone(pCoreBone);

    // add a core skeleton mapping of the bone's name for quick reference later
    pCoreSkeleton->mapCoreBoneName(boneId, pCoreBone->getName());

  }

  // calculate state of the core skeleton
  pCoreSkeleton->calculateState();

  return pCoreSkeleton;
}

 /*****************************************************************************/
/** Loads a core bone instance.
  *
  * This function loads a core bone instance from a data source.
  *
  * @param dataSrc The data source to load the core bone instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreBone *CalLoader::loadCoreBones(CalDataSource& dataSrc, int version)
{
  bool hasNodeLights = (version >= Cal::FIRST_FILE_VERSION_WITH_NODE_LIGHTS);

  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // read the name of the bone
  std::string strName;
  dataSrc.readString(strName);

  // get the translation of the bone
  float tx, ty, tz;
  dataSrc.readFloat(tx);
  dataSrc.readFloat(ty);
  dataSrc.readFloat(tz);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  dataSrc.readFloat(rx);
  dataSrc.readFloat(ry);
  dataSrc.readFloat(rz);
  dataSrc.readFloat(rw);

  // get the bone space translation of the bone
  float txBoneSpace, tyBoneSpace, tzBoneSpace;
  dataSrc.readFloat(txBoneSpace);
  dataSrc.readFloat(tyBoneSpace);
  dataSrc.readFloat(tzBoneSpace);

  // get the bone space rotation of the bone
  float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
  dataSrc.readFloat(rxBoneSpace);
  dataSrc.readFloat(ryBoneSpace);
  dataSrc.readFloat(rzBoneSpace);
  dataSrc.readFloat(rwBoneSpace);

  // get the parent bone id
  int parentId;
  dataSrc.readInteger(parentId);

  // get the lgith type and light color
  int lightType = LIGHT_TYPE_NONE;
  CalVector lightColor;
  if(hasNodeLights){
     dataSrc.readInteger(lightType);
     CalVectorFromDataSrc(dataSrc, &lightColor);
  }

  CalQuaternion rot(rx,ry,rz,rw);
  CalQuaternion rotbs(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace);
  CalVector trans(tx,ty,tz);

  if (loadingMode & LOADER_ROTATE_X_AXIS)
  {
    if (parentId == -1) // only root bone necessary
    {
      // Root bone must have quaternion rotated
      CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
      rot *= x_axis_90;
      // Root bone must have translation rotated also
      trans *= x_axis_90;
    }
  }
  

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // allocate a new core bone instance
  std::auto_ptr<CalCoreBone> pCoreBone( new(std::nothrow) CalCoreBone(strName) );
  if(pCoreBone.get() == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // set the parent of the bone
  pCoreBone->setParentId(parentId);

  // set all attributes of the bone
  pCoreBone->setTranslation(trans);
  pCoreBone->setRotation(rot);
  pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
  pCoreBone->setRotationBoneSpace(rotbs);

  if( hasNodeLights ) {
     pCoreBone->setLightType( (CalLightType)lightType );
     pCoreBone->setLightColor( lightColor );
  }

  // read the number of children
  int childCount;
  if(!dataSrc.readInteger(childCount) || (childCount < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // load all children ids
  for(; childCount > 0; childCount--)
  {
    int childId;
    if(!dataSrc.readInteger(childId) || (childId < 0))
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    pCoreBone->addChildId(childId);
  }

  return pCoreBone.release();
}

bool
CalLoader::usesAnimationCompression( int version )
{
   return (version >= Cal::FIRST_FILE_VERSION_WITH_ANIMATION_COMPRESSION);
}

#if 0
void
TestAnimationCompression( CalCoreBone * coreboneOrNull, int version, 
                         CalCoreKeyframe * prevCoreKeyframe,
                         bool translationRequired, bool highRangeRequired, bool translationIsDynamic, 
                         CalVector * translationInOut, 
                         CalQuaternion * rotationInOut, 
                         float * calTimeInOut )
{
   highRangeRequired = true;
   bool needTranslation = true;
   unsigned char buf[ 100 ];
   unsigned char * p = buf;
   std::string strFilename( "test" );
   unsigned int bytesWritten = CalLoader::writeCompressedKeyframe( buf, 100, strFilename, * translationInOut, * rotationInOut, * calTimeInOut, 
      Cal::CURRENT_FILE_VERSION, needTranslation, highRangeRequired );
   assert( bytesWritten != 0 );
   unsigned int bytesRead = CalLoader::readCompressedKeyframe( buf, bytesWritten, coreboneOrNull, 
      translationInOut, rotationInOut, calTimeInOut,
      prevCoreKeyframe,
      translationRequired, highRangeRequired, translationIsDynamic,
      useAnimationCompression);
   assert( bytesWritten == bytesRead );
}
#endif


 /*****************************************************************************/
/** Loads a core keyframe instance.
  *
  * This function loads a core keyframe instance from a data source.
  *
  * @param dataSrc The data source to load the core keyframe instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core keyframe
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreKeyframe* CalLoader::loadCoreKeyframe(
   CalDataSource& dataSrc, CalCoreBone * coreboneOrNull, int version, 
   CalCoreKeyframe * prevCoreKeyframe,
   bool translationRequired, bool highRangeRequired, bool translationIsDynamic,
   bool useAnimationCompression)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  float time;
  float tx, ty, tz;
  float rx, ry, rz, rw;
  if( useAnimationCompression ) {
     unsigned int bytesRequired = compressedKeyframeRequiredBytes( prevCoreKeyframe, translationRequired, highRangeRequired, translationIsDynamic );
     assert( bytesRequired < 100 );
     unsigned char buf[ 100 ];
     if( !dataSrc.readBytes( buf, bytesRequired ) ) {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
        return NULL;
     }
     CalVector vec;
     CalQuaternion quat;
     unsigned int bytesRead = readCompressedKeyframe( buf, bytesRequired, coreboneOrNull, 
        & vec, & quat, & time, prevCoreKeyframe,
        translationRequired, highRangeRequired, translationIsDynamic,
        useAnimationCompression);
     if( bytesRead != bytesRequired ) {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
        return NULL;
     }
     tx = vec.x;
     ty = vec.y;
     tz = vec.z;
     rx = quat.x;
     ry = quat.y;
     rz = quat.z;
     rw = quat.w;
     if(version < Cal::FIRST_FILE_VERSION_WITH_ANIMATION_COMPRESSION6 ) {
        if(version >= Cal::FIRST_FILE_VERSION_WITH_ANIMATION_COMPRESSION4 ) {
           if( version >= Cal::FIRST_FILE_VERSION_WITH_ANIMATION_COMPRESSION5 ) {
              if( TranslationWritten( prevCoreKeyframe, translationRequired, translationIsDynamic ) ) {
                 dataSrc.readFloat(tx);
                 dataSrc.readFloat(ty);
                 dataSrc.readFloat(tz);
              }
           }

           // get the rotation of the bone
           dataSrc.readFloat(rx);
           dataSrc.readFloat(ry);
           dataSrc.readFloat(rz);
           dataSrc.readFloat(rw);
        }
     }
  } else {
     dataSrc.readFloat(time);

     // get the translation of the bone
     dataSrc.readFloat(tx);
     dataSrc.readFloat(ty);
     dataSrc.readFloat(tz);

     if (coreboneOrNull && TranslationInvalid(CalVector(tx, ty, tz))) {
        CalVector tv = coreboneOrNull->getTranslation();
        tx = tv.x;
        ty = tv.y;
        tz = tv.z;
     }

     // get the rotation of the bone
     dataSrc.readFloat(rx);
     dataSrc.readFloat(ry);
     dataSrc.readFloat(rz);
     dataSrc.readFloat(rw);
  }  

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return false;
  }

  // allocate a new core keyframe instance
  CalCoreKeyframe *pCoreKeyframe = new(std::nothrow) CalCoreKeyframe();

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



// Return the number of bytes required by the compressed binary format of a keyframe with these attributes.
unsigned int
CalLoader::compressedKeyframeRequiredBytes( CalCoreKeyframe * lastCoreKeyframe, bool translationRequired, bool highRangeRequired, bool translationIsDynamic )
{
   unsigned int bytes = 0;
   if( translationRequired ) {

      // If I am not the first keyframe in the track, and the translations are not dynamic (meaning
      // they are the same for all the keyframes in the track, though different from the skeleton), then
      // just use the translation from the last keyframe.
      if( lastCoreKeyframe && !translationIsDynamic ) {
      } else {
         if( highRangeRequired ) {
            bytes += keyframePosBytes;
         } else {
            bytes += keyframePosBytesSmall;
         }
      }
   }
   bytes += 6;
   return bytes;
}



static float const InvalidCoord = 1e10;

void
SetTranslationInvalid( float * xResult, float * yResult, float * zResult )
{
   * xResult = InvalidCoord;
   * yResult = InvalidCoord;
   * zResult = InvalidCoord;
}

void
SetTranslationInvalid( CalVector * result )
{
   result->set( InvalidCoord, InvalidCoord, InvalidCoord );
}

bool
TranslationInvalid(float x, float y, float z)
{
   return x == InvalidCoord
      && y == InvalidCoord
      && z == InvalidCoord;
}

bool
TranslationInvalid( CalVector const & result )
{
   return result.x == InvalidCoord
      && result.y == InvalidCoord
      && result.z == InvalidCoord;
}


// Pass in the number of bytes that are valid.
// Returns number of byts read.
unsigned int
CalLoader::readCompressedKeyframe(
                                  unsigned char * buf, unsigned int bytes, CalCoreBone * coreboneOrNull, 
                                  CalVector * vecResult, CalQuaternion * quatResult, float * timeResult,
                                  CalCoreKeyframe * lastCoreKeyframe,
                                  bool translationRequired, bool highRangeRequired, bool translationIsDynamic,
                                  bool useAnimationCompression)
{
   unsigned char * bufStart = buf;

   // Read in the translation or get it from the skeleton or zero it out as a last resort.
   if( translationRequired ) {

      // If I am not the first keyframe in the track, and the translations are not dynamic (meaning
      // they are the same for all the keyframes in the track, though different from the skeleton), then
      // just use the translation from the last keyframe.
      if( lastCoreKeyframe && !translationIsDynamic ) {
         * vecResult = lastCoreKeyframe->getTranslation();
      } else {
         unsigned int data;
         float tx, ty, tz;
         if( highRangeRequired ) {
            BitReader br( buf );

            // Read x.
            br.read( & data, keyframeBitsPerUnsignedPosComponent );
            tx = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponent ) * keyframePosRange;
            br.read( & data, 1 );
            if( data ) tx = - tx;

            // Read y.
            br.read( & data, keyframeBitsPerUnsignedPosComponent );
            ty = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponent ) * keyframePosRange;
            br.read( & data, 1 );
            if( data ) ty = - ty;

            // Read z.
            br.read( & data, keyframeBitsPerUnsignedPosComponent );
            tz = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponent ) * keyframePosRange;
            br.read( & data, 1 );
            if( data ) tz = - tz;

            // Now even it off to n bytes.
            br.read( & data, keyframeBitsPerPosPadding );
            assert( br.bytesRead() == keyframePosBytes );
            buf += keyframePosBytes;
         } else {
            BitReader br( buf );

            // Read x.
            br.read( & data, keyframeBitsPerUnsignedPosComponentSmall );
            tx = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponentSmall ) * keyframePosRangeSmall;
            br.read( & data, 1 );
            if( data ) tx = - tx;

            // Read y.
            br.read( & data, keyframeBitsPerUnsignedPosComponentSmall );
            ty = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponentSmall ) * keyframePosRangeSmall;
            br.read( & data, 1 );
            if( data ) ty = - ty;

            // Read z.
            br.read( & data, keyframeBitsPerUnsignedPosComponentSmall );
            tz = FixedPointToFloatZeroToOne( data, keyframeBitsPerUnsignedPosComponentSmall ) * keyframePosRangeSmall;
            br.read( & data, 1 );
            if( data ) tz = - tz;

            // Now even it off to n bytes.
            br.read( & data, keyframeBitsPerPosPaddingSmall );
            assert( br.bytesRead() == keyframePosBytesSmall );
            buf += keyframePosBytesSmall;
         }
         vecResult->set( tx, ty, tz );
      }
   } else {
      SetTranslationInvalid( vecResult );
      if( coreboneOrNull ) {
         *vecResult = coreboneOrNull->getTranslation();
      }
   }

   // Read in the quat and time.
   float quat[ 4 ];
   unsigned int steps;
   unsigned int bytesRead = ReadQuatAndExtra( buf, quat, & steps, keyframeBitsPerOriComponent, keyframeBitsPerTime );
   buf += 6;
   assert( bytesRead == 6 );
   quatResult->set( quat[ 0 ], quat[ 1 ], quat[ 2 ], quat[ 3 ] );
   * timeResult = steps / 30.0f;
   return buf - bufStart;
}



unsigned int
CalLoader::writeCompressedKeyframe( unsigned char * buf, unsigned int bufLen, const std::string& strFilename, 
                                   CalVector const & translation, CalQuaternion const & rotation, float caltime,
                                   int version, 
                                   bool translationWritten, bool highRangeRequired )
{
   assert( CalLoader::usesAnimationCompression( version ) );
   assert( bufLen >= CalLoader::keyframePosBytes );

   // Write the translation iff necessary.
   float posRange;
   unsigned int posBits;
   unsigned int padBits;
   unsigned int bytesRequired;
   unsigned int bytesWritten = 0;
   if( highRangeRequired ) {
      padBits = CalLoader::keyframeBitsPerPosPadding;
      posBits = CalLoader::keyframeBitsPerUnsignedPosComponent;
      posRange = CalLoader::keyframePosRange;
      bytesRequired = CalLoader::keyframePosBytes;
   } else {
      padBits = CalLoader::keyframeBitsPerPosPaddingSmall;
      posBits = CalLoader::keyframeBitsPerUnsignedPosComponentSmall;
      posRange = CalLoader::keyframePosRangeSmall;
      bytesRequired = CalLoader::keyframePosBytesSmall;
   }
   if( translationWritten ) {
      BitWriter bw( buf );
      float len;
      unsigned int sign;
      unsigned int data;
      unsigned int i;
      for( i = 0; i < 3; i++ ) {
         sign = 0;
         len = translation[ i ] / posRange;
         if( len < 0.0f ) {
            sign = 1;
            len = - len;
         }
         if( len > 1.0f ) {
            CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
            return 0;
         }
         data = FloatZeroToOneToFixedPoint( len, posBits );
         bw.write( data, posBits );
         bw.write( sign, 1 );
      }

      // Now even it off to 4 or 8 bytes, depending on highRangeRequired.
      bw.write( data, padBits );
      assert( bw.bytesWritten() == bytesRequired );
      buf += bytesRequired;
      bytesWritten += bytesRequired;
   }

   // Write the quat and time.
   float wquat[] = { rotation.x, rotation.y, rotation.z, rotation.w };

   unsigned int steps = ( unsigned int ) floor( caltime * 30 + 0.5 );

   /*
   Removed the animation time limit so Matt can do his ice skating room.

   if( steps >= keyframeTimeMax ) {
   CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
   return 0;
   }
   */

   unsigned int bw = WriteQuatAndExtra( buf, wquat, steps, CalLoader::keyframeBitsPerOriComponent, CalLoader::keyframeBitsPerTime );
   assert( bw == 6 );
   buf += 6;
   bytesWritten += 6;
   return bytesWritten;
}



/*****************************************************************************/
/** Loads a core morphKeyframe instance.
*
* This function loads a core morphKeyframe instance from a data source.
*
* @param dataSrc The data source to load the core morphKeyframe instance from.
*
* @return One of the following values:
*         \li a pointer to the core morphKeyframe
*         \li \b 0 if an error happened
*****************************************************************************/

CalCoreMorphKeyframe *CalLoader::loadCoreMorphKeyframe(CalDataSource& dataSrc)
{
   if(!dataSrc.ok())
   {
      dataSrc.setError();
      return 0;
   }

   // get the time of the morphKeyframe
   float time;
   dataSrc.readFloat(time);

   // get the translation of the bone
   float weight;
   dataSrc.readFloat(weight);

   // check if an error happened
   if(!dataSrc.ok())
   {
      dataSrc.setError();
      return 0;
   }

   // allocate a new core morphKeyframe instance
   CalCoreMorphKeyframe *pCoreMorphKeyframe;
   pCoreMorphKeyframe = new CalCoreMorphKeyframe();
   if(pCoreMorphKeyframe == 0)
   {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return 0;
   }

   // create the core morphKeyframe instance
   if(!pCoreMorphKeyframe->create())
   {
      delete pCoreMorphKeyframe;
      return 0;
   }

   // set all attributes of the morphKeyframe
   pCoreMorphKeyframe->setTime(time);
   pCoreMorphKeyframe->setWeight(weight);

   return pCoreMorphKeyframe;
}

 /*****************************************************************************/
/** Loads a core compressed keyframe instance.
  *
  * This function loads a core compressed keyframe instance from a data source.
  *
  * @param dataSrc The data source to load the core compressed keyframe instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core keyframe
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreKeyframe *CalLoader::loadCompressedCoreKeyframe(CalDataSource& dataSrc, const CalVector& trackMinPt, const CalVector& trackScale, float trackDuration)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // get the time of the keyframe
	unsigned short itime;
  dataSrc.readShort((short&)itime);
	float time = (itime / 65535.0f) * trackDuration;

  // get the translation of the bone
  float tx, ty, tz;

	unsigned pt;
	dataSrc.readInteger((int&)pt);

	unsigned ptx = pt & 0x7ff;
	unsigned pty = (pt >> 11) & 0x7ff;
	unsigned ptz = pt >> 22;

	tx = ptx * trackScale.x + trackMinPt.x;
	ty = pty * trackScale.y + trackMinPt.y;
	tz = ptz * trackScale.z + trackMinPt.z;

  // get the rotation of the bone
	short s0, s1, s2;
	dataSrc.readShort(s0);
	dataSrc.readShort(s1);
	dataSrc.readShort(s2);
	CalQuaternion quat;
	quat.decompress(s0, s1, s2);

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // allocate a new core keyframe instance
  CalCoreKeyframe *pCoreKeyframe = new(std::nothrow) CalCoreKeyframe();

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
	pCoreKeyframe->setRotation(quat);

  return pCoreKeyframe;
}

 /*****************************************************************************/
/** Loads a core submesh instance.
  *
  * This function loads a core submesh instance from a data source.
  *
  * @param dataSrc The data source to load the core submesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSubmesh *CalLoader::loadCoreSubmesh(CalDataSource& dataSrc, int version)
{ 
	if(!dataSrc.ok())
	{
		dataSrc.setError();
		return 0;
	}

   bool hasVertexColors = (version >= Cal::FIRST_FILE_VERSION_WITH_VERTEX_COLORS);
   bool hasMorphTargetsInMorphFiles = (version >= Cal::FIRST_FILE_VERSION_WITH_MORPH_TARGETS_IN_MORPH_FILES);

	// get the material thread id of the submesh
	int coreMaterialThreadId;
	dataSrc.readInteger(coreMaterialThreadId);

	// get the number of vertices, faces, level-of-details and springs
	int vertexCount;
	dataSrc.readInteger(vertexCount);

	int faceCount;
	dataSrc.readInteger(faceCount);

	int lodCount;
	dataSrc.readInteger(lodCount);

	int springCount;
	dataSrc.readInteger(springCount);

	// get the number of texture coordinates per vertex
	int textureCoordinateCount;
	dataSrc.readInteger(textureCoordinateCount);

   int morphCount = 0;
   if( hasMorphTargetsInMorphFiles ) {
      dataSrc.readInteger(morphCount);
   }

	// check if an error happened
	if(!dataSrc.ok())
	{
		dataSrc.setError();
		return 0;
	}

	// allocate a new core submesh instance
	std::auto_ptr<CalCoreSubmesh> pCoreSubmesh( new(std::nothrow) CalCoreSubmesh() );
	if(pCoreSubmesh.get() == 0)
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}
	
	// set the LOD step count
	pCoreSubmesh->setLodCount(lodCount);

	// set the core material id
	pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

	// reserve memory for all the submesh data
	if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}

	// load the tangent space enable flags.
	int textureCoordinateId;
	for (textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; textureCoordinateId++)
	{
		pCoreSubmesh->enableTangents(textureCoordinateId, false);
	}

	// load all vertices and their influences
   pCoreSubmesh->setHasNonWhiteVertexColors( false );
	int vertexId;
	std::vector<CalCoreSubmesh::Vertex>&	vertexVector( pCoreSubmesh->getVectorVertex() );
	for(vertexId = 0; vertexId < vertexCount; ++vertexId)
	{
		CalCoreSubmesh::Vertex& vertex( vertexVector[ vertexId ] );

		// load data of the vertex
		dataSrc.readFloat(vertex.position.x);
		dataSrc.readFloat(vertex.position.y);
		dataSrc.readFloat(vertex.position.z);
		dataSrc.readFloat(vertex.normal.x);
		dataSrc.readFloat(vertex.normal.y);
		dataSrc.readFloat(vertex.normal.z);
      vertex.vertexColor.x = 1.0f;
      vertex.vertexColor.y = 1.0f;
      vertex.vertexColor.z = 1.0f;
      if( hasVertexColors ) {
         dataSrc.readFloat(vertex.vertexColor.x);
         dataSrc.readFloat(vertex.vertexColor.y);
         dataSrc.readFloat(vertex.vertexColor.z);
         if( vertex.vertexColor.x != 1.0f
            || vertex.vertexColor.y != 1.0f
            || vertex.vertexColor.z != 1.0f ) {
               pCoreSubmesh->setHasNonWhiteVertexColors( true );
         }
      }
		dataSrc.readInteger(vertex.collapseId);
		dataSrc.readInteger(vertex.faceCollapseCount);

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// load all texture coordinates of the vertex
		int textureCoordinateId;
		for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
		{
			CalCoreSubmesh::TextureCoordinate textureCoordinate;

			// load data of the influence
			dataSrc.readFloat(textureCoordinate.u);
			dataSrc.readFloat(textureCoordinate.v);

			if (loadingMode & LOADER_INVERT_V_COORD)
			{
				textureCoordinate.v = 1.0f - textureCoordinate.v;
			}

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
				return 0;
			}

			// set texture coordinate in the core submesh instance
			pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
		}

		// get the number of influences
		int influenceCount;
		if(!dataSrc.readInteger(influenceCount) || (influenceCount < 0))
		{
			dataSrc.setError();
			return 0;
		}

		// reserve memory for the influences in the vertex
		vertex.vectorInfluence.resize(influenceCount);

		// load all influences of the vertex
		int influenceId;
		for(influenceId = 0; influenceId < influenceCount; ++influenceId)
		{
			// load data of the influence
			dataSrc.readInteger(vertex.vectorInfluence[influenceId].boneId),
			dataSrc.readFloat(vertex.vectorInfluence[influenceId].weight);

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
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
			dataSrc.readFloat(physicalProperty.weight);

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
				return 0;
			}

			// set the physical property in the core submesh instance
			pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);
		}
	}

	// load all springs
	int springId;
	for(springId = 0; springId < springCount; ++springId)
	{
		CalCoreSubmesh::Spring spring;

		// load data of the spring
		dataSrc.readInteger(spring.vertexId[0]);
		dataSrc.readInteger(spring.vertexId[1]);
		dataSrc.readFloat(spring.springCoefficient);
		dataSrc.readFloat(spring.idleLength);

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// set spring in the core submesh instance
		pCoreSubmesh->setSpring(springId, spring);
	}

   for( int morphId = 0; morphId < morphCount; morphId++ ) {
      CalCoreSubMorphTarget * morphTarget = new CalCoreSubMorphTarget();
      if( !morphTarget ) {
         dataSrc.setError();
         return false;
      }
      if( !morphTarget->reserve(vertexCount) ) {
         dataSrc.setError();
         return false;
      }

      std::string morphName;
      dataSrc.readString(morphName);
      morphTarget->setName(morphName);

      int blendVertId;
      dataSrc.readInteger(blendVertId);

      for( int blendVertI = 0; blendVertI < vertexCount; blendVertI++ )
      {
         CalCoreSubMorphTarget::BlendVertex Vertex;
         Vertex.textureCoords.clear();
         Vertex.textureCoords.reserve(textureCoordinateCount);

         bool copyOrig;

         if( blendVertI < blendVertId ) {
            copyOrig = true;
         } else {
            copyOrig = false;
         }

         if( !copyOrig ) {
            CalVectorFromDataSrc(dataSrc, &Vertex.position);
            CalVectorFromDataSrc(dataSrc, &Vertex.normal);
            int textureCoordinateId;
            for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
            {
               CalCoreSubmesh::TextureCoordinate textureCoordinate;
               dataSrc.readFloat(textureCoordinate.u);
               dataSrc.readFloat(textureCoordinate.v);

               if (loadingMode & LOADER_INVERT_V_COORD)
               {
                  textureCoordinate.v = 1.0f - textureCoordinate.v;
               }
               Vertex.textureCoords.push_back(textureCoordinate);
            }
            if( ! dataSrc.ok() ) {
               dataSrc.setError();
               return false;
            }

            morphTarget->setBlendVertex(blendVertI, Vertex);
            dataSrc.readInteger(blendVertId);
         } 
      }
      pCoreSubmesh->addCoreSubMorphTarget(morphTarget);
   }

	// load all faces
	int faceId;
	int justOnce = 0;
	bool flipModel = false;
	for(faceId = 0; faceId < faceCount; ++faceId)
	{
		CalCoreSubmesh::Face face;

		// load data of the face

		int tmp[4];
		dataSrc.readInteger(tmp[0]);
		dataSrc.readInteger(tmp[1]);
		dataSrc.readInteger(tmp[2]);

		if(sizeof(CalIndex)==2)
		{
			if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
			{      
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
				return 0;
			}
		}
		face.vertexId[0]=tmp[0];
		face.vertexId[1]=tmp[1];
		face.vertexId[2]=tmp[2];

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// check if left-handed coord system is used by the object
		// can be done only once since the object has one system for all faces
		if (justOnce==0)
		{
			// get vertexes of first face
			std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
			CalCoreSubmesh::Vertex& v1 = vectorVertex[tmp[0]];
			CalCoreSubmesh::Vertex& v2 = vectorVertex[tmp[1]];
			CalCoreSubmesh::Vertex& v3 = vectorVertex[tmp[2]];

			CalVector point1 = CalVector(v1.position.x, v1.position.y, v1.position.z);
			CalVector point2 = CalVector(v2.position.x, v2.position.y, v2.position.z);
			CalVector point3 = CalVector(v3.position.x, v3.position.y, v3.position.z);

			// gets vectors (v1-v2) and (v3-v2)
			CalVector vect1 = point1 - point2;
			CalVector vect2 = point3 - point2;

			// calculates normal of face
			CalVector cross = vect1 % vect2;
			float	crossLength = cross.length();
			if (crossLength == 0.0f)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
				return 0;
			}
			CalVector faceNormal = cross / crossLength;

			// compare the calculated normal with the normal of a vertex
			CalVector maxNorm = v1.normal;

			// if the two vectors point to the same direction then the poly needs flipping
			// so if the dot product > 0 it needs flipping
			if (faceNormal*maxNorm>0)
				flipModel = true;

			// flip the winding order if the loading flags request it
			if (loadingMode & LOADER_FLIP_WINDING)
				flipModel = !flipModel;

			justOnce = 1;
		}


		// flip if needed
		if (flipModel) 
		{
			tmp[3] = face.vertexId[1];
			face.vertexId[1]=face.vertexId[2];
			face.vertexId[2]=tmp[3];
		}

		// set face in the core submesh instance
		pCoreSubmesh->setFace(faceId, face);
	}

	return pCoreSubmesh.release();
}



/*****************************************************************************/
/** Loads a core track instance.
*
* This function loads a core track instance from a data source.
*
* @param dataSrc The data source to load the core track instance from.
*
* @return One of the following values:
*         \li a pointer to the core track
*         \li \b 0 if an error happened
*****************************************************************************/

CalCoreTrack *CalLoader::loadCoreTrack(
                                       CalDataSource& dataSrc, CalCoreSkeleton *skel,
                                       int version, bool useAnimationCompression)
{
   if(!dataSrc.ok())
   {
      dataSrc.setError();
      return 0;
   }

   // Read the bone id.
   int coreBoneId;
   bool translationRequired = true;
   bool highRangeRequired = true;
   bool translationIsDynamic = true;
   int keyframeCount;
   static unsigned char buf[ 4 ];

   // If this file version supports animation compression, then I store the boneId in 15 bits,
   // and use the 16th bit to record if translation is required.
   if( useAnimationCompression ) {
      if( !dataSrc.readBytes( buf, 4 ) ) {
         CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
         return NULL;
      }

      // Stored low byte first.  Top 3 bits of coreBoneId are compression flags.
      coreBoneId = buf[ 0 ] + ( unsigned int ) ( buf[ 1 ] & 0x1f ) * 256;
      translationRequired = ( buf[ 1 ] & 0x80 ) ? true : false;
      highRangeRequired = ( buf[ 1 ] & 0x40 ) ? true : false;
      translationIsDynamic = ( buf[ 1 ] & 0x20 ) ? true : false;
      keyframeCount = buf[ 2 ] + ( unsigned int ) buf[ 3 ] * 256;
      //if( keyframeCount > keyframeTimeMax ) {
      //  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      //  return NULL;
      //}
   } else {
      if(!dataSrc.readInteger(coreBoneId) || (coreBoneId < 0)) {
         CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
         return 0;
      }

      // Read the number of keyframes.
      if(!dataSrc.readInteger(keyframeCount) || (keyframeCount <= 0))
      {
         CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
         return 0;
      }
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
   pCoreTrack->create();

   // link the core track to the appropriate core bone instance
   pCoreTrack->setCoreBoneId(coreBoneId);
   CalCoreBone * cb = NULL;
   if( skel ) {
      cb = skel->getCoreBone( coreBoneId );
   }


   // load all core keyframes
   int keyframeId;
   CalCoreKeyframe * lastCoreKeyframe = NULL;
   for(keyframeId = 0; keyframeId < keyframeCount; ++keyframeId)
   {
      // load the core keyframe
      CalCoreKeyframe *pCoreKeyframe = loadCoreKeyframe(
         dataSrc, cb, version, lastCoreKeyframe, translationRequired, highRangeRequired, translationIsDynamic,
         useAnimationCompression);
      lastCoreKeyframe = pCoreKeyframe;
      if(pCoreKeyframe == 0)
      {
         pCoreTrack->destroy();
         delete pCoreTrack;
         return 0;
      }
      if (loadingMode & LOADER_ROTATE_X_AXIS)
      {
         // Check for anim rotation
         if (skel && skel->getCoreBone(coreBoneId)->getParentId() == -1)  // root bone
         {
            // rotate root bone quaternion
            CalQuaternion rot = pCoreKeyframe->getRotation();
            CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
            rot *= x_axis_90;
            pCoreKeyframe->setRotation(rot);
            // rotate root bone displacement
            CalVector vec = pCoreKeyframe->getTranslation();
            vec *= x_axis_90;
            pCoreKeyframe->setTranslation(vec);
         }
      }    

      // add the core keyframe to the core track instance
      pCoreTrack->addCoreKeyframe(pCoreKeyframe);
   }

   // Whenever I load the track, I update its translationRequired status.  The status can
   // go from required to not required, but not the other way around.
   pCoreTrack->setTranslationRequired( translationRequired );
   pCoreTrack->setHighRangeRequired( highRangeRequired );
   pCoreTrack->setTranslationIsDynamic( translationIsDynamic );
   if( collapseSequencesOn ) {
      pCoreTrack->collapseSequences( translationTolerance, rotationToleranceDegrees );
   }
   if( loadingCompressionOn ) {

      // This function MIGHT call setTranslationRequired() on the track.
      // Alas, you may be passing me NULL for skel, in which case compress() won't update the 
      // translationRequired flag; instead it will leave it, as above.
      pCoreTrack->compress( translationTolerance, rotationToleranceDegrees, skel );
   }

   return pCoreTrack;
}


/*****************************************************************************/
/** Loads a core morphTrack instance.
*
* This function loads a core morphTrack instance from a data source.
*
* @param dataSrc The data source to load the core morphTrack instance from.
*
* @return One of the following values:
*         \li a pointer to the core morphTrack
*         \li \b 0 if an error happened
*****************************************************************************/

CalCoreMorphTrack *CalLoader::loadCoreMorphTrack(CalDataSource& dataSrc)
{
   if(!dataSrc.ok())
   {
      dataSrc.setError();
      return 0;
   }

   // read the morph name
   std::string morphName;
   if(!dataSrc.readString(morphName))
   {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
   }

   // allocate a new core morphTrack instance
   CalCoreMorphTrack *pCoreMorphTrack;
   pCoreMorphTrack = new CalCoreMorphTrack();
   if(pCoreMorphTrack == 0)
   {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return 0;
   }

   // create the core morphTrack instance
   if(!pCoreMorphTrack->create())
   {
      delete pCoreMorphTrack;
      return 0;
   }

   // link the core morphTrack to the appropriate morph name
   pCoreMorphTrack->setMorphName(morphName);

   // read the number of keyframes
   int keyframeCount;
   if(!dataSrc.readInteger(keyframeCount) || (keyframeCount <= 0))
   {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
   }

   // load all core keyframes
   int keyframeId;
   for(keyframeId = 0; keyframeId < keyframeCount; ++keyframeId)
   {
      // load the core keyframe
      CalCoreMorphKeyframe *pCoreKeyframe;
      pCoreKeyframe = loadCoreMorphKeyframe(dataSrc);
      if(pCoreKeyframe == 0)
      {
         pCoreMorphTrack->destroy();
         delete pCoreMorphTrack;
         return 0;
      }

      // add the core keyframe to the core morphTrack instance
      pCoreMorphTrack->addCoreMorphKeyframe(pCoreKeyframe);
   }

   return pCoreMorphTrack;
}



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
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadXmlCoreSkeleton(const std::string& strFilename)
{
	TiXmlDocument doc(strFilename);
	if(!doc.LoadFile())
	{
		CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
		return 0;
	}

	return loadXmlCoreSkeleton( doc );
}



void
BitWriter::write( unsigned int data, unsigned int numBits )
{
   // I write out full bytes, so the most bits I can have residually is 7.
   assert( bitsInBuf_ <= 7 );

   // 7 + 25 = 32.
   assert( numBits <= 25 );
   buf_ |= ( data << bitsInBuf_ );
   bitsInBuf_ += numBits;

   // Write it out low order byte first.
   while( bitsInBuf_ >= 8 ) {
      dest_[ bytesWritten_ ] = ( unsigned char ) buf_;
      buf_ >>= 8;
      bitsInBuf_ -= 8;
      bytesWritten_++;
   }
}

void
BitWriter::flush()
{
   if( bitsInBuf_ != 0 ) {
      dest_[ bytesWritten_ ] = ( unsigned char ) buf_;
      bytesWritten_++;
      bitsInBuf_ = 0;
      dest_ = NULL;
   }
}

void 
CalLoader::setAnimationCollapseSequencesOn( bool p ) 
{ 
   collapseSequencesOn = p; 
}
void 
CalLoader::setAnimationLoadingCompressionOn( bool p ) 
{ 
   loadingCompressionOn = p; 
}
void 
CalLoader::setAnimationTranslationTolerance( double p )
{ 
   translationTolerance = p; 
}
void 
CalLoader::setAnimationRotationToleranceDegrees( double p ) 
{ 
   rotationToleranceDegrees = p; 
}

//****************************************************************************//

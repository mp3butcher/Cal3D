#if defined(_MSC_VER) && _MSC_VER <= 1200
#pragma warning(disable: 4786)
#endif

//****************************************************************************//
// coremodel.cpp                                                              //
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

#include "cal3d/coremodel.h"
#include "cal3d/error.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coremesh.h"
#include "cal3d/corematerial.h"
#include "cal3d/loader.h"
#include "cal3d/saver.h"

static unsigned int const CalCoreModelMagic = 0x77884455;

 /*****************************************************************************/
/** Constructs the core model instance.
  *
  * This function is the default constructor of the core model instance.
  *****************************************************************************/

CalCoreModel::CalCoreModel(const std::string& name)
: m_strName(name)
, m_pCoreSkeleton(0)
, m_userData(0)
{
  m_magic = CalCoreModelMagic;
}

 /*****************************************************************************/
/** Constructs a copy of a core model instance.
  *
  * This function is the copy constructor of the core model instance.
  *****************************************************************************/
CalCoreModel::CalCoreModel(const CalCoreModel& inOther)
	: m_strName( inOther.m_strName )
	, m_pCoreSkeleton( inOther.m_pCoreSkeleton )
	, m_vectorCoreAnimation( inOther.m_vectorCoreAnimation )
	, m_vectorCoreMesh( inOther.m_vectorCoreMesh )
	, m_vectorCoreMaterial( inOther.m_vectorCoreMaterial )
	, m_mapmapCoreMaterialThread( inOther.m_mapmapCoreMaterialThread )
	, m_userData( inOther.m_userData )
	, m_animationName( inOther.m_animationName )
	, m_materialName( inOther.m_materialName )
	, m_meshName( inOther.m_meshName )
{
}

 /*****************************************************************************/
/** Destructs the core model instance.
  *
  * This function is the destructor of the core model instance.
  *****************************************************************************/

CalCoreModel::~CalCoreModel()
{
}



int 

CalCoreModel::getNumCoreAnimations() const
{
  int num = m_vectorCoreAnimation.size();

  int r = 0;

  for(int i = 0; i < num; ++i)
  {
    if( m_vectorCoreAnimation[ i ] ) ++r;
  }

  return r;
}



int 

CalCoreModel::getNumCoreAnimatedMorphs() const
{
  int num = m_vectorCoreAnimatedMorph.size();

  int r = 0;

  for(int i = 0; i < num; ++i)
  {
    if( m_vectorCoreAnimatedMorph[ i ] ) ++r;
  }

  return r;
}



 /*****************************************************************************/
/** Adds a core animation.
  *
  * This function adds a core animation to the core model instance.
  *
  * @param pCoreAnimation A pointer to the core animation that should be added.
  *
  * @return \li the assigned animation \b ID of the added core animation
  *****************************************************************************/

int CalCoreModel::addCoreAnimation(CalCoreAnimation *pCoreAnimation)
{
  int num = m_vectorCoreAnimation.size();

  for(int i = 0; i < num; ++i)
  {
    if( !m_vectorCoreAnimation[ i ] )
    {
      m_vectorCoreAnimation[ i ] = pCoreAnimation;
      return i;
    }
  }

  m_vectorCoreAnimation.push_back(pCoreAnimation);
  return num;
}



/*

// Return true if removed.

bool CalCoreModel::removeCoreAnimation(CalCoreAnimation *pCoreAnimation)

{

  int num = m_vectorCoreAnimation.size();

  int i;

  for( i = 0; i < num; i++ ) {

    if( m_vectorCoreAnimation[ i ] == pCoreAnimation ) {

      m_vectorCoreAnimation[ i ] = NULL;

      return true;

    }

  }

  return false;

}

*/



bool CalCoreModel::removeCoreAnimation( int id )

{

  int num = m_vectorCoreAnimation.size();

  if( id >= num || id < 0) return false;

  if( !m_vectorCoreAnimation[ id ] ) return false;

  m_vectorCoreAnimation[ id ] = NULL;

  return true;

}



 /*****************************************************************************/
/** Adds a core animated morph (different from a morph animation).
  *
  * Loads a core animated morph (different from a morph animation), and adds
  * it to the model instance.  The model instance will free the loaded core
  * animated more when the model instance is freed.
  *
  * @param pCoreAnimation A pointer to the core animated morph that should be added.
  *
  * @return One of the following values:
  *         \li the assigned animation \b ID of the added core animated morph
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::addCoreAnimatedMorph(CalCoreAnimatedMorph *pCoreAnimatedMorph)
{
  int num = m_vectorCoreAnimatedMorph.size();

  for(int i = 0; i < num; ++i)
  {
    if( !m_vectorCoreAnimatedMorph[ i ] )
    {
      m_vectorCoreAnimatedMorph[ i ] = pCoreAnimatedMorph;
      return i;
    }
  }

  m_vectorCoreAnimatedMorph.push_back(pCoreAnimatedMorph);
  return num;
}

 /*****************************************************************************/
/** Retrieves the ID of the animated morph referenced by a string
  *
  * This function returns an animated morph ID
  *
  * @param name A string that is associated with an anim morph ID number.
  * @return Returns:
  *         \li \b -1 if there is no anim morph ID associated with the input string
  *         \li \b the ID number of the anim morph asssociated with the input string
  *****************************************************************************/
int CalCoreModel::getCoreAnimatedMorphId(const std::string& name) const
{
  if (m_animatedMorphName.count( name ) < 1)
  {
    return -1;
  }

  int id = -1;
  std::map<std::string, int>::const_iterator i = m_animatedMorphName.find(name);
  if (i != m_animatedMorphName.end())
  {
    id = (*i).second;
  }
  else
  {
    // if control flows here, entry does not exist in map,
    // and therefore there has been an error
    return -1;
  }

  if (getCoreAnimation(id) == NULL)
  {
    return -1;
  }

  return id;
}


bool CalCoreModel::removeCoreAnimatedMorph( int id )
{
  int num = m_vectorCoreAnimatedMorph.size();

  if( id >= num || id < 0) return false;

  if( !m_vectorCoreAnimatedMorph[ id ] ) return false;

  m_vectorCoreAnimatedMorph[ id ] = NULL;

  return true;
}



 /*****************************************************************************/
/** Adds a core material.
  *
  * This function adds a core material to the core model instance.
  *
  * @param pCoreMaterial A pointer to the core material that should be added.
  *
  * @return One of the following values:
  *         \li the assigned material \b ID of the added core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::addCoreMaterial(CalCoreMaterial *pCoreMaterial)
{
  int num = m_vectorCoreMaterial.size();

  for(int i = 0; i < num; ++i )
  {
     if( !m_vectorCoreMaterial[ i ] )
     {
        m_vectorCoreMaterial[ i ] = pCoreMaterial;
        return i;
     }
  }

  m_vectorCoreMaterial.push_back(pCoreMaterial);
  return num;
}


 /*****************************************************************************/
/** Replace each core material by a copy.
  *
  *****************************************************************************/
void CalCoreModel::cloneCoreMaterials()
{
	std::vector<CalCoreMaterialPtr>		cloneVec;
	
	for (std::vector<CalCoreMaterialPtr>::iterator i = m_vectorCoreMaterial.begin();
		i != m_vectorCoreMaterial.end(); ++i)
	{
		cloneVec.push_back( new CalCoreMaterial( **i ) );
	}
	
	m_vectorCoreMaterial.swap( cloneVec );
}

 /*****************************************************************************/
/** Adds a core mesh.
  *
  * This function adds a core mesh to the core model instance.
  *
  * @param pCoreMesh A pointer to the core mesh that should be added.
  *
  * @return One of the following values:
  *         \li the assigned mesh \b ID of the added core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::addCoreMesh(CalCoreMesh *pCoreMesh)
{
  int num = m_vectorCoreMesh.size();

  for(int i = 0; i < num; ++i )
  {
     if( !m_vectorCoreMesh[ i ] )
     {
        m_vectorCoreMesh[ i ] = pCoreMesh;
        return i;
     }
  }

  m_vectorCoreMesh.push_back(pCoreMesh);
  return num;
}

 /*****************************************************************************/
/** Replaces a core mesh.
  *
  * This function replaces a core mesh in the core model instance. - JWWalker
  *
  * @param coreMeshId  The ID of the core mesh that should be replaced.
  * @param pCoreMesh A pointer to the core mesh that should be added.
  *
  *****************************************************************************/
void CalCoreModel::replaceCoreMesh( int coreMeshId, CalCoreMesh *pCoreMesh )
{
	m_vectorCoreMesh[ coreMeshId ] = pCoreMesh;
}

 /*****************************************************************************/
/** Returns the name.
  *
  * This function returns the name of the core model instance.
  *
  * @return The name as string.
  *****************************************************************************/

const std::string& CalCoreModel::getName() const
{
  return m_strName;
}

 /*****************************************************************************/
/** Changes the name.
  *
  * This function sets the name of the core model instance.
  *
  * @param The name as string.
  *****************************************************************************/
void CalCoreModel::setName( const char* inName )
{
	m_strName.assign( inName );
}


 /*****************************************************************************/
/** Creates a core material thread.
  *
  * This function creates a new core material thread with the given ID.
  *
  * @param coreMaterialThreadId The ID of the core material thread that should
  *                             be created.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::createCoreMaterialThread(int coreMaterialThreadId)
{
  // insert an empty core material thread with a given id
  std::map<int, int> mapCoreMaterialThreadId;
  m_mapmapCoreMaterialThread.insert(std::make_pair(coreMaterialThreadId, mapCoreMaterialThreadId));

  return true;
}

 /*****************************************************************************/
/** Provides access to a core animation.
  *
  * This function returns the core animation with the given ID.
  *
  * @param coreAnimationId The ID of the core animation that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimation *CalCoreModel::getCoreAnimation(int coreAnimationId)
{
  if((coreAnimationId < 0) || (coreAnimationId >= (int)m_vectorCoreAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreAnimation[coreAnimationId].get();
}

 /*****************************************************************************/
/** Provides access to a core animation.
  *
  * This function returns the core animation with the given ID.
  *
  * @param coreAnimationId The ID of the core animation that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreAnimation *CalCoreModel::getCoreAnimation(int coreAnimationId) const
{
  if((coreAnimationId < 0) 

    || (coreAnimationId >= (int)m_vectorCoreAnimation.size())

    || !m_vectorCoreAnimation[ coreAnimationId ] )
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreAnimation[coreAnimationId].get();
}



 /*****************************************************************************/
/** Returns the number of core animations.
  *
  * This function returns the number of core animations in the core model
  * instance.
  *
  * @return The number of core animations.
  *****************************************************************************/

int CalCoreModel::getCoreAnimationCount() const
{
  return m_vectorCoreAnimation.size();
}

/*****************************************************************************/
/** Provides access to a core morph animation.
*
* This function returns the core morph animation with the given ID.
*
* @param coreAnimatedMorphId The ID of the core morph animation that should be returned.
*
* @return One of the following values:
*         \li a pointer to the core morph animation
*         \li \b 0 if an error happend
*****************************************************************************/

CalCoreAnimatedMorph *CalCoreModel::getCoreAnimatedMorph(int coreAnimatedMorphId)
{
   if((coreAnimatedMorphId < 0) 

      || (coreAnimatedMorphId >= (int)m_vectorCoreAnimatedMorph.size())

      || !m_vectorCoreAnimatedMorph[ coreAnimatedMorphId ] )

   {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return 0;
   }

   return m_vectorCoreAnimatedMorph[coreAnimatedMorphId];
}

const CalCoreAnimatedMorph *CalCoreModel::getCoreAnimatedMorph(int coreAnimatedMorphId) const
{
   if((coreAnimatedMorphId < 0) 

      || (coreAnimatedMorphId >= (int)m_vectorCoreAnimatedMorph.size())

      || !m_vectorCoreAnimatedMorph[ coreAnimatedMorphId ] )

   {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return 0;
   }

   return m_vectorCoreAnimatedMorph[coreAnimatedMorphId];
}

int CalCoreModel::getCoreMorphAnimationCount() const
{
   return int(m_vectorCoreAnimatedMorph.size());
}


 /*****************************************************************************/
/** Provides access to a core material.
  *
  * This function returns the core material with the given ID.
  *
  * @param coreMaterialId The ID of the core material that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterial *CalCoreModel::getCoreMaterial(int coreMaterialId)
{
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMaterial[coreMaterialId].get();
}

 /*****************************************************************************/
/** Provides access to a core material.
  *
  * This function returns the core material with the given ID.
  *
  * @param coreMaterialId The ID of the core material that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreMaterial *CalCoreModel::getCoreMaterial(int coreMaterialId) const
{
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMaterial[coreMaterialId].get();
}

 /*****************************************************************************/
/** Returns the number of core materials.
  *
  * This function returns the number of core materials in the core model
  * instance.
  *
  * @return The number of core materials.
  *****************************************************************************/

int CalCoreModel::getCoreMaterialCount() const
{
  return m_vectorCoreMaterial.size();
}

 /*****************************************************************************/
/** Returns a specified core material ID.
  *
  * This function returns the core material ID for a specified core material
  * thread / core material set pair.
  *
  * @param coreMaterialThreadId The ID of the core material thread.
  * @param coreMaterialSetId The ID of the core material set.
  *
  * @return One of the following values:
  *         \li the \b ID of the core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::getCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId) const
{
  // find the core material thread
  std::map<int, std::map<int, int> >::const_iterator iteratorCoreMaterialThread;
  iteratorCoreMaterialThread = m_mapmapCoreMaterialThread.find(coreMaterialThreadId);
  if(iteratorCoreMaterialThread == m_mapmapCoreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // get the core material thread
  const std::map<int, int>& coreMaterialThread = (*iteratorCoreMaterialThread).second;

  // find the material id for the given set
  std::map<int, int>::const_iterator iteratorSet;
  iteratorSet = coreMaterialThread.find(coreMaterialSetId);
  if(iteratorSet == coreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  return (*iteratorSet).second;
}

 /*****************************************************************************/
/** Provides access to a core mesh.
  *
  * This function returns the core mesh with the given ID.
  *
  * @param coreMeshId The ID of the core mesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMesh *CalCoreModel::getCoreMesh(int coreMeshId)
{
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMesh[coreMeshId].get();
}

 /*****************************************************************************/
/** Provides access to a core mesh.
  *
  * This function returns the core mesh with the given ID.
  *
  * @param coreMeshId The ID of the core mesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreMesh *CalCoreModel::getCoreMesh(int coreMeshId) const
{
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMesh[coreMeshId].get();
}

 /*****************************************************************************/
/** Returns the number of core meshes.
  *
  * This function returns the number of core meshes in the core model instance.
  *
  * @return The number of core meshes.
  *****************************************************************************/

int CalCoreModel::getCoreMeshCount() const
{
  return m_vectorCoreMesh.size();
}

 /*****************************************************************************/
/** Provides access to the core skeleton.
  *
  * This function returns the core skeleton.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeleton *CalCoreModel::getCoreSkeleton()
{
  return m_pCoreSkeleton.get();
}

 /*****************************************************************************/
/** Provides access to the core skeleton.
  *
  * This function returns the core skeleton.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreSkeleton *CalCoreModel::getCoreSkeleton() const
{
  return m_pCoreSkeleton.get();
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the core model instance.
  *
  * @return The user data stored in the core model instance.
  *****************************************************************************/

Cal::UserData CalCoreModel::getUserData()
{
  return m_userData;
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the core model instance.
  *
  * @return The user data stored in the core model instance.
  *****************************************************************************/

const Cal::UserData CalCoreModel::getUserData() const
{
  return m_userData;
}

 /*****************************************************************************/
/** Loads a core animation.
  *
  * This function loads a core animation from a file.
  *
  * @param strFilename The file from which the core animation should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreAnimation(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  if(!m_pCoreSkeleton)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core animation
  CalCoreAnimationPtr pCoreAnimation = CalLoader::loadCoreAnimation(strFilename, m_pCoreSkeleton.get());
  if(!pCoreAnimation) return -1;
  

  // add core animation to this core model
  return addCoreAnimation(pCoreAnimation.get());
}

 /*****************************************************************************/
/** Loads a core animation and bind it to a name.
  *
  * This function loads a core animation from a file. It is equivalent
  * to calling addAnimName(strAnimationName, loadCoreAnimation(strFilename)).
  * If strAnimationName is already associated to a coreAnimationId because
  * of a previous call to addAnimName, the same coreAnimationId will
  * be used. 
  *
  * @param strFilename The file from which the core animation should be loaded
  *                    from.
  * @param strAnimationName A string that is associated with an anim ID number.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreAnimation(const std::string& strFilename, const std::string& strAnimationName)
{
  int id = -1;
  std::map<std::string, int>::iterator it = m_animationName.find(strAnimationName);
  if (it != m_animationName.end())
  {
    id=(*it).second;

    // the core skeleton has to be loaded already
    if(!m_pCoreSkeleton)
    {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return -1;
    }
    if(m_vectorCoreAnimation[id])
    {
      CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
      return -1;
    }
    CalCoreAnimationPtr pCoreAnimation = CalLoader::loadCoreAnimation(strFilename, m_pCoreSkeleton.get());
    if(!pCoreAnimation) return -1;
    pCoreAnimation->setName(strAnimationName);
    m_vectorCoreAnimation[id] = pCoreAnimation;
  }
  else
  {
    id = loadCoreAnimation(strFilename);
    if(id >= 0)
      addAnimationName(strAnimationName, id);
  }

  return id;
}

int CalCoreModel::loadCoreAnimation(void* buffer, const std::string& strAnimationName)
{
   int id = -1;
   std::map<std::string, int>::iterator it = m_animationName.find(strAnimationName);
   if (it != m_animationName.end())
   {
      id=(*it).second;

      // the core skeleton has to be loaded already
      if(!m_pCoreSkeleton)
      {
         CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
         return -1;
      }
      if(m_vectorCoreAnimation[id])
      {
         CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
         return -1;
      }
      CalCoreAnimationPtr pCoreAnimation = CalLoader::loadCoreAnimation(buffer);
      if(!pCoreAnimation) return -1;
      pCoreAnimation->setName(strAnimationName);
      m_vectorCoreAnimation[id] = pCoreAnimation;
   }
   else
   {
      id = loadCoreAnimation(buffer);
      if(id >= 0)
         addAnimationName(strAnimationName, id);
   }

   return id;
}

 /*****************************************************************************/
/** Delete the resources used by the named core animation. The name must 
  * be associated with a valid core animation Id with the function
  * getAnimationId. The caller must ensure that the corresponding is not
  * referenced anywhere otherwise unpredictable results will occur.
  *
  * @param name The symbolic name of the core animation to unload.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::unloadCoreAnimation(const std::string& name)
{
  int id = getCoreAnimationId(name);
  if(id >= 0)
    return unloadCoreAnimation(id);
  else
    return -1;
}

 /*****************************************************************************/
/** Delete the resources used by a core animation. The caller must
  * ensure that the corresponding is not referenced anywhere otherwise
  * unpredictable results will occur.
  *
  * @param coreAnimationId The ID of the core animation that should be unloaded.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/


int CalCoreModel::unloadCoreAnimation(int coreAnimationId)
{
  if((coreAnimationId < 0) || (coreAnimationId >= (int)m_vectorCoreAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  m_vectorCoreAnimation[coreAnimationId] = CalCoreAnimationPtr (0);

  return coreAnimationId;
}

 /*****************************************************************************/
/** Loads a core animation.
  *
  * This function loads a core animation from a buffer.
  *
  * @param buffer 	The buffer from which the core animation should be loaded.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreAnimation(void* buffer)
{
  // the core skeleton has to be loaded already
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core animation
  CalCoreAnimationPtr pCoreAnimation = CalLoader::loadCoreAnimation(buffer);
  if (!pCoreAnimation) return -1;

  // add core animation to this core model
  int animationId = addCoreAnimation(pCoreAnimation.get());
  if(animationId == -1)
  {
    return -1;
  }

  return animationId;
}

 /*****************************************************************************/
/** Loads a core animatedMorph.
  *
  * This function loads a core animatedMorph from a file.
  *
  * @param strFilename The file from which the core animatedMorph should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core animation
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreAnimatedMorph(const std::string& strFilename)
{
  // load a new core AnimatedMorph
  CalCoreAnimatedMorph* pCoreAnimatedMorph = CalLoader::loadCoreAnimatedMorph(strFilename);
  if(pCoreAnimatedMorph == 0) return -1;

  std::string name = strFilename;

  size_t found = strFilename.find_last_of("/\\");
  name = strFilename.substr(found + 1);
  pCoreAnimatedMorph->setName(name);

  // add core AnimatedMorph to this core model
  int animatedMorphId = addCoreAnimatedMorph(pCoreAnimatedMorph);
  if(animatedMorphId == -1)
  {
    delete pCoreAnimatedMorph;
    return -1;
  }

  return animatedMorphId;
}

int CalCoreModel::loadCoreAnimatedMorph(const std::string& strFilename, const std::string& name)
{
   int id = -1;
   std::map<std::string, int>::iterator it = m_animatedMorphName.find(name);
   if (it != m_animatedMorphName.end())
   {
      id=(*it).second;

      if(m_vectorCoreAnimatedMorph[id])
      {
         CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
         return -1;
      }

      CalCoreAnimatedMorph* pCoreAnimMorph = CalLoader::loadCoreAnimatedMorph(strFilename);
      if(!pCoreAnimMorph) return -1;
      pCoreAnimMorph->setName(name);
      m_vectorCoreAnimatedMorph[id] = pCoreAnimMorph;
   }
   else
   {
      id = loadCoreAnimatedMorph(strFilename);
      if(id >= 0)
         addAnimatedMorphName(name, id);
   }

   return id;
}

int CalCoreModel::loadCoreAnimatedMorph(void* buffer, unsigned int bufferLength)
{
   // load a new core animation
   CalCoreAnimatedMorph* pCoreAnimatedMorph = CalLoader::loadCoreAnimatedMorphFromBuffer(buffer, bufferLength);
   if (!pCoreAnimatedMorph) return -1;

   // add core animation to this core model
   int id = addCoreAnimatedMorph(pCoreAnimatedMorph);
   if(id == -1)
   {
      return -1;
   }

   return id;
}

int CalCoreModel::loadCoreAnimatedMorph(void* buffer, unsigned int bufferLength,
                                        const std::string& name)
{
   int id = -1;
   std::map<std::string, int>::iterator it = m_animatedMorphName.find(name);
   if (it != m_animatedMorphName.end())
   {
      id=(*it).second;

      if(m_vectorCoreAnimatedMorph[id])
      {
         CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
         return -1;
      }

      CalCoreAnimatedMorph* pCoreAnimMorph = CalLoader::loadCoreAnimatedMorphFromBuffer(buffer, bufferLength);
      if(!pCoreAnimMorph) return -1;
      pCoreAnimMorph->setName(name);
      m_vectorCoreAnimatedMorph[id] = pCoreAnimMorph;
   }
   else
   {
      id = loadCoreAnimatedMorph(buffer, bufferLength);
      if(id >= 0)
         addAnimatedMorphName(name, id);
   }

   return id;
}

 /*****************************************************************************/
/** Delete the resources used by the named core animated morph. The name must 
  * be associated with a valid core animated morph Id with the function
  * getAnimatedMorphId. The caller must ensure that the corresponding is not
  * referenced anywhere otherwise unpredictable results will occur.
  *
  * @param name The symbolic name of the core animated morph to unload.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core animated morph
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::unloadCoreAnimatedMorph(const std::string& name)
{
  int id = getCoreAnimatedMorphId(name);
  if(id >= 0)
    return unloadCoreAnimatedMorph(id);
  else
    return -1;
}

 /*****************************************************************************/
/** Delete the resources used by a core animated orph. The caller must
  * ensure that the corresponding is not referenced anywhere otherwise
  * unpredictable results will occur.
  *
  * @param coreAnimatedMorphId The ID of the core animated morph that should be unloaded.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core animated morph
  *         \li \b -1 if an error happened
  *****************************************************************************/
int CalCoreModel::unloadCoreAnimatedMorph(int coreAnimatedMorphId)
{
  if((coreAnimatedMorphId < 0) || (coreAnimatedMorphId >= (int)m_vectorCoreAnimatedMorph.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  m_vectorCoreAnimatedMorph[coreAnimatedMorphId] = (CalCoreAnimatedMorph*)(0);

  return coreAnimatedMorphId;
}


 /*****************************************************************************/
/** Loads a core material.
  *
  * This function loads a core material from a file.
  *
  * @param strFilename The file from which the core material should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMaterial(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  if(!m_pCoreSkeleton)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core material
  CalCoreMaterialPtr pCoreMaterial = CalLoader::loadCoreMaterial(strFilename);
  if(!pCoreMaterial) return -1;

  // add core material to this core model
  return addCoreMaterial(pCoreMaterial.get());
}

 /*****************************************************************************/
/** Loads a core material and bind it to a name.
  *
  * This function loads a core material from a file. It is equivalent
  * to calling addMaterialName(strMaterialName, loadCoreMaterial(strFilename)).
  * If strMaterialName is already associated to a coreMaterialId because
  * of a previous call to addMaterialName, the same coreMaterialId will
  * be used. 
  *
  * @param strFilename The file from which the core material should be loaded
  *                    from.
  * @param strMaterialName A string that is associated with an anim ID number.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMaterial(const std::string& strFilename, const std::string& strMaterialName)
{
  int id = -1;
  std::map<std::string, int>::iterator it = m_materialName.find(strMaterialName);
  if (it != m_materialName.end())
  {
    id=(*it).second;

    // the core skeleton has to be loaded already
    if(!m_pCoreSkeleton)
    {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return -1;
    }
    if(m_vectorCoreMaterial[id])
    {
      CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
      return -1;
    }
    CalCoreMaterialPtr pCoreMaterial = CalLoader::loadCoreMaterial(strFilename);
    if(!pCoreMaterial) return -1;
    pCoreMaterial->setName(strMaterialName);
    m_vectorCoreMaterial[id] = pCoreMaterial;
  }
  else
  {
    id = loadCoreMaterial(strFilename);
    if(id >= 0)
      addMaterialName(strMaterialName, id);
  }

  return id;
}

int CalCoreModel::loadCoreMaterial(void* buffer, const std::string& strMaterialName)
{
   int id = -1;
   std::map<std::string, int>::iterator it = m_materialName.find(strMaterialName);
   if (it != m_materialName.end())
   {
      id=(*it).second;

      // the core skeleton has to be loaded already
      if(!m_pCoreSkeleton)
      {
         CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
         return -1;
      }
      if(m_vectorCoreMaterial[id])
      {
         CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
         return -1;
      }
      CalCoreMaterialPtr pCoreMaterial = CalLoader::loadCoreMaterial(buffer);
      if(!pCoreMaterial) return -1;
      pCoreMaterial->setName(strMaterialName);
      m_vectorCoreMaterial[id] = pCoreMaterial;
   }
   else
   {
      id = loadCoreMaterial(buffer);
      if(id >= 0)
         addMaterialName(strMaterialName, id);
   }

   return id;
}


 /*****************************************************************************/
/** Loads a core material.
  *
  * This function loads a core material from a buffer.
  *
  * @param buffer 	The buffer from which the core material should be loaded.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMaterial(void* buffer)
{
  // the core skeleton has to be loaded already
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core material
  CalCoreMaterialPtr pCoreMaterial = CalLoader::loadCoreMaterial(buffer);
  if(!pCoreMaterial) return -1;

  // add core material to this core model
  return addCoreMaterial(pCoreMaterial.get());
}

 /*****************************************************************************/
/** Delete the resources used by the named core material. The name must 
  * be associated with a valid core material Id with the function
  * getMaterialId. The caller must ensure that the corresponding is not
  * referenced anywhere otherwise unpredictable results will occur.
  *
  * @param name The symbolic name of the core material to unload.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core material
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::unloadCoreMaterial(const std::string& name)
{
  int id = getCoreMaterialId(name);
  if(id >= 0)
    return unloadCoreMaterial(id);
  else
    return -1;
}

 /*****************************************************************************/
/** Delete the resources used by a core material. The caller must
  * ensure that the corresponding is not referenced anywhere otherwise
  * unpredictable results will occur.
  *
  * @param coreMaterialId The ID of the core material that should be unloaded.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core material
  *         \li \b -1 if an error happened
  *****************************************************************************/


int CalCoreModel::unloadCoreMaterial(int coreMaterialId)
{
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  m_vectorCoreMaterial[coreMaterialId] = CalCoreMaterialPtr(0);

  return coreMaterialId;
}

 /*****************************************************************************/
/** Loads a core mesh.
  *
  * This function loads a core mesh from a file.
  *
  * @param strFilename The file from which the core mesh should be loaded from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core mesh
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMesh(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  if(!m_pCoreSkeleton)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core mesh
  CalCoreMeshPtr pCoreMesh = CalLoader::loadCoreMesh(strFilename);
  if(!pCoreMesh) return -1;

  // add core mesh to this core model
  return addCoreMesh(pCoreMesh.get());
}

 /*****************************************************************************/
/** Loads a core mesh and bind it to a name.
  *
  * This function loads a core mesh from a file. It is equivalent
  * to calling addMeshName(strMeshName, loadCoreMesh(strFilename)).
  * If strMeshName is already associated to a coreMeshId because
  * of a previous call to addMeshName, the same coreMeshId will
  * be used. 
  *
  * @param strFilename The file from which the core mesh should be loaded
  *                    from.
  * @param strMeshName A string that is associated with an anim ID number.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core mesh
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMesh(const std::string& strFilename, const std::string& strMeshName)
{
  int id = -1;
  std::map<std::string, int>::iterator it = m_meshName.find(strMeshName);
  if (it != m_meshName.end())
  {
    id=(*it).second;

    // the core skeleton has to be loaded already
    if(!m_pCoreSkeleton)
    {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return -1;
    }
    if(m_vectorCoreMesh[id])
    {
      CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
      return -1;
    }
    CalCoreMeshPtr pCoreMesh = CalLoader::loadCoreMesh(strFilename);
    if(!pCoreMesh) return -1;
    pCoreMesh->setName(strMeshName);
    m_vectorCoreMesh[id] = pCoreMesh;
  }
  else
  {
    id = loadCoreMesh(strFilename);
    if(id >= 0)
      addMeshName(strMeshName, id);
  }

  return id;
}

int CalCoreModel::loadCoreMesh(void* buffer, const std::string& strMeshName)
{
   int id = -1;
   std::map<std::string, int>::iterator it = m_meshName.find(strMeshName);
   if (it != m_meshName.end())
   {
      id=(*it).second;

      // the core skeleton has to be loaded already
      if(!m_pCoreSkeleton)
      {
         CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
         return -1;
      }
      if(m_vectorCoreMesh[id])
      {
         CalError::setLastError(CalError::INDEX_BUILD_FAILED, __FILE__, __LINE__);
         return -1;
      }
      CalCoreMeshPtr pCoreMesh = CalLoader::loadCoreMesh(buffer);
      if(!pCoreMesh) return -1;
      pCoreMesh->setName(strMeshName);
      m_vectorCoreMesh[id] = pCoreMesh;
   }
   else
   {
      id = loadCoreMesh(buffer);
      if(id >= 0)
         addMeshName(strMeshName, id);
   }

   return id;
}

 /*****************************************************************************/
/** Loads a core mesh.
  *
  * This function loads a core mesh from a buffer.
  *
  * @param buffer 	The buffer from which the core mesh should be loaded.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core mesh
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::loadCoreMesh(void* buffer)
{
  // the core skeleton has to be loaded already
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }


  // load a new core mesh
  CalCoreMeshPtr pCoreMesh = CalLoader::loadCoreMesh(buffer);
  if (!pCoreMesh) return -1;

  // add core mesh to this core model
  return addCoreMesh(pCoreMesh.get());
}


 /*****************************************************************************/
/** Delete the resources used by the named core mesh. The name must 
  * be associated with a valid core mesh Id with the function
  * getMeshId. The caller must ensure that the corresponding is not
  * referenced anywhere otherwise unpredictable results will occur.
  *
  * @param name The symbolic name of the core mesh to unload.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core mesh
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreModel::unloadCoreMesh(const std::string& name)
{
  int id = getCoreMeshId(name);
  if(id >= 0)
    return unloadCoreMesh(id);
  else
    return -1;
}

 /*****************************************************************************/
/** Delete the resources used by a core mesh. The caller must
  * ensure that the corresponding is not referenced anywhere otherwise
  * unpredictable results will occur.
  *
  * @param coreMeshId The ID of the core mesh that should be unloaded.
  *
  * @return One of the following values:
  *         \li the core \b ID of the unloaded core mesh
  *         \li \b -1 if an error happened
  *****************************************************************************/


int CalCoreModel::unloadCoreMesh(int coreMeshId)
{
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  m_vectorCoreMesh[coreMeshId] = CalCoreMeshPtr(0);

  return coreMeshId;
}


 /*****************************************************************************/
/** Loads the core skeleton.
  *
  * This function loads the core skeleton from a file.
  *
  * @param strFilename The file from which the core skeleton should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::loadCoreSkeleton(const std::string& strFilename)
{
  // load a new core skeleton
  m_pCoreSkeleton = CalLoader::loadCoreSkeleton(strFilename);
  return bool(m_pCoreSkeleton);
}

 /*****************************************************************************/
/** Loads the core skeleton.
  *
  * This function loads the core skeleton from a buffer.
  *
  * @param buffer 	The buffer from which the core skeleton should be loaded.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::loadCoreSkeleton(void* buffer)
{
  // load a new core skeleton
  m_pCoreSkeleton = CalLoader::loadCoreSkeleton(buffer);
  if(!m_pCoreSkeleton) return false;

  return true;
}

 /*****************************************************************************/
/** Saves a core animation.
  *
  * This function saves a core animation to a file.
  *
  * @param strFilename The file to which the core animation should be saved to.
  * @param coreAnimationId The ID of the core animation that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::saveCoreAnimation(const std::string& strFilename, int coreAnimationId) const
{
  // check if the core animation id is valid
  if((coreAnimationId < 0) 

    || (coreAnimationId >= (int)m_vectorCoreAnimation.size())

    || !m_vectorCoreAnimation[ coreAnimationId ] )
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  if(!CalSaver::saveCoreAnimation(strFilename, m_vectorCoreAnimation[coreAnimationId].get()))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core animated morph.
  *
  * This function saves a core animated morph to a file.
  *
  * @param strFilename The file to which the core animated morph should be saved to.
  * @param coreAnimatedMorphId The ID of the core animated morph that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::saveCoreAnimatedMorph(const std::string& strFilename, int coreAnimatedMorphId) const
{
  // check if the core animation id is valid
  if((coreAnimatedMorphId < 0) 

    || (coreAnimatedMorphId >= (int)m_vectorCoreAnimatedMorph.size())

    || !m_vectorCoreAnimatedMorph[ coreAnimatedMorphId ] )
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  if(!CalSaver::saveCoreAnimatedMorph(strFilename, m_vectorCoreAnimatedMorph[coreAnimatedMorphId]))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core material.
  *
  * This function saves a core material to a file.
  *
  * @param strFilename The file to which the core material should be saved to.
  * @param coreMaterialId The ID of the core material that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::saveCoreMaterial(const std::string& strFilename, int coreMaterialId) const
{
  // check if the core material id is valid
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  return CalSaver::saveCoreMaterial(strFilename, m_vectorCoreMaterial[coreMaterialId].get());
}

 /*****************************************************************************/
/** Saves a core mesh.
  *
  * This function saves a core mesh to a file.
  *
  * @param strFilename The file to which the core mesh should be saved to.
  * @param coreMeshId The ID of the core mesh that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::saveCoreMesh(const std::string& strFilename, int coreMeshId) const
{
  // check if the core mesh id is valid
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  return CalSaver::saveCoreMesh(strFilename, m_vectorCoreMesh[coreMeshId].get());
}

 /*****************************************************************************/
/** Saves the core skeleton.
  *
  * This function saves the core skeleton to a file.
  *
  * @param strFilename The file to which the core skeleton should be saved to.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::saveCoreSkeleton(const std::string& strFilename) const
{
  // check if we have a core skeleton in this code model
  if(!m_pCoreSkeleton)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core skeleton
  return CalSaver::saveCoreSkeleton(strFilename, m_pCoreSkeleton.get());
}

 /*****************************************************************************/
/** Sets a core material ID.
  *
  * This function sets a core material ID for a core material thread / core
  * material set pair.
  *
  * @param coreMaterialThreadId The ID of the core material thread.
  * @param coreMaterialSetId The ID of the core maetrial set.
  * @param coreMaterialId The ID of the core maetrial.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreModel::setCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId)
{
  // find the core material thread
  std::map<int, std::map<int, int> >::iterator iteratorCoreMaterialThread;
  iteratorCoreMaterialThread = m_mapmapCoreMaterialThread.find(coreMaterialThreadId);
  if(iteratorCoreMaterialThread == m_mapmapCoreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core material thread
  std::map<int, int>& coreMaterialThread = (*iteratorCoreMaterialThread).second;

  // remove a possible entry in the core material thread
  coreMaterialThread.erase(coreMaterialSetId);

  // set the given set id in the core material thread to the given core material id
  coreMaterialThread.insert(std::make_pair(coreMaterialSetId, coreMaterialId));

  return true;
}

 /*****************************************************************************/
/** Sets the core skeleton.
  *
  * This function sets the core skeleton of the core model instance..
  *
  * @param pCoreSkeleton The core skeleton that should be set.
  *****************************************************************************/

void CalCoreModel::setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton)
{
  m_pCoreSkeleton = pCoreSkeleton;  
}

 /*****************************************************************************/
/** Stores user data.
  *
  * This function stores user data in the core model instance.
  *
  * @param userData The user data that should be stored.
  *****************************************************************************/

void CalCoreModel::setUserData(Cal::UserData userData)
{
  m_userData = userData;
}

 /*****************************************************************************/
/** Creates or overwrites a string-to-boneId mapping
  *
  * This function makes a bone ID reference-able by a string name.
  *
  * @param strBoneName The string that will be associated with the ID.
  * @param boneId The ID number of the bone that will be referenced by the string.
  *****************************************************************************/

void CalCoreModel::addBoneName(const std::string& strBoneName, int boneId)
{
  //Make sure the skeleton has been loaded first
  if (m_pCoreSkeleton)
  {
    //Map the bone ID to the name
    m_pCoreSkeleton->mapCoreBoneName(boneId, strBoneName);
  }
}

 /*****************************************************************************/
/** Retrieves the ID of the bone referenced by a string
  *
  * This function returns a bone ID
  *
  * @param strBoneName A string that is associated with a bone ID number.
  * @return Returns:
  *         \li \b -1 if there is no bone ID associated with the input string
  *         \li \b the ID number of the bone asssociated with the input string
  *****************************************************************************/

int CalCoreModel::getBoneId(const std::string& strBoneName) const
{
  if (m_pCoreSkeleton)
  {
    return m_pCoreSkeleton->getCoreBoneId(strBoneName);
  }
  return -1;
}

 /*****************************************************************************/
/** Creates or overwrites a string-to-animation ID mapping
  *
  * This function makes an animation ID reference-able by a string name.
  * Note that we don't verify that the ID is valid because the animation
  * may be added later.
  * Also, if there is already a helper with this name, it will be overwritten
  * without warning.
  *
  * @param strAnimationName The string that will be associated with the ID.
  * @param coreAnimationId The ID number of the animation to be referenced by the string.
  *****************************************************************************/

bool CalCoreModel::addAnimationName(const std::string& strAnimationName, int coreAnimationId)
{
  // check if the core animation id is valid
  if((coreAnimationId < 0) || (coreAnimationId >= (int)m_vectorCoreAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_vectorCoreAnimation[ coreAnimationId ]->setName(strAnimationName);
  m_animationName[ strAnimationName ] = coreAnimationId;
  return true;
}

bool CalCoreModel::addAnimatedMorphName(const std::string& strAnimatedMorphName, int coreAnimatedMorphId)
{
   // check if the core animation id is valid
   if((coreAnimatedMorphId < 0) || (coreAnimatedMorphId >= (int)m_vectorCoreAnimatedMorph.size()))
   {
      CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
      return false;
   }

   m_vectorCoreAnimatedMorph[ coreAnimatedMorphId ]->setName(strAnimatedMorphName);
   m_animatedMorphName[ strAnimatedMorphName ] = coreAnimatedMorphId;
   return true;
}

 /*****************************************************************************/
/** Retrieves the ID of the animation referenced by a string
  *
  * This function returns an animation ID
  *
  * @param strAnimationName A string that is associated with an anim ID number.
  * @return Returns:
  *         \li \b -1 if there is no anim ID associated with the input string
  *         \li \b the ID number of the anim asssociated with the input string
  *****************************************************************************/

int CalCoreModel::getCoreAnimationId(const std::string& strAnimationName) const
{
  if (m_animationName.count( strAnimationName ) < 1)
  {
    return -1;
  }

  /*
  DJMC note: We cannot simply do the following:

  m_animationName[strAnimationName];

  and still remain a const method since the [] operation might
  add a setting if it is not found inside. We must seek an
  alternative.

  Note, according to http://www.sgi.com/tech/stl/Map.html, m[k]
  should be equivalent to:

  (*((m.insert(value_type(k, data_type()))).first)).second

  ...thus, we should be able to find a read-only analogue by
  changing the line from an insert to a find, so that we may
  remain a const method.
  //*/

  int animationID;
  std::map<std::string, int>::const_iterator i = m_animationName.find(strAnimationName);
  if (i != m_animationName.end())
  {
    animationID = (*i).second;
  }
  else
  {
    // if control flows here, entry does not exist in map,
    // and therefore there has been an error
    return -1;
  }

  if (getCoreAnimation(animationID) == NULL)
  {
    return -1;
  }

  return animationID;
}

 /*****************************************************************************/
/** Creates or overwrites a string-to-core-material ID mapping
  *
  * This function makes a core material ID reference-able by a string name.
  * Note that we don't verify that the ID is valid because the material
  * may be added later.
  * Also, if there is already a helper with this name, it will be overwritten
  * without warning.
  *
  * @param strMaterialName The string that will be associated with the ID.
  * @param coreMaterialId The core ID number of the material to be referenced by the string.
  *****************************************************************************/

bool CalCoreModel::addMaterialName(const std::string& strMaterialName, int coreMaterialId)
{
  
  // check if the core material id is valid
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_vectorCoreMaterial[ coreMaterialId ]->setName(strMaterialName);
  m_materialName[ strMaterialName ] = coreMaterialId;
  return true;
}

 /*****************************************************************************/
/** Retrieves the ID of the core material referenced by a string
  *
  * This function returns a core material ID
  *
  * @param strMaterialName A string that is associated with a core material ID number.
  * @return Returns:
  *         \li \b -1 if there is no core material ID associated with the input string
  *         \li \b the core ID number of the material associated with the input string
  *****************************************************************************/

int CalCoreModel::getCoreMaterialId(const std::string& strMaterialName) const
{
  if (m_materialName.count( strMaterialName ) < 1)
  {
    return -1;
  }

  /*
  DJMC note: We cannot simply do the following:

  m_materialName[strMaterialName];

  and still remain a const method since the [] operation might
  add a setting if it is not found inside. We must seek an
  alternative.

  Note, according to http://www.sgi.com/tech/stl/Map.html, m[k]
  should be equivalent to:

  (*((m.insert(value_type(k, data_type()))).first)).second

  ...thus, we should be able to find a read-only analogue by
  changing the line from an insert to a find, so that we may
  remain a const method.
  //*/

  int materialID;
  std::map<std::string, int>::const_iterator i = m_materialName.find(strMaterialName);
  if (i != m_materialName.end())
  {
    materialID = (*i).second;
  }
  else
  {
    // if control flows here, entry does not exist in map,
    // and therefore there has been an error
    return -1;
  }

  if (getCoreMaterial(materialID) == NULL)
  {
    return -1;
  }

  return materialID;
}


 /*****************************************************************************/
/** Creates or overwrites a string-to-core-mesh ID mapping
  *
  * This function makes a core mesh ID reference-able by a string name.
  * Note that we don't verify that the ID is valid because the mesh
  * may be added later.
  * Also, if there is already a helper with this name, it will be overwritten
  * without warning.
  *
  * @param strMeshName The string that will be associated with the ID.
  * @param coreMeshId The core ID number of the mesh to be referenced by the string.
  *****************************************************************************/

bool CalCoreModel::addMeshName(const std::string& strMeshName, int coreMeshId)
{
  // check if the core mesh id is valid
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_vectorCoreMesh[ coreMeshId ]->setName(strMeshName);
  m_meshName[ strMeshName ] = coreMeshId;
  return true;
}

 /*****************************************************************************/
/** Retrieves the ID of the core mesh referenced by a string
  *
  * This function returns a core mesh ID
  *
  * @param strMeshName A string that is associated with a core mesh ID number.
  * @return Returns:
  *         \li \b -1 if there is no core mesh ID associated with the input string
  *         \li \b the core ID number of the mesh asssociated with the input string
  *****************************************************************************/

int CalCoreModel::getCoreMeshId(const std::string& strMeshName) const
{
  if (m_meshName.count( strMeshName ) < 1)
  {
    return -1;
  }

  /*
  DJMC note: We cannot simply do the following:

  m_meshName[strMeshName];

  and still remain a const method since the [] operation might
  add a setting if it is not found inside. We must seek an
  alternative.

  Note, according to http://www.sgi.com/tech/stl/Map.html, m[k]
  should be equivalent to:

  (*((m.insert(value_type(k, data_type()))).first)).second

  ...thus, we should be able to find a read-only analogue by
  changing the line from an insert to a find, so that we may
  remain a const method.
  //*/

  int meshID;
  std::map<std::string, int>::const_iterator i = m_meshName.find(strMeshName);
  if (i != m_meshName.end())
  {
    meshID = (*i).second;
  }
  else
  {
    // if control flows here, entry does not exist in map,
    // and therefore there has been an error
    return -1;
  }

  if (getCoreMesh(meshID) == NULL)
  {
    return -1;
  }

  return meshID;
}

 /*****************************************************************************/
/** Scale the core model.
  *
  * This function rescale all data that are in the core model instance
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreModel::scale(float factor)
{
   if(m_pCoreSkeleton)
   {
      m_pCoreSkeleton->scale(factor);

      for(unsigned int animationId = 0; animationId < m_vectorCoreAnimation.size(); animationId++)
      {
         if( m_vectorCoreAnimation[animationId] )
         {
            m_vectorCoreAnimation[animationId]->scale(factor);
         }
      }

      for(size_t meshId = 0; meshId < m_vectorCoreMesh.size(); meshId++)
      {
         if(m_vectorCoreMesh[meshId])
         {
            m_vectorCoreMesh[meshId]->scale(factor);
         }
      }
   }
}

//****************************************************************************//

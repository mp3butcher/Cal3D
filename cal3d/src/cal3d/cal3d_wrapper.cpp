//****************************************************************************//
// cal3d_wrapper.cpp                                                          //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
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
// Defines                                                                    //
//****************************************************************************//

#define CAL3D_WRAPPER_EXPORTS

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/cal3d.h"
#include "cal3d/cal3d_wrapper.h"
using namespace cal3d;

//****************************************************************************//
// CalAnimation wrapper functions definition                                  //
//****************************************************************************//

void CalAnimation_Delete(CalAnimation *self)
{
  delete self;
}

CalCoreAnimation *CalAnimation_GetCoreAnimation(CalAnimation *self)
{
  return self->getCoreAnimation();
}

CalAnimationState CalAnimation_GetState(CalAnimation *self)
{
  return (CalAnimationState)(self->getState());
}

float CalAnimation_GetTime(CalAnimation *self)
{
  return self->getTime();
}

CalAnimationType CalAnimation_GetType(CalAnimation *self)
{
  return (CalAnimationType)(self->getType());
}

float CalAnimation_GetWeight(CalAnimation *self)
{
  return self->getWeight();
}

//****************************************************************************//
// CalAnimationAction wrapper functions definition                            //
//****************************************************************************//

void CalAnimationAction_Delete(CalAnimationAction *self)
{
  delete self;
}

CalBoolean CalAnimationAction_Execute(CalAnimationAction *self, float delayIn, float delayOut)
{
  return self->execute(delayIn, delayOut) ? True : False;
}

CalAnimationAction *CalAnimationAction_New(CalCoreAnimation *pCoreAnimation)
{
  return new(std::nothrow) CalAnimationAction(pCoreAnimation);
}

CalBoolean CalAnimationAction_Update(CalAnimationAction *self, float deltaTime)
{
  return self->update(deltaTime) ? True : False;
}

void CalCoreAnimation_Scale(CalCoreAnimation *self, float factor)
{
  self->scale(factor);
}


//****************************************************************************//
// CalAnimationCycle wrapper functions definition                             //
//****************************************************************************//

CalBoolean CalAnimationCycle_Blend(CalAnimationCycle *self, float weight, float delay)
{
  return self->blend(weight, delay) ? True : False;
}

void CalAnimationCycle_Delete(CalAnimationCycle *self)
{
  delete self;
}

CalAnimationCycle *CalAnimationCycle_New(CalCoreAnimation *pCoreAnimation)
{
  return new(std::nothrow) CalAnimationCycle(pCoreAnimation);
}

void CalAnimationCycle_SetAsync(CalAnimationCycle *self, float time, float duration)
{
  self->setAsync(time, duration);
}

CalBoolean CalAnimationCycle_Update(CalAnimationCycle *self, float deltaTime)
{
  return self->update(deltaTime) ? True : False;
}

//****************************************************************************//
// CalBone wrapper functions definition                                           //
//****************************************************************************//

void CalBone_BlendState(CalBone *self, float weight, CalVector *pTranslation, CalQuaternion *pRotation)
{
  self->blendState(weight, *pTranslation, *pRotation);
}

void CalBone_CalculateState(CalBone *self)
{
  self->calculateState();
}

void CalBone_ClearState(CalBone *self)
{
  self->clearState();
}

void CalBone_Delete(CalBone *self)
{
  delete self;
}

CalCoreBone *CalBone_GetCoreBone(CalBone *self)
{
  return self->getCoreBone();
}

CalQuaternion *CalBone_GetRotation(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotation()));
}

CalQuaternion *CalBone_GetRotationAbsolute(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationAbsolute()));
}

CalQuaternion *CalBone_GetRotationBoneSpace(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationBoneSpace()));
}

CalVector *CalBone_GetTranslation(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslation()));
}

CalVector *CalBone_GetTranslationAbsolute(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationAbsolute()));
}

CalVector *CalBone_GetTranslationBoneSpace(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationBoneSpace()));
}

void CalBone_LockState(CalBone *self)
{
  self->lockState();
}

CalBone *CalBone_New(CalCoreBone* coreBone)
{
  return new(std::nothrow) CalBone(coreBone);
}

void CalBone_SetSkeleton(CalBone *self, CalSkeleton *pSkeleton)
{
  self->setSkeleton(pSkeleton);
}

void CalBone_SetRotation(CalBone *self, CalQuaternion *pRotation)
{
  self->setRotation(*pRotation);
}

void CalBone_SetTranslation(CalBone *self, CalVector *pTranslation)
{
  self->setTranslation(*pTranslation);
}

void CalBone_SetCoreState(CalBone *self)
{
  self->setCoreState();
}

void CalBone_SetCoreStateRecursive(CalBone *self)
{
  self->setCoreStateRecursive();
}

CalBoolean CalBone_GetBoundingBox( struct CalBone *self, struct CalCoreModel* model,
  													float* outEightPoints )
{
	CalBoolean	gotBounds = False;
	
	CalCoreBone*	coreBone = self->getCoreBone();
	
	if (!coreBone->isBoundingBoxPrecomputed())
	{
		coreBone->calculateBoundingBox( model );
	}
	
	// If a bone owns no vertices, then the box of the core bone will stay at its initialized value,
	// in which the d members are -1e32.
	CalBoundingBox&	coreBox( coreBone->getBoundingBox() );
	if (fabsf(coreBox.plane[0].d) < 1e7)
	{
		self->calculateBoundingBox();

		CalBoundingBox&	box( self->getBoundingBox() );
		box.computePoints( reinterpret_cast<CalVector*>(outEightPoints) );
		gotBounds = True;
	}
	
	return gotBounds;
}


//****************************************************************************//
// CalCoreAnimation wrapper functions definition                              //
//****************************************************************************//

CalCoreAnimation *CalCoreAnimation_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreAnimation());
}

void CalCoreAnimation_Delete(CalCoreAnimation* self)
{
    explicitDecRef(self);
}

float CalCoreAnimation_GetDuration(CalCoreAnimation *self)
{
  return self->getDuration();
}

void CalCoreAnimation_SetDuration(CalCoreAnimation *self, float duration)
{
  self->setDuration(duration);
}

//****************************************************************************//
// CalCoreBone wrapper functions definition                                   //
//****************************************************************************//

CalBoolean CalCoreBone_AddChildId(CalCoreBone *self, int childId)
{
  return self->addChildId(childId) ? True : False;
}

void CalCoreBone_CalculateState(CalCoreBone *self)
{
  self->calculateState();
}

void CalCoreBone_Delete(CalCoreBone *self)
{
  delete self;
}

/*
std::list<int>& CalCoreBone_GetListChildId(CalCoreBone *self)
{
  return self->getListChildId();
}
*/

const char *CalCoreBone_GetName(CalCoreBone *self)
{
  return self->getName().c_str();
}

void CalCoreBone_SetName(struct CalCoreBone *self, const char* name )
{
	try
	{
		self->setName( name );
	}
	catch (...)
	{
	}
}

int CalCoreBone_GetParentId(CalCoreBone *self)
{
  return self->getParentId();
}

int CalCoreBone_GetChildCount( struct CalCoreBone *self )
{
	return self->getListChildId().size();
}

int CalCoreBone_GetChildId( struct CalCoreBone *self, int childIndex )
{
	int	childBoneID = -1;
	std::list<int>&		childBoneIDs( self->getListChildId() );
	if (childIndex < childBoneIDs.size())
	{
		std::list<int>::iterator i = childBoneIDs.begin();
		while (childIndex > 0)
		{
			++i;
			--childIndex;
		}
		childBoneID = *i;
	}
	return childBoneID;
}

CalQuaternion *CalCoreBone_GetRotation(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotation()));
}

CalQuaternion *CalCoreBone_GetRotationAbsolute(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationAbsolute()));
}

CalQuaternion *CalCoreBone_GetRotationBoneSpace(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationBoneSpace()));
}

CalVector *CalCoreBone_GetTranslation(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslation()));
}

CalVector *CalCoreBone_GetTranslationAbsolute(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationAbsolute()));
}

CalVector *CalCoreBone_GetTranslationBoneSpace(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationBoneSpace()));
}

CalUserData CalCoreBone_GetUserData(CalCoreBone *self)
{
  return self->getUserData();
}

CalCoreBone *CalCoreBone_New(const char* name)
{
  return new(std::nothrow) CalCoreBone(name);
}

void CalCoreBone_SetCoreSkeleton(CalCoreBone *self, CalCoreSkeleton *pCoreSkeleton)
{
  self->setCoreSkeleton(pCoreSkeleton);
}

void CalCoreBone_SetParentId(CalCoreBone *self, int parentId)
{
  self->setParentId(parentId);
}

void CalCoreBone_SetRotation(CalCoreBone *self, CalQuaternion *pRotation)
{
  self->setRotation(*pRotation);
}

void CalCoreBone_SetRotationBoneSpace(CalCoreBone *self, CalQuaternion *pRotation)
{
  self->setRotationBoneSpace(*pRotation);
}

void CalCoreBone_SetTranslation(CalCoreBone *self, CalVector *pTranslation)
{
  self->setTranslation(*pTranslation);
}

void CalCoreBone_SetTranslationBoneSpace(CalCoreBone *self, CalVector *pTranslation)
{
  self->setTranslationBoneSpace(*pTranslation);
}

void CalCoreBone_SetUserData(CalCoreBone *self, CalUserData userData)
{
  self->setUserData(userData);
}

CalBoolean CalCoreBone_GetBoundingBox( struct CalCoreBone *self, struct CalCoreModel* model,
  								float* outEightPoints )
{
	CalBoolean	gotBounds = False;
	
	if (! self->isBoundingBoxPrecomputed())
	{
		self->calculateBoundingBox( model );
	}
	
	CalBoundingBox&	box( self->getBoundingBox() );
	
	// If a bone owns no vertices, then the box will stay at its initialized value,
	// in which the d members are -1e32.
	if (box.plane[0].d > -1e31)
	{
		box.computePoints( reinterpret_cast<CalVector*>(outEightPoints) );
		gotBounds = True;
	}
	
	return gotBounds;
}

//****************************************************************************//
// CalCoreMaterial wrapper functions definition                               //
//****************************************************************************//

CalCoreMaterial *CalCoreMaterial_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreMaterial());
}

void CalCoreMaterial_Delete(CalCoreMaterial *self)
{
    explicitDecRef(self);
}

/*
CalCoreMaterial::Color *CalCoreMaterial_GetAmbientColor(CalCoreMaterial *self)
{
  return &(self->getAmbientColor());
}
*/

/*
CalCoreMaterial::Color *CalCoreMaterial_GetDiffuseColor(CalCoreMaterial *self)
{
  return &(self->getDiffuseColor());
}
*/

static CalColorValue	ColorToValue( const CalCoreMaterial::Color& inColor )
{
	CalColorValue	theValue = (((CalColorValue) inColor.red) << 24) |
		(((CalColorValue) inColor.green) << 16) |
		(((CalColorValue) inColor.blue) << 8) |
		(((CalColorValue) inColor.alpha) << 0);
	return theValue;
}

static CalCoreMaterial::Color ValueToColor( CalColorValue inValue )
{
	CalCoreMaterial::Color	theColor = {
		inValue >> 24,
		(inValue & 0x00FF0000) >> 16,
		(inValue & 0x0000FF00) >> 8,
		(inValue & 0x000000FF)
	};
	return theColor;
}

CalColorValue CalCoreMaterial_GetAmbientColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getAmbientColor() );
}

CalColorValue CalCoreMaterial_GetDiffuseColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getDiffuseColor() );
}

CalColorValue CalCoreMaterial_GetSpecularColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getSpecularColor() );
}

void CalCoreMaterial_SetAmbientColor(struct CalCoreMaterial *self, CalColorValue pAmbientColor)
{
	self->setAmbientColor( ValueToColor( pAmbientColor ) );
}

void CalCoreMaterial_SetDiffuseColor(struct CalCoreMaterial *self, CalColorValue pDiffuseColor)
{
	self->setDiffuseColor( ValueToColor( pDiffuseColor ) );
}

void CalCoreMaterial_SetSpecularColor(struct CalCoreMaterial *self, CalColorValue pSpecularColor)
{
	self->setSpecularColor( ValueToColor( pSpecularColor ) );
}

const char* CalCoreMaterial_GetName(struct CalCoreMaterial *self)
{
	return self->getName().c_str();
}

void CalCoreMaterial_SetName(struct CalCoreMaterial *self, const char* inName)
{
	try
	{
		self->setName( std::string(inName) );
	}
	catch (...)
	{
	}
}

int CalCoreMaterial_GetMapCount(CalCoreMaterial *self)
{
  return self->getMapCount();
}

const char *CalCoreMaterial_GetMapFilename(CalCoreMaterial *self, int mapId)
{
  return self->getMapFilename(mapId).c_str();
}

CalUserData CalCoreMaterial_GetMapUserData(CalCoreMaterial *self, int mapId)
{
  return self->getMapUserData(mapId);
}

float CalCoreMaterial_GetShininess(CalCoreMaterial *self)
{
  return self->getShininess();
}

/*
CalCoreMaterial::Color *CalCoreMaterial_GetSpecularColor(CalCoreMaterial *self)
{
  return &(self->getSpecularColor());
}
*/

CalUserData CalCoreMaterial_GetUserData(CalCoreMaterial *self)
{
  return self->getUserData();
}

/*
std::vector<Map>& CalCoreMaterial_GetVectorMap(CalCoreMaterial *self)
{
  return self->getVectorMap();
}
*/

CalBoolean CalCoreMaterial_Reserve(CalCoreMaterial *self, int mapCount)
{
  return self->reserve(mapCount) ? True : False;
}

/*
void CalCoreMaterial_SetAmbientColor(CalCoreMaterial *self, CalCoreMaterial::Color *pAmbientColor)
{
  return self->setAmbientColor(*pAmbientColor);
}
*/

/*
void CalCoreMaterial_SetDiffuseColor(CalCoreMaterial *self, CalCoreMaterial::Color *pDiffuseColor)
{
  self->setDiffuseColor(*pDiffuseColor);
}
*/

/*
CalBoolean CalCoreMaterial_SetMap(CalCoreMaterial *self, int mapId, CalCoreMaterial::Map *pMap)
{
  return self->setMap(mapId, *pMap) ? True : False;
}
*/

CalBoolean CalCoreMaterial_SetMapUserData(CalCoreMaterial *self, int mapId, CalUserData userData)
{
  return self->setMapUserData(mapId, userData) ? True : False;
}

void CalCoreMaterial_SetShininess(CalCoreMaterial *self, float shininess)
{
  self->setShininess(shininess);
}

/*
void CalCoreMaterial_SetSpecularColor(CalCoreMaterial *self, CalCoreMaterial::Color *pSpecularColor)
{
  self->setSpecularColor(*pSpecularColor);
}
*/

void CalCoreMaterial_SetUserData(CalCoreMaterial *self, CalUserData userData)
{
  self->setUserData(userData);
}

//****************************************************************************//
// CalCoreMesh wrapper functions definition                                   //
//****************************************************************************//

CalCoreMesh *CalCoreMesh_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreMesh());
}

void CalCoreMesh_Delete(CalCoreMesh *self)
{
    explicitDecRef(self);
}

int CalCoreMesh_AddCoreSubmesh(CalCoreMesh *self, CalCoreSubmesh *pCoreSubmesh)
{
	try
	{
		return self->addCoreSubmesh(pCoreSubmesh);
	}
	catch (...)
	{
	}
	return -1;
}

void CalCoreMesh_RemoveCoreSubmesh( struct CalCoreMesh *self, int coreSubmeshID )
{
	self->removeCoreSubmesh( coreSubmeshID );
}

const char* CalCoreMesh_GetName(struct CalCoreMesh *self)
{
	return self->getName().c_str();
}

void CalCoreMesh_SetName(struct CalCoreMesh *self, const char* inName)
{
	try
	{
		self->setName( std::string(inName) );
	}
	catch (...)
	{
	}
}

CalCoreSubmesh *CalCoreMesh_GetCoreSubmesh(CalCoreMesh *self, int id)
{
  return self->getCoreSubmesh(id);
}

int CalCoreMesh_GetCoreSubmeshCount(CalCoreMesh *self)
{
  return self->getCoreSubmeshCount();
}

/*
std::vector<CalCoreSubmesh *>& CalCoreMesh_GetVectorCoreSubmesh(CalCoreMesh *self)
{
  return self->getVectorCoreSubmesh();
}
*/

void CalCoreMesh_Scale(CalCoreMesh *self,float factor)
{
  self->scale(factor);
}

int CalCoreMesh_AddAsMorphTarget(struct CalCoreMesh *self, struct CalCoreMesh *target)
{
	try
	{
		return self->addAsMorphTarget(target, target->getName());
	}
	catch (...)
	{
	}
	return -1;
}

//****************************************************************************//
// CalCoreModel wrapper functions definition                                  //
//****************************************************************************//

int CalCoreModel_AddCoreAnimation(CalCoreModel *self, CalCoreAnimation *pCoreAnimation)
{
	try
	{
		return self->addCoreAnimation(pCoreAnimation);
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_AddCoreMaterial(CalCoreModel *self, CalCoreMaterial *pCoreMaterial)
{
	try
	{
		return self->addCoreMaterial(pCoreMaterial);
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_AddCoreMesh(CalCoreModel *self, CalCoreMesh *pCoreMesh)
{
	try
	{
		return self->addCoreMesh(pCoreMesh);
	}
	catch (...)
	{
	}
	return -1;
}

CalBoolean CalCoreModel_CreateCoreMaterialThread(CalCoreModel *self, int coreMaterialThreadId)
{
	try
	{
		return self->createCoreMaterialThread(coreMaterialThreadId) ? True : False;
	}
	catch (...)
	{
	}
	return False;
}

void CalCoreModel_Delete(CalCoreModel *self)
{
  delete self;
}

CalCoreAnimation *CalCoreModel_GetCoreAnimation(CalCoreModel *self, int coreAnimationId)
{
  return self->getCoreAnimation(coreAnimationId);
}

int CalCoreModel_GetCoreAnimationCount(CalCoreModel *self)
{
  return self->getCoreAnimationCount();
}

CalCoreMaterial *CalCoreModel_GetCoreMaterial(CalCoreModel *self, int coreMaterialId)
{
  return self->getCoreMaterial(coreMaterialId);
}

int CalCoreModel_GetCoreMaterialCount(CalCoreModel *self)
{
  return self->getCoreMaterialCount();
}

int CalCoreModel_GetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId)
{
  return self->getCoreMaterialId(coreMaterialThreadId, coreMaterialSetId);
}

CalCoreMesh *CalCoreModel_GetCoreMesh(CalCoreModel *self, int coreMeshId)
{
  return self->getCoreMesh(coreMeshId);
}

int CalCoreModel_GetCoreMeshCount(CalCoreModel *self)
{
  return self->getCoreMeshCount();
}

int CalCoreModel_GetCoreMeshId(struct CalCoreModel *self, const char* coreMeshName )
{
	int	theID = -1;
	try
	{
		std::string	theName( coreMeshName );
		theID = self->getCoreMeshId( theName );
	}
	catch (...)
	{
	}
	return theID;
}

CalCoreSkeleton *CalCoreModel_GetCoreSkeleton(CalCoreModel *self)
{
  return self->getCoreSkeleton();
}

CalUserData CalCoreModel_GetUserData(CalCoreModel *self)
{
  return self->getUserData();
}


CalBoolean CalCoreModel_AddMeshName(struct CalCoreModel *self, const char* name, int coreMeshId )
{
	CalBoolean	success = False;
	
	try
	{
		std::string		meshName( name );
		if (self->addMeshName( meshName, coreMeshId ))
		{
			success = True;
		}
	}
	catch (...)
	{
	}
	
	return success;
}

void CalCoreModel_CloneCoreMaterials(struct CalCoreModel *self )
{
	try
	{
		self->cloneCoreMaterials();
	}
	catch (...)
	{
	}
}

int CalCoreModel_LoadCoreAnimation(CalCoreModel *self, const char *strFilename)
{
	try
	{
		return self->loadCoreAnimation(strFilename);
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_LoadCoreAnimationFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreAnimation(const_cast<void*>(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_LoadCoreMaterial(CalCoreModel *self, const char *strFilename)
{
	try
	{
		return self->loadCoreMaterial(strFilename);
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_LoadCoreMaterialFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreMaterial(const_cast<void*>(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_LoadCoreMesh(CalCoreModel *self, const char *strFilename)
{
	try
	{
		return self->loadCoreMesh(strFilename);
	}
	catch (...)
	{
	}
	return -1;
}

int CalCoreModel_LoadCoreMeshFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreMesh(const_cast<void*>(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

CalBoolean CalCoreModel_LoadCoreSkeleton(CalCoreModel *self, const char *strFilename)
{
	try
	{
		return self->loadCoreSkeleton(strFilename) ? True : False;
	}
	catch (...)
	{
	}
	return False;
}

CalBoolean CalCoreModel_LoadCoreSkeletonFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreSkeleton(const_cast<void*>(buffer)) ? True : False;
	}
	catch (...)
	{
	}
	return False;
}

CalCoreModel *CalCoreModel_New(const char* name)
{
  return new(std::nothrow) CalCoreModel(name);
}

CalCoreModel *CalCoreModel_NewCopy( CalCoreModel* other )
{
  return new(std::nothrow) CalCoreModel(*other);
}

void CalCoreModel_ReplaceCoreMesh(struct CalCoreModel *self, int coreMeshId, struct CalCoreMesh *pCoreMesh)
{
	self->replaceCoreMesh( coreMeshId, pCoreMesh );
}

CalBoolean CalCoreModel_SaveCoreAnimation(CalCoreModel *self, const char *strFilename, int coreAnimationId)
{
  return self->saveCoreAnimation(strFilename, coreAnimationId) ? True : False;
}

CalBoolean CalCoreModel_SaveCoreMaterial(CalCoreModel *self, const char *strFilename, int coreMaterialId)
{
  return self->saveCoreMaterial(strFilename, coreMaterialId) ? True : False;
}

CalBoolean CalCoreModel_SaveCoreMesh(CalCoreModel *self, const char *strFilename, int coreMeshId)
{
  return self->saveCoreMesh(strFilename, coreMeshId) ? True : False;
}

CalBoolean CalCoreModel_SaveCoreSkeleton(CalCoreModel *self, const char *strFilename)
{
  return self->saveCoreSkeleton(strFilename) ? True : False;
}

CalBoolean CalCoreModel_SetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId)
{
  return self->setCoreMaterialId(coreMaterialThreadId, coreMaterialSetId, coreMaterialId) ? True : False;
}

void CalCoreModel_SetCoreSkeleton(CalCoreModel *self, CalCoreSkeleton *pCoreSkeleton)
{
  self->setCoreSkeleton(pCoreSkeleton);
}

void CalCoreModel_SetUserData(CalCoreModel *self, CalUserData userData)
{
  self->setUserData(userData);
}

const char *CalCoreModel_GetName(CalCoreModel *self)
{
  return self->getName().c_str();
}

void CalCoreModel_SetName(struct CalCoreModel *self, const char* inName)
{
	try
	{
		self->setName( inName );
	}
	catch (...)
	{
	}
}


//****************************************************************************//
// CalCoreSkeleton wrapper functions definition                               //
//****************************************************************************//

int CalCoreSkeleton_AddCoreBone(CalCoreSkeleton *self, CalCoreBone *pCoreBone)
{
	try
	{
		return self->addCoreBone(pCoreBone);
	}
	catch (...)
	{
	}
	return -1;
}

void CalCoreSkeleton_CalculateState(CalCoreSkeleton *self)
{
  self->calculateState();
}


CalCoreSkeleton *CalCoreSkeleton_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreSkeleton());
}

void CalCoreSkeleton_Delete(CalCoreSkeleton *self)
{
    explicitDecRef(self);
}

CalCoreBone *CalCoreSkeleton_GetCoreBone(CalCoreSkeleton *self, int coreBoneId)
{
  return self->getCoreBone(coreBoneId);
}

int CalCoreSkeleton_GetCoreBoneCount(CalCoreSkeleton *self)
{
	return self->getVectorCoreBone().size();
}

int CalCoreSkeleton_GetCoreBoneId(CalCoreSkeleton *self, const char *strName)
{
  return self->getCoreBoneId(strName);
}

/*
std::list<int>& CalCoreSkeleton_GetListRootCoreBoneId(CalCoreSkeleton *self)
{
  return self->getlistRootCoreBoneId();
}
*/


/*
std::vector<CalCoreBone *>& CalCoreSkeleton_GetVectorCoreBone(CalCoreSkeleton *self)
{
  return self->getVectorCoreBone();
}
*/

void CalCoreSkeleton_Scale(CalCoreSkeleton *self,float factor)
{
	self->scale(factor);
}

CalBoolean CalCoreSkeleton_MapCoreBoneName( struct CalCoreSkeleton *self, int coreBoneID,
										const char* name )
{
	return static_cast<CalBoolean>( self->mapCoreBoneName( coreBoneID, name ) );
}

int CalCoreSkeleton_GetRootCoreBoneCount(CalCoreSkeleton *self)
{
	return self->getVectorRootCoreBoneId().size();
}

CalCoreBone *CalCoreSkeleton_GetRootCoreBone(CalCoreSkeleton *self, int rootBoneIndex)
{
	CalCoreBone*	theBone = NULL;
	std::vector<int>&	rootIDs( self->getVectorRootCoreBoneId() );
	if ( (rootBoneIndex >= 0) && (rootBoneIndex < rootIDs.size()) )
	{
		int	boneID = rootIDs[ rootBoneIndex ];
		theBone = CalCoreSkeleton_GetCoreBone( self, boneID );
	}
	return theBone;
}

void CalCoreSkeleton_CalculateBoundingBoxes( struct CalCoreModel* coreModel )
{
	CalCoreSkeleton*	coreSkel = CalCoreModel_GetCoreSkeleton( coreModel );
	coreSkel->calculateBoundingBoxes( coreModel );
}

//****************************************************************************//
// CalCoreSubmesh wrapper functions definition                                //
//****************************************************************************//

void CalCoreSubmesh_Delete(CalCoreSubmesh *self)
{
  delete self;
}

int CalCoreSubmesh_GetCoreMaterialThreadId(CalCoreSubmesh *self)
{
  return self->getCoreMaterialThreadId();
}

int CalCoreSubmesh_GetFaceCount(CalCoreSubmesh *self)
{
  return self->getFaceCount();
}

int CalCoreSubmesh_GetLodCount(CalCoreSubmesh *self)
{
  return self->getLodCount();
}

int CalCoreSubmesh_GetSpringCount(CalCoreSubmesh *self)
{
  return self->getSpringCount();
}

/*
std::vector<CalCoreSubmesh::Face>& CalCoreSubmesh_GetVectorFace(CalCoreSubmesh *self)
{
  return self->getVectorFace();
}
*/

/*
std::vector<CalCoreSubmesh::PhysicalProperty>& CalCoreSubmesh_GetVectorPhysicalProperty(CalCoreSubmesh *self)
{
  return self->getVectorPhysicalProperty();
}
*/

/*
std::vector<CalCoreSubmesh::Spring>& CalCoreSubmesh_GetVectorSpring(CalCoreSubmesh *self)
{
  return self->getVectorSpring();
}
*/

/*
std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& CalCoreSubmesh_GetVectorVectorTextureCoordinate(CalCoreSubmesh *self)
{
  return self->getVectorVectorTextureCoordinate();
}
*/

/*
std::vector<CalCoreSubmesh::Vertex>& CalCoreSubmesh_GetVectorVertex(CalCoreSubmesh *self)
{
  return self->getVectorVertex();
}
*/

int CalCoreSubmesh_GetVertexCount(CalCoreSubmesh *self)
{
  return self->getVertexCount();
}

int CalCoreSubmesh_GetVertexInfluenceCount( struct CalCoreSubmesh *self, int vertID )
{
	return self->getVectorVertex()[ vertID ].vectorInfluence.size();
}

int CalCoreSubmesh_GetVertexInfluence( struct CalCoreSubmesh *self, int vertID,
									int influenceID, float* outWeight )
{
	CalCoreSubmesh::Influence&	theInfluence =
		self->getVectorVertex()[ vertID ].vectorInfluence[ influenceID ];
	
	*outWeight = theInfluence.weight;
	return theInfluence.boneId;
}

CalCoreSubmesh *CalCoreSubmesh_New()
{
  return new(std::nothrow) CalCoreSubmesh();
}

void CalCoreSubmesh_GetVertex( struct CalCoreSubmesh* self, int vertID, float* outPosition, float* outNormal )
{
	std::vector<CalCoreSubmesh::Vertex>&	vertices( self->getVectorVertex() );
	if ( (vertID >= 0) && (vertID < vertices.size()) )
	{
		CalCoreSubmesh::Vertex&		theVertex( vertices[ vertID ] );
		
		outPosition[0] = theVertex.position.x;
		outPosition[1] = theVertex.position.y;
		outPosition[2] = theVertex.position.z;

		outNormal[0] = theVertex.normal.x;
		outNormal[1] = theVertex.normal.y;
		outNormal[2] = theVertex.normal.z;
	}
}

CalBoolean CalCoreSubmesh_Reserve(CalCoreSubmesh *self, int vertexCount, int textureCoordinateCount, int faceCount, int springCount)
{
  return self->reserve(vertexCount, textureCoordinateCount, faceCount, springCount) ? True : False;
}

void CalCoreSubmesh_SetCoreMaterialThreadId(CalCoreSubmesh *self, int coreMaterialThreadId)
{
  self->setCoreMaterialThreadId(coreMaterialThreadId);
}

/*
CalBoolean CalCoreSubmesh_SetFace(CalCoreSubmesh *self, int faceId, CalCoreSubmesh::Face *pFace)
{
  return self->setFace(faceId, *pFace) ? True : False;
}
*/

void CalCoreSubmesh_SetLodCount(CalCoreSubmesh *self, int lodCount)
{
  self->setLodCount(lodCount);
}

/*
CalBoolean CalCoreSubmesh_SetPhysicalProperty(CalCoreSubmesh *self, int vertexId, CalCoreSubmesh::PhysicalProperty *pPhysicalProperty)
{
  return self->setPhysicalProperty(vertexId, *pPhysicalProperty) ? True : False;
}
*/

/*
CalBoolean CalCoreSubmesh_SetSpring(CalCoreSubmesh *self, int springId, CalCoreSubmesh::Spring *pSpring)
{
  return self->setSpring(springId, *pSpring) ? True : False;
}
*/

/*
CalBoolean CalCoreSubmesh_SetTextureCoordinate(CalCoreSubmesh *self, int vertexId, int textureCoordinateId, CalCoreSubmesh::TextureCoordinate *pTextureCoordinate)
{
  return self->setTextureCoordinate(vertexId, textureCoordinateId, *pTextureCoordinate) ? True : False;
}
*/

/*
CalBoolean CalCoreSubmesh_SetVertex(CalCoreSubmesh *self, int vertexId, CalCoreSubmesh::Vertex *pVertex)
{
  return self->setVertex(vertexId, *pVertex) ? True : False;
}
*/

CalBoolean CalCoreSubmesh_IsTangentsEnabled(CalCoreSubmesh *self, int mapId)
{
  return self->isTangentsEnabled(mapId) ? True : False;
}

CalBoolean CalCoreSubmesh_EnableTangents(struct CalCoreSubmesh *self, int mapId, enum CalBoolean enabled)
{
  return self->enableTangents(mapId, enabled) ? True : False;
}

int CalCoreSubmesh_AddSubMorphTarget( CalCoreSubmesh *self, CalCoreSubMorphTargetDiffMap* inTarget )
{
	int	morphTargetID = -1;
	try
	{
		morphTargetID = self->addCoreSubMorphTarget( inTarget );
	}
	catch (...)
	{
	}
	return morphTargetID;
}


const CalIndex* CalCoreSubmesh_GetFaces( struct CalCoreSubmesh *self )
{
	return &self->getVectorFace()[0].vertexId[0];
}


//****************************************************************************//
// CalCoreSubMorphTargetDiffMap wrapper functions definition                  //
//****************************************************************************//
CalCoreSubMorphTargetDiffMap* CalCoreSubMorphTargetDiffMap_New()
{
	CalCoreSubMorphTargetDiffMap*	theMap = NULL;
	try
	{
		theMap = new CalCoreSubMorphTargetDiffMap;
	}
	catch (...)
	{
	}
	return theMap;
}

CalCoreSubMorphTargetDiffMap* CalCoreSubMorphTargetDiffMap_Clone(
	const CalCoreSubMorphTargetDiffMap* inOther )
{
	CalCoreSubMorphTargetDiffMap*	theMap = NULL;
	try
	{
		theMap = new CalCoreSubMorphTargetDiffMap( *inOther );
	}
	catch (...)
	{
	}
	return theMap;
}

void CalCoreSubMorphTargetDiffMap_Delete( CalCoreSubMorphTargetDiffMap* inSelf )
{
	delete inSelf;
}

bool CalCoreSubMorphTargetDiffMap_Reserve( CalCoreSubMorphTargetDiffMap* inSelf,
	int inNumDifferences )
{
	return inSelf->reserve( inNumDifferences );
}

bool CalCoreSubMorphTargetDiffMap_AppendVertex( CalCoreSubMorphTargetDiffMap* inSelf,
	int inVertexID, float* inPositionOffset, float* inNormalOffset )
{
	CalCoreSubMorphTarget::BlendVertex	theVertex;
	theVertex.position.set( inPositionOffset[0], inPositionOffset[1], inPositionOffset[2] );
	theVertex.normal.set( inNormalOffset[0], inNormalOffset[1], inNormalOffset[2] );
	
	return inSelf->appendBlendVertex( inVertexID, theVertex );
}


//****************************************************************************//
// CalError wrapper functions definition                                      //
//****************************************************************************//

CalErrorCode CalError_GetLastErrorCode()
{
  return (CalErrorCode)CalError::getLastErrorCode();
}

const char *CalError_GetLastErrorDescription()
{
  static std::string strDescription;
  strDescription = CalError::getLastErrorDescription();

  return strDescription.c_str();
}

const char *CalError_GetLastErrorFile()
{
  return CalError::getLastErrorFile().c_str();
}

int CalError_GetLastErrorLine()
{
  return CalError::getLastErrorLine();
}

const char *CalError_GetLastErrorText()
{
  return CalError::getLastErrorText().c_str();
}

void CalError_PrintLastError()
{
  CalError::printLastError();
}

/*
void CalError_SetLastError(CalErrorCode code, char *strFile, int line, char *strText)
{
  CalError::setLastError((CalError::Code)code, strFile, line, strText);
}
*/

//****************************************************************************//
// CalLoader wrapper functions definition                                     //
//****************************************************************************//

void CalLoader_Delete(CalLoader *self)
{
  delete self;
}

CalCoreAnimation *CalLoader_LoadCoreAnimation(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreAnimation(strFilename).get());
}

CalCoreMaterial *CalLoader_LoadCoreMaterial(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreMaterial(strFilename).get());
}

CalCoreMesh *CalLoader_LoadCoreMesh(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreMesh(strFilename).get());
}

CalCoreMesh *CalLoader_LoadCoreMeshFromBuffer(const void *data)
{
	try
	{
		CalCoreMeshPtr	theMesh( CalLoader::loadCoreMesh(const_cast<void*>(data)) );
		
		if (theMesh.get() != NULL)
		{
			explicitIncRef( theMesh.get() );
		}
		return theMesh.get();
	}
	catch (...)
	{
	}
	return 0;
}

CalCoreSkeleton *CalLoader_LoadCoreSkeleton(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreSkeleton(strFilename).get());
}

CalLoader *CalLoader_New()
{
  return new CalLoader();
}

void CalLoader_SetLoadingMode( int flags )
{
	CalLoader::setLoadingMode( flags );
}

//****************************************************************************//
// CalMesh wrapper functions definition                                       //
//****************************************************************************//

void CalMesh_Delete(CalMesh *self)
{
  delete self;
}

CalCoreMesh *CalMesh_GetCoreMesh(CalMesh *self)
{
  return self->getCoreMesh();
}

CalSubmesh *CalMesh_GetSubmesh(CalMesh *self, int id)
{
  return self->getSubmesh(id);
}

int CalMesh_GetSubmeshCount(CalMesh *self)
{
  return self->getSubmeshCount();
}

/*
std::vector<CalSubmesh *>& CalMesh_GetVectorSubmesh(CalMesh *self)
{
  return self->getVectorSubmesh();
}
*/

CalMesh *CalMesh_New(CalCoreMesh *pCoreMesh)
{
  return new(std::nothrow) CalMesh(pCoreMesh);
}

void CalMesh_SetLodLevel(CalMesh *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

void CalMesh_SetMaterialSet(CalMesh *self, int setId)
{
  self->setMaterialSet(setId);
}

void CalMesh_SetModel(CalMesh *self, CalModel *pModel)
{
  self->setModel(pModel);
}

//****************************************************************************//
// CalMixer wrapper functions definition                                      //
//****************************************************************************//

CalBoolean CalMixer_BlendCycle(CalMixer *self, int id, float weight, float delay)
{
  return self->blendCycle(id, weight, delay) ? True : False;
}

CalBoolean CalMixer_ClearCycle(CalMixer *self, int id, float delay)
{
  return self->clearCycle(id, delay) ? True : False;
}

void CalMixer_Delete(CalMixer *self)
{
  delete self;
}

CalBoolean CalMixer_ExecuteAction(CalMixer *self, int id, float delayIn, float delayOut, enum CalBoolean autoLock)
{
  return self->executeAction(id, delayIn, delayOut, 1.0f,  autoLock == True ? true : false) ? True : False;
}

CalMixer *CalMixer_New(CalModel* pModel)
{
  return new(std::nothrow) CalMixer(pModel);
}

void CalMixer_UpdateAnimation(CalMixer *self, float deltaTime)
{
  self->updateAnimation(deltaTime);
}

void CalMixer_UpdateSkeleton(CalMixer *self)
{
  self->updateSkeleton();
}

void CalMixer_RemoveAction(CalMixer *self,int id)
{
  self->removeAction(id);
}

//****************************************************************************//
// CalMorphTargetMixer wrapper functions definition                           //
//****************************************************************************//

float CalMorphTargetMixer_GetCurrentWeight( struct CalMorphTargetMixer* self,
											int morphAnimID )
{
	return self->getCurrentWeight( morphAnimID );
}

CalBoolean CalMorphTargetMixer_Blend( struct CalMorphTargetMixer* self,
  							int morphAnimID, float weight, float delayIn, float delayOut, bool looping )
{
  return self->blend(morphAnimID, weight, delayIn, delayOut, looping) ? True : False;
}

CalBoolean CalMorphTargetMixer_Clear(struct CalMorphTargetMixer *self, int id, float delay)
{
  return self->clear(id, delay) ? True : False;
}

void CalMorphTargetMixer_Delete(struct CalMorphTargetMixer *self)
{
  delete self;
}

struct CalMorphTargetMixer *CalMorphTargetMixer_New(struct CalModel *pModel)
{
  return new(std::nothrow) CalMorphTargetMixer(pModel);
}

void CalMorphTargetMixer_Update(struct CalMorphTargetMixer *self, float deltaTime)
{
  self->update(deltaTime);
}

int CalMorphTargetMixer_GetMorphTargetCount(struct CalMorphTargetMixer *self)
{
  return self->getMorphTargetCount();
}

CalBoolean CalMorphTargetMixer_Copy( struct CalMorphTargetMixer* self,
  										const struct CalMorphTargetMixer* toCopy )
{
	return (CalBoolean) self->copy( *toCopy );
}

//****************************************************************************//
// CalModel wrapper functions definition                                      //
//****************************************************************************//

CalBoolean CalModel_AttachMesh(CalModel *self, int coreMeshId)
{
	try
	{
		return self->attachMesh(coreMeshId) ? True : False;
	}
	catch (...)
	{
	}
	return False;
}

void CalModel_Delete(CalModel *self)
{
  delete self;
}

CalBoolean CalModel_DetachMesh(CalModel *self, int coreMeshId)
{
  return self->detachMesh(coreMeshId) ? True : False;
}

CalCoreModel *CalModel_GetCoreModel(CalModel *self)
{
  return self->getCoreModel();
}

CalMesh *CalModel_GetMesh(CalModel *self, int coreMeshId)
{
  return self->getMesh(coreMeshId);
}

const CalMesh *CalModel_GetMeshConst(const CalModel *self, int coreMeshId)
{
  return self->getMesh(coreMeshId);
}

CalMixer *CalModel_GetMixer(CalModel *self)
{
  return self->getMixer();
}

CalMorphTargetMixer *CalModel_GetMorphTargetMixer(CalModel *self)
{
  return self->getMorphTargetMixer();
}

const CalMorphTargetMixer *CalModel_GetMorphTargetMixerConst(const CalModel *self)
{
  return self->getMorphTargetMixer();
}

CalPhysique *CalModel_GetPhysique(CalModel *self)
{
  return self->getPhysique();
}

const CalPhysique *CalModel_GetPhysiqueConst(const CalModel *self)
{
  return self->getPhysique();
}

void CalModel_SetPhysique(struct CalModel *self, struct CalPhysique *phys)
{
	self->setPhysique( phys );
}

CalRenderer *CalModel_GetRenderer(CalModel *self)
{
  return self->getRenderer();
}

CalSkeleton *CalModel_GetSkeleton(CalModel *self)
{
  return self->getSkeleton();
}

CalSpringSystem *CalModel_GetSpringSystem(CalModel *self)
{
  return self->getSpringSystem();
}

const CalSpringSystem *CalModel_GetSpringSystemConst(const CalModel *self)
{
  return self->getSpringSystem();
}

CalUserData CalModel_GetUserData(CalModel *self)
{
  return self->getUserData();
}

/*
std::vector<CalMesh *>& CalModel_GetVectorMesh(CalModel *self)
{
  return self->getVectorMesh();
}
*/

int CalModel_GetMeshCount(struct CalModel *self)
{
	return self->getVectorMesh().size();
}

struct CalMesh *CalModel_GetMeshByMeshID(struct CalModel *self, int meshId)
{
	CalMesh*	theMesh = 0;
	
	if ( (meshId >= 0) && (meshId < self->getVectorMesh().size()) )
	{
		theMesh = self->getVectorMesh()[ meshId ];
	}
	return theMesh;
}

CalModel *CalModel_New(CalCoreModel* pCoreModel)
{
  return new(std::nothrow) CalModel(pCoreModel);
}

void CalModel_SetLodLevel(CalModel *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

void CalModel_SetMaterialSet(CalModel *self, int setId)
{
  self->setMaterialSet(setId);
}

void CalModel_SetUserData(CalModel *self, CalUserData userData)
{
  self->setUserData(userData);
}

void CalModel_Update(CalModel *self, float deltaTime)
{
  self->update(deltaTime);
}

//****************************************************************************//
// CalPhysique wrapper functions definition                                   //
//****************************************************************************//

int CalPhysique_CalculateNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pNormalBuffer)
{
  return self->calculateNormals(pSubmesh, pNormalBuffer);
}

int CalPhysique_CalculateVertices(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  return self->calculateVertices(pSubmesh, pVertexBuffer);
}

int CalPhysique_CalculateVerticesAndNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  return self->calculateVerticesAndNormals(pSubmesh, pVertexBuffer);
}

int CalPhysique_CalculateVerticesNormalsAndTexCoords(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords)
{
  return self->calculateVerticesNormalsAndTexCoords(pSubmesh, pVertexBuffer, NumTexCoords);
}

int CalPhysique_CalculateTangentSpaces(CalPhysique *self, CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer)
{
  return self->calculateTangentSpaces(pSubmesh, mapId, pTangentSpaceBuffer);
}

void CalPhysique_Delete(CalPhysique *self)
{
  delete self;
}

CalPhysique *CalPhysique_New(CalModel* pModel)
{
  return new(std::nothrow) CalPhysique(pModel);
}

CalPhysique *CalPhysiqueDualQuat_New(CalModel* pModel)
{
  return new(std::nothrow) CalPhysiqueDualQuat(pModel);
}

void CalPhysique_Update(CalPhysique *self)
{
  self->update();
}

//****************************************************************************//
// CalPlatform wrapper functions definition                                   //
//****************************************************************************//

//****************************************************************************//
// CalQuaternion wrapper functions definition                                 //
//****************************************************************************//

void CalQuaternion_Blend(CalQuaternion *self, float d, const CalQuaternion *pQ)
{
  self->blend(d, *pQ);
}

void CalQuaternion_Clear(CalQuaternion *self)
{
  self->clear();
}

void CalQuaternion_Conjugate(CalQuaternion *self)
{
  self->conjugate();
}

void CalQuaternion_Invert(struct CalQuaternion *self)
{
	self->invert();
}

void CalQuaternion_Delete(CalQuaternion *self)
{
  delete self;
}

void CalQuaternion_Equal(CalQuaternion *self, const CalQuaternion *pQ)
{
  *self = *pQ;
}

float *CalQuaternion_Get(CalQuaternion *self)
{
  return &(self->x);
}

void CalQuaternion_Multiply(CalQuaternion *self, const CalQuaternion *pQ)
{
  *self *= *pQ;
}

void CalQuaternion_MultiplyVector(CalQuaternion *self, const CalVector *pV)
{
  *self *= *pV;
}

CalQuaternion *CalQuaternion_New()
{
  return new(std::nothrow) CalQuaternion();
}

void CalQuaternion_Op_Multiply(CalQuaternion *pResult, const CalQuaternion *pQ, const CalQuaternion *pR)
{
  *pResult = *pQ * *pR;
}

void CalQuaternion_Set(CalQuaternion *self, float qx, float qy, float qz, float qw)
{
  self->set(qx, qy, qz, qw);
}

//****************************************************************************//
// CalRender wrapper functions definition                                         //
//****************************************************************************//

CalBoolean CalRenderer_BeginRendering(CalRenderer *self)
{
  return self->beginRendering() ? True : False;
}

void CalRenderer_Delete(CalRenderer *self)
{
  delete self;
}

void CalRenderer_EndRendering(CalRenderer *self)
{
  self->endRendering();
}

void CalRenderer_GetAmbientColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getAmbientColor(pColorBuffer);
}

void CalRenderer_GetDiffuseColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getDiffuseColor(pColorBuffer);
}

int CalRenderer_GetFaceCount(CalRenderer *self)
{
  return self->getFaceCount();
}

int CalRenderer_GetFaces(CalRenderer *self, CalIndex *pFaceBuffer)
{
  return self->getFaces(pFaceBuffer);
}

int CalRenderer_GetMapCount(CalRenderer *self)
{
  return self->getMapCount();
}

CalUserData CalRenderer_GetMapUserData(CalRenderer *self, int mapId)
{
  return (CalUserData)self->getMapUserData(mapId);
}

int CalRenderer_GetMeshCount(CalRenderer *self)
{
  return self->getMeshCount();
}

int CalRenderer_GetNormals(CalRenderer *self, float *pNormalBuffer)
{
  return self->getNormals(pNormalBuffer);
}

float CalRenderer_GetShininess(CalRenderer *self)
{
  return self->getShininess();
}

void CalRenderer_GetSpecularColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getSpecularColor(pColorBuffer);
}

int CalRenderer_GetSubmeshCount(CalRenderer *self, int meshId)
{
  return self->getSubmeshCount(meshId);
}

int CalRenderer_GetTextureCoordinates(CalRenderer *self, int mapId, float *pTextureCoordinateBuffer)
{
  return self->getTextureCoordinates(mapId, pTextureCoordinateBuffer);
}

int CalRenderer_GetVertexCount(CalRenderer *self)
{
  return self->getVertexCount();
}

int CalRenderer_GetVertices(CalRenderer *self, float *pVertexBuffer)
{
  return self->getVertices(pVertexBuffer);
}

int CalRenderer_GetVerticesAndNormals(CalRenderer *self, float *pVertexBuffer)
{
  return self->getVerticesAndNormals(pVertexBuffer);
}

int CalRenderer_GetVerticesNormalsAndTexCoords(CalRenderer *self, float *pVertexBuffer, int NumTexCoords)
{
  return self->getVerticesNormalsAndTexCoords(pVertexBuffer,NumTexCoords);
}

int CalRenderer_GetTangentSpaces(CalRenderer *self, int mapId, float *pTangentSpaceBuffer)
{
  return self->getTangentSpaces(mapId, pTangentSpaceBuffer);
}

CalBoolean CalRenderer_IsTangentsEnabled(CalRenderer *self, int mapId)
{
  return self->isTangentsEnabled(mapId) ? True : False;
}

CalRenderer *CalRenderer_New(CalModel* pModel)
{
  return new(std::nothrow) CalRenderer(pModel);
}

CalBoolean CalRenderer_SelectMeshSubmesh(CalRenderer *self, int meshId, int submeshId)
{
  return self->selectMeshSubmesh(meshId, submeshId) ? True : False;
}

//****************************************************************************//
// CalPlatform wrapper functions definition                                   //
//****************************************************************************//

void CalSaver_Delete(CalSaver *self)
{
  delete self;
}

CalSaver *CalSaver_New()
{
  return new(std::nothrow) CalSaver();
}

CalBoolean CalSaver_SaveCoreAnimation(CalSaver *self, const char *strFilename, CalCoreAnimation *pCoreAnimation)
{
  return self->saveCoreAnimation(strFilename, pCoreAnimation) ? True : False;
}

CalBoolean CalSaver_SaveCoreMaterial(CalSaver *self, const char *strFilename, CalCoreMaterial *pCoreMaterial)
{
  return self->saveCoreMaterial(strFilename, pCoreMaterial) ? True : False;
}

CalBoolean CalSaver_SaveCoreMesh(CalSaver *self, const char *strFilename, CalCoreMesh *pCoreMesh)
{
  return self->saveCoreMesh(strFilename, pCoreMesh) ? True : False;
}

CalBoolean CalSaver_SaveCoreSkeleton(CalSaver *self, const char *strFilename, CalCoreSkeleton *pCoreSkeleton)
{
  return self->saveCoreSkeleton(strFilename, pCoreSkeleton) ? True : False;
}

//****************************************************************************//
// CalSkeleton wrapper functions definition                                   //
//****************************************************************************//

void CalSkeleton_CalculateState(CalSkeleton *self)
{
  self->calculateState();
}

void CalSkeleton_ClearState(CalSkeleton *self)
{
  self->clearState();
}

void CalSkeleton_Delete(CalSkeleton *self)
{
  delete self;
}

CalBone *CalSkeleton_GetBone(CalSkeleton *self, int boneId)
{
  return self->getBone(boneId);
}

const CalBone *CalSkeleton_GetBoneConst(const CalSkeleton *self, int boneId)
{
  return self->getBone(boneId);
}

int CalSkeleton_GetBoneCount(const CalSkeleton *self)
{
	return self->getVectorBone().size();
}

CalCoreSkeleton *CalSkeleton_GetCoreSkeleton(CalSkeleton *self)
{
  return self->getCoreSkeleton();
}

const CalCoreSkeleton *CalSkeleton_GetCoreSkeletonConst(const CalSkeleton *self)
{
  return self->getCoreSkeleton();
}

/*
std::vector<CalBone *>& CalSkeleton_GetVectorBone(CalSkeleton *self)
{
  return self->getVectorBone();
}
*/

void CalSkeleton_LockState(CalSkeleton *self)
{
  self->lockState();
}

CalSkeleton *CalSkeleton_New(CalCoreSkeleton *pCoreSkeleton)
{
  return new(std::nothrow) CalSkeleton(pCoreSkeleton);
}

void CalSkeleton_GetBoneBoundingBox(CalSkeleton *self, float *min, float *max)
{
	self->getBoneBoundingBox(min, max);
}

void CalSkeleton_CalculateBoundingBoxes(CalSkeleton *self)
{
	self->calculateBoundingBoxes();
}

// DEBUG-CODE
int CalSkeleton_GetBonePoints(CalSkeleton *self, float *pPoints)
{
  return self->getBonePoints(pPoints);
}

// DEBUG-CODE
int CalSkeleton_GetBonePointsStatic(CalSkeleton *self, float *pPoints)
{
  return self->getBonePointsStatic(pPoints);
}

// DEBUG-CODE
int CalSkeleton_GetBoneLines(CalSkeleton *self, float *pLines)
{
  return self->getBoneLines(pLines);
}

// DEBUG-CODE
int CalSkeleton_GetBoneLinesStatic(CalSkeleton *self, float *pLines)
{
  return self->getBoneLinesStatic(pLines);
}

//****************************************************************************//
// CalSpringSystem wrapper functions definition                               //
//****************************************************************************//

void CalSpringSystem_CalculateForces(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime)
{
  self->calculateForces(pSubmesh, deltaTime);
}

void CalSpringSystem_CalculateVertices(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime)
{
  self->calculateVertices(pSubmesh, deltaTime);
}

void CalSpringSystem_Delete(CalSpringSystem *self)
{
  delete self;
}

CalSpringSystem *CalSpringSystem_New(CalModel* pModel)
{
  return new(std::nothrow) CalSpringSystem(pModel);
}

void CalSpringSystem_Update(CalSpringSystem *self, float deltaTime)
{
  self->update(deltaTime);
}

const CalVector *CalSpringSystem_GetGravityVector(const CalSpringSystem *self)
{
	return &self->getGravityVector();
}

const CalVector *CalSpringSystem_GetForceVector(const CalSpringSystem *self)
{
	return &self->getForceVector();
}

void CalSpringSystem_SetGravityVector(CalSpringSystem *self, CalVector* grav)
{
	self->setGravityVector( *grav );
}

void CalSpringSystem_SetForceVector(CalSpringSystem *self, CalVector* grav)
{
	self->setForceVector( *grav );
}

void CalSpringSystem_ResetPositions(struct CalSpringSystem *self)
{
	self->resetPositions();
}

//****************************************************************************//
// CalSubmesh wrapper functions definition                                    //
//****************************************************************************//

void CalSubmesh_Delete(CalSubmesh *self)
{
  delete self;
}

CalCoreSubmesh *CalSubmesh_GetCoreSubmesh(CalSubmesh *self)
{
  return self->getCoreSubmesh();
}

int CalSubmesh_GetCoreMaterialId(CalSubmesh *self)
{
  return self->getCoreMaterialId();
}

int CalSubmesh_GetFaceCount(CalSubmesh *self)
{
  return self->getFaceCount();
}

int CalSubmesh_GetFaces(CalSubmesh *self, CalIndex *pFaceBuffer)
{
  return self->getFaces(pFaceBuffer);
}

/*
std::vector<CalVector>& CalSubmesh_GetVectorNormal(CalSubmesh *self)
{
  return self->getVectorNormal();
}
*/
/*
std::vector<CalSubmesh::PhysicalProperty>& CalSubmesh_GetVectorPhysicalProperty(CalSubmesh *self)
{
  return self->getVectorPhysicalProperty();
}
*/

/*
std::vector<CalVector>& CalSubmesh_GetVectorVertex(CalSubmesh *self)
{
  return self->getVectorVertex();
}
*/

int CalSubmesh_GetVertexCount(CalSubmesh *self)
{
  return self->getVertexCount();
}

CalBoolean CalSubmesh_HasInternalData(CalSubmesh *self)
{
  return self->hasInternalData() ? True : False;
}

CalSubmesh *CalSubmesh_New(CalCoreSubmesh* coreSubmesh)
{
  return new(std::nothrow) CalSubmesh(coreSubmesh);
}

void CalSubmesh_SetCoreMaterialId(CalSubmesh *self, int coreMaterialId)
{
  self->setCoreMaterialId(coreMaterialId);
}

void CalSubmesh_SetLodLevel(CalSubmesh *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

//****************************************************************************//
// CalVector wrapper functions definition                                     //
//****************************************************************************//

void CalVector_Add(CalVector *self, CalVector *pV)
{
  *self += *pV;
}

void CalVector_Blend(CalVector *self, float d, CalVector *pV)
{
  self->blend(d, *pV);
}

void CalVector_Clear(CalVector *self)
{
  self->clear();
}

void CalVector_Delete(CalVector *self)
{
  delete self;
}

void CalVector_Equal(CalVector *self, CalVector *pV)
{
  *self = *pV;
}

void CalVector_InverseScale(CalVector *self, float d)
{
  *self /= d;
}

float *CalVector_Get(CalVector *self)
{
  return &(self->x);
}

float CalVector_Length(CalVector *self)
{
  return self->length();
}

CalVector *CalVector_New()
{
  return new(std::nothrow) CalVector();
}

float CalVector_Normalize(CalVector *self)
{
  return self->normalize();
}

void CalVector_Op_Add(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV + *pU;
}

void CalVector_Op_Subtract(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV - *pU;
}

void CalVector_CalVector_Op_Scale(CalVector *pResult, CalVector *pV, float d)
{
  *pResult = *pV * d;
}

void CalVector_CalVector_Op_InverseScale(CalVector *pResult, CalVector *pV, float d)
{
  *pResult = *pV / d;
}

float CalVector_Op_Scalar(CalVector *pV, CalVector *pU)
{
  return *pV * *pU;
}

void CalVector_Op_Cross(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV % *pU;
}

void CalVector_Scale(CalVector *self, float d)
{
  *self *= d;
}

void CalVector_Set(CalVector *self, float vx, float vy, float vz)
{
  self->set(vx, vy, vz);
}

void CalVector_Subtract(CalVector *self, CalVector *pV)
{
  *self = *pV;
}

void CalVector_Transform(CalVector *self, CalQuaternion *pQ)
{
  *self *= *pQ;
}

//****************************************************************************//

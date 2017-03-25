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

void cal3d::CalAnimation_Delete(CalAnimation *self)
{
  delete self;
}

CalCoreAnimation *cal3d::CalAnimation_GetCoreAnimation(CalAnimation *self)
{
  return self->getCoreAnimation();
}

CalAnimationState cal3d::CalAnimation_GetState(CalAnimation *self)
{
  return (CalAnimationState)(self->getState());
}

float cal3d::CalAnimation_GetTime(CalAnimation *self)
{
  return self->getTime();
}

CalAnimationType cal3d::CalAnimation_GetType(CalAnimation *self)
{
  return (CalAnimationType)(self->getType());
}

float cal3d::CalAnimation_GetWeight(CalAnimation *self)
{
  return self->getWeight();
}

//****************************************************************************//
// CalAnimationAction wrapper functions definition                            //
//****************************************************************************//

void cal3d::CalAnimationAction_Delete(CalAnimationAction *self)
{
  delete self;
}

CalBoolean cal3d::CalAnimationAction_Execute(CalAnimationAction *self, float delayIn, float delayOut)
{
  return self->execute(delayIn, delayOut) ? True : False;
}

CalAnimationAction *cal3d::CalAnimationAction_New(CalCoreAnimation *pCoreAnimation)
{
  return new(std::nothrow) CalAnimationAction(pCoreAnimation);
}

CalBoolean cal3d::CalAnimationAction_Update(CalAnimationAction *self, float deltaTime)
{
  return self->update(deltaTime) ? True : False;
}

void cal3d::CalCoreAnimation_Scale(CalCoreAnimation *self, float factor)
{
  self->scale(factor);
}


//****************************************************************************//
// CalAnimationCycle wrapper functions definition                             //
//****************************************************************************//

CalBoolean cal3d::CalAnimationCycle_Blend(CalAnimationCycle *self, float weight, float delay)
{
  return self->blend(weight, delay) ? True : False;
}

void cal3d::CalAnimationCycle_Delete(CalAnimationCycle *self)
{
  delete self;
}

CalAnimationCycle *cal3d::CalAnimationCycle_New(CalCoreAnimation *pCoreAnimation)
{
  return new(std::nothrow) CalAnimationCycle(pCoreAnimation);
}

void cal3d::CalAnimationCycle_SetAsync(CalAnimationCycle *self, float time, float duration)
{
  self->setAsync(time, duration);
}

CalBoolean cal3d::CalAnimationCycle_Update(CalAnimationCycle *self, float deltaTime)
{
  return self->update(deltaTime) ? True : False;
}

//****************************************************************************//
// CalBone wrapper functions definition                                           //
/***********************************************************************

void CalBone_BlendState(CalBone *self, float weight, CalVector *pTranslation, CalQuaternion *pRotation)
{
  self->blendState(weight, *pTranslation, *pRotation);
}*/

void cal3d::CalBone_CalculateState(CalBone *self)
{
  self->calculateState();
}

void cal3d::CalBone_ClearState(CalBone *self)
{
  self->clearState();
}

void cal3d::CalBone_Delete(CalBone *self)
{
  delete self;
}

CalCoreBone *cal3d::CalBone_GetCoreBone(CalBone *self)
{
  return self->getCoreBone();
}

CalQuaternion *cal3d::CalBone_GetRotation(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotation()));
}

CalQuaternion *cal3d::CalBone_GetRotationAbsolute(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationAbsolute()));
}

CalQuaternion *cal3d::CalBone_GetRotationBoneSpace(CalBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationBoneSpace()));
}

CalVector *cal3d::CalBone_GetTranslation(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslation()));
}

CalVector *cal3d::CalBone_GetTranslationAbsolute(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationAbsolute()));
}

CalVector *cal3d::CalBone_GetTranslationBoneSpace(CalBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationBoneSpace()));
}

void cal3d::CalBone_LockState(CalBone *self)
{
  self->lockState();
}

CalBone *cal3d::CalBone_New(CalCoreBone* coreBone)
{
  return new(std::nothrow) CalBone(coreBone);
}

void cal3d::CalBone_SetSkeleton(CalBone *self, CalSkeleton *pSkeleton)
{
  self->setSkeleton(pSkeleton);
}

void cal3d::CalBone_SetRotation(CalBone *self, CalQuaternion *pRotation)
{
  self->setRotation(*pRotation);
}

void cal3d::CalBone_SetTranslation(CalBone *self, CalVector *pTranslation)
{
  self->setTranslation(*pTranslation);
}

void cal3d::CalBone_SetCoreState(CalBone *self)
{
  self->setCoreState();
}

void cal3d::CalBone_SetCoreStateRecursive(CalBone *self)
{
  self->setCoreStateRecursive();
}

CalBoolean cal3d::CalBone_GetBoundingBox(struct CalBone *self, struct CalCoreModel* model,
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

		const CalBoundingBox&	box( self->getBoundingBox() );
		box.computePoints( reinterpret_cast<CalVector*>(outEightPoints) );
		gotBounds = True;
	}

	return gotBounds;
}


//****************************************************************************//
// CalCoreAnimation wrapper functions definition                              //
//****************************************************************************//

CalCoreAnimation *cal3d::CalCoreAnimation_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreAnimation());
}

void cal3d::CalCoreAnimation_Delete(CalCoreAnimation* self)
{
    explicitDecRef(self);
}

float cal3d::CalCoreAnimation_GetDuration(CalCoreAnimation *self)
{
  return self->getDuration();
}

void cal3d::CalCoreAnimation_SetDuration(CalCoreAnimation *self, float duration)
{
  self->setDuration(duration);
}

//****************************************************************************//
// CalCoreBone wrapper functions definition                                   //
//****************************************************************************//

CalBoolean cal3d::CalCoreBone_AddChildId(CalCoreBone *self, int childId)
{
	self->addChildId(childId); return True;
}

void cal3d::CalCoreBone_CalculateState(CalCoreBone *self)
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

const char *cal3d::CalCoreBone_GetName(CalCoreBone *self)
{
  return self->getName().c_str();
}

void cal3d::CalCoreBone_SetName(struct CalCoreBone *self, const char* name)
{
	try
	{
		self->setName( name );
	}
	catch (...)
	{
	}
}

int cal3d::CalCoreBone_GetParentId(CalCoreBone *self)
{
  return self->getParentId();
}

int cal3d::CalCoreBone_GetChildCount(struct CalCoreBone *self)
{
	return self->getListChildId().size();
}

int cal3d::CalCoreBone_GetChildId(struct CalCoreBone *self, int childIndex)
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

CalQuaternion *cal3d::CalCoreBone_GetRotation(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotation()));
}

CalQuaternion *cal3d::CalCoreBone_GetRotationAbsolute(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationAbsolute()));
}

CalQuaternion *cal3d::CalCoreBone_GetRotationBoneSpace(CalCoreBone *self)
{
  return &(const_cast<CalQuaternion&>(self->getRotationBoneSpace()));
}

CalVector *cal3d::CalCoreBone_GetTranslation(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslation()));
}

CalVector *cal3d::CalCoreBone_GetTranslationAbsolute(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationAbsolute()));
}

CalVector *cal3d::CalCoreBone_GetTranslationBoneSpace(CalCoreBone *self)
{
  return &(const_cast<CalVector&>(self->getTranslationBoneSpace()));
}

CalUserData cal3d::CalCoreBone_GetUserData(CalCoreBone *self)
{
  return self->getUserData();
}

CalCoreBone *cal3d::CalCoreBone_New(const char* name)
{
  return new(std::nothrow) CalCoreBone(name);
}

void cal3d::CalCoreBone_SetCoreSkeleton(CalCoreBone *self, CalCoreSkeleton *pCoreSkeleton)
{
  self->setCoreSkeleton(pCoreSkeleton);
}

void cal3d::CalCoreBone_SetParentId(CalCoreBone *self, int parentId)
{
  self->setParentId(parentId);
}

void cal3d::CalCoreBone_SetRotation(CalCoreBone *self, CalQuaternion *pRotation)
{
  self->setRotation(*pRotation);
}

void cal3d::CalCoreBone_SetRotationBoneSpace(CalCoreBone *self, CalQuaternion *pRotation)
{
  self->setRotationBoneSpace(*pRotation);
}

void cal3d::CalCoreBone_SetTranslation(CalCoreBone *self, CalVector *pTranslation)
{
  self->setTranslation(*pTranslation);
}

void cal3d::CalCoreBone_SetTranslationBoneSpace(CalCoreBone *self, CalVector *pTranslation)
{
  self->setTranslationBoneSpace(*pTranslation);
}

void cal3d::CalCoreBone_SetUserData(CalCoreBone *self, CalUserData userData)
{
  self->setUserData(userData);
}

CalBoolean cal3d::CalCoreBone_GetBoundingBox(struct CalCoreBone *self, struct CalCoreModel* model,
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

CalCoreMaterial *cal3d::CalCoreMaterial_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreMaterial());
}

void cal3d::CalCoreMaterial_Delete(CalCoreMaterial *self)
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

static CalColorValue	ColorToValue(const CalCoreMaterial::Color& inColor)
{
	CalColorValue	theValue = (((CalColorValue) inColor.red) << 24) |
		(((CalColorValue) inColor.green) << 16) |
		(((CalColorValue) inColor.blue) << 8) |
		(((CalColorValue) inColor.alpha) << 0);
	return theValue;
}

static CalCoreMaterial::Color ValueToColor(CalColorValue inValue)
{
	CalCoreMaterial::Color	theColor = {
		inValue >> 24,
		(inValue & 0x00FF0000) >> 16,
		(inValue & 0x0000FF00) >> 8,
		(inValue & 0x000000FF)
	};
	return theColor;
}

CalColorValue cal3d::CalCoreMaterial_GetAmbientColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getAmbientColor() );
}

CalColorValue cal3d::CalCoreMaterial_GetDiffuseColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getDiffuseColor() );
}

CalColorValue cal3d::CalCoreMaterial_GetSpecularColor(struct CalCoreMaterial *self)
{
	return ColorToValue( self->getSpecularColor() );
}

void cal3d::CalCoreMaterial_SetAmbientColor(struct CalCoreMaterial *self, CalColorValue pAmbientColor)
{
	self->setAmbientColor( ValueToColor( pAmbientColor ) );
}

void cal3d::CalCoreMaterial_SetDiffuseColor(struct CalCoreMaterial *self, CalColorValue pDiffuseColor)
{
	self->setDiffuseColor( ValueToColor( pDiffuseColor ) );
}

void cal3d::CalCoreMaterial_SetSpecularColor(struct CalCoreMaterial *self, CalColorValue pSpecularColor)
{
	self->setSpecularColor( ValueToColor( pSpecularColor ) );
}

const char* cal3d::CalCoreMaterial_GetName(struct CalCoreMaterial *self)
{
	return self->getName().c_str();
}

void cal3d::CalCoreMaterial_SetName(struct CalCoreMaterial *self, const char* inName)
{
	try
	{
		self->setName( std::string(inName) );
	}
	catch (...)
	{
	}
}

int cal3d::CalCoreMaterial_GetMapCount(CalCoreMaterial *self)
{
  return self->getMapCount();
}

const char *cal3d::CalCoreMaterial_GetMapFilename(CalCoreMaterial *self, int mapId)
{
  return self->getMapFilename(mapId).c_str();
}

CalUserData cal3d::CalCoreMaterial_GetMapUserData(CalCoreMaterial *self, int mapId)
{
  return self->getMapUserData(mapId);
}

float cal3d::CalCoreMaterial_GetShininess(CalCoreMaterial *self)
{
  return self->getShininess();
}

/*
CalCoreMaterial::Color *CalCoreMaterial_GetSpecularColor(CalCoreMaterial *self)
{
  return &(self->getSpecularColor());
}
*/

CalUserData cal3d::CalCoreMaterial_GetUserData(CalCoreMaterial *self)
{
  return self->getUserData();
}

/*
std::vector<Map>& CalCoreMaterial_GetVectorMap(CalCoreMaterial *self)
{
  return self->getVectorMap();
}
*/

CalBoolean cal3d::CalCoreMaterial_Reserve(CalCoreMaterial *self, int mapCount)
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

CalBoolean cal3d::CalCoreMaterial_SetMapUserData(CalCoreMaterial *self, int mapId, CalUserData userData)
{
  return self->setMapUserData(mapId, userData) ? True : False;
}

void cal3d::CalCoreMaterial_SetShininess(CalCoreMaterial *self, float shininess)
{
  self->setShininess(shininess);
}

/*
void CalCoreMaterial_SetSpecularColor(CalCoreMaterial *self, CalCoreMaterial::Color *pSpecularColor)
{
  self->setSpecularColor(*pSpecularColor);
}
*/

void cal3d::CalCoreMaterial_SetUserData(CalCoreMaterial *self, CalUserData userData)
{
  self->setUserData(userData);
}

//****************************************************************************//
// CalCoreMesh wrapper functions definition                                   //
//****************************************************************************//

CalCoreMesh *cal3d::CalCoreMesh_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreMesh());
}

void cal3d::CalCoreMesh_Delete(CalCoreMesh *self)
{
    explicitDecRef(self);
}

int cal3d::CalCoreMesh_AddCoreSubmesh(CalCoreMesh *self, CalCoreSubmesh *pCoreSubmesh)
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

void cal3d::CalCoreMesh_RemoveCoreSubmesh(struct CalCoreMesh *self, int coreSubmeshID)
{
	self->removeCoreSubmesh( coreSubmeshID );
}

const char* cal3d::CalCoreMesh_GetName(struct CalCoreMesh *self)
{
	return self->getName().c_str();
}

void cal3d::CalCoreMesh_SetName(struct CalCoreMesh *self, const char* inName)
{
	try
	{
		self->setName( std::string(inName) );
	}
	catch (...)
	{
	}
}

CalCoreSubmesh *cal3d::CalCoreMesh_GetCoreSubmesh(CalCoreMesh *self, int id)
{
  return self->getCoreSubmesh(id);
}

int cal3d::CalCoreMesh_GetCoreSubmeshCount(CalCoreMesh *self)
{
  return self->getCoreSubmeshCount();
}

void cal3d::CalCoreMesh_Scale(CalCoreMesh *self, float factor)
{
  self->scale(factor);
}

int cal3d::CalCoreMesh_AddAsMorphTarget(struct CalCoreMesh *self, struct CalCoreMesh *target)
{
	try
	{
		return self->addAsMorphTarget(target);//, target->getName());
	}
	catch (...)
	{
	}
	return -1;
}

//****************************************************************************//
// CalCoreModel wrapper functions definition                                  //
//****************************************************************************//

int cal3d::CalCoreModel_AddCoreAnimation(CalCoreModel *self, CalCoreAnimation *pCoreAnimation)
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

int cal3d::CalCoreModel_AddCoreMaterial(CalCoreModel *self, CalCoreMaterial *pCoreMaterial)
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

int cal3d::CalCoreModel_AddCoreMesh(CalCoreModel *self, CalCoreMesh *pCoreMesh)
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

CalBoolean cal3d::CalCoreModel_CreateCoreMaterialThread(CalCoreModel *self, int coreMaterialThreadId)
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

void cal3d::CalCoreModel_Delete(CalCoreModel *self)
{
  delete self;
}

CalCoreAnimation *cal3d::CalCoreModel_GetCoreAnimation(CalCoreModel *self, int coreAnimationId)
{
  return self->getCoreAnimation(coreAnimationId);
}

int cal3d::CalCoreModel_GetCoreAnimationCount(CalCoreModel *self)
{
  return self->getCoreAnimationCount();
}

CalCoreMaterial *cal3d::CalCoreModel_GetCoreMaterial(CalCoreModel *self, int coreMaterialId)
{
  return self->getCoreMaterial(coreMaterialId);
}

int cal3d::CalCoreModel_GetCoreMaterialCount(CalCoreModel *self)
{
  return self->getCoreMaterialCount();
}

int cal3d::CalCoreModel_GetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId)
{
  return self->getCoreMaterialId(coreMaterialThreadId, coreMaterialSetId);
}

CalCoreMesh *cal3d::CalCoreModel_GetCoreMesh(CalCoreModel *self, int coreMeshId)
{
  return self->getCoreMesh(coreMeshId);
}

int cal3d::CalCoreModel_GetCoreMeshCount(CalCoreModel *self)
{
  return self->getCoreMeshCount();
}

int cal3d::CalCoreModel_GetCoreMeshId(struct CalCoreModel *self, const char* coreMeshName)
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

CalCoreSkeleton *cal3d::CalCoreModel_GetCoreSkeleton(CalCoreModel *self)
{
  return self->getCoreSkeleton();
}

CalUserData cal3d::CalCoreModel_GetUserData(CalCoreModel *self)
{
  return self->getUserData();
}


CalBoolean cal3d::CalCoreModel_AddMeshName(struct CalCoreModel *self, const char* name, int coreMeshId)
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

void cal3d::CalCoreModel_CloneCoreMaterials(struct CalCoreModel *self)
{
	try
	{
		self->cloneCoreMaterials();
	}
	catch (...)
	{
	}
}

int cal3d::CalCoreModel_LoadCoreAnimation(CalCoreModel *self, const char *strFilename)
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

int cal3d::CalCoreModel_LoadCoreAnimationFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreAnimationFromXMLstring( (const char*)(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

int cal3d::CalCoreModel_LoadCoreMaterial(CalCoreModel *self, const char *strFilename)
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

int cal3d::CalCoreModel_LoadCoreMaterialFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreMaterialFromXMLstring((const char*)(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

int cal3d::CalCoreModel_LoadCoreMesh(CalCoreModel *self, const char *strFilename)
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

int cal3d::CalCoreModel_LoadCoreMeshFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreMeshFromXMLstring((const char*)(buffer));
	}
	catch (...)
	{
	}
	return -1;
}

CalBoolean cal3d::CalCoreModel_LoadCoreSkeleton(CalCoreModel *self, const char *strFilename)
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

CalBoolean cal3d::CalCoreModel_LoadCoreSkeletonFromBuffer(CalCoreModel *self, const void* buffer)
{
	try
	{
		return self->loadCoreSkeletonFromXMLstring((const char*)(buffer)) ? True : False;
	}
	catch (...)
	{
	}
	return False;
}

CalCoreModel *cal3d::CalCoreModel_New(const char* name)
{
  return new(std::nothrow) CalCoreModel(name);
}

CalCoreModel *cal3d::CalCoreModel_NewCopy(CalCoreModel* other)
{
  return new(std::nothrow) CalCoreModel(*other);
}

void cal3d::CalCoreModel_ReplaceCoreMesh(struct CalCoreModel *self, int coreMeshId, struct CalCoreMesh *pCoreMesh)
{
	self->replaceCoreMesh( coreMeshId, pCoreMesh );
}

CalBoolean cal3d::CalCoreModel_SaveCoreAnimation(CalCoreModel *self, const char *strFilename, int coreAnimationId)
{
  return self->saveCoreAnimation(strFilename, coreAnimationId) ? True : False;
}

CalBoolean cal3d::CalCoreModel_SaveCoreMaterial(CalCoreModel *self, const char *strFilename, int coreMaterialId)
{
  return self->saveCoreMaterial(strFilename, coreMaterialId) ? True : False;
}

CalBoolean cal3d::CalCoreModel_SaveCoreMesh(CalCoreModel *self, const char *strFilename, int coreMeshId)
{
  return self->saveCoreMesh(strFilename, coreMeshId) ? True : False;
}

CalBoolean cal3d::CalCoreModel_SaveCoreSkeleton(CalCoreModel *self, const char *strFilename)
{
  return self->saveCoreSkeleton(strFilename) ? True : False;
}

CalBoolean cal3d::CalCoreModel_SetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId)
{
  return self->setCoreMaterialId(coreMaterialThreadId, coreMaterialSetId, coreMaterialId) ? True : False;
}

void cal3d::CalCoreModel_SetCoreSkeleton(CalCoreModel *self, CalCoreSkeleton *pCoreSkeleton)
{
  self->setCoreSkeleton(pCoreSkeleton);
}

void cal3d::CalCoreModel_SetUserData(CalCoreModel *self, CalUserData userData)
{
  self->setUserData(userData);
}

const char *cal3d::CalCoreModel_GetName(CalCoreModel *self)
{
  return self->getName().c_str();
}

void cal3d::CalCoreModel_SetName(struct CalCoreModel *self, const char* inName)
{
	try
	{
                std::string temp(inName );
		self->setName(temp );
	}
	catch (...)
	{
	}
}


//****************************************************************************//
// CalCoreSkeleton wrapper functions definition                               //
//****************************************************************************//

int cal3d::CalCoreSkeleton_AddCoreBone(CalCoreSkeleton *self, CalCoreBone *pCoreBone)
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

void cal3d::CalCoreSkeleton_CalculateState(CalCoreSkeleton *self)
{
  self->calculateState();
}


CalCoreSkeleton *cal3d::CalCoreSkeleton_New()
{
    return explicitIncRef(new(std::nothrow) CalCoreSkeleton());
}

void cal3d::CalCoreSkeleton_Delete(CalCoreSkeleton *self)
{
    explicitDecRef(self);
}

CalCoreBone *cal3d::CalCoreSkeleton_GetCoreBone(CalCoreSkeleton *self, int coreBoneId)
{
  return self->getCoreBone(coreBoneId);
}

int cal3d::CalCoreSkeleton_GetCoreBoneCount(CalCoreSkeleton *self)
{
	return self->getVectorCoreBone().size();
}

int cal3d::CalCoreSkeleton_GetCoreBoneId(CalCoreSkeleton *self, const char *strName)
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

void cal3d::CalCoreSkeleton_Scale(CalCoreSkeleton *self, float factor)
{
	self->scale(factor);
}

CalBoolean cal3d::CalCoreSkeleton_MapCoreBoneName(struct CalCoreSkeleton *self, int coreBoneID,
										const char* name )
{
	return static_cast<CalBoolean>( self->mapCoreBoneName( coreBoneID, name ) );
}

int cal3d::CalCoreSkeleton_GetRootCoreBoneCount(CalCoreSkeleton *self)
{
	return self->getVectorRootCoreBoneId().size();
}

CalCoreBone *cal3d::CalCoreSkeleton_GetRootCoreBone(CalCoreSkeleton *self, int rootBoneIndex)
{
	CalCoreBone*	theBone = NULL;
	const std::vector<int>&	rootIDs(self->getVectorRootCoreBoneId());
	if ( (rootBoneIndex >= 0) && (rootBoneIndex < rootIDs.size()) )
	{
		int	boneID = rootIDs[ rootBoneIndex ];
		theBone = CalCoreSkeleton_GetCoreBone( self, boneID );
	}
	return theBone;
}

void cal3d::CalCoreSkeleton_CalculateBoundingBoxes(struct CalCoreModel* coreModel)
{
	CalCoreSkeleton*	coreSkel = CalCoreModel_GetCoreSkeleton( coreModel );
	coreSkel->calculateBoundingBoxes( coreModel );
}

//****************************************************************************//
// CalCoreSubmesh wrapper functions definition                                //
//****************************************************************************//

void cal3d::CalCoreSubmesh_Delete(CalCoreSubmesh *self)
{
  delete self;
}

int cal3d::CalCoreSubmesh_GetCoreMaterialThreadId(CalCoreSubmesh *self)
{
  return self->getCoreMaterialThreadId();
}

int cal3d::CalCoreSubmesh_GetFaceCount(CalCoreSubmesh *self)
{
  return self->getFaceCount();
}

int cal3d::CalCoreSubmesh_GetLodCount(CalCoreSubmesh *self)
{
  return self->getLodCount();
}

int cal3d::CalCoreSubmesh_GetSpringCount(CalCoreSubmesh *self)
{
  return self->getSpringCount();
}

/*
std::vector<CalCoreSubmesh::Face>& cal3d::CalCoreSubmesh_GetVectorFace(CalCoreSubmesh *self)
{
  return self->getVectorFace();
}
*/

/*
std::vector<CalCoreSubmesh::PhysicalProperty>& cal3d::CalCoreSubmesh_GetVectorPhysicalProperty(CalCoreSubmesh *self)
{
  return self->getVectorPhysicalProperty();
}
*/

/*
std::vector<CalCoreSubmesh::Spring>& cal3d::CalCoreSubmesh_GetVectorSpring(CalCoreSubmesh *self)
{
  return self->getVectorSpring();
}
*/

/*
std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& cal3d::CalCoreSubmesh_GetVectorVectorTextureCoordinate(CalCoreSubmesh *self)
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

int cal3d::CalCoreSubmesh_GetVertexCount(CalCoreSubmesh *self)
{
  return self->getVertexCount();
}

int cal3d::CalCoreSubmesh_GetVertexInfluenceCount(struct CalCoreSubmesh *self, int vertID)
{
	return self->getVectorVertex()[ vertID ].vectorInfluence.size();
}

int cal3d::CalCoreSubmesh_GetVertexInfluence(struct CalCoreSubmesh *self, int vertID,
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

void cal3d::CalCoreSubmesh_GetVertex(struct CalCoreSubmesh* self, int vertID, float* outPosition, float* outNormal)
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

CalBoolean cal3d::CalCoreSubmesh_Reserve(CalCoreSubmesh *self, int vertexCount, int textureCoordinateCount, int faceCount, int springCount)
{
  return self->reserve(vertexCount, textureCoordinateCount, faceCount, springCount) ? True : False;
}

void cal3d::CalCoreSubmesh_SetCoreMaterialThreadId(CalCoreSubmesh *self, int coreMaterialThreadId)
{
  self->setCoreMaterialThreadId(coreMaterialThreadId);
}

/*
CalBoolean CalCoreSubmesh_SetFace(CalCoreSubmesh *self, int faceId, CalCoreSubmesh::Face *pFace)
{
  return self->setFace(faceId, *pFace) ? True : False;
}
*/

void cal3d::CalCoreSubmesh_SetLodCount(CalCoreSubmesh *self, int lodCount)
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

CalBoolean cal3d::CalCoreSubmesh_IsTangentsEnabled(CalCoreSubmesh *self, int mapId)
{
  return self->isTangentsEnabled(mapId) ? True : False;
}

CalBoolean cal3d::CalCoreSubmesh_EnableTangents(struct CalCoreSubmesh *self, int mapId, enum CalBoolean enabled)
{
  return self->enableTangents(mapId, enabled) ? True : False;
}

int cal3d::CalCoreSubmesh_AddSubMorphTarget(CalCoreSubmesh *self, CalCoreSubMorphTargetDiffMap* inTarget)
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


const CalIndex* cal3d::CalCoreSubmesh_GetFaces(struct CalCoreSubmesh *self)
{
	return &self->getVectorFace()[0].vertexId[0];
}


//****************************************************************************//
// CalCoreSubMorphTargetDiffMap wrapper functions definition                  //
//****************************************************************************//
CalCoreSubMorphTargetDiffMap* cal3d::CalCoreSubMorphTargetDiffMap_New()
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

CalCoreSubMorphTargetDiffMap* cal3d::CalCoreSubMorphTargetDiffMap_Clone(
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

void cal3d::CalCoreSubMorphTargetDiffMap_Delete(CalCoreSubMorphTargetDiffMap* inSelf)
{
	delete inSelf;
}

bool cal3d::CalCoreSubMorphTargetDiffMap_Reserve(CalCoreSubMorphTargetDiffMap* inSelf,
	int inNumDifferences )
{
	return inSelf->reserve( inNumDifferences );
}

bool cal3d::CalCoreSubMorphTargetDiffMap_AppendVertex(CalCoreSubMorphTargetDiffMap* inSelf,
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

CalErrorCode cal3d::CalError_GetLastErrorCode()
{
  return (CalErrorCode)CalError::getLastErrorCode();
}

const char *cal3d::CalError_GetLastErrorDescription()
{
  static std::string strDescription;
  strDescription = CalError::getLastErrorDescription();

  return strDescription.c_str();
}

const char *cal3d::CalError_GetLastErrorFile()
{
  return CalError::getLastErrorFile().c_str();
}

int cal3d::CalError_GetLastErrorLine()
{
  return CalError::getLastErrorLine();
}

const char *cal3d::CalError_GetLastErrorText()
{
  return CalError::getLastErrorText().c_str();
}

void cal3d::CalError_PrintLastError()
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

void cal3d::CalLoader_Delete(CalLoader *self)
{
  delete self;
}

CalCoreAnimation *cal3d::CalLoader_LoadCoreAnimation(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreAnimation(strFilename).get());
}

CalCoreMaterial *CalLoader_LoadCoreMaterial(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreMaterial(strFilename).get());
}

CalCoreMesh *cal3d::CalLoader_LoadCoreMesh(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreMesh(strFilename).get());
}

CalCoreMesh *cal3d::CalLoader_LoadCoreMeshFromBuffer(const void *data)
{
	try
	{
		CalCoreMeshPtr	theMesh(CalLoader::loadCoreMesh((const char*)(data)));

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

CalCoreSkeleton *cal3d::CalLoader_LoadCoreSkeleton(CalLoader *self, const char *strFilename)
{
  return explicitIncRef(self->loadCoreSkeleton(strFilename).get());
}

CalLoader *cal3d::CalLoader_New()
{
  return new CalLoader();
}

void cal3d::CalLoader_SetLoadingMode(int flags)
{
	CalLoader::setLoadingMode( flags );
}

//****************************************************************************//
// CalMesh wrapper functions definition                                       //
//****************************************************************************//

void cal3d::CalMesh_Delete(CalMesh *self)
{
  delete self;
}

CalCoreMesh *cal3d::CalMesh_GetCoreMesh(CalMesh *self)
{
  return self->getCoreMesh();
}

CalSubmesh *cal3d::CalMesh_GetSubmesh(CalMesh *self, int id)
{
  return self->getSubmesh(id);
}

int cal3d::CalMesh_GetSubmeshCount(CalMesh *self)
{
  return self->getSubmeshCount();
}

/*
std::vector<CalSubmesh *>& CalMesh_GetVectorSubmesh(CalMesh *self)
{
  return self->getVectorSubmesh();
}
*/

CalMesh *cal3d::CalMesh_New(CalCoreMesh *pCoreMesh)
{
  return new(std::nothrow) CalMesh(pCoreMesh);
}

void cal3d::CalMesh_SetLodLevel(CalMesh *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

void cal3d::CalMesh_SetMaterialSet(CalMesh *self, int setId, CalCoreModel *core)
{
  self->setMaterialSet(setId,core);
}



//****************************************************************************//
// CalMixer wrapper functions definition                                      //
//****************************************************************************//

CalBoolean cal3d::CalMixer_BlendCycle(CalMixer *self, int id, float weight, float delay)
{
  return self->blendCycle(id, weight, delay) ? True : False;
}

CalBoolean cal3d::CalMixer_ClearCycle(CalMixer *self, int id, float delay)
{
  return self->clearCycle(id, delay) ? True : False;
}

void cal3d::CalMixer_Delete(CalMixer *self)
{
  delete self;
}

CalBoolean cal3d::CalMixer_ExecuteAction(CalMixer *self, int id, float delayIn, float delayOut, enum CalBoolean autoLock)
{
  return self->executeAction(id, delayIn, delayOut, 1.0f,  autoLock == True ? true : false) ? True : False;
}

CalMixer *cal3d::CalMixer_New(CalModel* pModel)
{
  return new(std::nothrow) CalMixer(pModel);
}

void cal3d::CalMixer_UpdateAnimation(CalMixer *self, float deltaTime)
{
  self->updateAnimation(deltaTime);
}

void cal3d::CalMixer_UpdateSkeleton(CalMixer *self)
{
  self->updateSkeleton();
}

void cal3d::CalMixer_RemoveAction(CalMixer *self, int id)
{
  self->stopAction(id);
}

//****************************************************************************//
// CalMorphTargetMixer wrapper functions definition                           //
//****************************************************************************//

float cal3d::CalMorphTargetMixer_GetCurrentWeight(struct CalMorphTargetMixer* self,
											int morphAnimID )
{
	return self->getCurrentWeight( morphAnimID );
}

CalBoolean cal3d::CalMorphTargetMixer_Blend(struct CalMorphTargetMixer* self,
  							int morphAnimID, float weight, float delayIn, float delayOut, bool looping )
{
  return self->blend(morphAnimID, weight, delayIn, delayOut, looping) ? True : False;
}

CalBoolean cal3d::CalMorphTargetMixer_Clear(struct CalMorphTargetMixer *self, int id, float delay)
{
  return self->clear(id, delay) ? True : False;
}

void cal3d::CalMorphTargetMixer_Delete(struct CalMorphTargetMixer *self)
{
  delete self;
}

struct cal3d::CalMorphTargetMixer *CalMorphTargetMixer_New(struct CalModel *pModel)
{
  return new(std::nothrow) CalMorphTargetMixer(pModel);
}

void cal3d::CalMorphTargetMixer_Update(struct CalMorphTargetMixer *self, float deltaTime)
{
  self->update(deltaTime);
}
/*
int CalMorphTargetMixer_GetMorphTargetCount(struct CalMorphTargetMixer *self)
{
  return self->getMorphTargetCount();
}
*/
CalBoolean cal3d::CalMorphTargetMixer_Copy(struct CalMorphTargetMixer* self,
  										const struct CalMorphTargetMixer* toCopy )
{
	return (CalBoolean) self->copy( *toCopy );
}

//****************************************************************************//
// CalModel wrapper functions definition                                      //
//****************************************************************************//

CalBoolean cal3d::CalModel_AttachMesh(CalModel *self, int coreMeshId)
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

void cal3d::CalModel_Delete(CalModel *self)
{
  delete self;
}

CalBoolean cal3d::CalModel_DetachMesh(CalModel *self, int coreMeshId)
{
  return self->detachMesh(coreMeshId) ? True : False;
}

CalCoreModel *cal3d::CalModel_GetCoreModel(CalModel *self)
{
  return self->getCoreModel();
}

CalMesh *cal3d::CalModel_GetMesh(CalModel *self, int coreMeshId)
{
  return self->getMesh(coreMeshId);
}

const CalMesh *cal3d::CalModel_GetMeshConst(const CalModel *self, int coreMeshId)
{
  return self->getMesh(coreMeshId);
}

CalMixer *cal3d::CalModel_GetMixer(CalModel *self)
{
  return self->getMixer();
}

CalMorphTargetMixer *cal3d::CalModel_GetMorphTargetMixer(CalModel *self)
{
  return self->getMorphTargetMixer();
}

const CalMorphTargetMixer *cal3d::CalModel_GetMorphTargetMixerConst(const CalModel *self)
{
  return self->getMorphTargetMixer();
}

CalPhysique *cal3d::CalModel_GetPhysique(CalModel *self)
{
  return self->getPhysique();
}

const CalPhysique *cal3d::CalModel_GetPhysiqueConst(const CalModel *self)
{
  return self->getPhysique();
}

void cal3d::CalModel_SetPhysique(struct CalModel *self, struct CalPhysique *phys)
{
	self->setPhysique( phys );
}

CalRenderer *cal3d::CalModel_GetRenderer(CalModel *self)
{
  return self->getRenderer();
}

CalSkeleton *cal3d::CalModel_GetSkeleton(CalModel *self)
{
  return self->getSkeleton();
}

CalSpringSystem *cal3d::CalModel_GetSpringSystem(CalModel *self)
{
  return self->getSpringSystem();
}

const CalSpringSystem *cal3d::CalModel_GetSpringSystemConst(const CalModel *self)
{
  return self->getSpringSystem();
}

CalUserData cal3d::CalModel_GetUserData(CalModel *self)
{
  return self->getUserData();
}

/*
std::vector<CalMesh *>& CalModel_GetVectorMesh(CalModel *self)
{
  return self->getVectorMesh();
}
*/

int cal3d::CalModel_GetMeshCount(struct CalModel *self)
{
	return self->getVectorMesh().size();
}

struct CalMesh *cal3d::CalModel_GetMeshByMeshID(struct CalModel *self, int meshId)
{
	CalMesh*	theMesh = 0;

	if ( (meshId >= 0) && (meshId < self->getVectorMesh().size()) )
	{
		theMesh = self->getVectorMesh()[ meshId ];
	}
	return theMesh;
}

CalModel *cal3d::CalModel_New(CalCoreModel* pCoreModel)
{
  return new(std::nothrow) CalModel(pCoreModel);
}

void cal3d::CalModel_SetLodLevel(CalModel *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

void cal3d::CalModel_SetMaterialSet(CalModel *self, int setId)
{
  self->setMaterialSet(setId);
}

void cal3d::CalModel_SetUserData(CalModel *self, CalUserData userData)
{
  self->setUserData(userData);
}

void cal3d::CalModel_Update(CalModel *self, float deltaTime)
{
  self->update(deltaTime);
}

//****************************************************************************//
// CalPhysique wrapper functions definition                                   //
//****************************************************************************//

int cal3d::CalPhysique_CalculateNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pNormalBuffer)
{
  return self->calculateNormals(pSubmesh, pNormalBuffer);
}

int cal3d::CalPhysique_CalculateVertices(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  return self->calculateVertices(pSubmesh, pVertexBuffer);
}

int cal3d::CalPhysique_CalculateVerticesAndNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  return self->calculateVerticesAndNormals(pSubmesh, pVertexBuffer);
}

int cal3d::CalPhysique_CalculateVerticesNormalsAndTexCoords(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords)
{
  return self->calculateVerticesNormalsAndTexCoords(pSubmesh, pVertexBuffer, NumTexCoords);
}

int cal3d::CalPhysique_CalculateTangentSpaces(CalPhysique *self, CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer)
{
  return self->calculateTangentSpaces(pSubmesh, mapId, pTangentSpaceBuffer);
}

void cal3d::CalPhysique_Delete(CalPhysique *self)
{
  delete self;
}

CalPhysique *cal3d::CalPhysique_New(CalModel* pModel)
{
  return new(std::nothrow) CalPhysique(pModel);
}

CalPhysique *cal3d::CalPhysiqueDualQuat_New(CalModel* pModel)
{
  return new(std::nothrow) CalPhysiqueDualQuat(pModel);
}

void cal3d::CalPhysique_Update(CalPhysique *self)
{
  self->update();
}

//****************************************************************************//
// CalPlatform wrapper functions definition                                   //
//****************************************************************************//

//****************************************************************************//
// CalQuaternion wrapper functions definition                                 //
//****************************************************************************//

void cal3d::CalQuaternion_Blend(CalQuaternion *self, float d, const CalQuaternion *pQ)
{
  self->blend(d, *pQ);
}

void cal3d::CalQuaternion_Clear(CalQuaternion *self)
{
  self->clear();
}

void cal3d::CalQuaternion_Conjugate(CalQuaternion *self)
{
  self->conjugate();
}

void cal3d::CalQuaternion_Invert(struct CalQuaternion *self)
{
	self->invert();
}

void cal3d::CalQuaternion_Delete(CalQuaternion *self)
{
  delete self;
}

void cal3d::CalQuaternion_Equal(CalQuaternion *self, const CalQuaternion *pQ)
{
  *self = *pQ;
}

float *cal3d::CalQuaternion_Get(CalQuaternion *self)
{
  return &(self->x);
}

void cal3d::CalQuaternion_Multiply(CalQuaternion *self, const CalQuaternion *pQ)
{
  *self *= *pQ;
}

void cal3d::CalQuaternion_MultiplyVector(CalQuaternion *self, const CalVector *pV)
{
  *self *= *pV;
}

CalQuaternion *cal3d::CalQuaternion_New()
{
  return new(std::nothrow) CalQuaternion();
}

void cal3d::CalQuaternion_Op_Multiply(CalQuaternion *pResult, const CalQuaternion *pQ, const CalQuaternion *pR)
{
  *pResult = *pQ * *pR;
}

void cal3d::CalQuaternion_Set(CalQuaternion *self, float qx, float qy, float qz, float qw)
{
  self->set(qx, qy, qz, qw);
}

//****************************************************************************//
// CalRender wrapper functions definition                                         //
//****************************************************************************//

CalBoolean cal3d::CalRenderer_BeginRendering(CalRenderer *self)
{
  return self->beginRendering() ? True : False;
}

void cal3d::CalRenderer_Delete(CalRenderer *self)
{
  delete self;
}

void cal3d::CalRenderer_EndRendering(CalRenderer *self)
{
  self->endRendering();
}

void cal3d::CalRenderer_GetAmbientColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getAmbientColor(pColorBuffer);
}

void cal3d::CalRenderer_GetDiffuseColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getDiffuseColor(pColorBuffer);
}

int cal3d::CalRenderer_GetFaceCount(CalRenderer *self)
{
  return self->getFaceCount();
}

int cal3d::CalRenderer_GetFaces(CalRenderer *self, CalIndex *pFaceBuffer)
{
  return self->getFaces(pFaceBuffer);
}

int cal3d::CalRenderer_GetMapCount(CalRenderer *self)
{
  return self->getMapCount();
}

CalUserData cal3d::CalRenderer_GetMapUserData(CalRenderer *self, int mapId)
{
  return (CalUserData)self->getMapUserData(mapId);
}

int cal3d::CalRenderer_GetMeshCount(CalRenderer *self)
{
  return self->getMeshCount();
}

int cal3d::CalRenderer_GetNormals(CalRenderer *self, float *pNormalBuffer)
{
  return self->getNormals(pNormalBuffer);
}

float cal3d::CalRenderer_GetShininess(CalRenderer *self)
{
  return self->getShininess();
}

void cal3d::CalRenderer_GetSpecularColor(CalRenderer *self, unsigned char *pColorBuffer)
{
  self->getSpecularColor(pColorBuffer);
}

int cal3d::CalRenderer_GetSubmeshCount(CalRenderer *self, int meshId)
{
  return self->getSubmeshCount(meshId);
}

int cal3d::CalRenderer_GetTextureCoordinates(CalRenderer *self, int mapId, float *pTextureCoordinateBuffer)
{
  return self->getTextureCoordinates(mapId, pTextureCoordinateBuffer);
}

int cal3d::CalRenderer_GetVertexCount(CalRenderer *self)
{
  return self->getVertexCount();
}

int cal3d::CalRenderer_GetVertices(CalRenderer *self, float *pVertexBuffer)
{
  return self->getVertices(pVertexBuffer);
}

int cal3d::CalRenderer_GetVerticesAndNormals(CalRenderer *self, float *pVertexBuffer)
{
  return self->getVerticesAndNormals(pVertexBuffer);
}

int cal3d::CalRenderer_GetVerticesNormalsAndTexCoords(CalRenderer *self, float *pVertexBuffer, int NumTexCoords)
{
  return self->getVerticesNormalsAndTexCoords(pVertexBuffer,NumTexCoords);
}

int cal3d::CalRenderer_GetTangentSpaces(CalRenderer *self, int mapId, float *pTangentSpaceBuffer)
{
  return self->getTangentSpaces(mapId, pTangentSpaceBuffer);
}

CalBoolean cal3d::CalRenderer_IsTangentsEnabled(CalRenderer *self, int mapId)
{
  return self->isTangentsEnabled(mapId) ? True : False;
}

CalRenderer *cal3d::CalRenderer_New(CalModel* pModel)
{
  return new(std::nothrow) CalRenderer(pModel);
}

CalBoolean cal3d::CalRenderer_SelectMeshSubmesh(CalRenderer *self, int meshId, int submeshId)
{
  return self->selectMeshSubmesh(meshId, submeshId) ? True : False;
}

//****************************************************************************//
// CalPlatform wrapper functions definition                                   //
//****************************************************************************//

void cal3d::CalSaver_Delete(CalSaver *self)
{
  delete self;
}

CalSaver *cal3d::CalSaver_New()
{
  return new(std::nothrow) CalSaver();
}

CalBoolean cal3d::CalSaver_SaveCoreAnimation(CalSaver *self, const char *strFilename, CalCoreAnimation *pCoreAnimation)
{
  return self->saveCoreAnimation(strFilename, pCoreAnimation) ? True : False;
}

CalBoolean cal3d::CalSaver_SaveCoreMaterial(CalSaver *self, const char *strFilename, CalCoreMaterial *pCoreMaterial)
{
  return self->saveCoreMaterial(strFilename, pCoreMaterial) ? True : False;
}

CalBoolean cal3d::CalSaver_SaveCoreMesh(CalSaver *self, const char *strFilename, CalCoreMesh *pCoreMesh)
{
  return self->saveCoreMesh(strFilename, pCoreMesh) ? True : False;
}

CalBoolean cal3d::CalSaver_SaveCoreSkeleton(CalSaver *self, const char *strFilename, CalCoreSkeleton *pCoreSkeleton)
{
  return self->saveCoreSkeleton(strFilename, pCoreSkeleton) ? True : False;
}

//****************************************************************************//
// CalSkeleton wrapper functions definition                                   //
//****************************************************************************//

void cal3d::CalSkeleton_CalculateState(CalSkeleton *self)
{
  self->calculateState();
}

void cal3d::CalSkeleton_ClearState(CalSkeleton *self)
{
  self->clearState();
}

void cal3d::CalSkeleton_Delete(CalSkeleton *self)
{
  delete self;
}

CalBone *cal3d::CalSkeleton_GetBone(CalSkeleton *self, int boneId)
{
  return self->getBone(boneId);
}

const CalBone *cal3d::CalSkeleton_GetBoneConst(const CalSkeleton *self, int boneId)
{
  return self->getBone(boneId);
}

int cal3d::CalSkeleton_GetBoneCount(const CalSkeleton *self)
{
	return self->getVectorBone().size();
}

CalCoreSkeleton *cal3d::CalSkeleton_GetCoreSkeleton(CalSkeleton *self)
{
  return self->getCoreSkeleton();
}

const CalCoreSkeleton *cal3d::CalSkeleton_GetCoreSkeletonConst(const CalSkeleton *self)
{
  return self->getCoreSkeleton();
}

std::vector<CalBone *>& cal3d::CalSkeleton_GetVectorBone(CalSkeleton *self)
{
  return self->getVectorBone();
}


void cal3d::CalSkeleton_LockState(CalSkeleton *self)
{
  self->lockState();
}

CalSkeleton *cal3d::CalSkeleton_New(CalCoreSkeleton *pCoreSkeleton)
{
  return new(std::nothrow) CalSkeleton(pCoreSkeleton);
}

void cal3d::CalSkeleton_GetBoneBoundingBox(CalSkeleton *self, float *min, float *max)
{
	self->getBoneBoundingBox(min, max);
}

void cal3d::CalSkeleton_CalculateBoundingBoxes(CalSkeleton *self)
{
	self->calculateBoundingBoxes();
}

//****************************************************************************//
// CalSpringSystem wrapper functions definition                               //
//****************************************************************************//

void cal3d::CalSpringSystem_CalculateForces(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime)
{
  self->calculateForces(pSubmesh, deltaTime);
}

void cal3d::CalSpringSystem_CalculateVertices(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime)
{
  self->calculateVertices(pSubmesh, deltaTime);
}

void cal3d::CalSpringSystem_Delete(CalSpringSystem *self)
{
  delete self;
}

CalSpringSystem *cal3d::CalSpringSystem_New(CalModel* pModel)
{
  return new(std::nothrow) CalSpringSystem(pModel);
}

void cal3d::CalSpringSystem_Update(CalSpringSystem *self, float deltaTime)
{
  self->update(deltaTime);
}

const CalVector *cal3d::CalSpringSystem_GetGravityVector(const CalSpringSystem *self)
{
	return &self->getGravityVector();
}

const CalVector *cal3d::CalSpringSystem_GetForceVector(const CalSpringSystem *self)
{
	return &self->getForceVector();
}

void cal3d::CalSpringSystem_SetGravityVector(CalSpringSystem *self, CalVector* grav)
{
	self->setGravityVector( *grav );
}

void cal3d::CalSpringSystem_SetForceVector(CalSpringSystem *self, CalVector* grav)
{
	self->setForceVector( *grav );
}

void cal3d::CalSpringSystem_ResetPositions(struct CalSpringSystem *self)
{
	self->resetPositions();
}

//****************************************************************************//
// CalSubmesh wrapper functions definition                                    //
//****************************************************************************//

void cal3d::CalSubmesh_Delete(CalSubmesh *self)
{
  delete self;
}

CalCoreSubmesh *cal3d::CalSubmesh_GetCoreSubmesh(CalSubmesh *self)
{
  return self->getCoreSubmesh();
}

int cal3d::CalSubmesh_GetCoreMaterialId(CalSubmesh *self)
{
  return self->getCoreMaterialId();
}

int cal3d::CalSubmesh_GetFaceCount(CalSubmesh *self)
{
  return self->getFaceCount();
}

int cal3d::CalSubmesh_GetFaces(CalSubmesh *self, CalIndex *pFaceBuffer)
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

int cal3d::CalSubmesh_GetVertexCount(CalSubmesh *self)
{
  return self->getVertexCount();
}

CalBoolean cal3d::CalSubmesh_HasInternalData(CalSubmesh *self)
{
  return self->hasInternalData() ? True : False;
}

CalSubmesh *cal3d::CalSubmesh_New(CalCoreSubmesh* coreSubmesh)
{
  return new(std::nothrow) CalSubmesh(coreSubmesh);
}

void cal3d::CalSubmesh_SetCoreMaterialId(CalSubmesh *self, int coreMaterialId)
{
  self->setCoreMaterialId(coreMaterialId);
}

void cal3d::CalSubmesh_SetLodLevel(CalSubmesh *self, float lodLevel)
{
  self->setLodLevel(lodLevel);
}

//****************************************************************************//
// CalVector wrapper functions definition                                     //
//****************************************************************************//

void cal3d::CalVector_Add(CalVector *self, CalVector *pV)
{
  *self += *pV;
}

void cal3d::CalVector_Blend(CalVector *self, float d, CalVector *pV)
{
  self->blend(d, *pV);
}

void cal3d::CalVector_Clear(CalVector *self)
{
  self->clear();
}

void cal3d::CalVector_Delete(CalVector *self)
{
  delete self;
}

void cal3d::CalVector_Equal(CalVector *self, CalVector *pV)
{
  *self = *pV;
}

void cal3d::CalVector_InverseScale(CalVector *self, float d)
{
  *self /= d;
}

float *cal3d::CalVector_Get(CalVector *self)
{
  return &(self->x);
}

float cal3d::CalVector_Length(CalVector *self)
{
  return self->length();
}

CalVector *cal3d::CalVector_New()
{
  return new(std::nothrow) CalVector();
}

float cal3d::CalVector_Normalize(CalVector *self)
{
  return self->normalize();
}

void cal3d::CalVector_Op_Add(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV + *pU;
}

void cal3d::CalVector_Op_Subtract(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV - *pU;
}

void cal3d::CalVector_CalVector_Op_Scale(CalVector *pResult, CalVector *pV, float d)
{
  *pResult = *pV * d;
}

void cal3d::CalVector_CalVector_Op_InverseScale(CalVector *pResult, CalVector *pV, float d)
{
  *pResult = *pV / d;
}

float cal3d::CalVector_Op_Scalar(CalVector *pV, CalVector *pU)
{
  return *pV * *pU;
}

void cal3d::CalVector_Op_Cross(CalVector *pResult, CalVector *pV, CalVector *pU)
{
  *pResult = *pV % *pU;
}

void cal3d::CalVector_Scale(CalVector *self, float d)
{
  *self *= d;
}

void cal3d::CalVector_Set(CalVector *self, float vx, float vy, float vz)
{
  self->set(vx, vy, vz);
}

void cal3d::CalVector_Subtract(CalVector *self, CalVector *pV)
{
  *self = *pV;
}

void cal3d::CalVector_Transform(CalVector *self, CalQuaternion *pQ)
{
  *self *= *pQ;
}

//****************************************************************************//

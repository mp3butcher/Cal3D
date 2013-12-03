//----------------------------------------------------------------------------//
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (c) 2012 Samsung Electronics Co., Ltd.                           //
// http://www.samsung.com                     								  //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef CAL3DLOADER_H_
#define CAL3DLOADER_H_

#include <FBase.h>
#include <FSystem.h>
#include <FApp.h>
#include <FGraphicsOpengl.h>

#include "cal3d.h"
#include "tga.h"

using namespace Tizen::Graphics::Opengl;

class Cal3DLoader {
public:
	Cal3DLoader();
	void UpdateModel(float elapsedSeconds);
	CalModel* getCurrentCalModel();
	void NextModel();
	float getScale();
	virtual ~Cal3DLoader();

public:
	static const int MODEL_SKELETON = 0;
	static const int MODEL_CALLY = 1;
	static const int MODEL_PALADIN = 2;

private:
	void CreateCal3d(std::string modelName, float* scale, CalCoreModel* calCoreModel);
	void InitCal3d(CalCoreModel* calCoreModel, CalModel* calModel);

	bool ParseModelConfiguration(const std::string& strFilename, const std::string& filesDirectory, CalCoreModel*, float* scale);
	GLuint LoadTexture(const std::string& strFilename);

	std::string GetModelName(int modelIndex);
	std::string GetResourcesPath();
	std::string ToUtf8(const Tizen::Base::String& tizenStr);
	void ChangeModel(int modelIndex);

private:
	int 	__currentModel;

	CalCoreModel* __pCurrentCalCoreModel;
	CalCoreModel* __pSkeletonCalCoreModel;
	CalCoreModel* __pCallyCalCoreModel;
	CalCoreModel* __pPaladinCalCoreModel;

	CalModel* __pCurrentCalModel;
	CalModel* __pSkeletonCalModel;
	CalModel* __pCallyCalModel;
	CalModel* __pPaladinCalModel;

	float 	__scale;
	float 	__scaleSkeleton;
	float 	__scaleCally;
	float 	__scalePaladin;
	int 	__currentAnimationId;
	float 	__leftAnimationTime;
	float 	__blendTime;
};

#endif /* CAL3DLOADER_H_ */

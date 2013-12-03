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

#include "Cal3DLoader.h"

using namespace Tizen::App;

Cal3DLoader::Cal3DLoader()
	: __currentModel(MODEL_SKELETON)

	, __pCurrentCalCoreModel(null)
	, __pSkeletonCalCoreModel(new CalCoreModel(GetModelName(MODEL_SKELETON)))
	, __pCallyCalCoreModel(new CalCoreModel(GetModelName(MODEL_CALLY)))
	, __pPaladinCalCoreModel(new CalCoreModel(GetModelName(MODEL_PALADIN)))

	, __pCurrentCalModel(null)
	, __pSkeletonCalModel(null)
	, __pCallyCalModel(null)
	, __pPaladinCalModel(null)

	, __scale(1.0f)
	, __scaleSkeleton(1.0f)
	, __scaleCally(1.0f)
	, __scalePaladin(1.0f)
	, __blendTime(0.3f)
{
	this->CreateCal3d(GetModelName(MODEL_SKELETON), &__scaleSkeleton, __pSkeletonCalCoreModel);
	__pSkeletonCalModel = new CalModel(__pSkeletonCalCoreModel);
	ChangeModel(MODEL_SKELETON);
	this->InitCal3d(__pSkeletonCalCoreModel, __pSkeletonCalModel);

	this->CreateCal3d(GetModelName(MODEL_CALLY), &__scaleCally, __pCallyCalCoreModel);
	__pCallyCalModel = new CalModel(__pCallyCalCoreModel);
	ChangeModel(MODEL_CALLY);
	this->InitCal3d(__pCallyCalCoreModel, __pCallyCalModel);

	this->CreateCal3d(GetModelName(MODEL_PALADIN), &__scalePaladin, __pPaladinCalCoreModel);
	__pPaladinCalModel = new CalModel(__pPaladinCalCoreModel);
	ChangeModel(MODEL_PALADIN);
	this->InitCal3d(__pPaladinCalCoreModel, __pPaladinCalModel);
}

Cal3DLoader::~Cal3DLoader() {
	if (__pSkeletonCalCoreModel)
	{
		delete __pSkeletonCalCoreModel;
		__pSkeletonCalCoreModel = null;
	}

	if (__pCallyCalCoreModel)
	{
		delete __pCallyCalCoreModel;
		__pCallyCalCoreModel = null;
	}

	if (__pPaladinCalCoreModel)
	{
		delete __pPaladinCalCoreModel;
		__pPaladinCalCoreModel = null;
	}

	if (__pSkeletonCalModel)
	{
		delete __pSkeletonCalModel;
		__pSkeletonCalModel = null;
	}

	if (__pCallyCalModel)
	{
		delete __pCallyCalModel;
		__pCallyCalModel = null;
	}

	if (__pPaladinCalModel)
	{
		delete __pPaladinCalModel;
		__pPaladinCalModel = null;
	}
}

void
Cal3DLoader::CreateCal3d(std::string modelName, float* scale, CalCoreModel* calCoreModel)
{
	// show some information
	//Might need MinGW installed for proper cout function working
	AppLog("o----------------------------------------------------------------o");
	AppLog("|                    Cal3D MiniViewer for Tizen                  |");
	AppLog("|     (C) 2001, 2002, 2003 Bruno 'Beosil' Heidelberger           |");
	AppLog("|     Copyright (c) 2012 Samsung Electronics Co., Ltd.           |");
	AppLog("|                  http://www.samsung.com                        |");
	AppLog("o----------------------------------------------------------------o");
	AppLog("| This program is free software; you can redistribute it and/or  |");
	AppLog("| modify it under the terms of the GNU General Public License as |");
	AppLog("| published by the Free Software Foundation; either version 2 of |");
	AppLog("| the License, or (at your option) any later version.            |");
	AppLog("o----------------------------------------------------------------o");

	bool bModelConfiguration;

	std::string filePath;
	std::string resourcesPath = GetResourcesPath();
	filePath.append(resourcesPath);
	filePath.append(modelName);
	filePath.append("/");
	filePath.append(modelName);
	filePath.append(".cfg");
	// parse the model configuration file
	AppLog("filePath: %s", filePath.c_str());

	if (!ParseModelConfiguration(filePath, resourcesPath, calCoreModel, scale)) {
		bModelConfiguration = false;
	} else {
		// set model configuration flag
		bModelConfiguration = true;
	}

	// check if we have successfully loaded a model configuration
	if (!bModelConfiguration) {
		AppLog("No model configuration file given.");
	}

	// make one material thread for each material
	// NOTE: this is not the right way to do it, but this viewer can't do the right
	// mapping without further information on the model etc., so this is the only
	// thing we can do here.
	int materialId;
	for (materialId = 0; materialId < calCoreModel->getCoreMaterialCount();
			materialId++) {
		// create the a material thread
		calCoreModel->createCoreMaterialThread(materialId);

		// initialize the material thread
		calCoreModel->setCoreMaterialId(materialId, 0, materialId);
	}
}

//----------------------------------------------------------------------------//
// Initialize the demo                                                        //
//----------------------------------------------------------------------------//

void Cal3DLoader::InitCal3d(CalCoreModel* calCoreModel,
		CalModel* calModel) {
	// load all textures and store the opengl texture id in the corresponding map in the material
	for (int materialId = 0; materialId < calCoreModel->getCoreMaterialCount();
			materialId++) {
		// get the core material
		CalCoreMaterial *pCoreMaterial = calCoreModel->getCoreMaterial(
				materialId);

		// loop through all maps of the core material
		for (int mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++) {
			// get the filename of the texture
			std::string strFilename = pCoreMaterial->getMapFilename(mapId);

			// load the texture from the file
			GLuint textureId = LoadTexture(strFilename);

			// store the opengl texture id in the user data of the map
			pCoreMaterial->setMapUserData(mapId, (Cal::UserData) textureId);
		}
	}

	// attach all meshes to the model
	for (int meshId = 0; meshId < calCoreModel->getCoreMeshCount(); meshId++) {
		calModel->attachMesh(meshId);
	}

	// set the material set of the whole model
	calModel->setMaterialSet(0);

	// set initial animation state
	if (calCoreModel->getCoreAnimationCount() > 0) {
		__currentAnimationId = 0;
		__leftAnimationTime = calCoreModel->getCoreAnimation(
				__currentAnimationId)->getDuration() - __blendTime;
		if (calCoreModel->getCoreAnimationCount() > 1) {
			calModel->getMixer()->executeAction(__currentAnimationId, 0.0f,
					__blendTime);
		} else {
			calModel->getMixer()->blendCycle(__currentAnimationId, 1.0f, 0.0f);
		}
	} else {
		__currentAnimationId = -1;
		__leftAnimationTime = -1.0f;
	}

	// we're done
	AppLog("Initialization done.");
}

void
Cal3DLoader::UpdateModel(float elapsedSeconds)
{
	// check if the time has come to blend to the next animation
	if (__pCurrentCalCoreModel->getCoreAnimationCount() > 1) {
		__leftAnimationTime -= elapsedSeconds;
		if (__leftAnimationTime <= __blendTime) {
			// get the next animation
			__currentAnimationId = (__currentAnimationId + 1)
					% __pCurrentCalCoreModel->getCoreAnimationCount();

			// fade in the new animation
			__pCurrentCalModel->getMixer()->executeAction(__currentAnimationId,
					__leftAnimationTime, __blendTime);

			// adjust the animation time left until next animation flip
			__leftAnimationTime = __pCurrentCalCoreModel->getCoreAnimation(
					__currentAnimationId)->getDuration() - __blendTime;
		}
	}

	__pCurrentCalModel->update(elapsedSeconds);
}

std::string
Cal3DLoader::GetResourcesPath() {
	std::string filePath;

	Tizen::Base::String resPath = App::GetInstance()->GetAppResourcePath();
	filePath.append(ToUtf8(resPath).c_str());
	return filePath;
}

bool
Cal3DLoader::ParseModelConfiguration(const std::string& strFilename,
		const std::string& filesDirectory, CalCoreModel* calCoreModel,
		float* scale) {
	// open the model configuration file

	std::ifstream file;
	file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
	if (!file) {
		AppLog("Failed to open model configuration file '");
		return false;
	}

	// parse all lines from the model configuration file
	int line;
	for (line = 1;; line++) {
		// read the next model configuration line
		std::string strBuffer;
		std::getline(file, strBuffer);

		// stop if we reached the end of file
		if (file.eof())
			break;

		// check if an error happend while reading from the file
		if (!file) {
			AppLog("Error while reading from the model configuration file '%s'",
					strFilename.c_str());
			return false;
		}

		// find the first non-whitespace character
		std::string::size_type pos;
		pos = strBuffer.find_first_not_of(" \t");

		// check for empty lines
		if ((pos == std::string::npos) || (strBuffer[pos] == '\n')
				|| (strBuffer[pos] == '\r') || (strBuffer[pos] == 0))
			continue;

		// check for comment lines
		if (strBuffer[pos] == '#')
			continue;

		// get the key
		std::string strKey;
		strKey = strBuffer.substr(pos,
				strBuffer.find_first_of(" =\t\n\r", pos) - pos);
		pos += strKey.size();

		// get the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos);
		if ((pos == std::string::npos) || (strBuffer[pos] != '=')) {
			std::cerr << strFilename << "(" << line << "): Invalid syntax."
					<< std::endl;
			return false;
		}

		// find the first non-whitespace character after the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos + 1);

		// get the data
		std::string strData;
		strData = strBuffer.substr(pos,
				strBuffer.find_first_of("\n\r", pos) - pos);

		// handle the model creation
		if (strKey == "scale") {
			// set rendering scale factor
			*scale = atof(strData.c_str());
		} else if (strKey == "skeleton") {
			// load core skeleton
			AppLog("Loading skeleton '%s' ...", strData.c_str());
			if (!calCoreModel->loadCoreSkeleton(filesDirectory + strData)) {
				CalError::printLastError();
				return false;
			}
		} else if (strKey == "animation") {
			// load core animation
			AppLog("Loading animation '%s' ...", strData.c_str());
			if (calCoreModel->loadCoreAnimation(filesDirectory + strData)
					== -1) {
				CalError::printLastError();
				return false;
			}
		} else if (strKey == "mesh") {
			// load core mesh
			AppLog("Loading mesh '%s' ...", strData.c_str());
			if (calCoreModel->loadCoreMesh(filesDirectory + strData) == -1) {
				CalError::printLastError();
				return false;
			}
		} else if (strKey == "material") {
			// load core material
			AppLog("Loading material '%s' ...", strData.c_str());
			if (calCoreModel->loadCoreMaterial(filesDirectory + strData)
					== -1) {
				CalError::printLastError();
				return false;
			}
		} else {
			// everything else triggers an error message, but is ignored
			AppLog("%s (%d)): Invalid syntax.", strFilename.c_str(), line);
		}
	}

	// explicitely close the file
	file.close();

	return true;
}

//----------------------------------------------------------------------------//
// Load and create a texture from a given file                                //
//----------------------------------------------------------------------------//

GLuint
Cal3DLoader::LoadTexture(const std::string& strFilename)
{
	GLuint textureId = 0;
	const char * pch;
	pch = strrchr(strFilename.c_str(), '.');
	if (pch != NULL) {
		if (stricmp(pch, ".raw") == 0) {

			// open the texture file
			std::ifstream file;
			file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
			if (!file) {
				AppLog("Texture file '%s' not found.", strFilename.c_str());
				return 0;
			}

			// load the dimension of the texture
			int width;
			file.read((char *) &width, 4);
			int height;
			file.read((char *) &height, 4);
			int depth;
			file.read((char *) &depth, 4);

			// allocate a temporary buffer to load the texture to
			unsigned char *pBuffer;
			pBuffer = new unsigned char[2 * width * height * depth];
			if (pBuffer == 0) {
				AppLog("Memory allocation for texture '%s' failed.", strFilename.c_str());
				return 0;
			}

			// load the texture
			file.read((char *) pBuffer, width * height * depth);

			// explicitely close the file
			file.close();

			// flip texture around y-axis (-> opengl-style)
			int y;
			for (y = 0; y < height; y++) {
				memcpy(&pBuffer[(height + y) * width * depth],
						&pBuffer[(height - y - 1) * width * depth], width * depth);
			}

			// generate texture
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width,
					height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE,
					&pBuffer[width * height * depth]);
			// free the allocated memory

			delete[] pBuffer;
		} else if (stricmp(pch, ".tga") == 0) {
			std::string filePath;
			filePath.append(GetResourcesPath());
			filePath.append(GetModelName(__currentModel));
			filePath.append("/");
			filePath.append(strFilename.c_str());
			CTga *Tga;
			Tga = new CTga();
			if (Tga->ReadFile(filePath.c_str()) == 0) {
				Tga->Release();
				delete Tga;
				return 0;
			}

			if (Tga->Bpp() != 32) {
				Tga->Release();
				delete Tga;
				return 0;
			}

			//Flip texture
			int width = Tga->GetSizeX();
			int height = Tga->GetSizeY();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &textureId);

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, (char*) Tga->GetPointer());

			Tga->Release();
			delete Tga;
		}
	} else {
		AppLog("Incorrect texture path");
	}
	return textureId;
}

std::string
Cal3DLoader::GetModelName(int modelIndex)
{
	if (modelIndex == MODEL_SKELETON) return std::string("skeleton");
	else if (modelIndex == MODEL_CALLY) return std::string("cally");
	else return std::string("paladin");
}

std::string
Cal3DLoader::ToUtf8(const Tizen::Base::String& tizenStr)
{
	std::string utf8Str;
	if (tizenStr.GetLength() == 0)
		return utf8Str;

	Tizen::Base::ByteBuffer* pBuffer = Tizen::Base::Utility::StringUtil::StringToUtf8N(tizenStr);
	if (pBuffer != null)
	{
		int byteCount = pBuffer->GetLimit();
		char* chPtrBuf = new char[byteCount + 1];
		if (chPtrBuf != null) {
			pBuffer->GetArray((byte*) chPtrBuf, 0, byteCount);
			utf8Str.assign(chPtrBuf, byteCount);
			delete[] chPtrBuf;
		}
		if (pBuffer != null)
			delete pBuffer;
	}
	return utf8Str;
}

void
Cal3DLoader::ChangeModel(int modelIndex)
{
	switch(modelIndex)
	{
	case MODEL_SKELETON:
		__currentModel = MODEL_SKELETON;
		__scale = __scaleSkeleton;
		__pCurrentCalCoreModel = __pSkeletonCalCoreModel;
		__pCurrentCalModel = __pSkeletonCalModel;
		break;
	case MODEL_CALLY:
		__currentModel = MODEL_CALLY;
		__scale = __scaleCally;
		__pCurrentCalCoreModel = __pCallyCalCoreModel;
		__pCurrentCalModel = __pCallyCalModel;
		break;
	case MODEL_PALADIN:
		__currentModel = MODEL_PALADIN;
		__scale = __scalePaladin;
		__pCurrentCalCoreModel = __pPaladinCalCoreModel;
		__pCurrentCalModel = __pPaladinCalModel;
		break;
	default:
		break;
	}
}

void
Cal3DLoader::NextModel()
{
	__currentModel++;
	if (__currentModel == 3)
		ChangeModel(0);
	else
		ChangeModel(__currentModel);
}

CalModel*
Cal3DLoader::getCurrentCalModel()
{
	return __pCurrentCalModel;
}

float
Cal3DLoader::getScale()
{
	return __scale;
}

//----------------------------------------------------------------------------//
// Cal3DViewer.cpp                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (c) 2012 Samsung Electronics Co., Ltd.                           //
// http://www.samsung.com                     								  //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "Cal3DViewer.h"
#include "tga.h"
#include <math.h>

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Base::Runtime;
using namespace Osp::Graphics;
using namespace Osp::Locales;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::System;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics::Opengl;
using namespace Osp::Base::Utility;


#define TIME_OUT  10
#define DISPLAY_FPS

const std::string Cal3DViewer::FILES_PATH = "/Res/";

class ViewerForm :
	public Osp::Ui::Controls::Form
{
public:
	ViewerForm(Cal3DViewer* pApp)
	: __pApp(pApp)
	{
	}
	virtual ~ViewerForm(void)
	{
	}

public:
	virtual result OnDraw(void)
	{
		if (__pApp)
		{
			__pApp->Draw();
		}
		return E_SUCCESS;
	}

private:
	Cal3DViewer* __pApp;
};


namespace
{
	typedef float GlUnit;

	GlUnit GetGlUnit(float vaule)
	{
		return vaule;
	}
	GlUnit GetGlUnit(int val)
	{
		return float(val);
	}

	#define glClearColorEx glClearColor
	#define glFrustum      glFrustumf
	#define glFog          glFogf
	#define glTranslate    glTranslatef
	#define glRotate       glRotatef
	#define GL_TFIXED      GL_FLOAT
}


Cal3DViewer::Cal3DViewer()
:
	__eglDisplay(EGL_DEFAULT_DISPLAY),
	__eglSurface(EGL_NO_SURFACE),
	__eglConfig(null),
	__eglContext(EGL_NO_CONTEXT),
	__pbuffer_surface(EGL_NO_SURFACE),
	__pixmap_surface(EGL_NO_SURFACE),
	__pTimer(null),
	__pForm(null),

	__pCurrentCalCoreModel(null),
	__pSkeletonCalCoreModel(new CalCoreModel(GetModelName(MODEL_SKELETON))),
	__pCallyCalCoreModel(new CalCoreModel(GetModelName(MODEL_CALLY))),
	__pPaladinCalCoreModel(new CalCoreModel(GetModelName(MODEL_PALADIN))),

	__lastTouchPositionX(0),
	__lastTouchPositionY(0),
	__tiltAngle(-70.0f),
	__twistAngle(-45.0f),
	__distance(270.0f),
	__lastTick(0),
	__pCurrentCalModel(null),
	__pSkeletonCalModel(null),
	__pCallyCalModel(null),
	__pPaladinCalModel(null),
	__currentModel(MODEL_SKELETON),
	__scale(1.0f),
	__scaleSkeleton(1.0f),
	__scaleCally(1.0f),
	__scalePaladin(1.0f),
	__blendTime(0.3f),
	__lodLevel(1.0f),
	__vertexCount(0),
	__faceCount(0),

	__fpsDuration(0.0f),
	__fpsFrames(0),
	__fps(0),
	__timeScale(1.0f),

	__multiTouch(false),
	__currentDistance(0),
	__changeModel(false)
{
}


Cal3DViewer::~Cal3DViewer()
{
}

void
Cal3DViewer::Cleanup()
{
	if (__pTimer)
	{
		__pTimer->Cancel();
		delete __pTimer;
		__pTimer = null;
	}

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

	DestroyGL();
}


Application*
Cal3DViewer::CreateInstance(void)
{
	return new Cal3DViewer();
}


bool
Cal3DViewer::OnAppInitializing(AppRegistry& appRegistry)
{
	Timer* tempTimer = null;
	result r = E_SUCCESS;

	__pForm = new ViewerForm(this);
	if (null == __pForm)
	{
		AppLog("[Cal3DViewer] Allocation of CalViewer has failed.\n");
		goto CATCH_2;
	}

	r = __pForm->Construct(FORM_STYLE_NORMAL);
	if (IsFailed(r))
	{
		AppLog("[Cal3DViewer] __pForm->Construct(FORM_STYLE_NORMAL) has failed.\n");
		goto CATCH_1;
	}

	r = GetAppFrame()->GetFrame()->AddControl(*__pForm);
	if (IsFailed(r))
	{
		AppLog("[Cal3DViewer] GetAppFrame()->GetFrame()->AddControl(*__pForm) has failed.\n");
		goto CATCH_1;
	}

	if (!InitEGL())
	{
		AppLog("[Cal3DViewer] Cal3DViewer::InitEGL() has failed.\n");
		goto CATCH_2;
	}

	if (!InitGL())
	{
		AppLog("[Cal3DViewer] Cal3DViewer::InitGL() has failed.\n");
		goto CATCH_2;
	}

	tempTimer = new Timer;
	if (null == tempTimer)
	{
		goto CATCH_2;
	}

	r = tempTimer->Construct(*this);
	if (IsFailed(r))
	{
		goto CATCH_2;
	}

	__pTimer  = tempTimer;
	tempTimer = 0;

	this->InitMultiTouch();

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

	return true;

CATCH_1:
	delete __pForm;
	__pForm = null;

CATCH_2:
	delete tempTimer;
	Cleanup();
	return false;
}


bool
Cal3DViewer::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	Cleanup();

	return true;
}


void
Cal3DViewer::OnForeground(void)
{
	if (__pTimer)
	{
		__pTimer->Start(TIME_OUT);
	}
}


void
Cal3DViewer::OnBackground(void)
{
	if (__pTimer)
	{
		__pTimer->Cancel();
	}
}


void
Cal3DViewer::OnLowMemory(void)
{
}


void
Cal3DViewer::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
}


void
Cal3DViewer::OnTimerExpired(Timer& timer)
{
	if (!__pTimer)
	{
		return;
	}

	__pTimer->Start(TIME_OUT);

	if (!Draw())
	{
		AppLog("[Cal3DViewer] Cal3DViewer::Draw() has failed.\n");
	}
}

void
Cal3DViewer::InitMultiTouch()
{
	__pForm->AddTouchEventListener(*this);
    Touch touch;
    touch.SetMultipointEnabled(*__pForm, true);
}

bool
Cal3DViewer::InitEGL()
{
	EGLint numConfigs = 1;
	EGLint eglConfigList[] = {
		EGL_RED_SIZE,	5,
		EGL_GREEN_SIZE,	6,
		EGL_BLUE_SIZE,	5,
		EGL_ALPHA_SIZE,	0,
		EGL_DEPTH_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_NONE
	};
	EGLint eglContextList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};

	eglBindAPI(EGL_OPENGL_ES_API);

	if (__eglDisplay)
	{
		DestroyGL();
	}

	__eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	if (EGL_NO_DISPLAY == __eglDisplay)
	{
		AppLog("[Cal3DViewer] eglGetDisplay() is failed. [0x%x]\n", eglGetError());
		goto CATCH;
	}

	if (EGL_FALSE == eglInitialize(__eglDisplay, null, null) ||
		EGL_SUCCESS != eglGetError())
	{
		AppLog("[Cal3DViewer] eglInitialize() is failed. [0x%x]\n", eglGetError());
		goto CATCH;
	}

	if (EGL_FALSE == eglChooseConfig(__eglDisplay, eglConfigList, &__eglConfig, 1, &numConfigs) ||
		EGL_SUCCESS != eglGetError())
	{
		AppLog("[Cal3DViewer] eglChooseConfig() is failed. [0x%x]\n", eglGetError());
		goto CATCH;
	}

	if (!numConfigs)
	{
		AppLog("[Cal3DViewer] eglChooseConfig() has been failed. because of matching config doesn't exist \n");
		goto CATCH;
	}

	__eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, (EGLNativeWindowType)__pForm, null);

	if (EGL_NO_SURFACE == __eglSurface ||
		EGL_SUCCESS != eglGetError())
	{
		AppLog("[Cal3DViewer] eglCreateWindowSurface() has been failed. EGL_NO_SURFACE [0x%x]\n", eglGetError());
		goto CATCH;
	}

	__eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextList);
	if (EGL_NO_CONTEXT == __eglContext ||
		EGL_SUCCESS != eglGetError())
	{
		AppLog("[Cal3DViewer] eglCreateContext() has been failed. [0x%x]\n", eglGetError());
		goto CATCH;
	}

	if (false == eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) ||
		EGL_SUCCESS != eglGetError())
	{
		AppLog("[Cal3DViewer] eglMakeCurrent() has been failed. [0x%x]\n", eglGetError());
		goto CATCH;
	}

	return true;

CATCH:
	{
		AppLog("[Cal3DViewer] There are a few reasons for the problem:");
		AppLog("[Cal3DViewer]    1. The current device(real-target or emulator) does not support OpenGL ES(R) 1.1. Check the Release Notes.");
		AppLog("[Cal3DViewer]    2. The system running on emulator cannot support OpenGL(R) 1.5 or later. Try with other system.");
		AppLog("[Cal3DViewer]    3. The system running on emulator does not maintain the latest graphics driver. Update the graphics driver.");
	}

	DestroyGL();
	return false;
}

bool
Cal3DViewer::InitGL()
{
	int x, y, width, height;
	__pForm->GetBounds(x, y, width, height);

	EGLint surfaceType;

	eglGetConfigAttrib(__eglDisplay, __eglConfig, EGL_SURFACE_TYPE, &surfaceType);

	glShadeModel(GL_SMOOTH);

	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	return true;
}


void
Cal3DViewer::DestroyGL()
{

	if (EGL_NO_DISPLAY != __eglDisplay)
	{
		eglMakeCurrent(__eglDisplay, null, null, null);

		if (__pbuffer_surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(__eglDisplay, __pbuffer_surface);
			__pbuffer_surface = EGL_NO_SURFACE;
		}

		if (__pixmap_surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(__eglDisplay, __pixmap_surface);
			__pixmap_surface = EGL_NO_SURFACE;
		}

		if (__eglContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(__eglDisplay, __eglContext);
			__eglContext = EGL_NO_CONTEXT;
		}

		if (__eglSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(__eglDisplay, __eglSurface);
			__eglSurface = EGL_NO_SURFACE;
		}

		eglTerminate(__eglDisplay);
		__eglDisplay = EGL_NO_DISPLAY;
	}

	__eglConfig     = null;

	return;
}

bool
Cal3DViewer::Draw()
{
	{
		this->UpdateModel();
		this->RenderCal3d();

		glFlush();
		glFinish();
	}

	if (__pixmap_surface == EGL_NO_SURFACE)
	{
		eglSwapBuffers(__eglDisplay, __eglSurface);
	}
	else
	{
		eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext);
	}

#if defined(DISPLAY_FPS)
	static float     fps = 0.0f;
	static float     updateInterval = 1000.0f;
	static float     timeSinceLastUpdate = 0.0f;
	static float     frameCount = 0;
	static long long currentTick;
	static long long lastTick;
	static bool      isFirst = true;

	if (isFirst)
	{
		SystemTime::GetTicks(currentTick);
		lastTick = currentTick;
		isFirst = false;
	}

	frameCount++;
	SystemTime::GetTicks(currentTick);

	float elapsed = currentTick - lastTick;

	lastTick = currentTick;
	timeSinceLastUpdate += elapsed;

	if (timeSinceLastUpdate > updateInterval)
	{
		if (timeSinceLastUpdate)
		{
			fps = (frameCount / timeSinceLastUpdate) * 1000.f;
			AppLog("[Cal3DViewer] FPS: %f frames/sec\n", fps);

			frameCount = 0;
			timeSinceLastUpdate -= updateInterval;
		}
	}
#endif

	return true;
}

void
Cal3DViewer::OnScreenOn (void)
{
}

void
Cal3DViewer::OnScreenOff (void)
{
}

void
Cal3DViewer::HandleTouchEvents(const Control &source)
{
    Touch touch;
    IList *pList = null;
    pList = touch.GetTouchInfoListN(source);
    if (pList)
    {
    	if (pList->GetCount() == 1)
    	{
    		if (!__multiTouch)
    		{
    			TouchInfo *pTouchInfo = (TouchInfo *)(pList->GetAt(0));

    		    __twistAngle += ((float)(pTouchInfo->position.x - __lastTouchPositionX))/2.0f;
    		    __tiltAngle += ((float)(pTouchInfo->position.y - __lastTouchPositionY))/3.0f;

    		    __lastTouchPositionX = pTouchInfo->position.x;
    		    __lastTouchPositionY = pTouchInfo->position.y;
    		}
    	} else {
    		if (!__multiTouch)
    		{
    			TouchInfo *pTouchInfoPoint0 = static_cast<TouchInfo *>(pList->GetAt(0));
    			TouchInfo *pTouchInfoPoint1 = static_cast<TouchInfo *>(pList->GetAt(1));

    			if (pTouchInfoPoint0->position.x < LEFT_MARGIN && pTouchInfoPoint1->position.x < LEFT_MARGIN)
    				__changeModel = true;

        	  	int deltaX = Math::Abs(pTouchInfoPoint0->position.x - pTouchInfoPoint1->position.x);
        	  	int deltaY = Math::Abs(pTouchInfoPoint0->position.y - pTouchInfoPoint1->position.y);
        	  	__currentDistance = Math::Sqrt(deltaX*deltaX + deltaY*deltaY);
        	  	__multiTouch = true;
    		}


			TouchInfo *pTouchInfoPoint0 = static_cast<TouchInfo *>(pList->GetAt(0));
			TouchInfo *pTouchInfoPoint1 = static_cast<TouchInfo *>(pList->GetAt(1));

			if (__changeModel && pTouchInfoPoint0->position.x > (WIDTH-RIGHT_MARGIN) && pTouchInfoPoint1->position.x > (WIDTH-RIGHT_MARGIN))
			{
				__currentModel++;
				if (__currentModel == 3)
					ChangeModel(0);
				else
					ChangeModel(__currentModel);

				__changeModel = false;
			}

    	  	int deltaX = Math::Abs(pTouchInfoPoint0->position.x - pTouchInfoPoint1->position.x);
    	  	int deltaY = Math::Abs(pTouchInfoPoint0->position.y - pTouchInfoPoint1->position.y);
    	  	int newDistance = Math::Sqrt(deltaX*deltaX + deltaY*deltaY);

    	    __distance -= (float)(newDistance - __currentDistance) / 3.0f;
    	    if(__distance < 0.0f)
    	    	__distance = 0.0f;

    	    __currentDistance = newDistance;
    	}
        pList->RemoveAll(true);
        delete pList;
    }
}

void
Cal3DViewer::OnTouchDoublePressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
}

void
Cal3DViewer::OnTouchFocusIn(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
}

void
Cal3DViewer::OnTouchFocusOut(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
}

void
Cal3DViewer::OnTouchLongPressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
}

void
Cal3DViewer::OnTouchMoved(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
    HandleTouchEvents(source);
}

void
Cal3DViewer::OnTouchPressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
    __lastTouchPositionX = currentPosition.x;
    __lastTouchPositionY = currentPosition.y;
    AppLog("OnTouchPressed is called. [%d]%d,%d", touchInfo.GetPointId(), currentPosition.x, currentPosition.y);
    HandleTouchEvents(source);
}

void
Cal3DViewer::OnTouchReleased(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo)
{
    AppLog("OnTouchReleased is called. [%d]%d,%d", touchInfo.GetPointId(), currentPosition.x, currentPosition.y);
    Touch touch;
    IList *pList = null;
    pList = touch.GetTouchInfoListN(source);
    if (pList)
    {
    	if (pList->GetCount() == 1)
    		__multiTouch = false;
    }
    __changeModel = false;
}


//----------------------------------------------------------------------------//
// Load and create a texture from a given file                                //
//----------------------------------------------------------------------------//

GLuint Cal3DViewer::LoadTexture(const std::string& strFilename)
{
  GLuint textureId=0;
  if(stricmp(strrchr(strFilename.c_str(),'.'),".raw")==0)
  {

     // open the texture file
     std::ifstream file;
     file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
     if(!file)
     {
       std::cerr << "Texture file '" << strFilename << "' not found." << std::endl;
       return 0;
     }

     // load the dimension of the texture
     int width;
     file.read((char *)&width, 4);
     int height;
     file.read((char *)&height, 4);
     int depth;
     file.read((char *)&depth, 4);

     // allocate a temporary buffer to load the texture to
     unsigned char *pBuffer;
     pBuffer = new unsigned char[2 * width * height * depth];
     if(pBuffer == 0)
     {
       std::cerr << "Memory allocation for texture '" << strFilename << "' failed." << std::endl;
       return 0;
     }

     // load the texture
     file.read((char *)pBuffer, width * height * depth);

     // explicitely close the file
     file.close();

     // flip texture around y-axis (-> opengl-style)
     int y;
     for(y = 0; y < height; y++)
     {
       memcpy(&pBuffer[(height + y) * width * depth], &pBuffer[(height - y - 1) * width * depth], width * depth);
     }

     // generate texture
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     glGenTextures(1, &textureId);
     glBindTexture(GL_TEXTURE_2D, textureId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	 glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, &pBuffer[width * height * depth]);
     // free the allocated memory

     delete [] pBuffer;
  }
  else if(stricmp(strrchr(strFilename.c_str(),'.'),".tga")==0)
  {
	  std::string filePath;
	  filePath.append(GetModelPath(GetModelName(__currentModel)));
	  filePath.append(strFilename.c_str());
	  CTga *Tga;
	  Tga = new CTga();
	  if(Tga->ReadFile(filePath.c_str())==0)
	  {
		  Tga->Release();
		  return 0;
	  }

	  if(Tga->Bpp()!=32)
	  {
		  Tga->Release();
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

	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA , GL_UNSIGNED_BYTE, (char*)Tga->GetPointer() );

	  Tga->Release();

  }


  return textureId;
}

//----------------------------------------------------------------------------//
// Create the viewer                                                          //
//----------------------------------------------------------------------------//

void Cal3DViewer::CreateCal3d(std::string modelName, float* scale, CalCoreModel* calCoreModel)
{

  // show some information
  //Might need MinGW installed for proper cout function working
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "|                    Cal3D MiniViewer for bada                   |" << std::endl;
  std::cout << "|     (C) 2001, 2002, 2003 Bruno 'Beosil' Heidelberger           |" << std::endl;
  std::cout << "|     Copyright (c) 2012 Samsung Electronics Co., Ltd.           |" << std::endl;
  std::cout << "|                  http://www.samsung.com                        |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "| This program is free software; you can redistribute it and/or  |" << std::endl;
  std::cout << "| modify it under the terms of the GNU General Public License as |" << std::endl;
  std::cout << "| published by the Free Software Foundation; either version 2 of |" << std::endl;
  std::cout << "| the License, or (at your option) any later version.            |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << std::endl;

  bool bModelConfiguration;
  bModelConfiguration = false;

    {
	  std::string filePath;
	  filePath.append(GetModelPath(modelName));
	  filePath.append(modelName);
	  filePath.append(".cfg");
      // parse the model configuration file
      if(!ParseModelConfiguration(filePath, calCoreModel, scale)) bModelConfiguration = false;

      // set model configuration flag
      bModelConfiguration = true;
    }

  // check if we have successfully loaded a model configuration
  if(!bModelConfiguration)
  {
    std::cerr << "No model configuration file given." << std::endl;
  }

  // make one material thread for each material
  // NOTE: this is not the right way to do it, but this viewer can't do the right
  // mapping without further information on the model etc., so this is the only
  // thing we can do here.
  int materialId;
  for(materialId = 0; materialId < calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // create the a material thread
	  calCoreModel->createCoreMaterialThread(materialId);

    // initialize the material thread
    calCoreModel->setCoreMaterialId(materialId, 0, materialId);
  }
}


//----------------------------------------------------------------------------//
// Handle an idle event                                                       //
//----------------------------------------------------------------------------//

void Cal3DViewer::UpdateModel()
{
	unsigned int tick;
	long long time;
	Osp::System::SystemTime::GetTicks(time);
	tick = (unsigned int)(time);

  // calculate the amount of elapsed seconds
  float elapsedSeconds = (float)(tick - __lastTick) / 1000.0f;

  // adjust fps counter
  __fpsDuration += elapsedSeconds;
  if(__fpsDuration >= 1.0f)
  {
    __fps = (int)((float)__fpsFrames / __fpsDuration);
    __fpsDuration = 0.0f;
    __fpsFrames = 0;
  }

  elapsedSeconds *= __timeScale;

	// check if the time has come to blend to the next animation
	if(__pCurrentCalCoreModel->getCoreAnimationCount() > 1)
	{
	  __leftAnimationTime -= elapsedSeconds;
	  if(__leftAnimationTime <= __blendTime)
	  {
		// get the next animation
		__currentAnimationId = (__currentAnimationId + 1) % __pCurrentCalCoreModel->getCoreAnimationCount();

		// fade in the new animation
		__pCurrentCalModel->getMixer()->executeAction(__currentAnimationId, __leftAnimationTime, __blendTime);

		// adjust the animation time left until next animation flip
		__leftAnimationTime = __pCurrentCalCoreModel->getCoreAnimation(__currentAnimationId)->getDuration() - __blendTime;
	  }
	}

    __pCurrentCalModel->update(elapsedSeconds);

  // current tick will be last tick next round
  __lastTick = tick;

}

//----------------------------------------------------------------------------//
// Render the current scene                                                   //
//----------------------------------------------------------------------------//

void Cal3DViewer::RenderCal3d()
{
  // clear the vertex and face counters
  __vertexCount = 0;
  __faceCount = 0;

  // clear all the buffers
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  SetPerspective(45.0f, (double)WIDTH / (double)HEIGHT, 0.01, 10000);

  // set the model transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // light attributes
  const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
  const GLfloat light_diffuse[]  = { 0.52f, 0.5f, 0.5f, 1.0f };
  const GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

  // setup the light attributes
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  // set the light position
  GLfloat lightPosition[] = { 0.0f, -1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  // set camera position
  glTranslatef(0.0f, 0.0f, -__distance * __scale);
  glRotatef(__tiltAngle, 1.0f, 0.0f, 0.0f);
  glRotatef(__twistAngle, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f, 0.0f, -90.0f * __scale);


  // render the model
  RenderModel();

  // increase frame counter
  __fpsFrames++;

}


//----------------------------------------------------------------------------//
// Initialize the demo                                                        //
//----------------------------------------------------------------------------//

void Cal3DViewer::InitCal3d(CalCoreModel* calCoreModel, CalModel* calModel)
{
  // load all textures and store the opengl texture id in the corresponding map in the material
  for(int materialId = 0; materialId < calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // get the core material
    CalCoreMaterial *pCoreMaterial = calCoreModel->getCoreMaterial(materialId);

    // loop through all maps of the core material
    for(int mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      // get the filename of the texture
      std::string strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      GLuint textureId = LoadTexture(strFilename);

      // store the opengl texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
    }
  }

  // attach all meshes to the model
  for(int meshId = 0; meshId < calCoreModel->getCoreMeshCount(); meshId++)
  {
	  calModel->attachMesh(meshId);
  }

  // set the material set of the whole model
  calModel->setMaterialSet(0);

  // set initial animation state
  if(calCoreModel->getCoreAnimationCount() > 0)
  {
    __currentAnimationId = 0;
    __leftAnimationTime = calCoreModel->getCoreAnimation(__currentAnimationId)->getDuration() - __blendTime;
    if(calCoreModel->getCoreAnimationCount() > 1)
    {
    	calModel->getMixer()->executeAction(__currentAnimationId, 0.0f, __blendTime);
    }
    else
    {
    	calModel->getMixer()->blendCycle(__currentAnimationId, 1.0f, 0.0f);
    }
  }
  else
  {
    __currentAnimationId = -1;
    __leftAnimationTime = -1.0f;
  }

  // we're done
  std::cout << "Initialization done." << std::endl;
  std::cout << std::endl;
}


bool Cal3DViewer::ParseModelConfiguration(const std::string& strFilename, CalCoreModel* calCoreModel, float* scale)
{
  // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
    return false;
  }

  // parse all lines from the model configuration file
  int line;
  for(line = 1; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof()) break;

    // check if an error happend while reading from the file
    if(!file)
    {
      std::cerr << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') continue;

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

    // handle the model creation
    if(strKey == "scale")
    {
      // set rendering scale factor
    	*scale = atof(strData.c_str());
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!calCoreModel->loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(calCoreModel->loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(calCoreModel->loadCoreMesh(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(calCoreModel->loadCoreMaterial(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      // everything else triggers an error message, but is ignored
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
    }
  }

  // explicitely close the file
  file.close();

  return true;
}

//----------------------------------------------------------------------------//
// Render the model                                                           //
//----------------------------------------------------------------------------//

void Cal3DViewer::RenderModel()
{

  // get the renderer of the model
  CalRenderer *pCalRenderer = __pCurrentCalModel->getRenderer();

  // begin the rendering loop
  if(pCalRenderer->beginRendering())
  {
    // set global OpenGL states
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // we will use vertex arrays, so enable them
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // get the number of meshes
    int meshCount = pCalRenderer->getMeshCount();

    // render all meshes of the model
    for(int meshId = 0; meshId < meshCount; meshId++)
    {
      // get the number of submeshes
      int submeshCount = pCalRenderer->getSubmeshCount(meshId);

      // render all submeshes of the mesh
      for(int submeshId = 0; submeshId < submeshCount; submeshId++)
      {
        // select mesh and submesh for further data access
        if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
        {
          unsigned char meshColor[4];
          GLfloat materialColor[4];

          // set the material ambient color
          pCalRenderer->getAmbientColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

          // set the material diffuse color
          pCalRenderer->getDiffuseColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

          // set the material specular color
          pCalRenderer->getSpecularColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

          // set the material shininess factor
          float shininess;
          shininess = 50.0f; //pCalRenderer->getShininess();
          glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

          // get the transformed vertices of the submesh
          static float meshVertices[30000][3];
          int vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

          // get the transformed normals of the submesh
          static float meshNormals[30000][3];
          pCalRenderer->getNormals(&meshNormals[0][0]);

          // get the texture coordinates of the submesh
          static float meshTextureCoordinates[30000][2];
          int textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

          // get the faces of the submesh
          static CalIndex meshFaces[50000][3];
          int faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

          // set the vertex and normal buffers
          glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
          glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

          // set the texture coordinate buffer and state if necessary
          if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_COLOR_MATERIAL);

            // set the texture id we stored in the map user data
            glBindTexture(GL_TEXTURE_2D, (GLuint)pCalRenderer->getMapUserData(0));

            // set the texture coordinate buffer
            glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
          }

          // draw the submesh
          if(sizeof(CalIndex)==2)
            glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
          else
            glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);

          // disable the texture coordinate state if necessary
          if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          {
            glDisable(GL_COLOR_MATERIAL);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
          }

          // adjust the vertex and face counter
          __vertexCount += vertexCount;
          __faceCount += faceCount;

        }
      }
    }

    // clear vertex array state
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // clear light
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);

    // end the rendering
    pCalRenderer->endRendering();
  }
}

void
Cal3DViewer::SetPerspective(double fovy, double aspect, double zNear, double zFar)
{
   double xmin, xmax, ymin, ymax;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;


   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

std::string
Cal3DViewer::GetModelName(int modelIndex)
{
	if (modelIndex == MODEL_SKELETON) return std::string("skeleton");
	else if (modelIndex == MODEL_CALLY) return std::string("cally");
	else return std::string("paladin");
}

std::string
Cal3DViewer::GetModelPath(std::string modelName)
{
	std::string filePath;
	filePath.append(FILES_PATH);
	filePath.append(modelName);
	filePath.append("/");
	std::cout << filePath << std::endl;
	return filePath;
}

void
Cal3DViewer::ChangeModel(int modelIndex)
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



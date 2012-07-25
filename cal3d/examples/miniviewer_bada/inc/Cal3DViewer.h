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

#ifndef _CAL3DVIEWER_H_
#define _CAL3DVIEWER_H_


#include <FBase.h>
#include <FGraphics.h>
#include <FApp.h>
#include <FGraphicsOpengl.h>
#include <FSystem.h>
#include <FUi.h>

#ifdef INVALID_HANDLE
#undef INVALID_HANDLE
#endif

#include "cal3d.h"


class Cal3DViewer : 
	public Osp::App::Application,
	public Osp::System::IScreenEventListener,
	public Osp::Base::Runtime::ITimerEventListener,
	public Osp::Ui::ITouchEventListener

{
public:
	static Osp::App::Application* CreateInstance(void);

	static const int WIDTH = 480;
	static const int HEIGHT = 800;
	static const int LEFT_MARGIN = 260;
	static const int RIGHT_MARGIN = 90;
	static const int MODEL_SKELETON = 0;
	static const int MODEL_CALLY = 1;
	static const int MODEL_PALADIN = 2;

	static const std::string FILES_PATH;


public:
	Cal3DViewer();
	~Cal3DViewer();


public:
	bool OnAppInitializing(Osp::App::AppRegistry& appRegistry);
	bool OnAppTerminating(Osp::App::AppRegistry& appRegistry, bool forcedTermination = false);
	void OnForeground(void);
	void OnBackground(void);
	void OnLowMemory(void);
	void OnBatteryLevelChanged(Osp::System::BatteryLevel batteryLevel);
	void OnScreenOn (void);
	void OnScreenOff (void);

	virtual result OnInitializing(void);
    virtual void OnTouchDoublePressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchFocusIn(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchFocusOut(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchLongPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchMoved(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchPressed(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);
    virtual void OnTouchReleased(const Osp::Ui::Control &source, const Osp::Graphics::Point &currentPosition, const Osp::Ui::TouchEventInfo &touchInfo);

    void HandleTouchEvents(const Osp::Ui::Control &source);


public:
	void OnTimerExpired(Osp::Base::Runtime::Timer& timer);
	bool Draw();

private:
	void InitMultiTouch();

	bool InitEGL();
	bool InitGL();
	void Cleanup();
	void DestroyGL();

	void CreateCal3d(std::string modelName, float* scale, CalCoreModel* calCoreModel);
	void InitCal3d(CalCoreModel* calCoreModel, CalModel* calModel);
	Osp::Graphics::Opengl::GLuint LoadTexture(const std::string& strFilename);
	void RenderCal3d(void);
	bool ParseModelConfiguration(const std::string& strFilename, CalCoreModel*, float* scale);
	void RenderModel();
	void UpdateModel();
	void SetPerspective(double fovy, double aspect, double zNear, double zFar);

	std::string GetModelName(int modelIndex);
	std::string GetModelPath(std::string modelName);
	void ChangeModel(int modelIndex);

private:

	Osp::Graphics::Opengl::EGLDisplay __eglDisplay;
	Osp::Graphics::Opengl::EGLSurface __eglSurface;
	Osp::Graphics::Opengl::EGLConfig  __eglConfig;
	Osp::Graphics::Opengl::EGLContext __eglContext;
	
	Osp::Graphics::Opengl::EGLSurface __pbuffer_surface;
	Osp::Graphics::Opengl::EGLSurface __pixmap_surface;

	Osp::Base::Runtime::Timer*        __pTimer;

	Osp::Ui::Controls::Form*          __pForm;

	CalCoreModel* __pCurrentCalCoreModel;
	CalCoreModel* __pSkeletonCalCoreModel;
	CalCoreModel* __pCallyCalCoreModel;
	CalCoreModel* __pPaladinCalCoreModel;


	int 			__lastTouchPositionX;
	int 			__lastTouchPositionY;
	float 			__tiltAngle;
	float 			__twistAngle;
	float 			__distance;
	unsigned int 	__lastTick;

	CalModel* __pCurrentCalModel;
	CalModel* __pSkeletonCalModel;
	CalModel* __pCallyCalModel;
	CalModel* __pPaladinCalModel;

	int 	__currentModel;
	float 	__scale;
	float 	__scaleSkeleton;
	float 	__scaleCally;
	float 	__scalePaladin;
	int 	__currentAnimationId;
	float 	__leftAnimationTime;
	float 	__blendTime;
	float 	__lodLevel;
	int 	__vertexCount;
	int 	__faceCount;

	float 	__fpsDuration;
	int 	__fpsFrames;
	int 	__fps;
	float 	__timeScale;

	bool 	__multiTouch;
	int 	__currentDistance;
	bool 	__changeModel;
};

#endif

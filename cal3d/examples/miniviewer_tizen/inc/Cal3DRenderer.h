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

#ifndef _GLRENDERERTEMPLATE_H_
#define _GLRENDERERTEMPLATE_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>
#include <FGrpIGlRenderer.h>

#include "Cal3DLoader.h"

class Cal3DRenderer :
	public Tizen::Graphics::Opengl::IGlRenderer
{
public:
	Cal3DRenderer(Cal3DLoader* cal3dLoader);
	~Cal3DRenderer(void);

	virtual bool InitializeGl(void);
	virtual bool TerminateGl(void);

	virtual bool Draw(void);

	virtual bool Pause(void);
	virtual bool Resume(void);

	virtual int GetTargetControlWidth(void);
	virtual int GetTargetControlHeight(void);
	virtual void SetTargetControlWidth(int width);
	virtual void SetTargetControlHeight(int height);

	void IncreaseTiltAngle(float addTiltAngle);
	void IncreaseTwistAngle(float addTwistAngle);
	void IncreaseDistance(float addDistance);
	void ChangeModel();

private:
	float UpdateTick(void);
	void RenderCal3d(void);
	void RenderModel(void);

private:
	int __controlWidth;
	int __controlHeight;
	int __angle;

	Tizen::Graphics::Opengl::EGLDisplay __eglDisplay;
	Tizen::Graphics::Opengl::EGLSurface __eglSurface;
	Tizen::Graphics::Opengl::EGLConfig  __eglConfig;
	Tizen::Graphics::Opengl::EGLContext __eglContext;

	Tizen::Graphics::Opengl::EGLSurface __pbuffer_surface;
	Tizen::Graphics::Opengl::EGLSurface __pixmap_surface;

	float 			__tiltAngle;
	float 			__twistAngle;
	float 			__distance;
	unsigned int 	__lastTick;
	float 	__fpsDuration;
	int 	__fpsFrames;
	int 	__fps;
	float 	__timeScale;

	int 	__vertexCount;
	int 	__faceCount;

	Cal3DLoader* __cal3dLoader;
};

#endif /* _GLRENDERERTEMPLATE_H_ */

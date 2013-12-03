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

#ifndef _CAL3DVIEWER_H_
#define _CAL3DVIEWER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>
#include <FGrpGlPlayer.h>

#include "Cal3DRenderer.h"
#include "Cal3DLoader.h"

/**
 * [Cal3DViewerApp] UiApp must inherit from UiApp class
 * which provides basic features necessary to define an UiApp.
 */
class Cal3DViewerApp
	: public Tizen::App::UiApp
	, public Tizen::System::IScreenEventListener
{
public:
	/**
	 * [Test] UiApp must have a factory method that creates an instance of itself.
	 */
	static Tizen::App::UiApp* CreateInstance(void);

public:
	Cal3DViewerApp(void);
	virtual~Cal3DViewerApp(void);

public:
	// Called when the UiApp is initializing.
	virtual bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);

	// Called when the UiApp initializing is finished. 
	virtual bool OnAppInitialized(void); 

	// Called when the UiApp is requested to terminate. 
	virtual bool OnAppWillTerminate(void); 

	// Called when the UiApp is terminating.
	virtual bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false);

	// Called when the UiApp's frame moves to the top of the screen.
	virtual void OnForeground(void);

	// Called when this UiApp's frame is moved from top of the screen to the background.
	virtual void OnBackground(void);

	// Called when the system memory is not sufficient to run the UiApp any further.
	virtual void OnLowMemory(void);

	// Called when the battery level changes.
	virtual void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	virtual void OnScreenOn(void);

	// Called when the screen turns off.
	virtual void OnScreenOff(void);

private:
	Tizen::Graphics::Opengl::GlPlayer* __player;
	Cal3DRenderer* __renderer;
	Cal3DLoader* __cal3dLoader;
};

#endif // _CAL3DVIEWER_H_

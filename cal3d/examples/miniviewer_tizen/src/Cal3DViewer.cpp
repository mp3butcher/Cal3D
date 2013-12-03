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

#include "Cal3DViewer.h"
#include "Cal3DViewerFrame.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

Cal3DViewerApp::Cal3DViewerApp(void)
{
}

Cal3DViewerApp::~Cal3DViewerApp(void)
{
}

UiApp*
Cal3DViewerApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new Cal3DViewerApp();
}

bool
Cal3DViewerApp::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize Frame and App specific data.
	// The App's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// TODO:
	// Add your initialization code here
	return true;
}

bool
Cal3DViewerApp::OnAppInitialized(void)
{
	// Create a Frame
	Cal3DViewerFrame* pCal3DViewerFrame = new Cal3DViewerFrame();
	pCal3DViewerFrame->Construct();
	pCal3DViewerFrame->SetName(L"Cal3DViewer");
	AddFrame(*pCal3DViewerFrame);

	{
		__player = new Tizen::Graphics::Opengl::GlPlayer;
		__player->Construct(Tizen::Graphics::Opengl::EGL_CONTEXT_CLIENT_VERSION_1_X, pCal3DViewerFrame->GetCurrentForm());

		__player->SetFps(60);
		__player->SetEglAttributePreset(Tizen::Graphics::Opengl::EGL_ATTRIBUTES_PRESET_RGB565);

		__player->Start();
	}

	__cal3dLoader = new Cal3DLoader();
	__renderer = new Cal3DRenderer(__cal3dLoader);
	pCal3DViewerFrame->AddCalRenderer(__renderer);
	__player->SetIGlRenderer(__renderer);

	return true;
}

bool
Cal3DViewerApp::OnAppWillTerminate(void)
{
	// TODO: Add code to do something before application termination.
	return true;
}


bool
Cal3DViewerApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.

	__player->Stop();

	if(__renderer != null)
	{
		delete __renderer;
	}
	if(__cal3dLoader != null)
	{
		delete __cal3dLoader;
	}
	delete __player;

	return true;
}

void
Cal3DViewerApp::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
}

void
Cal3DViewerApp::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void
Cal3DViewerApp::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void
Cal3DViewerApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void
Cal3DViewerApp::OnScreenOn(void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void
Cal3DViewerApp::OnScreenOff(void)
{
	// TODO:
	// Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device
	// to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a
	// callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}

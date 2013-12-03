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
//
// This file contains the Tizen application entry point.
//
#include "Cal3DViewer.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//
// The entry function of Tizen application called by the operating system.
//
_EXPORT_ int
OspMain(int argc, char *pArgv[])
{
           AppLog("Application started.");
           ArrayList args(SingleObjectDeleter);
           args.Construct();
           for (int i = 0; i < argc; i++)
           {
                     args.Add(new (std::nothrow) String(pArgv[i]));
           }

           result r = Tizen::App::UiApp::Execute(Cal3DViewerApp::CreateInstance, &args);
           TryLog(r == E_SUCCESS, "[%s] Application execution failed", GetErrorMessage(r));
           AppLog("Application finished.");

           return static_cast<int>(r);
}
#ifdef __cplusplus
}
#endif // __cplusplus

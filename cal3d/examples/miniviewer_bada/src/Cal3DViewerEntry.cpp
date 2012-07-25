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

/**
 * This file contains the bada application entry point.
 */
#include "Cal3DViewer.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


_EXPORT_ int OspMain(int argc, char *pArgv[]);
#ifdef _PROFILE
extern void start_profile (void);
extern void end_profile (void);
#else
#define start_profile()
#define end_profile()
#endif


/**
 * The entry function of bada application called by the operating system.
 */
int
OspMain(int argc, char *pArgv[])
{
	result r = E_SUCCESS;

	AppLog("Application started.");
	ArrayList* pArgs = new ArrayList();
	pArgs->Construct();
	for (int i = 0; i < argc; i++)
		pArgs->Add(*(new String(pArgv[i])));

	start_profile();
	r = Osp::App::Application::Execute(Cal3DViewer::CreateInstance, pArgs);
	if (IsFailed(r))
	{
		AppLogException("Application execution failed-[%s].", GetErrorMessage(r));
		r &= 0x0000FFFF;
	}
	end_profile();

	pArgs->RemoveAll(true);
	delete pArgs;
	AppLog("Application finished.");

	return static_cast<int>(r);
}
#ifdef __cplusplus
}
#endif // __cplusplus

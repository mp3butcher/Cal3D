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

#ifndef _CAL3DVIEWERFRAME_H_
#define _CAL3DVIEWERFRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

#include "Cal3DViewerForm.h"

class Cal3DViewerFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	Cal3DViewerFrame(void);
	virtual ~Cal3DViewerFrame(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	void AddCalRenderer(Cal3DRenderer* cal3dRenderer);

private:
	Cal3DViewerForm* __pCal3DViewerForm;
};

#endif  //_CAL3DVIEWERFRAME_H_

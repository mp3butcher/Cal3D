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

#include "Cal3DViewerFrame.h"
#include "Cal3DViewerForm.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

Cal3DViewerFrame::Cal3DViewerFrame(void)
{
}

Cal3DViewerFrame::~Cal3DViewerFrame(void)
{
}

result
Cal3DViewerFrame::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Create a form
	__pCal3DViewerForm = new Cal3DViewerForm();
	__pCal3DViewerForm->Initialize();

	// Add the form to the frame
	AddControl(__pCal3DViewerForm);

	// Set the current form
	SetCurrentForm(__pCal3DViewerForm);

	// Draw the form
	__pCal3DViewerForm->Invalidate(true);

	return r;
}

result
Cal3DViewerFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void
Cal3DViewerFrame::AddCalRenderer(Cal3DRenderer* cal3dRenderer)
{
	__pCal3DViewerForm->AddCalRenderer(cal3dRenderer);
}

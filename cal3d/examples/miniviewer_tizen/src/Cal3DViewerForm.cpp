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

#include <math.h>

#include "Cal3DViewerForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Utility;

Cal3DViewerForm::Cal3DViewerForm(void)
	: __lastTouchPositionX(0)
	, __lastTouchPositionY(0)
	, __multiTouch(false)
	, __changeModel(false)
	, __currentDistance(0)
{
}

Cal3DViewerForm::~Cal3DViewerForm(void)
{
}

bool
Cal3DViewerForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL);
	InitMultiTouch();

	return true;
}
result
Cal3DViewerForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	SetFormBackEventListener(this);

	return r;
}

result
Cal3DViewerForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void
Cal3DViewerForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		break;
	}
}

void
Cal3DViewerForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

void
Cal3DViewerForm::InitMultiTouch()
{
	SetFormBackEventListener(this);
	AddTouchEventListener(*this);
	SetMultipointTouchEnabled(true);
}

// ITouchEventListener
void
Cal3DViewerForm::OnTouchCanceled (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
}
void
Cal3DViewerForm::OnTouchFocusIn (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
}
void
Cal3DViewerForm::OnTouchFocusOut (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
}
void
Cal3DViewerForm::OnTouchMoved (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
	HandleTouchEvents(source);
}
void
Cal3DViewerForm::OnTouchPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
    __lastTouchPositionX = currentPosition.x;
    __lastTouchPositionY = currentPosition.y;
    HandleTouchEvents(source);
}
void
Cal3DViewerForm::OnTouchReleased (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
{
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

void
Cal3DViewerForm::AddCalRenderer(Cal3DRenderer* cal3dRenderer)
{
	__pCal3dRenderer = cal3dRenderer;
}

void
Cal3DViewerForm::HandleTouchEvents(const Control &source)
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

    			__pCal3dRenderer->IncreaseTwistAngle(((float)(pTouchInfo->position.x - __lastTouchPositionX))/2.0f);
    			__pCal3dRenderer->IncreaseTiltAngle(((float)(pTouchInfo->position.y - __lastTouchPositionY))/3.0f);

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

			if (__changeModel && pTouchInfoPoint0->position.x > (__pCal3dRenderer->GetTargetControlWidth()-RIGHT_MARGIN) && pTouchInfoPoint1->position.x > (__pCal3dRenderer->GetTargetControlWidth()-RIGHT_MARGIN))
			{
				__pCal3dRenderer->ChangeModel();
				__changeModel = false;
			}

    	  	int deltaX = Math::Abs(pTouchInfoPoint0->position.x - pTouchInfoPoint1->position.x);
    	  	int deltaY = Math::Abs(pTouchInfoPoint0->position.y - pTouchInfoPoint1->position.y);
    	  	int newDistance = Math::Sqrt(deltaX*deltaX + deltaY*deltaY);

    	  	__pCal3dRenderer->IncreaseDistance((float)(newDistance - __currentDistance) / 3.0f);

    	    __currentDistance = newDistance;
    	}

        pList->RemoveAll(true);
        delete pList;
    }
}

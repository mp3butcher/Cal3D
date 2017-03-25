//****************************************************************************//
// animation.cpp                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cal3d/animation.h"
#include "cal3d/error.h"
#include "cal3d/coreanimation.h"
#include "cal3d/animcallback.h"
#include "cal3d/model.h"

using namespace cal3d;
CalAnimation::CalAnimation(CalCoreAnimation *pCoreAnimation)
  : m_pCoreAnimation(pCoreAnimation)
  , m_type(TYPE_NONE)
  , m_state(STATE_NONE)
  , m_time(0.0f)
  , m_timeFactor(1.0f)
  , m_weight(0.0f)
{
  assert(pCoreAnimation);

  std::vector<CalCoreAnimation::CallbackRecord>& list = m_pCoreAnimation->getCallbackList();
  for (size_t i=0; i<list.size(); i++)
    m_lastCallbackTimes.push_back(0.0F);  // build up the last called list
}



void CalAnimation::checkCallbacks(float animationTime, CalModel *model)
{
  std::vector<CalCoreAnimation::CallbackRecord>& list = m_pCoreAnimation->getCallbackList();

  for (size_t i=0; i<list.size(); i++)
  {
	if (m_lastCallbackTimes.size() <= i)                // need these two lines to allow dynamic adding of callbacks. 
		m_lastCallbackTimes.push_back(animationTime);

    list[i].callback->AnimationUpdate(animationTime, model, model->getUserData());
    if (animationTime > 0 && animationTime < m_lastCallbackTimes[i])  // looped
        m_lastCallbackTimes[i] -= m_pCoreAnimation->getDuration();
    else if (animationTime < 0 && animationTime > m_lastCallbackTimes[i])     // reverse-looped  
        m_lastCallbackTimes[i] += m_pCoreAnimation->getDuration();
  
    if ((animationTime >= 0 && animationTime >= m_lastCallbackTimes[i] + list[i].min_interval) ||
        (animationTime <  0 && animationTime <= m_lastCallbackTimes[i] - list[i].min_interval))
    {
      list[i].callback->AnimationUpdate(animationTime,model, model->getUserData());
      m_lastCallbackTimes[i] = animationTime;
    }
  }
}

void CalAnimation::completeCallbacks(CalModel *model)
{
  std::vector<CalCoreAnimation::CallbackRecord>& list = m_pCoreAnimation->getCallbackList();
  for (size_t i=0; i<list.size(); i++)
    list[i].callback->AnimationComplete(model, model->getUserData());
}

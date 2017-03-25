//****************************************************************************//
// animation_action.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_ANIMATION_ACTION_H
#define CAL_ANIMATION_ACTION_H


#include "cal3d/global.h"
#include "cal3d/animation.h"

namespace cal3d{
	class CalCoreAnimation;


	class CAL3D_API CalAnimationAction : public CalAnimation
	{
	public:
		CalAnimationAction(CalCoreAnimation *pCoreAnimation);
		virtual ~CalAnimationAction() { }

		/** set scale of this animation in the mixer (!scale aren't normalized among mixed animations)*/
		bool setScale(float);
		/** get scale of this animation in the mixer (!scale aren't normalized among mixed animations)*/
		float getScale();
		/*** set the rampValue, from 0-1, scaling the blend weight.  If the blending function
		* is Replace, the rampValue also scales the blend weight of non-Replace and
		* lower priority animations by 1 - rampValue.  Default should be 1.0.**/
		bool setRampValue(float);
		/** get rampValue scaling the weight **/
		float getRampValue();
		/** Sets the composition function, which controls how animation blends with other simultaneous animations.
		*
		* If you set it to Replace, then when the animation is fully ramped on, all non-Replace
		* and lower priority Replace animations will have zero influence.  This
		* factor does not apply to cycling animations.  The priority of animations is,
		* firstly whether they are Replace or not, and secondly how recently the animations were
		* added, the most recently added animations having higher priority.**/
		bool setCompositionFunction(CompositionFunction);
		/** get the CompositionFunction used in mixer for this animation **/
		CompositionFunction getCompositionFunction();

		/** Configures the action to be a manual action, and on.**/
		bool setManual();
		bool isManual();

		/** Sets the manual animation on or off.  **/
		bool setManualAnimationActionOn(bool p);
		/** set the weight of the manual animation.
		Manual animations do not blend toward a weight target, so you set the weight directly, not a weight target.**/
		bool setManualAnimationActionWeight(float);


		bool execute(float delayIn, float delayOut, float weightTarget = 1.0f, bool autoLock = false);
		bool update(float deltaTime);




	protected:
		friend class CalMixer;
		/**Tells mixer whether the animation action is on, i.e., should it apply to bones.**/
		bool isOn();


		float m_delayIn;
		float m_delayOut;
		float m_delayTarget;
		float m_weightTarget;
		bool  m_autoLock;
		float m_scale;
		float m_rampValue;
		CompositionFunction m_compositionFunction;
		enum SequencingMode {
			SequencingModeNull = 0,
			SequencingModeAutomatic,
			SequencingModeManual
		} m_sequencingMode;
		bool m_manualOn;

	};
}
#endif

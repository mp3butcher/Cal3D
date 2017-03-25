//****************************************************************************//
// animation.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_ANIMATION_H
#define CAL_ANIMATION_H


#include "cal3d/global.h"

namespace cal3d{
	class CalCoreAnimation;
	class CalModel;

	class CAL3D_API CalAnimation
	{
	public:
		enum Type
		{
			TYPE_NONE = 0,
			TYPE_CYCLE,
			TYPE_POSE,
			TYPE_ACTION
		};

		enum State
		{
			STATE_NONE = 0,
			STATE_SYNC,
			STATE_ASYNC,
			STATE_IN,
			STATE_STEADY,
			STATE_OUT,
			STATE_STOPPED
		};
		enum CompositionFunction {
			CompositionFunctionNull = 0,
			CompositionFunctionReplace,
			CompositionFunctionAverage,
			CompositionFunctionCrossFade
		};

	public:
		CalAnimation(CalCoreAnimation *pCoreAnimation);
		virtual ~CalAnimation() {  }

		/** returns the core animation on which this animation instance **/
		inline CalCoreAnimation *getCoreAnimation(){ return m_pCoreAnimation; }
		/** returns the core animation on which this animation instance **/
		const CalCoreAnimation *getCoreAnimation() const{ return m_pCoreAnimation; }

		/** get the time of the animation **/
		inline float getTime() const{ return m_time; }
		/** set the time of the animation **/
		inline void setTime(float time){ m_time = time; }

		/** set the timefactor of the animation **/
		inline void setTimeFactor(float timeFactor){ m_timeFactor = timeFactor; }
		/** get the timefactor of the animation **/
		inline float getTimeFactor() const{ return m_timeFactor; }

		/**set the state of the animation **/
		void setState(State state) { m_state = state; }
		/** returns the state of the animation **/
		State getState() const{ return m_state; }

		/**set the type of the animation **/
		void setType(Type type) { m_type = type; }
		/** returns the type of the animation **/
		inline Type getType() const{ return m_type; }

		/**set the weight of the animation**/
		void setWeight(float weight) { m_weight = weight; }
		/** returns the weight of the animation **/
		inline float getWeight() const{ return m_weight; }

		void checkCallbacks(float animationTime, CalModel *model);
		void completeCallbacks(CalModel *model);

	protected:


		CalCoreAnimation *m_pCoreAnimation;
		std::vector<float> m_lastCallbackTimes;
		Type m_type;
		State m_state;
		float m_time;
		float m_timeFactor;
		float m_weight;
	};
}
#endif

//****************************************************************************//

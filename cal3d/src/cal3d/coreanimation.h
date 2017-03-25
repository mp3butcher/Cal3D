//****************************************************************************//
// coreanimation.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREANIMATION_H
#define CAL_COREANIMATION_H

#include "cal3d/global.h"
#include "cal3d/quaternion.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"

namespace cal3d{
	struct CalAnimationCallback;
	class CalCoreTrack;
	class CalCoreSkeleton;

	class CAL3D_API CalCoreAnimation : public cal3d::RefCounted
	{
	protected:
		~CalCoreAnimation();

	public:
		CalCoreAnimation();

		size_t size();

		/** Set the duration of the animation **/
		inline float getDuration() const              { return m_duration; }
		/** Set the duration of the animation **/
		inline void setDuration(float duration)       { m_duration = duration; }

		/** Set the symbolic name of the core animation. **/
		inline void setName(const std::string& name)  { m_name = name; }
		/** Get the symbolic name of the core animation. **/
		inline const std::string& getName(void) const { return m_name; }

		/** return the count of tracks **/
		inline unsigned int getTrackCount() const     { return m_listCoreTrack.size(); }
		/** add a track**/
		bool addCoreTrack(CalCoreTrack *pCoreTrack);
		/** remove a track**/
		bool removeCoreTrack(CalCoreTrack *pCoreTrack);
		/** get a track by its index NULL if index invalid **/
		CalCoreTrack *getCoreTrack(int coreBoneId);
		/** return the list of tracks **/
		inline const std::list<CalCoreTrack *>& getListCoreTrack() const { return m_listCoreTrack; }

		/** return keyframe count of all tracks **/
		unsigned int getTotalKeyframesCount() const;

		/** rescale all the animation data that are in the core animation
		 * @param factor A float with the scale factor   **/
		void scale(float factor);

		struct CallbackRecord
		{
			CalAnimationCallback *callback;
			float                 min_interval;
		};

		/** Add a callback to the current list of callbacks for this core animation.
		 * @param  callback     Ptr to a subclass of this abstract class implementing the callback function.
		 * @param  min_interval Minimum interval (in seconds) between callbacks.  Specifying 0 means call every update().**/
		void registerCallback(CalAnimationCallback *callback, float min_interval);
		/** remove a callback from the current list of callbacks for this core animation.**/
		void removeCallback(CalAnimationCallback *callback);
		/** return the list of callback for tihs core animation.**/
		std::vector<CallbackRecord>& getCallbackList(){ return m_listCallbacks; }

	private:

		std::vector<CallbackRecord> m_listCallbacks;

		float m_duration;
		std::list<CalCoreTrack *> m_listCoreTrack;
		std::string m_name;
		std::string m_filename;
	};

	typedef cal3d::RefPtr<CalCoreAnimation> CalCoreAnimationPtr;
}
#endif

//****************************************************************************//

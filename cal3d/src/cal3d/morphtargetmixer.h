//****************************************************************************//
// morphtargetmixer.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// 2008 Erik Johnson                                                          //
// 2012,2017 Julien 'mp3butcher' Valentin                                     //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MORPHTARGETMIXER_H
#define CAL_MORPHTARGETMIXER_H


#include "cal3d/global.h"
#include <cal3d/coremorphkeyframe.h>
#include <cal3d/coreanimatedmorph.h>

namespace cal3d{
	class CalModel;

	class CAL3D_API CalMorphTargetMixer
	{
	public:
		CalMorphTargetMixer(CalModel *model);
		~CalMorphTargetMixer() { }
		bool copy(const CalMorphTargetMixer& inOther);

		/** Interpolate the weight of a morph target a new value
	* in a given amount of time.
	* @param id The ID of the morph target that should be blended.
	* @param weight The weight to interpolate the morph target to.
	* @param delayIn The time in seconds until the new weight should be reached.
	* @param delayOut The time in seconds until the weight goes back to zero at the end of the animation.
	* @param looping True if the morph animation should continuously loop.
	* @return \b true if successful   \b false if an error happened**/
		bool blend(int id, float weight, float delayIn, float delayOut, bool looping);

		/** Set a morph target animation at a specific time interval.
	* @param id The ID of the morph target that should be blended.
	* @param weight The weight to interpolate the morph target to.
	* @param time The current time value to set the animation at.
	* @return  true if successful false if an error happened **/
		bool manualBlend(int id, float weight, float time);

		/** Fade a morph target out in a given amount of time.
	* @param id The ID of the morph target that should be faded out.
	* @param delay The time in seconds until the the morph target is completely removed.
	* @return true if successful false if an error happened**/
		bool clear(int id, float delay);

		/** Get the weight of a morph animation.
	* @param id The id of the morph target which weight you want**/
		float getCurrentWeight(int id) const;

		/** Get the name of  morph animation .
		* @param id The id of the morph animation.**/
		const std::string& getMorphName(int id) const;

		/** Get the number of tracks for a morph animation.
		* @param id The id of the morph animation.**/
		int getTrackCount(int id) const;

		/** Get the tracks foraanimated morph animation.
	* @param id The id of the animated morph animation.**/
		const std::list<CalCoreMorphTrack>& getMorphTracks(int id) const;

		/** Get the number of keyframes for a animated morph animation.
			* @param id The id of the animated morph animation.**/
		int getKeyframeCount(int id) const;

		/** Get the morph anim duration for a animated morph .
		* @param id The id of the animated morph animation.**/
		float getDuration(int id) const;

		/** Updates all morph targets of the mixer instance for a given amount of time.**/
		void update(float deltaTime);

	protected:

		virtual float CalcKeyframeWeight(const std::vector<CalCoreMorphKeyframe> &keyframes, float elapsedTime);
		struct MorphAnimData
		{
			bool  isManual;
			int   animatedMorphID;

			float weight;
			bool  looping;

			float playTime;
			float currentWeight;
			float fadeIn;
			float fadeInTime;
			float fadeOut;
			float fadeOutTime;
		};

		std::vector<MorphAnimData> mAnimList;

		CalModel          *m_pModel;

		void SetTrackWeights(const CalCoreAnimatedMorph& morph, MorphAnimData& data);

	};
}
#endif

//****************************************************************************//

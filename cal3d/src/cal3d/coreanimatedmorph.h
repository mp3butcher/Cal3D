//****************************************************************************//
// coreanimatedMorph.h                                                        //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (C) 2001, 2002 Julien 'MpEbUtCh3r' Valentin                      //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREANIMATEDMORPH_H
#define CAL_COREANIMATEDMORPH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/coremorphtrack.h"
namespace cal3d{

	class CalCoreMorphTrack;

	/*****************************************************************************/
	/** The core animatedMorph class.
	  *****************************************************************************/


	class CAL3D_API CalCoreAnimatedMorph
	{
		// member variables
	protected:
		std::string m_name;
		float m_duration;
		std::list<CalCoreMorphTrack> m_listCoreTrack;

		// constructors/destructor
	public:
		CalCoreAnimatedMorph();
		virtual ~CalCoreAnimatedMorph();

		// member functions
	public:
		/** get Name of the animated morph **/
		inline const std::string& getName() const                             { return m_name; }
		/** set Name of the animated morph**/
		inline void setName(const std::string& name)                          { m_name = name; }
		/** set the duration of the animated morph **/
		inline float getDuration() const                                      { return m_duration; }
		/** set the duration of the animated morph **/
		inline void setDuration(float duration)                               { m_duration = duration; }
		/** add a track to the animated morph **/
		bool addCoreTrack(CalCoreMorphTrack *pCoreTrack);
		/** get a track of the animated morph by its index **/
		CalCoreMorphTrack *getCoreTrack(const unsigned int &trackId);
		/** get all tracks of the animated morph **/
		inline std::list<CalCoreMorphTrack>& getListCoreTrack()               { return m_listCoreTrack; }
		/** get all tracks of the animated morph **/
		inline const std::list<CalCoreMorphTrack>& getListCoreTrack() const   { return m_listCoreTrack; }


		void scale(float factor);
		/**remove tracks with zero scale**/
		void removeZeroScaleTracks();
	};
}
#endif

//****************************************************************************//

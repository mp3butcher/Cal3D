//****************************************************************************//
// corekeyframe.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREKEYFRAME_H
#define CAL_COREKEYFRAME_H

#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"

/*****************************************************************************/
/** The core keyframe class.
  *****************************************************************************/
namespace cal3d{
	class CAL3D_API CalCoreKeyframe
	{
		// member variables

	public:
		// constructors/destructor
		CalCoreKeyframe();
		virtual ~CalCoreKeyframe();

		unsigned int size();

		inline void setRotation(const CalQuaternion& rotation)    { m_rotation = rotation; }
		inline const CalQuaternion& getRotation() const           { return m_rotation; }
		inline void setTime(float time)                           { m_time = time; }
		inline float getTime()const                               { return m_time; }
		inline const CalVector& getTranslation() const            { return m_translation; }
		inline void setTranslation(const CalVector& translation)  { m_translation = translation; }
	protected:
		float m_time;
		CalVector m_translation;
		CalQuaternion m_rotation;
	};
}
#endif

//****************************************************************************//

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
#include "cal3d/transform.h"


struct CalAnimationCallback;

class CAL3D_API CalCoreAnimation : public cal3d::RefCounted
{
protected:
  ~CalCoreAnimation();

public:
  CalCoreAnimation();

  float getDuration() const;
  void setDuration(float duration);
  void scale(float factor);
  void setFilename(const std::string& filename);
  const std::string& getFilename(void) const;
  void setName(const std::string& name);
  const std::string& getName(void) const;

  void registerCallback(CalAnimationCallback *callback,float min_interval);
  void removeCallback(CalAnimationCallback *callback);

  /// Sets the poses that describe this animation.
  void setPoses(const std::vector<CalTransform>& poses, unsigned int numTracks);

  /// Gets the poses that describe this animation.
  const std::vector<CalTransform>& getPoses() const;

  /// Gets the number of tracks in this animation
  unsigned int getTrackCount() const;

  /// Sets the track assignments for this animation.
  void setTrackAssignments(const std::vector<int>& trackAssignments);

  /// Gets the pose of the bone tracks in the animation at the given time.
  void getPose(float time, std::vector<CalTransform>& pose) const;

  /// Gets the track in the animation assigned to the given bone id
  int getTrackAssignment(unsigned bone) const
  {
    if (bone >= m_skeletonMapping.size())
    {
      return -1;
    }
    return m_skeletonMapping[bone];
  }

  /// Gets the bone id for which the given track in the animation is assigned
  int getBoneAssignment(unsigned track) const
  {
    for (unsigned bone = 0; bone < m_skeletonMapping.size(); ++bone)
    {
      if (m_skeletonMapping[bone] == (int)track)
      {
        return bone;
      }
    }
    return -1;
  }

  struct CallbackRecord
  {
    CalAnimationCallback *callback;
    float                 min_interval;
  };

  std::vector<CallbackRecord>& getCallbackList() { return m_listCallbacks; }

private:

  /// The number of bone tracks in this animation.
  unsigned int m_numBoneTracks;

  /// Interleaved array of poses for the skeleton.
  std::vector<cal3d::Transform> m_poses;

  /// Mapping of the skeleton bone IDs to the bone tracks in the animation.
  /// Non-mapped bones are have a -1 assignment.
  std::vector<int> m_skeletonMapping;

  std::vector<CallbackRecord> m_listCallbacks;

  float m_duration;
  std::string m_name;
  std::string m_filename;
};
typedef cal3d::RefPtr<CalCoreAnimation> CalCoreAnimationPtr;

#endif

//****************************************************************************//

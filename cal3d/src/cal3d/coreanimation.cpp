//****************************************************************************//
// coreanimation.cpp                                                          //
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


#include "cal3d/coreanimation.h"


CalCoreAnimation::CalCoreAnimation()
  : m_numBoneTracks(0)
{
}


CalCoreAnimation::~CalCoreAnimation()
{
}

 /*****************************************************************************/
/** Returns the duration.
  *
  * This function returns the duration of the core animation instance.
  *
  * @return The duration in seconds.
  *****************************************************************************/

float CalCoreAnimation::getDuration() const
{
  return m_duration;
}

 /*****************************************************************************/
/** Sets the duration.
  *
  * This function sets the duration of the core animation instance.
  *
  * @param duration The duration in seconds that should be set.
  *****************************************************************************/

void CalCoreAnimation::setDuration(float duration)
{
  m_duration = duration;
}

 /*****************************************************************************/
/** Scale the core animation.
  *
  * This function rescale all the skeleton data that are in the core animation instance
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/

void CalCoreAnimation::scale(float factor)
{
  // Scale the translation of every bone by the given factor
  for (unsigned index = 0; index < m_poses.size(); ++index)
  {
    CalVector translation = m_poses[index].getTranslation();
    translation *= factor;
    m_poses[index].setTranslation(translation);
  }
}


 /*****************************************************************************/
/** 
  * Set the name of the file in which the core animation is stored, if any.
  *
  * @param filename The path of the file.
  *****************************************************************************/

void CalCoreAnimation::setFilename(const std::string& filename)
{
  m_filename = filename;
}

 /*****************************************************************************/
/** 
  * Get the name of the file in which the core animation is stored, if any.
  *
  * @return One of the following values:
  *         \li \b empty string if the animation was not stored in a file
  *         \li \b the path of the file
  *
  *****************************************************************************/

const std::string& CalCoreAnimation::getFilename(void) const
{
  return m_filename;
}

 /*****************************************************************************/
/** 
  * Set the symbolic name of the core animation.
  *
  * @param name A symbolic name.
  *****************************************************************************/

void CalCoreAnimation::setName(const std::string& name)
{
  m_name = name;
}

 /*****************************************************************************/
/** 
  * Get the symbolic name the core animation.
  *
  * @return One of the following values:
  *         \li \b empty string if the animation was no associated to a symbolic name
  *         \li \b the symbolic name
  *
  *****************************************************************************/

const std::string& CalCoreAnimation::getName(void) const
{
  return m_name;
}


/*****************************************************************************/
/** 
  * Add a callback to the current list of callbacks for this CoreAnim.
  *
  * @param  callback     Ptr to a subclass of this abstract class implementing the callback function.
  * @param  min_interval Minimum interval (in seconds) between callbacks.  Specifying 0 means call every update().
  *
  *****************************************************************************/

void CalCoreAnimation::registerCallback(CalAnimationCallback *callback,float min_interval)
{
  CallbackRecord record;
  record.callback     = callback;
  record.min_interval = min_interval;

  m_listCallbacks.push_back(record);
}


/*****************************************************************************/
/** 
  * Remove a callback from the current list of callbacks for this Anim.
  * Callback objects not removed this way will be deleted in the dtor of the Anim.
  *
  * @param  callback     Ptr to a subclass of this abstract class implementing the callback function to remove.
  *
  *****************************************************************************/

void CalCoreAnimation::removeCallback(CalAnimationCallback *callback)
{
  for (std::vector<CallbackRecord>::iterator i = m_listCallbacks.begin(); i != m_listCallbacks.end(); i++)
  {
    if ((*i).callback == callback)
    {
      m_listCallbacks.erase(i);
      return;
    }
  }
}

/*****************************************************************************/
/** 
  * Sets the pose data for this animation.
  *
  * @param  poses       list of interleaved coordinate transforms for each bone track.
  * @param  numTracks   the number of bone tracks stored in the pose array.
  *
  *****************************************************************************/
void CalCoreAnimation::setPoses(const std::vector<CalTransform>& poses, unsigned int numTracks)
{
  m_poses.resize(poses.size());
  for (unsigned pose_index = 0; pose_index < poses.size(); ++pose_index)
  {
    m_poses[pose_index] = poses[pose_index];
  }

  m_numBoneTracks = numTracks;
}

/*****************************************************************************/
/** 
  * Gets the pose data for this animation.
  *
  *****************************************************************************/

const std::vector<CalTransform>& CalCoreAnimation::getPoses() const
{
  return m_poses;
}

/*****************************************************************************/
/** 
  * Gets the number of tracks in the animation.
  *
  *****************************************************************************/
unsigned int CalCoreAnimation::getTrackCount() const
{
  return m_numBoneTracks;
}

/*****************************************************************************/
/** 
  * Sets the track assignments for this animation. This maps each track in the
  * animation to a specific bone in the target skeleton.
  *
  *****************************************************************************/

void CalCoreAnimation::setTrackAssignments(const std::vector<intptr_t>& trackAssignments)
{
  m_skeletonMapping.resize(trackAssignments.size());
  for (unsigned index = 0; index < trackAssignments.size(); ++index)
  {
    m_skeletonMapping[index] = trackAssignments[index];
  }
}


/*****************************************************************************/
/** 
  * Queries this animation for the pose of the skeleton at the given time in
  * the animation.
  *
  * @param  time      a time value in seconds within the animation's period
  * @param  pose      filled with the local transforms for the bones
  *
  *****************************************************************************/

void CalCoreAnimation::getPose(float time, std::vector<CalTransform>& pose) const
{
  assert(time >= 0.0f && "time must be > 0");
  assert(pose.size() >= m_numBoneTracks && "Pose does not have enough capacity.");

  const unsigned frame_width = m_numBoneTracks;

  // If the time is within the duration, interpolate between the nearest keyframes
  if (time < m_duration)
  {
    const unsigned num_poses = m_poses.size() / m_numBoneTracks;
    float time_per_frame = m_duration / (num_poses - 1);

    // Init the pose to the previous keyframe
    int previous_frame = static_cast<int>(time / time_per_frame);
	unsigned index;
    for (index = 0; index < m_numBoneTracks; ++index)
    {
      pose[index] = m_poses[(frame_width * previous_frame) + index];
    }

    // Compute the distance between frames
    float lerp_distance = (time / time_per_frame) - previous_frame;

    // Blend in the next frame
    int next_frame = previous_frame + 1;
    for (index = 0; index < m_numBoneTracks; ++index)
    {
      pose[index].blend(lerp_distance, m_poses[(frame_width * next_frame) + index]);
    }
  }

  // Hold the last frame if past the end of the animation's last frame
  else
  {
    unsigned last_frame = (m_poses.size() / frame_width) - 1;
    for (unsigned index = 0; index < m_numBoneTracks; ++index)
    {
      pose[index] = m_poses[(frame_width * last_frame) + index];
    }
  }
}

//****************************************************************************//

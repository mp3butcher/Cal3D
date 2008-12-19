//****************************************************************************//
// coretrack.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORETRACK_H
#define CAL_CORETRACK_H


#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"


class CalCoreBone;
class CalCoreKeyframe;
class CalCoreSkeleton;


class CAL3D_API CalCoreTrack
{
private:
  /// The index of the associated CoreBone in the CoreSkeleton.
  int m_coreBoneId;

  // If translationRequired is false, then the translations are the same as the
  // skeleton's translations.
  bool m_translationRequired;
  bool m_highRangeRequired;
  bool m_translationIsDynamic;
  static int m_translationRequiredCount;
  static int m_translationNotRequiredCount;

  /// List of keyframes, always sorted by time.
  std::vector<CalCoreKeyframe*> m_keyframes;

// constructors/destructor
public:
  CalCoreTrack();
  ~CalCoreTrack();

  void create();
  void destroy();
  unsigned int size();

  bool getState(float time, CalVector& translation, CalQuaternion& rotation) const;

  /*****************************************************************************/
  /** Returns the ID of the core bone.
  *
  * This function returns the ID of the core bone to which the core track
  * instance is attached to.
  *
  * @return One of the following values:
  *         \li the \b ID of the core bone
  *         \li \b -1 if an error happened
  *****************************************************************************/
  inline int getCoreBoneId() const
  {
	  return m_coreBoneId;
  }

  bool setCoreBoneId(int coreBoneId);
  
  int getCoreKeyframeCount() const;
  CalCoreKeyframe *getCoreKeyframe(int idx);
  const CalCoreKeyframe *getCoreKeyframe(int idx) const;

  static int translationRequiredCount() { return m_translationRequiredCount; }
  static int translationNotRequiredCount() { return m_translationNotRequiredCount; }
  bool addCoreKeyframe(CalCoreKeyframe *pCoreKeyframe);
	void removeCoreKeyFrame(int _i) { m_keyframes.erase( m_keyframes.begin() + _i); }
  bool getTranslationRequired() { return m_translationRequired; }
  void setTranslationRequired( bool p ) { m_translationRequired = p; }
  bool getTranslationIsDynamic() { return m_translationIsDynamic; }
  void setTranslationIsDynamic( bool p ) { m_translationIsDynamic = p; }
  bool getHighRangeRequired() { return m_highRangeRequired; }
  void setHighRangeRequired( bool p ) { m_highRangeRequired = p; }
  void fillInvalidTranslations( CalVector const & trans );

  void scale(float factor);
  void compress( double translationTolerance, double rotationToleranceDegrees, CalCoreSkeleton * skelOrNull );
  bool roundTranslation( CalCoreKeyframe const * prev, CalCoreKeyframe * p, double translationTolerance );
  void translationCompressibility( 
    bool * transRequiredResult, bool * transDynamicResult, bool * highRangeRequiredResult,
    float threshold, float highRangeThreshold, CalCoreSkeleton * skel );
  void collapseSequences( double translationTolerance, double rotationToleranceDegrees );

private:
  std::vector<CalCoreKeyframe *>::const_iterator getUpperBound(float time) const;
  bool keyframeEliminatable( CalCoreKeyframe * prev, CalCoreKeyframe * p, CalCoreKeyframe * next,
	   double translationTolerance, double rotationToleranceDegrees);
};

#endif

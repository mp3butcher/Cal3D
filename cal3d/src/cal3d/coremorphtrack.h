//****************************************************************************//
// coreMorphTrack.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMORPHTRACK_H
#define CAL_COREMORPHTRACK_H


#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremorphkeyframe.h"

class CalCoreMorphKeyframe;

class CAL3D_API CalCoreMorphTrack
{
// member variables
protected:

  unsigned int m_morphID;
  unsigned int m_targetMeshID;
  std::vector<unsigned int> m_targetSubMeshIDs;

  /// List of keyframes, always sorted by time.
  std::vector<CalCoreMorphKeyframe> m_keyframes;
  std::vector<CalCoreMorphKeyframe*> m_keyframesToDelete;

// constructors/destructor
public:
  CalCoreMorphTrack();
  virtual ~CalCoreMorphTrack();

  bool create();
  void destroy();

  bool getState(float time, float & weightOut);

  const unsigned int& getMorphID() const{return m_morphID;}
  void setMorphID(const unsigned int &name){m_morphID=name;}

  inline const unsigned int& getTargetMesh() const {return m_targetMeshID;}
  inline void setTargetMesh(unsigned int name){m_targetMeshID=name;}

  ///TargetSubMeshes container if empty assume ALL submeshes have morphtarget
  const unsigned int getTargetSubMeshCount()const{ return m_targetSubMeshIDs.size(); }
  inline void addTargetSubMesh(unsigned int i){m_targetSubMeshIDs.push_back(i);}
  inline void removeTargetSubMesh(unsigned int name){
      for(std::vector<unsigned int>::iterator i=m_targetSubMeshIDs.begin();i!=m_targetSubMeshIDs.begin();i++){
          if(*i==name){
              m_targetSubMeshIDs.erase(i);
              return;
          }
      }
  }

  inline const unsigned int &getTargetSubMesh(const unsigned int &name)const{return m_targetSubMeshIDs[name];}

  int getCoreMorphKeyframeCount() const;
  void reserve(int);

  CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx);
  const CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx) const;

  bool addCoreMorphKeyframe(CalCoreMorphKeyframe *pCoreKeyframe);

  const std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes() const;
  std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes();

  void scale(float factor);

private:
  std::vector<CalCoreMorphKeyframe>::iterator getUpperBound(float time);
};

#endif

//****************************************************************************//

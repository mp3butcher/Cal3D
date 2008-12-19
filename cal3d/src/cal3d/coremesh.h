//****************************************************************************//
// coremesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMESH_H
#define CAL_COREMESH_H


#include "cal3d/global.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"


class CalCoreSubmesh;


class CAL3D_API CalCoreMesh : public cal3d::RefCounted
{
protected:
  ~CalCoreMesh();

public:
  CalCoreMesh();

  typedef std::vector<CalCoreSubmesh *> CalCoreSubmeshVector;
  unsigned int size();
  int addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh);
  CalCoreSubmesh *getCoreSubmesh(int id);
  const CalCoreSubmesh *getCoreSubmesh(int id) const;
  void removeCoreSubmesh( int submeshID );
  int getCoreSubmeshCount() const;
  void reserve(int submeshes) { m_vectorCoreSubmesh.reserve(submeshes); }
  std::vector<CalCoreSubmesh *>& getVectorCoreSubmesh();
  const std::vector<CalCoreSubmesh *>& getVectorCoreSubmesh() const;
  int addAsMorphTarget(CalCoreMesh *pCoreMesh, std::string const & morphTargetName);
  void scale(float factor);
  void setFilename(const std::string& filename);
  const std::string& getFilename(void) const;
  void setName(const std::string& name);
  const std::string& getName(void) const;

private:
  std::vector<CalCoreSubmesh *> m_vectorCoreSubmesh;
  std::string                   m_name;
  std::string                   m_filename;
};
typedef cal3d::RefPtr<CalCoreMesh> CalCoreMeshPtr;

#endif

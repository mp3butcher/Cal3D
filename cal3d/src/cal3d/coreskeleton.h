//****************************************************************************//
// coreskeleton.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESKELETON_H
#define CAL_CORESKELETON_H

#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"


class CalCoreBone;
class CalCoreModel;


class CAL3D_API CalCoreSkeleton : public cal3d::RefCounted
{
protected:
    ~CalCoreSkeleton();

public:
    CalCoreSkeleton();

    //name
    void setName(const std::string& name);
    const std::string& getName(void) const;

    //bones vector
    ///add a Bone
    int addCoreBone(CalCoreBone *pCoreBone);
    ///return bones count of the skeleton
    unsigned int getNumCoreBones() const { return ( unsigned int ) m_vectorCoreBone.size(); }

    CalCoreBone *getCoreBone(int coreBoneId);
    const CalCoreBone *getCoreBone(int coreBoneId) const;
    std::vector<CalCoreBone *>& getVectorCoreBone();
    const std::vector<CalCoreBone *>& getVectorCoreBone() const;

    /// Returns the root core bone id list.
    std::vector<int>& getVectorRootCoreBoneId();
    const std::vector<int>& getVectorRootCoreBoneId() const;

    void calculateState();
    void calculateBoundingBoxes(CalCoreModel *pCoreModel);


    //helpers

    ///associate a name to a bone
    bool mapCoreBoneName(int coreBoneId, const std::string& strName);
    ////return the index of the bone mapped the given name
    int getCoreBoneId(const std::string& strName) const;
    ////return the bone mapped the given name
    CalCoreBone *getCoreBone(const std::string& strName);
    ////return the bone mapped the given name
    const CalCoreBone *getCoreBone(const std::string& strName) const;

    ///scale all the skeleton inner data by factor
    void scale(float factor);
private:
    std::vector<CalCoreBone *>   m_vectorCoreBone;
    std::map< std::string, int > m_mapCoreBoneNames;
    std::vector<int>             m_vectorRootCoreBoneId;
    std::string                  m_name;
};
typedef cal3d::RefPtr<CalCoreSkeleton> CalCoreSkeletonPtr;

#endif

//****************************************************************************//

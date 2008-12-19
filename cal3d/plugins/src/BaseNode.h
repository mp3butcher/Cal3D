//----------------------------------------------------------------------------//
// BaseNode.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef BASE_NODE_H
#define BASE_NODE_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CBaseNode
{
// misc
public:
    enum Type
    {
		TYPE_OTHER = 0,
        TYPE_BONE,
        TYPE_DUMMY,
                TYPE_LIGHT,
        TYPE_MESH
    };

// constructors/destructor
public:
    virtual ~CBaseNode() { }

// member functions
public:
    virtual bool operator==(const CBaseNode& rhs) const = 0;

    virtual CBaseNode *GetChild(int childId) = 0;
    virtual int GetChildCount() = 0;
    virtual std::string GetName() = 0;
    virtual Type GetType() = 0;

        // not sure if this is the right class for these functions
        // for nodes of type TYPE_LIGHT, this additional data is available
  virtual CalLightType GetLightType() {
    return LIGHT_TYPE_NONE;
  }
  virtual void GetLightColor( CalVector &color ) {
  }
};

#endif

//----------------------------------------------------------------------------//

//****************************************************************************//
// corematerial.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMATERIAL_H
#define CAL_COREMATERIAL_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core material class.
  *****************************************************************************/

class CAL3D_API CalCoreMaterial
{
// misc
public:
  /// The core material Color.
  typedef struct
  {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
  } Color;

  /// The core material Map.
  typedef struct
  {
    std::string strFilename;
    Cal::UserData userData;
  } Map;

// member variables
protected:
  Color m_ambientColor;
  Color m_diffuseColor;
  Color m_specularColor;
  float m_shininess;
  std::vector<Map> m_vectorMap;
  Cal::UserData m_userData;

// constructors/destructor
public:
  CalCoreMaterial();
  virtual ~CalCoreMaterial();

// member functions
public:
  bool create();
  void destroy();
  Color& getAmbientColor();
  Color& getDiffuseColor();
  int getMapCount();
  const std::string& getMapFilename(int mapId);
  Cal::UserData getMapUserData(int mapId);
  float getShininess();
  Color& getSpecularColor();
  Cal::UserData getUserData();
  std::vector<Map>& getVectorMap();
  bool reserve(int mapCount);
  void setAmbientColor(const Color& ambientColor);
  void setDiffuseColor(const Color& diffuseColor);
  bool setMap(int mapId, const Map& map);
  bool setMapUserData(int mapId, Cal::UserData userData);
  void setShininess(float shininess);
  void setSpecularColor(const Color& specularColor);
  void setUserData(Cal::UserData userData);
};

#endif

//****************************************************************************//

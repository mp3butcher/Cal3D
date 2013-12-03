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


#include "cal3d/global.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"


// Maximum length of mapType string buffer, including terminating zero.
#define CalCoreMaterialMapTypeLengthMax ( 128 )
class CAL3D_API CalCoreMaterial : public cal3d::RefCounted
{
public:
  struct Color
  {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
  };

  struct Map
  {
    std::string strFilename;
    std::string mapType;
    Cal::UserData userData;
  };

  CalCoreMaterial();
  CalCoreMaterial( const CalCoreMaterial& inOther );

protected:
  ~CalCoreMaterial() { }

public:
  const Color& getAmbientColor() const;
  const Color& getDiffuseColor() const;
  int getMapCount() const;
  const std::string& getMapFilename(int mapId) const;
  const std::string& getMapType(int mapId);
  Cal::UserData getMapUserData(int mapId);
  const Cal::UserData getMapUserData(int mapId) const;
  float getShininess() const;
  const Color& getSpecularColor() const;
  Cal::UserData getUserData();
  const Cal::UserData getUserData() const;
  std::vector<Map>& getVectorMap();
  const std::vector<Map>& getVectorMap() const;
  bool reserve(int mapCount);
  void setAmbientColor(const Color& ambientColor);
  void setDiffuseColor(const Color& diffuseColor);
  bool setMap(int mapId, const Map& map);
  bool setMapUserData(int mapId, Cal::UserData userData);
  void setShininess(float shininess);
  void setSpecularColor(const Color& specularColor);
  void setFilename(const std::string& filename);
  const std::string& getFilename(void) const;
  void setName(const std::string& name);
  const std::string& getName(void) const;
  void setUserData(Cal::UserData userData);
  bool getAlphaBlending() { return false; } // No check box available in max.
  bool getTwoSided() { return getMapCount() > 1; } // Should come from check box.
  bool getSelfIllumination() { return false; } // Should come from check box.

private:
  Color            m_ambientColor;
  Color            m_diffuseColor;
  Color            m_specularColor;
  float            m_shininess;
  std::vector<Map> m_vectorMap;
  Cal::UserData    m_userData;
  std::string      m_name;
  std::string      m_filename;
};
typedef cal3d::RefPtr<CalCoreMaterial> CalCoreMaterialPtr;

#endif

//****************************************************************************//

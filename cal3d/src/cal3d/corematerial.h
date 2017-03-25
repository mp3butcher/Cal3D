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
namespace cal3d{
	class CAL3D_API CalCoreMaterial : public  RefCounted
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
		CalCoreMaterial(const CalCoreMaterial& inOther);

	protected:
		~CalCoreMaterial() { }

	public:
		inline void setAmbientColor(const Color& ambientColor)  { m_ambientColor = ambientColor; }
		inline const Color& getAmbientColor() const             { return m_ambientColor; }

		inline const Color& getDiffuseColor() const             { return m_diffuseColor; }
		inline void setDiffuseColor(const Color& diffuseColor)  { m_diffuseColor = diffuseColor; }

		inline void setShininess(float shininess)               { m_shininess = shininess; }
		inline const float& getShininess() const                { return m_shininess; }

		inline void setSpecularColor(const Color& specularColor){ m_specularColor = specularColor; }
		inline const Color& getSpecularColor() const            { return m_specularColor; }

		inline void setFilename(const std::string& filename)    { m_filename = filename; }
		inline const std::string& getFilename(void) const       { return m_filename; }

		inline void setName(const std::string& name)            { m_name = name; }
		inline const std::string& getName(void) const           { return m_name; }

		inline void setUserData(Cal::UserData userData)         { m_userData = userData; }
		inline Cal::UserData getUserData()                      { return m_userData; }
		inline const Cal::UserData getUserData() const          { return m_userData; }

		inline std::vector<Map>& getVectorMap()                 { return m_vectorMap; }
		inline const std::vector<Map>& getVectorMap() const     { return m_vectorMap; }

		int getMapCount() const;
		const std::string& getMapFilename(int mapId) const;
		const std::string& getMapType(int mapId);
		Cal::UserData getMapUserData(int mapId);
		const Cal::UserData getMapUserData(int mapId) const;

		bool reserve(int mapCount);
		bool setMap(int mapId, const Map& map);
		bool setMapUserData(int mapId, Cal::UserData userData);
		//bool getAlphaBlending() { return false; } // No check box available in max.
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
}
#endif

//****************************************************************************//

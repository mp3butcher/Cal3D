//****************************************************************************//
// coresubmesh.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESUBMESH_H
#define CAL_CORESUBMESH_H


#include "cal3d/global.h"
#include "cal3d/vector.h"

namespace cal3d{
	class CalCoreSubMorphTarget;


	enum CalMorphTargetType {
		CalMorphTargetTypeNull = 0,
		CalMorphTargetTypeAdditive,
		CalMorphTargetTypeClamped,
		CalMorphTargetTypeAverage,
		CalMorphTargetTypeExclusive
	};


	class CAL3D_API CalCoreSubmesh
	{
	public:
		struct TextureCoordinate
		{
			float u, v;
		};

		struct TangentSpace
		{
			CalVector tangent;
			float crossFactor;  // To get the binormal, use ((N x T) * crossFactor)
		};

		struct Influence
		{
			int boneId;
			float weight;
		};

		struct PhysicalProperty
		{
			float weight;
		};

		struct Vertex
		{
			CalVector position;
			CalVector normal;
			std::vector<Influence> vectorInfluence;
			int collapseId;
			int faceCollapseCount;
			CalVector vertexColor;
		};

		struct Face
		{
			CalIndex vertexId[3];
		};

		/// The core submesh Spring.
		struct Spring
		{
			int vertexId[2];
			float springCoefficient;
			float idleLength;
		};

	public:
		CalCoreSubmesh();
		~CalCoreSubmesh();


		typedef std::vector<CalCoreSubMorphTarget *> CoreSubMorphTargetVector;
		typedef std::vector<Face> VectorFace;
		typedef std::vector<PhysicalProperty> VectorPhysicalProperty;
		typedef std::vector<Spring> VectorSpring;
		typedef std::vector<TangentSpace> VectorTangentSpace;
		typedef std::vector<TextureCoordinate> VectorTextureCoordinate;
		typedef std::vector<VectorTangentSpace > VectorVectorTangentSpace;
		typedef std::vector<VectorTextureCoordinate > VectorVectorTextureCoordinate;
		typedef std::vector<Vertex> VectorVertex;
		typedef std::vector<Influence> VectorInfluence;
		unsigned int size();
		unsigned int sizeWithoutSubMorphTargets();

		//vertex array
		bool setVertex(int vertexId, const Vertex& vertex);
		int getVertexCount() const;
		std::vector<Vertex>& getVectorVertex();
		const std::vector<Vertex>& getVectorVertex() const;

		//faces array
		bool setFace(int faceId, const Face& face);
		int getFaceCount() const;
		std::vector<Face>& getVectorFace();
		const std::vector<Face>& getVectorFace() const;

		//texture coord array arrays
		bool setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate);
		std::vector<std::vector<TextureCoordinate> >& getVectorVectorTextureCoordinate();
		const std::vector<std::vector<TextureCoordinate> >& getVectorVectorTextureCoordinate() const;

		//vertices angent space
		bool setTangentSpace(int vertexId, int textureCoordinateId, const CalVector& tangent, float crossFactor);
		std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace();
		const std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace() const;

		//vertices physical
		bool setPhysicalProperty(int vertexId, const PhysicalProperty& physicalProperty);
		std::vector<PhysicalProperty>& getVectorPhysicalProperty();
		const std::vector<PhysicalProperty>& getVectorPhysicalProperty() const;


		//submesh material
		void setCoreMaterialThreadId(int coreMaterialThreadId);
		int getCoreMaterialThreadId() const;

		//submesh lod count
		void setLodCount(int lodCount);
		int getLodCount() const;

		//submesh springs
		bool setSpring(int springId, const Spring& spring);
		int getSpringCount() const;
		std::vector<Spring>& getVectorSpring();
		const std::vector<Spring>& getVectorSpring() const;

		//morphtargets
		int addCoreSubMorphTarget(CalCoreSubMorphTarget *pCoreSubMorphTarget);
		int getCoreSubMorphTargetCount() const;
		CalCoreSubMorphTarget *getCoreSubMorphTarget(int id);
		const CalCoreSubMorphTarget *getCoreSubMorphTarget(int id) const;
		std::vector<CalCoreSubMorphTarget *>& getVectorCoreSubMorphTarget();
		const std::vector<CalCoreSubMorphTarget *>& getVectorCoreSubMorphTarget() const;

		//enable/disable tangent space
		bool isTangentsEnabled(int mapId) const;
		bool enableTangents(int mapId, bool enabled);

		bool hasNonWhiteVertexColors() { return m_hasNonWhiteVertexColors; }
		void setHasNonWhiteVertexColors(bool p) { m_hasNonWhiteVertexColors = p; }


		bool reserve(int vertexCount, int textureCoordinateCount, int faceCount, int springCount);
		///scale all the mesh inner data by factor
		void scale(float factor);

	private:
		void UpdateTangentVector(int v0, int v1, int v2, int channel);

		std::vector<Vertex>                          m_vectorVertex;
		std::vector<bool>                            m_vectorTangentsEnabled;
		std::vector<std::vector<TangentSpace> >      m_vectorvectorTangentSpace;
		std::vector<std::vector<TextureCoordinate> > m_vectorvectorTextureCoordinate;
		std::vector<PhysicalProperty>                m_vectorPhysicalProperty;
		std::vector<Face>                            m_vectorFace;
		std::vector<Spring>                          m_vectorSpring;
		std::vector<CalCoreSubMorphTarget *>         m_vectorCoreSubMorphTarget;
		int                                          m_coreMaterialThreadId;
		int                                          m_lodCount;
		std::vector<unsigned int>                    m_vectorSubMorphTargetGroupIndex;
		bool                                         m_hasNonWhiteVertexColors;
	};
}
#endif

//****************************************************************************//

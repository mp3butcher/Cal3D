//****************************************************************************//
// hardwaremodel.h                                                         //
// Copyright (C) 2004 Desmecht Laurent                                        //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_HARDWAREMODEL_H
#define CAL_HARDWAREMODEL_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/coresubmesh.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalCoreMaterial;


//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

/*****************************************************************************/
/** The hardware model class.
*****************************************************************************/

class CAL3D_API CalHardwareModel
{
	
	// member variables
private:
	
	struct CalHardwareMesh
	{
		std::vector<int> m_vectorBonesIndices;
		
		int baseVertexIndex;
		int vertexCount;
		int startIndex;
		int faceCount;
		CalCoreMaterial *pCoreMaterial;	
	};
	
	std::vector<CalHardwareMesh> m_vectorHardwareMesh;
	std::vector<CalIndex> m_vectorVertexIndiceUsed;
	int m_selectedHardwareMesh;
	
	CalModel *m_pModel;
	
	
	char * m_pVertexBuffer;
	int m_vertexStride;
	char * m_pNormalBuffer;
	int m_normalStride;
	char * m_pWeightBuffer;
	int m_weightStride;
	char * m_pMatrixIndexBuffer;
	int m_matrixIndexStride;
	char * m_pTextureCoordBuffer[8];
	int m_textureCoordStride[8];
	int m_textureCoordNum;
	
	CalIndex * m_pIndexBuffer;

	int m_totalVertexCount;
	int m_totalFaceCount;

	
	
	// constructors/destructor
	public:
		CalHardwareModel();
		virtual ~CalHardwareModel();
		
		// member functions	
	public:
		bool create(CalModel *pModel);
		
		void setVertexBuffer( char * pVertexBuffer, int stride); 
		void setIndexBuffer( CalIndex * pIndexBuffer); 
		void setNormalBuffer( char * pNormalBuffer, int stride); 
		void setWeightBuffer( char * pWeightBuffer, int stride); 
		void setMatrixIndexBuffer( char * pMatrixIndexBuffer, int stride); 
		void setTextureCoordNum(int textureCoordNum);
		void setTextureCoordBuffer(int mapId, char * pTextureCoordBuffer, int stride); 

		bool load(int baseVertexIndex, int startIndex,int maxBonesPerMesh);
				
		void destroy();
		
		std::vector<CalHardwareMesh> & getVectorHardwareMesh();
		void getAmbientColor(unsigned char *pColorBuffer);
		void getDiffuseColor(unsigned char *pColorBuffer);
		void getSpecularColor(unsigned char *pColorBuffer);
		const CalQuaternion & getRotationBoneSpace(int boneId);
		const CalVector & getTranslationBoneSpace(int boneId);

		float getShininess();
		
		int getHardwareMeshCount();
		int getFaceCount();
		int getVertexCount();
		int getBoneCount();
	
		int getBaseVertexIndex();
		int getStartIndex();

		int getTotalFaceCount();
		int getTotalVertexCount();		
	
		Cal::UserData getMapUserData(int mapId);
		
		bool selectHardwareMesh(int meshId);  
		
	private:
		bool canAddFace(CalHardwareMesh &hardwareMesh, CalCoreSubmesh::Face & face,std::vector<CalCoreSubmesh::Vertex>& vectorVertex, int maxBonesPerMesh);
		int  addVertex(CalHardwareMesh &hardwareMesh, int indice ,std::vector<CalCoreSubmesh::Vertex>& vectorVertex,  std::vector< std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorTexCoord, int maxBonesPerMesh);
		int  addBoneIndice(CalHardwareMesh &hardwareMesh, int Indice, int maxBonesPerMesh);  
		
};

#endif

//****************************************************************************//

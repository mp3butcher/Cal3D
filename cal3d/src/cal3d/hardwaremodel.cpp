//****************************************************************************//
// hardwaremodel.cpp                                                       //
// Copyright (C) 2004 Desmecht Laurent                                        //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/error.h"
#include "cal3d/hardwaremodel.h"
#include "cal3d/coremodel.h"
#include "cal3d/model.h"
#include "cal3d/coremesh.h"
#include "cal3d/bone.h"
#include "cal3d/corematerial.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/skeleton.h"


 /*****************************************************************************/
/** Constructs the hardware model instance.
  *
  * This function is the default constructor of the hardware model instance.
  *****************************************************************************/


CalHardwareModel::CalHardwareModel() : m_selectedHardwareMesh(-1)
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pNormalBuffer = NULL;
	m_pWeightBuffer = NULL;
	m_pMatrixIndexBuffer = NULL;
	int i;
	for( i = 0 ; i < 8 ; i++)
		m_pTextureCoordBuffer[i]=NULL;
	m_textureCoordNum=0;	

	m_totalFaceCount=0;
	m_totalVertexCount=0;
}

 /*****************************************************************************/
/** Destructs the hardware model instance.
  *
  * This function is the destructor of the hardware model instance.
  *****************************************************************************/


CalHardwareModel::~CalHardwareModel()
{
	m_vectorHardwareMesh.clear();
	m_vectorVertexIndiceUsed.clear();
	
}

 /*****************************************************************************/
/** Creates the hardware model instance.
  *
  * This function creates the hardware model instance.
  *
  * @param pModel A pointer to the model that should be managed with this
  *               hardware model instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/


bool CalHardwareModel::create(CalCoreModel *pCoreModel)
{
	if(pCoreModel == 0)
	{
		CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
		return false;
	}
	
	m_pCoreModel = pCoreModel;
	
	return true;
	
}

 /*****************************************************************************/
/** Set the vertex (position) buffer of the hardware model instance.
  *
  * This function set the vertex (position) buffer the hardware model instance.
  *
  * @param pVertexBuffer A pointer to the vertex buffer.
  * @param stride  The number of byte to add to find the next element
  *
  *****************************************************************************/

void CalHardwareModel::setVertexBuffer( char * pVertexBuffer, int stride)
{
	m_pVertexBuffer = pVertexBuffer;
	m_vertexStride = stride;
}

 /*****************************************************************************/
/** Set the index buffer of the hardware model instance.
  *
  * This function set the index buffer the hardware model instance.
  *
  * @param pIndexBuffer A pointer to the index buffer.
  *
  *****************************************************************************/

void CalHardwareModel::setIndexBuffer( CalIndex * pIndexBuffer)
{
	m_pIndexBuffer= pIndexBuffer;
}

 /*****************************************************************************/
/** Set the normal buffer of the hardware model instance.
  *
  * This function set the normal buffer the hardware model instance.
  *
  * @param pNormalBuffer A pointer to the normal buffer.
  * @param stride  The number of byte to add to find the next element
  *
  *****************************************************************************/

void CalHardwareModel::setNormalBuffer( char * pNormalBuffer, int stride)
{
	m_pNormalBuffer = pNormalBuffer;
	m_normalStride = stride;
}

 /*****************************************************************************/
/** Set the weight buffer of the hardware model instance.
  *
  * This function set the weight buffer the hardware model instance.
  *
  * @param pWeightBuffer A pointer to the weight buffer.
  * @param stride  The number of byte to add to find the next element
  *
  *****************************************************************************/


void CalHardwareModel::setWeightBuffer( char * pWeightBuffer, int stride)
{
	m_pWeightBuffer = pWeightBuffer;
	m_weightStride = stride;
}

 /*****************************************************************************/
/** Set the matrix index buffer of the hardware model instance.
  *
  * This function set the matrix index buffer the hardware model instance.
  *
  * @param pMatrixIndexBuffer A pointer to the matrix index buffer.
  * @param stride  The number of byte to add to find the next element
  *
  *****************************************************************************/


void CalHardwareModel::setMatrixIndexBuffer( char * pMatrixIndexBuffer, int stride)
{
	m_pMatrixIndexBuffer = pMatrixIndexBuffer;
	m_matrixIndexStride = stride;
}

 /*****************************************************************************/
/** Set the number the texture coordinate of the hardware model instance.
  *
  * This function set the number the texture coordinate the hardware model instance.
  *
  * @param textureCoordNum A integer with the number the texture coordinate.
  *
  *****************************************************************************/

void CalHardwareModel::setTextureCoordNum(int textureCoordNum)
{
	if( 0<= textureCoordNum && textureCoordNum < 8)
	{
		m_textureCoordNum=textureCoordNum;       
	}      
}

 /*****************************************************************************/
/** Set the texture coordinate buffer of the hardware model instance.
  *
  * This function set the texture coordinate buffer the hardware model instance.
  *
  * @param mapId A integer to the texture stage
  * @param pTextureCoordBuffer A pointer to the weight buffer.
  * @param stride  The number of byte to add to find the next element
  *
  *****************************************************************************/


void CalHardwareModel::setTextureCoordBuffer(int mapId, char * pTextureCoordBuffer, int stride)
{
	if( 0<= mapId && mapId < 8)
	{
		m_pTextureCoordBuffer[mapId] = pTextureCoordBuffer;
		m_textureCoordStride[mapId] = stride;  	
	}	
}

 /*****************************************************************************/
/** Destroys the hardware model instance.
  *
  * This function destroys all data stored in the hardware model instance and frees
  * all allocated memory.
  *****************************************************************************/


void CalHardwareModel::destroy()
{
	int hardwareMeshIndex;
	for(hardwareMeshIndex = 0 ; hardwareMeshIndex < m_vectorHardwareMesh.size(); hardwareMeshIndex++)
	{
		m_vectorHardwareMesh[hardwareMeshIndex]. m_vectorBonesIndices.clear();
	}
	m_vectorHardwareMesh.clear();
	
}

 /*****************************************************************************/
/** Returns the hardware mesh vector.
  *
  * This function returns the vector that contains all hardware mesh of the
  * core mesh instance.
  *
  * @return A reference to the hardware mesh vector.
  *****************************************************************************/


std::vector<CalHardwareModel::CalHardwareMesh> & CalHardwareModel::getVectorHardwareMesh()
{
	return m_vectorHardwareMesh;
}

 /*****************************************************************************/
/** Provides access to the ambient color.
  *
  * This function returns the ambient color of the material of the selected
  * hardware mesh.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/


void CalHardwareModel::getAmbientColor(unsigned char *pColorBuffer)
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size() 
		&& m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial!=0)
	{
		CalCoreMaterial::Color& color = m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial->getAmbientColor();
		pColorBuffer[0] = color.red;
		pColorBuffer[1] = color.green;
		pColorBuffer[2] = color.blue;
		pColorBuffer[3] = color.alpha;
	}
	else
	{
		pColorBuffer[0] = 0;
		pColorBuffer[1] = 0;
		pColorBuffer[2] = 0;
		pColorBuffer[3] = 0;   	
	}	
}

 /*****************************************************************************/
/** Provides access to the diffuse color.
  *
  * This function returns the diffuse color of the material of the selected
  * hardware mesh.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/


void CalHardwareModel::getDiffuseColor(unsigned char *pColorBuffer)
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size() 
		&& m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial!=0)
	{
		CalCoreMaterial::Color& color = m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial->getDiffuseColor();
		pColorBuffer[0] = color.red;
		pColorBuffer[1] = color.green;
		pColorBuffer[2] = color.blue;
		pColorBuffer[3] = color.alpha;
	}
	else
	{
		pColorBuffer[0] = 0;
		pColorBuffer[1] = 0;
		pColorBuffer[2] = 0;
		pColorBuffer[3] = 0;   	
	}	
}
 
/*****************************************************************************/
/** Provides access to the specular color.
  *
  * This function returns the specular color of the material of the selected
  * hardware.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/

void CalHardwareModel::getSpecularColor(unsigned char *pColorBuffer)
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size() 
		&& m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial!=0)
	{
		CalCoreMaterial::Color& color = m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial->getSpecularColor();
		pColorBuffer[0] = color.red;
		pColorBuffer[1] = color.green;
		pColorBuffer[2] = color.blue;
		pColorBuffer[3] = color.alpha;
	}
	else
	{
		pColorBuffer[0] = 0;
		pColorBuffer[1] = 0;
		pColorBuffer[2] = 0;
		pColorBuffer[3] = 0;   	
	}	
}

/*****************************************************************************/
/** Returns the shininess factor.
  *
  * This function returns the shininess factor of the material of the selected
  * hardware mesh..
  *
  * @return The shininess factor.
  *****************************************************************************/


float CalHardwareModel::getShininess()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size() 
		&& m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial!=0)
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial->getShininess();
	}
	else
	{
		return 50.0f;
	}	
}

 /*****************************************************************************/
/** Returns the bone space rotation of the bone boneId.
  *
  * This function returns the rotation to bring a point into the
  *bone instance space of the bone boneId of the selected hardware mesh.
  *
  * @param boneId A integer with the bone number
  * @return The rotation to bring a point into bone space.
  *****************************************************************************/

const CalQuaternion & CalHardwareModel::getRotationBoneSpace(int boneId, CalSkeleton *pSkeleton)
{
	std::vector<CalBone *> vectorBone = pSkeleton->getVectorBone();
	return vectorBone[m_vectorHardwareMesh[m_selectedHardwareMesh].m_vectorBonesIndices[boneId]]->getRotationBoneSpace();
}

 /*****************************************************************************/
/** Returns the bone space translation of the bone boneId.
  *
  * This function returns the translation to bring a point into the
  *bone instance space of the bone boneId of the selected hardware mesh.
  *
  * @param boneId A integer with the bone number
  * @return The translation to bring a point into bone space.
  *****************************************************************************/


const CalVector & CalHardwareModel::getTranslationBoneSpace(int boneId, CalSkeleton *pSkeleton)
{
	std::vector<CalBone *> vectorBone = pSkeleton->getVectorBone();
	return vectorBone[m_vectorHardwareMesh[m_selectedHardwareMesh].m_vectorBonesIndices[boneId]]->getTranslationBoneSpace();
}

 /*****************************************************************************/
/** Returns the number of hardware meshes.
  *
  * This function returns the number of hardware meshes in the hardware model
  * instance.
  *
  * @return The number of hardware meshes.
  *****************************************************************************/



int CalHardwareModel::getHardwareMeshCount()
{
	return m_vectorHardwareMesh.size();
}

 /*****************************************************************************/
/** Returns the number of faces.
  *
  * This function returns the number of faces in the selected hardware mesh instance.
  *
  * @return The number of faces.
  *****************************************************************************/


int CalHardwareModel::getFaceCount()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].faceCount;
	}
	return 0;		
}

/*****************************************************************************/
/** Returns the number of vertex.
  *
  * This function returns the number of vertex in the selected hardware mesh instance.
  *
  * @return The number of vertex.
  *****************************************************************************/

int CalHardwareModel::getVertexCount()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].vertexCount;
	}
	return 0;		
}

/*****************************************************************************/
/** Returns the number of bone.
  *
  * This function returns the number of bone in the selected hardware mesh instance.
  *
  * @return The number of bone.
  *****************************************************************************/


int CalHardwareModel::getBoneCount()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].m_vectorBonesIndices.size();
	}
	return 0;		
}

/*****************************************************************************/
/** Returns the base vertex index.
  *
  * This function returns the base vertex index of the selected hardware mesh instance.
  *
  * @return a integer with the base vertex index.
  *****************************************************************************/


int CalHardwareModel::getBaseVertexIndex()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].baseVertexIndex;
	}
	return 0;		
}

/*****************************************************************************/
/** Returns the start index.
  *
  * This function returns the start index of the selected hardware mesh instance.
  *
  * @return a integer with the start index.
  *****************************************************************************/


int CalHardwareModel::getStartIndex()
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		return m_vectorHardwareMesh[m_selectedHardwareMesh].startIndex;
	}
	return 0;		
}


 /*****************************************************************************/
/** Selects a hardware mesh for rendering data queries.
  *
  * This function selects a hardware mesh for further rendering data queries.
  *
  * @param meshId The ID of the hardware mesh that should be used for further rendering
  *               data queries.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/


bool CalHardwareModel::selectHardwareMesh(int meshId)
{
	if( meshId >= 0 && meshId < m_vectorHardwareMesh.size()) 
	{
		m_selectedHardwareMesh=meshId;   	
		return true;
	}
	return false;		
}

/*****************************************************************************/
/** Returns the number of faces in the hardware model instance.
  *
  * This function returns the number of faces in the hardware model instance.
  *
  * @return The number of faces.
  *****************************************************************************/


int CalHardwareModel::getTotalFaceCount()
{
	return m_totalFaceCount;
}

/*****************************************************************************/
/** Returns the number of vertices in the hardware model instance.
  *
  * This function returns the number of vertices in the hardware model instance.
  *
  * @return The number of vertices.
  *****************************************************************************/


int CalHardwareModel::getTotalVertexCount()
{
	return m_totalVertexCount;
}


/*****************************************************************************/
/** Provides access to a specified map user data.
*
* This function returns the user data stored in the specified map of the
* material of the selected hardware mesh.
*
* @param mapId The ID of the map.
*
* @return One of the following values:
*         \li the user data stored in the specified map
*         \li \b 0 if an error happend
*****************************************************************************/

Cal::UserData CalHardwareModel::getMapUserData(int mapId)
{
	if( m_selectedHardwareMesh >= 0 && m_selectedHardwareMesh < m_vectorHardwareMesh.size()) 
	{
		if(m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial==0)
			return 0;
		
		// get the map vector
        std::vector<CalCoreMaterial::Map>& vectorMap = m_vectorHardwareMesh[m_selectedHardwareMesh].pCoreMaterial->getVectorMap();
		
		
        // check if the map id is valid
        if((mapId < 0) || (mapId >= (int)vectorMap.size()))
        {
			CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
			return 0;
        }
        
        return vectorMap[mapId].userData;
	}
	return 0;
}

/*****************************************************************************/
/** Compute the information needed to use the hardware model .
*
* This function Compute the information needed to use the hardware model,
* it fill vertex buffers with the model data
*
* @param baseVertexIndex The base vertex Index.
* @param startIndex The start index.
* @param maxBonesPerMesh The maximun of bone by hardware mesh
*
* @return One of the following values:
*         \li \b true if succeed
*         \li \b false if an error happend
*****************************************************************************/



bool CalHardwareModel::load(int baseVertexIndex, int startIndex,int maxBonesPerMesh)
{
	if(m_pVertexBuffer==NULL ||  m_pNormalBuffer ==NULL|| m_pWeightBuffer ==NULL || m_pMatrixIndexBuffer ==NULL)
	{
		CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
		return false;  	
	}

	int mapId;
	for(mapId = 0; mapId < m_textureCoordNum; mapId++)
	{
		if(m_pTextureCoordBuffer[mapId]==NULL)
		{
			CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
			return false;  	  		
		}  
	}	
	
	m_vectorVertexIndiceUsed.resize(50000);
	int vertexCount=baseVertexIndex;
	int faceIndexCount = startIndex;
				
	CalCoreSkeleton * pCoreSkeleton = m_pCoreModel->getCoreSkeleton();
	
	std::vector< CalCoreBone *>& vectorBone = pCoreSkeleton->getVectorCoreBone();
	
	int meshId;
	for(meshId = 0;meshId< m_pCoreModel->getCoreMeshCount();meshId++)
	{
		CalCoreMesh *pCoreMesh = m_pCoreModel->getCoreMesh(meshId);
		int submeshCount= pCoreMesh->getCoreSubmeshCount();
		int submeshId;
		for(submeshId = 0 ;submeshId<submeshCount;submeshId++)
		{		  
			CalCoreSubmesh *pCoreSubmesh = pCoreMesh->getCoreSubmesh(submeshId);
			
			std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
			std::vector<CalCoreSubmesh::Face>& vectorFace = pCoreSubmesh->getVectorFace();
			std::vector< std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorTex = pCoreSubmesh->getVectorVectorTextureCoordinate();
			
			CalHardwareMesh hardwareMesh;

			hardwareMesh.meshId = meshId;
			hardwareMesh.submeshId = submeshId;
			
			hardwareMesh.baseVertexIndex=vertexCount;		  
			hardwareMesh.startIndex=faceIndexCount;
			hardwareMesh.m_vectorBonesIndices.clear();					
			
			hardwareMesh.vertexCount=0;
			hardwareMesh.faceCount=0;		  
			
			int startIndex=hardwareMesh.startIndex;
			
			int faceId;		  
			for( faceId =0 ;faceId<pCoreSubmesh->getFaceCount();faceId++)
			{
				if(canAddFace(hardwareMesh,vectorFace[faceId],vectorVertex,maxBonesPerMesh))
				{
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3]=   addVertex(hardwareMesh,vectorFace[faceId].vertexId[0],vectorVertex,vectorTex,maxBonesPerMesh);
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3+1]= addVertex(hardwareMesh,vectorFace[faceId].vertexId[1],vectorVertex,vectorTex,maxBonesPerMesh);
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3+2]= addVertex(hardwareMesh,vectorFace[faceId].vertexId[2],vectorVertex,vectorTex,maxBonesPerMesh);
					hardwareMesh.faceCount++;
				}
				else
				{
					vertexCount+=hardwareMesh.vertexCount;
					faceIndexCount+=hardwareMesh.faceCount*3;
					hardwareMesh.pCoreMaterial= m_pCoreModel->getCoreMaterial(pCoreSubmesh->getCoreMaterialThreadId());
					
					m_vectorHardwareMesh.push_back(hardwareMesh);
					
					hardwareMesh.baseVertexIndex=vertexCount;
					hardwareMesh.startIndex=faceIndexCount;
					
					hardwareMesh.m_vectorBonesIndices.clear();
					hardwareMesh.vertexCount=0; 
					hardwareMesh.faceCount=0;
					
					startIndex=hardwareMesh.startIndex;
					
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3]=   addVertex(hardwareMesh,vectorFace[faceId].vertexId[0],vectorVertex,vectorTex,maxBonesPerMesh);
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3+1]= addVertex(hardwareMesh,vectorFace[faceId].vertexId[1],vectorVertex,vectorTex,maxBonesPerMesh);
					m_pIndexBuffer[startIndex+hardwareMesh.faceCount*3+2]= addVertex(hardwareMesh,vectorFace[faceId].vertexId[2],vectorVertex,vectorTex,maxBonesPerMesh);
					hardwareMesh.faceCount++;				  
				}
			}
			
			vertexCount+=hardwareMesh.vertexCount;
			faceIndexCount+=hardwareMesh.faceCount*3;
			hardwareMesh.pCoreMaterial= m_pCoreModel->getCoreMaterial(pCoreSubmesh->getCoreMaterialThreadId());
			
			m_vectorHardwareMesh.push_back(hardwareMesh);
			
		}
	}
	
    m_vectorVertexIndiceUsed.clear();


	m_totalFaceCount=0;
	m_totalVertexCount=0;

	int hardwareMeshId;	
	for(hardwareMeshId = 0; hardwareMeshId <  m_vectorHardwareMesh.size(); hardwareMeshId++)
	{
		m_totalFaceCount+=m_vectorHardwareMesh[hardwareMeshId].faceCount;
		m_totalVertexCount+=m_vectorHardwareMesh[hardwareMeshId].vertexCount;
	}

    
    return true;
}  



bool CalHardwareModel::canAddFace(CalHardwareMesh &hardwareMesh, CalCoreSubmesh::Face & face,std::vector<CalCoreSubmesh::Vertex>& vectorVertex, int maxBonesPerMesh)
{
	int boneCount=hardwareMesh.m_vectorBonesIndices.size();
	int faceIndex;
	
	for(faceIndex=0;faceIndex<3;faceIndex++)
	{
		unsigned int influenceIndex;
		for(influenceIndex=0;influenceIndex< vectorVertex[face.vertexId[faceIndex]].vectorInfluence.size();influenceIndex++)
		{
			int boneIndex=0;
			while(boneIndex< hardwareMesh.m_vectorBonesIndices.size() 
				&& hardwareMesh.m_vectorBonesIndices[boneIndex]!=vectorVertex[face.vertexId[faceIndex]].vectorInfluence[influenceIndex].boneId)
				boneIndex++;
			
			if(boneIndex==hardwareMesh.m_vectorBonesIndices.size())
				boneCount++;
		}
	}
	
	if(boneCount>maxBonesPerMesh)
		return false;
	
	return true;	
}

int CalHardwareModel::addVertex(CalHardwareMesh &hardwareMesh, int indice ,std::vector<CalCoreSubmesh::Vertex>& vectorVertex,  std::vector< std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorTexCoord, int maxBonesPerMesh)
{
	int i=0;
	
	while(i< hardwareMesh.vertexCount && m_vectorVertexIndiceUsed[i]!=indice)
		i++;
	
	if(i != hardwareMesh.vertexCount)
		return i;

	
	m_vectorVertexIndiceUsed[hardwareMesh.vertexCount]=indice;
	
	memcpy(&m_pVertexBuffer[(hardwareMesh.baseVertexIndex+i)*m_vertexStride],&vectorVertex[indice].position,sizeof(CalVector));

	memcpy(&m_pNormalBuffer[(hardwareMesh.baseVertexIndex+i)*m_normalStride],&vectorVertex[indice].normal,sizeof(CalVector));

	int mapId;	
	for(mapId = 0; mapId < m_textureCoordNum; mapId++)
	{
		if( vectorTexCoord.size() > mapId)
			memcpy(&m_pTextureCoordBuffer[mapId][(hardwareMesh.baseVertexIndex+i)*m_textureCoordStride[mapId]],&vectorTexCoord[mapId][indice],2*sizeof(float));
		else
			memset(&m_pTextureCoordBuffer[mapId][(hardwareMesh.baseVertexIndex+i)*m_textureCoordStride[mapId]],0,2*sizeof(float));
		
	}

	
	int l;
	for( l=0 ; l<4 ; l++)
	{
		if(l < vectorVertex[indice].vectorInfluence.size())
		{	
			int BoneId = vectorVertex[indice].vectorInfluence[l].boneId;

			float newBoneId = (float)addBoneIndice(hardwareMesh,BoneId,maxBonesPerMesh);
		
			memcpy(&m_pWeightBuffer[(hardwareMesh.baseVertexIndex+i)*m_weightStride+l * sizeof(float) ], &vectorVertex[indice].vectorInfluence[l].weight ,sizeof(float));			
			memcpy(&m_pMatrixIndexBuffer[(hardwareMesh.baseVertexIndex+i)*m_matrixIndexStride+l * sizeof(float) ], &newBoneId ,sizeof(float));			
		}
		else
		{
			memset(&m_pWeightBuffer[(hardwareMesh.baseVertexIndex+i)*m_weightStride+l * sizeof(float) ], 0 ,sizeof(float));			
			memset(&m_pMatrixIndexBuffer[(hardwareMesh.baseVertexIndex+i)*m_matrixIndexStride+l * sizeof(float) ], 0 ,sizeof(float));
		}
	}

    hardwareMesh.vertexCount++;
    return i;
}


int CalHardwareModel::addBoneIndice(CalHardwareMesh &hardwareMesh, int Indice, int maxBonesPerMesh)
{	
	int i=0;
	while(i< hardwareMesh.m_vectorBonesIndices.size()  && hardwareMesh.m_vectorBonesIndices[i]!=Indice)
		i++;

	if( i != hardwareMesh.m_vectorBonesIndices.size())
		return i;

	if(hardwareMesh.m_vectorBonesIndices.size()<maxBonesPerMesh)
	{
		hardwareMesh.m_vectorBonesIndices.push_back(Indice);
		return i;
	}
	else 
	{
		return -1;
	}
}


//****************************************************************************//

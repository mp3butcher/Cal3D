//----------------------------------------------------------------------------//
// MayaMesh.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#define MMESH_EPSILON 0.00001f
#define MMESH_HALFPI 1.5707963267948966192313216916398

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "StdAfx.h"
#include "MayaMesh.h"

#include "VertexCandidate.h"
#include "SkeletonCandidate.h"
#include "exporter.h"

#include "MayaInterface.h"
#include "BaseMaterial.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//
CMayaMesh::CMayaMesh()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//
CMayaMesh::~CMayaMesh()
{

}

//
// 
MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle )
{
	MIntArray   localIndex;
	unsigned    gv, gt;

	assert ( getTriangle.length() == 3 );    // Should always deal with a triangle

	for ( gt = 0; gt < getTriangle.length(); gt++ )
	{
		for ( gv = 0; gv < getVertices.length(); gv++ )
		{
			if ( getTriangle[gt] == getVertices[gv] )
			{
				localIndex.append( gv );
				break;
			}
		}

		// if nothing was added, add default "no match"
		if ( localIndex.length() == gt )
			localIndex.append( -1 );
	}

	return localIndex;
}

//----------------------------------------------------------------------------//
// Implementation                                                             //
//                                                                            //
// Based on: http://www.ewertb.com/maya/api/api_a18.html					  //
//----------------------------------------------------------------------------//
bool CMayaMesh::Create (const MDagPath &path, CMayaInterface *pInterface)
{
	MStatus stat;

	if (!path.hasFn (MFn::kMesh))
		return false;

	m_dagPath = path;
	m_pInterface = pInterface;

	MFnMesh			fnMesh (path, &stat);
	if (stat != MS::kSuccess)
		return false;
	
	// Get connected shaders
	if (fnMesh.getConnectedShaders (0, m_shaderArray, m_shaderIntArray) != MS::kSuccess)
		return false;

	// Get any attached skin cluster
	bool hasSkinCluster = false;

	//search the skin cluster affecting this geometry
	MItDependencyNodes kDepNodeIt( MFn::kSkinClusterFilter );

	// Go through each skin cluster in the scene until we find the one connected to this mesh.
	for( ;!kDepNodeIt.isDone() && !hasSkinCluster; kDepNodeIt.next())  
	{            
		MObject	kInputObject, kOutputObject;                    
		MObject kObject = kDepNodeIt.item();

		MFnSkinCluster kSkinClusterFn(kObject, &stat);
		if (stat != MS::kSuccess)
			continue;

		unsigned int uiNumGeometries = kSkinClusterFn.numOutputConnections();
        
		// go through each connection on the skin cluster until we get the one connecting to this mesh.
		for(unsigned int uiGeometry = 0; uiGeometry < uiNumGeometries && !hasSkinCluster; ++uiGeometry ) 
		{
			unsigned int uiIndex = kSkinClusterFn.indexForOutputConnection( uiGeometry, &stat );

			kInputObject = kSkinClusterFn.inputShapeAtIndex( uiIndex, &stat );
			kOutputObject = kSkinClusterFn.outputShapeAtIndex( uiIndex, &stat );

			if(kOutputObject == fnMesh.object()) 
			{    
				hasSkinCluster = true;

				kSkinClusterFn.influenceObjects (m_InfluenceObjects, NULL);

				// Go through each vertex and save the weights for each one
				MItGeometry kGeometryIt(kInputObject);
				for(int uiVertex = 0; !kGeometryIt.isDone(); kGeometryIt.next(), ++uiVertex ) 
				{
					MObject kComponent = kGeometryIt.component( 0 );

					MFloatArray kWeightArray;
					unsigned int uiNumInfluences;

					kSkinClusterFn.getWeights(m_dagPath, kComponent, kWeightArray, uiNumInfluences);
					m_VertexWeights.push_back(kWeightArray);
				}
			}
		}
	}

	// Go through and count the number of triangles
	MItMeshPolygon	itPoly (m_dagPath, MObject::kNullObj);
	int numFaces = 0;
	for ( ; !itPoly.isDone ();  itPoly.next ())
	{
		int triCount=0;
		itPoly.numTriangles (triCount);
		numFaces += triCount;
	}

	// Preallocate the array
	m_Faces.reserve (numFaces);

	//  Cache positions for each vertex
	MPointArray meshPoints;
	fnMesh.getPoints( meshPoints, MSpace::kObject );

	//  Cache normals for each vertex
	MFloatVectorArray  meshNormals;

	// Normals are per-vertex per-face..
	// use MItMeshPolygon::normalIndex() for index
	fnMesh.getNormals( meshNormals );

	// Get UVSets for this mesh
	MStringArray  UVSets;
	stat = fnMesh.getUVSetNames( UVSets );

	// Get all UVs for the first UV set.
	MFloatArray   u, v;
	fnMesh.getUVs( u, v, &UVSets[0] );

	// Go through each face again and build the face list
	itPoly.reset ();
	int polyFaceID = 0;
	for ( ; !itPoly.isDone ();  itPoly.next ())
	{
		// Get object-relative indices for the vertices in this face.
		MIntArray	polygonVertices;
		itPoly.getVertices( polygonVertices );

		int triCount=0;
		itPoly.numTriangles (triCount);

		// loop over each triangle in the poly
		for (int triIndex = 0; triIndex < triCount; triIndex++)
		{
			MPointArray                     nonTweaked;
			// object-relative vertex indices for each triangle
			MIntArray                       triangleVertices;
			// face-relative vertex indices for each triangle
			MIntArray                       localIndex;

			stat = itPoly.getTriangle( triIndex, nonTweaked, triangleVertices, MSpace::kWorld );
			if (stat == MS::kSuccess)
			{
				// Get face-relative vertex indices for this triangle
				localIndex = GetLocalIndex( polygonVertices, triangleVertices );

				// get the UVs for this triangle
				int uvID[3];

				// Get UV values for each vertex within this polygon
				for ( int vtxInPolygon = 0; vtxInPolygon < 3; vtxInPolygon++ )
					itPoly.getUVIndex( localIndex[vtxInPolygon], uvID[vtxInPolygon], &UVSets[0] );					

				int numTexMaps = GetMayaFaceMapCount (polyFaceID);

				// Build the triangle
				Triangle triangle;
				for (int tvi = 0; tvi < 3; tvi++)
				{
					// Get position and normal
					// we need to rotate around X axis by 90 degrees to put Z up
					triangle.v[tvi].pos = meshPoints[triangleVertices[tvi]];
					triangle.v[tvi].pos = triangle.v[tvi].pos.rotateBy (MVector::kXaxis, MMESH_HALFPI);

					triangle.v[tvi].normal = meshNormals[ itPoly.normalIndex(localIndex[tvi]) ];
					triangle.v[tvi].normal = triangle.v[tvi].normal.rotateBy (MVector::kXaxis, MMESH_HALFPI);

					// Zero out very small numbers
					if (fabs (triangle.v[tvi].normal.x) < MMESH_EPSILON) triangle.v[tvi].normal.x = 0;
					if (fabs (triangle.v[tvi].normal.y) < MMESH_EPSILON) triangle.v[tvi].normal.y = 0;
					if (fabs (triangle.v[tvi].normal.z) < MMESH_EPSILON) triangle.v[tvi].normal.z = 0;

					// UV
					for (int texMap = 0; texMap < numTexMaps; texMap++)
					{
						UV uv;
						uv.u = u[uvID[tvi]]; uv.v = 1.0f - v[uvID[tvi]];
						triangle.v[tvi].uvs.push_back(uv);
					}

					// Get the vertex weights
					for (unsigned int b = 0; b < m_InfluenceObjects.length (); b++)
					{
						MDagPath kJointPath = m_InfluenceObjects[b];

						int vIndex = triangleVertices[tvi];
						
						if (kJointPath.hasFn (MFn::kJoint) || vIndex >= (int)m_VertexWeights.size ())
						{
							// Get the name of this joint
							MFnDagNode kJoint (kJointPath, NULL);
							MString jointName = kJoint.name ();

							// Get the weights
							MFloatArray *pWeights = &m_VertexWeights[vIndex];
							
							// Insert a weight for each triangle vertex
							VertexWeight weightInfo;
							weightInfo.jointName = jointName;

							weightInfo.weight = (*pWeights)[b];
							if ( weightInfo.weight > MMESH_EPSILON )
								triangle.v[tvi].weights.push_back(weightInfo);
						}
						else
						{
							// Just insert a default weight. We'll pick up that we are attached
							// to a non joint later on.
							VertexWeight weightInfo;
							weightInfo.weight = 0;
							weightInfo.jointName = "";

							triangle.v[tvi].weights.push_back(weightInfo);							
						}
					}
				}

				triangle.faceID = polyFaceID;
				m_Faces.push_back(triangle);
			}
		}

		polyFaceID++;
	}	

	return true;
}

bool	CMayaMesh::GetShaderIndex (int index, MObject &obj)
{
	if (index < 0 || index >= GetMaterialCount())
		return false;

	obj = m_shaderArray[index];
	return true;
}

int CMayaMesh::GetFaceCount()
{
	return (int)m_Faces.size();	
}

int CMayaMesh::GetMaterialCount()
{
	return m_shaderArray.length();
}

int CMayaMesh::GetMayaFaceMaterialID (int faceID)
{
	int id = m_shaderIntArray[faceID];
	MObject shaderObj = m_shaderArray[id];

	return m_pInterface->GetMaterialIDFromShader (shaderObj);
}

int CMayaMesh::GetMayaFaceMapCount (int faceID)
{
	int materialID = GetMayaFaceMaterialID(faceID);
	if (materialID == -1)
		return 0;

	CBaseMaterial *pMaterial  = m_pInterface->GetMaterial (materialID);

	int mapCount = 0;
	if (pMaterial)
	{
		mapCount = pMaterial->GetMapCount ();
		delete pMaterial;		
	}

	return mapCount;
}

int CMayaMesh::GetFaceMaterialId(int faceId)
{
	Triangle &tri = m_Faces[faceId];

	return GetMayaFaceMaterialID (tri.faceID);
}

int CMayaMesh::GetSubmeshMapCount(int submeshId)
{
	// Get number of texture maps on the "submesh"
	int materialID = m_pInterface->GetMaterialIDFromShader(m_shaderArray[submeshId]);
	
	CBaseMaterial *pMaterial  = m_pInterface->GetMaterial (materialID);
	
	int mapCount = 0;
	if (pMaterial)
	{
		mapCount = pMaterial->GetMapCount ();
		delete pMaterial;		
	}

	return mapCount;
}

int CMayaMesh::GetSubmeshMaterialThreadId(int submeshId)
{
	// Get number of texture maps on the "submesh"
	int materialID = m_pInterface->GetMaterialIDFromShader(m_shaderArray[submeshId]);
	
	CBaseMaterial *pMaterial =  m_pInterface->GetMaterial (materialID);
	if (!pMaterial)
		return -1;

	// get name of the material
	std::string strName = pMaterial->GetName ();
	
	// get positions of the material thread id
	std::string::size_type closePos;
	closePos = strName.find_last_of("_");
	std::string::size_type openPos;
	openPos = strName.find_last_of("_", closePos-1);
	if((openPos == std::string::npos) || (closePos == std::string::npos) || (++openPos >= closePos))
	{
		theExporter.SetLastError("Invalid material thread id in material.", __FILE__, __LINE__);
		return -1;
	}

	// extract material thread id from material name
	std::string strMaterialThreadId;
	strMaterialThreadId = strName.substr(openPos, closePos - openPos);

	int materialThreadId;
	materialThreadId = atoi(strMaterialThreadId.c_str());

	return materialThreadId;
}

CVertexCandidate *CMayaMesh::GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId)
{
	MStatus stat;

	MFnMesh	fnMesh (m_dagPath, &stat);
	if (stat != MS::kSuccess)
		return 0;

	if( (faceId < 0) || (faceId >= (int)m_Faces.size()) )
	{
		theExporter.SetLastError("Invalid face id found.", __FILE__, __LINE__);
		return 0;
	}

	if((faceVertexId < 0) || (faceVertexId >= 3))
	{
		theExporter.SetLastError("Invalid face vertex id found.", __FILE__, __LINE__);
		return 0;
	}

	Triangle &face = m_Faces[faceId];
	Vertex &vertex = face.v[faceVertexId];

	CVertexCandidate *pVertexCandidate;
	pVertexCandidate = new CVertexCandidate();
	if(pVertexCandidate == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return 0;
	}

	//
	// Set position and normal
	pVertexCandidate->SetPosition ((float)vertex.pos.x, (float)vertex.pos.y, (float)vertex.pos.z);
	pVertexCandidate->SetNormal ((float)vertex.normal.x, (float)vertex.normal.y, (float)vertex.normal.z);

	//
	// Add the UVs
	for (std::vector<UV>::iterator itUV = vertex.uvs.begin(); 
		itUV != vertex.uvs.end();
		itUV++)
	{
		pVertexCandidate->AddTextureCoordinate (itUV->u, itUV->v);
	}

	//
	// TODO: get the vertex weight (if possible)
	float physicalWeight = 0.0f;	

	// set the vertex candidate weight
	pVertexCandidate->SetPhysicalProperty(physicalWeight);

	//
	// Add the bone weights
	for (std::vector<VertexWeight>::iterator itWeight = vertex.weights.begin();
		itWeight!= vertex.weights.end();
		itWeight++)
	{
		// Get the ID for this joint name
		int boneId = pSkeletonCandidate->GetBoneId(itWeight->jointName.asChar ());
		if (boneId == -1)
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Invalid bone assignment.", __FILE__, __LINE__);			
			return 0;
		}
        
		pVertexCandidate->AddInfluence(boneId, itWeight->weight);
	}
	

	return pVertexCandidate;
}

/*CVertexCandidate *CMayaMesh::GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId)
{
	MStatus stat;
	
	MFnMesh	fnMesh (m_dagPath, &stat);
	if (stat != MS::kSuccess)
		return 0;

	if((faceId < 0) || (faceId >= fnMesh.numPolygons()))
	{
		theExporter.SetLastError("Invalid face id found.", __FILE__, __LINE__);
		return 0;
	}

	MIntArray	faceVerts;

	if (fnMesh.getPolygonVertices (faceId, faceVerts) != MS::kSuccess)
		return 0;
	
	if (faceVerts.length() > 3)
	{
		theExporter.SetLastError("Mesh not triangulated.", __FILE__, __LINE__);
		return 0;
	}

	
	if((faceVertexId < 0) || (faceVertexId >= 3))
	{
		theExporter.SetLastError("Invalid face vertex id found.", __FILE__, __LINE__);
		return 0;
	}

	// create the new vertex candidate
	CVertexCandidate *pVertexCandidate;
	pVertexCandidate = new CVertexCandidate();
	if(pVertexCandidate == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return 0;

	}

	if(!pVertexCandidate->Create())
	{
		delete pVertexCandidate;
		return 0;
	}

	
	unsigned int vertexIndex = faceVerts[faceVertexId];

	//
	// Get the position of the vert
	MSpace::Space space = MSpace::kWorld;		// try to get everything in world space.

	MPoint	pt;
	stat = fnMesh.getPoint (vertexIndex, pt, space);
	if (stat == MS::kInvalidParameter)
	{
		space = MSpace::kObject;		// try in object space
		stat = fnMesh.getPoint (vertexIndex, pt, space);
	}

	if (stat != MS::kSuccess)
	{
		delete pVertexCandidate;
		return 0;
	}

	pVertexCandidate->SetPosition((float)pt.x, (float)pt.y, (float)pt.z);

	//
	// Get the face's vertex normal
	MVector n;
	if (fnMesh.getFaceVertexNormal (faceId, faceVertexId, n, space) != MS::kSuccess)
	{
		delete pVertexCandidate;
		return 0;
	}

	n.normalize ();
	pVertexCandidate->SetNormal((float)n.x, (float)n.y, (float)n.z);
	

	//
	// TODO: get the vertex weight (if possible)
	float physicalWeight = 0.0f;	

	// set the vertex candidate weight
	pVertexCandidate->SetPhysicalProperty(physicalWeight);


	//
	// Extract the UV maps attached to this face
	for (int i = 0; i < GetMaterialCount(); i++)
	{
		for (int k = 0; k < GetSubmeshMapCount (i); k++)
		{
			float u = 0, v = 0;
			if (fnMesh.getPolygonUV (faceId, faceVertexId, u, v, NULL) != MS::kSuccess)
			{
				delete pVertexCandidate;
				return 0;
			}

			pVertexCandidate->AddTextureCoordinate (u, 1.0f - v);
		}
	}


	//
	// Get the bones and weights this vertex is attached to
	
	
	// loop through each maya joint bound to this vertex
	for (unsigned int b = 0; b < m_InfluenceObjects.length (); b++)
	{
		MDagPath kJointPath = m_InfluenceObjects[b];
        
		if (!kJointPath.hasFn (MFn::kJoint))
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Vertex skinned to non joint object!.", __FILE__, __LINE__);			
			return 0;
		}

		MFnDagNode kJoint (kJointPath, &stat);
		if (stat != MS::kSuccess)
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Vertex skinned to non joint object!.", __FILE__, __LINE__);			
			return 0;
		}

		// get the weight for this bone on this vertex
		if (vertexIndex >= m_VertexWeights.size ())
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Vertex index not in weights array!.", __FILE__, __LINE__);			
			return 0;
		}

		MFloatArray *pWeightsArray = &m_VertexWeights[vertexIndex];
		if (!pWeightsArray)
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Vertex index not in weights array!.", __FILE__, __LINE__);			
			return 0;
		}

		float weight = (*pWeightsArray)[b];

		// Get the name of this joint
		MString jointName = kJoint.name ();

		// Get the ID for this joint name
		int boneId = pSkeletonCandidate->GetBoneId(jointName.asChar ());
		if (boneId == -1)
		{
			delete pVertexCandidate;
			theExporter.SetLastError("Invalid bone assignment.", __FILE__, __LINE__);			
			return 0;
		}

		// Assign the bone to this ID and weight
		pVertexCandidate->AddInfluence(boneId, weight);
	}

	return pVertexCandidate;
}*/
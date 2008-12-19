//
// Copyright (C) 2004 Mekensleep
//
// Mekensleep
// 24 rue vieille du temple
// 75004 Paris
//       licensing@mekensleep.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//----------------------------------------------------------------------------//
// MaxMeshExport.cpp                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "StdAfx.h"
#include "Exporter.h"
#include "BaseInterface.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseNode.h"
#include "SkeletonExportSheet.h"
#include "AnimationExportSheet.h"
#include "MaxMeshExport.h"
#include "MaxInterface.h"
#include "MeshExportSheet.h"
#include "MaterialExportSheet.h"
#include "MeshCandidate.h"
#include "SubmeshCandidate.h"
#include "VertexCandidate.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"
#include "BaseNode.h"
#include "BaseMesh.h"
#include "MaxNode.h"


bool CMaxInterface::ExportMeshFromMaxscriptCall(const std::string& strFilename, void * MeshExportParams)
{
	if (! MeshExportParams)return false;

	const MeshMaxscriptExportParams* _param = reinterpret_cast<const MeshMaxscriptExportParams*>(MeshExportParams);

	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// build a mesh candidate
	CMeshCandidate meshCandidate;

	// build a skeleton candidate
	CSkeletonCandidate skeletonCandidate;

	//Remove user interface
	/*// show export wizard sheet
	CMeshExportSheet sheet("Cal3D Mesh Export", GetMainWnd());
	sheet.SetSkeletonCandidate(&skeletonCandidate);
	sheet.SetMeshCandidate(&meshCandidate);
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;
	*/

	//This block replaces the user interface interactions with the parameters passed to the Maxscript function
	{
		// create the skeleton candidate from the skeleton file
		if(! skeletonCandidate.CreateFromSkeletonFile(_param->m_SkeletonFilename))
		{
			AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		// Create the mesh candidate with maxnumbonespervertex and weightthreshold from maxscript params
		//First, create a CMaxNode for the Mesh node
		CMaxNode* maxnode;
		maxnode = new CMaxNode;
		if (! maxnode->Create(_param->m_MeshNode)) return false;

		if(! meshCandidate.Create( maxnode, &skeletonCandidate, _param->m_MaxNumBonesPerVertex, _param->m_WeightThreshold))
		{
			AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		// check if LOD needs to be applied
		if(_param->m_LODCreation)
		{
			// create the LOD data
			if(! meshCandidate.CalculateLOD())
			{
				AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}
		else
		{
			// disable the LOD data
			if(! meshCandidate.DisableLOD())
			{
				AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}

		//Do we need to use the springs system ?
		if(_param->m_springsystem)
		{
			// create the spring system data
			if(! meshCandidate.CalculateSpringSystem())
			{
				AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}

	}

	// create the core mesh instance
	CalCoreMeshPtr coreMesh = new CalCoreMesh;

	// get the submesh candidate vector
	std::vector<CSubmeshCandidate *>const & vectorSubmeshCandidate = meshCandidate.GetVectorSubmeshCandidate();

	// start the progress info
	StartProgressInfo("Exporting to mesh file...");

	size_t submeshCandidateId;
	for(submeshCandidateId = 0; submeshCandidateId < vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		// update the progress info
		SetProgressInfo(int(100.0f * (float)submeshCandidateId / (float)vectorSubmeshCandidate.size()));

		// get the submesh candidate
		CSubmeshCandidate *pSubmeshCandidate;
		pSubmeshCandidate = vectorSubmeshCandidate[submeshCandidateId];

		// get the face vector
		std::vector<CSubmeshCandidate::Face>& vectorFace = pSubmeshCandidate->GetVectorFace();

		// check if the submesh actually contains faces
		if(vectorFace.size() > 0)
		{
			// allocate new core submesh instance
			CalCoreSubmesh *pCoreSubmesh;
			pCoreSubmesh = new CalCoreSubmesh();
			if(pCoreSubmesh == 0)
			{
				theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
				StopProgressInfo();
				return false;
			}

			// set the core material id
			pCoreSubmesh->setCoreMaterialThreadId(pSubmeshCandidate->GetMaterialThreadId());

			// get the vertex candidate vector
			std::vector<CVertexCandidate *>& vectorVertexCandidate = pSubmeshCandidate->GetVectorVertexCandidate();

			// get the spring vector
			std::vector<CSubmeshCandidate::Spring>& vectorSpring = pSubmeshCandidate->GetVectorSpring();

			// reserve memory for all the submesh data
			if(!pCoreSubmesh->reserve(vectorVertexCandidate.size(), pSubmeshCandidate->GetMapCount(), vectorFace.size(), vectorSpring.size()))
			{
				theExporter.SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreSubmesh;
				StopProgressInfo();
				return false;
			}

			size_t vertexCandidateId;
			for(vertexCandidateId = 0; vertexCandidateId < vectorVertexCandidate.size(); vertexCandidateId++)
			{
				// get the vertex candidate
				CVertexCandidate *pVertexCandidate;
				pVertexCandidate = vectorVertexCandidate[vertexCandidateId];

				CalCoreSubmesh::Vertex vertex;

				// set the vertex position
				CalVector position;
				pVertexCandidate->GetPosition(vertex.position);

				// set the vertex normal
				CalVector normal;
				pVertexCandidate->GetNormal(vertex.normal);

        // set the vert color
        pVertexCandidate->GetVertColor(vertex.vertexColor);

				// set the collapse id
				vertex.collapseId = pVertexCandidate->GetCollapseId();

				// set the face collapse count
				vertex.faceCollapseCount = pVertexCandidate->GetFaceCollapseCount();

				// get the texture coordinate vector
				std::vector<CVertexCandidate::TextureCoordinate>& vectorTextureCoordinate = pVertexCandidate->GetVectorTextureCoordinate();

				// set all texture coordinates
				size_t textureCoordinateId;
				for(textureCoordinateId = 0; textureCoordinateId < vectorTextureCoordinate.size(); textureCoordinateId++)
				{
					CalCoreSubmesh::TextureCoordinate textureCoordinate;
					textureCoordinate.u = vectorTextureCoordinate[textureCoordinateId].u;
					textureCoordinate.v = vectorTextureCoordinate[textureCoordinateId].v;

					// set texture coordinate
					pCoreSubmesh->setTextureCoordinate(pVertexCandidate->GetLodId(), textureCoordinateId, textureCoordinate);
				}

				// get the influence vector
				std::vector<CVertexCandidate::Influence>& vectorInfluence = pVertexCandidate->GetVectorInfluence();

				// reserve memory for the influences in the vertex
				vertex.vectorInfluence.reserve(vectorInfluence.size());
				vertex.vectorInfluence.resize(vectorInfluence.size());

				// set all influences
				size_t influenceId;
				for(influenceId = 0; influenceId < vectorInfluence.size(); influenceId++)
				{
					vertex.vectorInfluence[influenceId].boneId = vectorInfluence[influenceId].boneId;
					vertex.vectorInfluence[influenceId].weight = vectorInfluence[influenceId].weight;
				}

				// set vertex in the core submesh instance
				pCoreSubmesh->setVertex(pVertexCandidate->GetLodId(), vertex);

				// set the physical property if there is a spring system
				if(vectorSpring.size() > 0)
				{
					// get the physical property vector
					CVertexCandidate::PhysicalProperty physicalPropertyCandidate;
					pVertexCandidate->GetPhysicalProperty(physicalPropertyCandidate);

					CalCoreSubmesh::PhysicalProperty physicalProperty;
					physicalProperty.weight = physicalPropertyCandidate.weight;

					// set the physical property in the core submesh instance
					pCoreSubmesh->setPhysicalProperty(vertexCandidateId, physicalProperty);

				}
			}

			size_t faceId;
			for(faceId = 0; faceId < vectorFace.size(); faceId++)
			{
				CalCoreSubmesh::Face face;

				// set the vertex ids
				face.vertexId[0] = vectorFace[faceId].vertexLodId[0];
				face.vertexId[1] = vectorFace[faceId].vertexLodId[1];
				face.vertexId[2] = vectorFace[faceId].vertexLodId[2];

				// set face in the core submesh instance
				pCoreSubmesh->setFace(vectorFace[faceId].lodId, face);
			}

			size_t springId;
			for(springId = 0; springId < vectorSpring.size(); springId++)
			{
				CalCoreSubmesh::Spring spring;

				// set the vertex ids
				spring.vertexId[0] = vectorSpring[springId].vertexId[0];
				spring.vertexId[1] = vectorSpring[springId].vertexId[1];
				spring.springCoefficient = vectorSpring[springId].springCoefficient;
				spring.idleLength = vectorSpring[springId].idleLength;

				// set face in the core submesh instance
				pCoreSubmesh->setSpring(springId, spring);
			}

			// set the LOD step count
			pCoreSubmesh->setLodCount(pSubmeshCandidate->GetLodCount());

			// add the core submesh to the core mesh instance
			coreMesh->addCoreSubmesh(pCoreSubmesh);
		}
	}

	// stop the progress info
	StopProgressInfo();

	// save core mesh to the file
	if(!CalSaver::saveCoreMesh(strFilename, coreMesh.get()))
	{
		theExporter.SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		return false;
	}

	return true;
}


//----------------------------------------------------------------------------//
// MayaMesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//
#ifndef MAYA_MESH_H
#define MAYA_MESH_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "BaseMesh.h"

#include <maya/MDagPath.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFloatArray.h>
#include <maya/MPointArray.h>
#include <maya/MDagPathArray.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshPolygon.h>

#include <list>

class CMayaInterface;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//
class CMayaMesh : public CBaseMesh
{
  MDagPath        m_dagPath;
  CMayaInterface* m_pInterface;

  MObjectArray m_shaderArray;
  MIntArray    m_shaderIntArray;

  std::vector<MFloatArray> m_VertexWeights;
  MDagPathArray m_InfluenceObjects;

  struct UV
  {
    float u,v;
  };

  struct VertexWeight
  {
    MString jointName;
    float   weight;
  };

  struct Vertex 
  {
    MVector                   pos;
    MVector                   normal;
    std::vector<VertexWeight> weights;
    std::vector<UV>           uvs;
  };

  struct Triangle
  {
    Vertex v[3];
    int faceID; // which maya mesh face this triangle belongs to
  };

  std::vector<Triangle> m_Faces;

public:
  int GetMayaFaceMaterialID (int faceID);
  int GetMayaFaceMapCount (int faceID);

// member functions
public:
  bool  Create (const MDagPath &path, CMayaInterface *pInterface);
  bool  GetShaderIndex (int index, MObject &obj);
  
  int GetFaceCount();
  int GetMaterialCount();
  int GetFaceMaterialId(int faceId);
  int GetSubmeshMapCount(int submeshId);
  int GetSubmeshMaterialThreadId(int submeshId);
  CVertexCandidate *GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId);
};

#endif
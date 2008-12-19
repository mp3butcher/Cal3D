//----------------------------------------------------------------------------//
// MorphAnimationCandidate.cpp                                                          //
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
#include "MorphAnimationCandidate.h"
#include "BaseInterface.h"
#include "BaseMesh.h"
#include "BaseNode.h"
#include "VertexCandidate.h"

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

CMorphAnimationCandidate::CMorphAnimationCandidate()
{
	m_pNode = 0;
	m_pMesh = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMorphAnimationCandidate::~CMorphAnimationCandidate()
{
	Clear();
}



//----------------------------------------------------------------------------//
// Clear this mesh candidate                                                  //
//----------------------------------------------------------------------------//

void CMorphAnimationCandidate::Clear()
{
  delete m_pNode;
  m_pNode = 0;
  
  delete m_pMesh;
  m_pMesh = 0;
}

//----------------------------------------------------------------------------//
// Create a mesh candidate                                                    //
//----------------------------------------------------------------------------//

bool CMorphAnimationCandidate::Create()
{
  // clear current content
  Clear();
  return true;
}

CBaseMesh *
CMorphAnimationCandidate::meshAtTime(float time)
{
  int meshCount;
  meshCount = 0;
  
  // loop through all the selected nodes
  int nodeId;
  for(nodeId = 0; nodeId < theExporter.GetInterface()->GetSelectedNodeCount(); nodeId++)
  {
    // get the selected node
    CBaseNode *pNode;
    pNode = theExporter.GetInterface()->GetSelectedNode(nodeId);
    
    // check if it is a mesh node
    if(theExporter.GetInterface()->IsMesh(pNode))
    {
      if(meshCount == 0) m_pNode = pNode;
      meshCount++;
    }
    
    // delete all nodes except the one that should be exported
    if(pNode != m_pNode) delete pNode;
  }
  
  // check if one (and only one!) node/mesh is selected
  if(meshCount != 1)
  {
    theExporter.SetLastError("Select one (and only one) mesh to export!", __FILE__, __LINE__);
    return 0;
  }
  // check if the node is a mesh candidate
  if((m_pNode == 0) || !theExporter.GetInterface()->IsMesh(m_pNode))
  {
    theExporter.SetLastError("No mesh selected!", __FILE__, __LINE__);
    return 0;
  }

  // get the mesh
  m_pMesh = theExporter.GetInterface()->GetMesh(m_pNode, time);
  return m_pMesh;
}




//----------------------------------------------------------------------------//

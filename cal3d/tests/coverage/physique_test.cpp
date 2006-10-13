/*******************************************************
* physique_test.cpp
*
* Unittest for testing physique.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/corebone.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/mesh.h"
#include "cal3d/model.h"
#include "cal3d/physique.h"
#include "cal3d/skeleton.h"
#include "cal3d/vector.h"

#include "cal3d/submesh.h"


TEST( CalPhysique_calculateVertex )
{

  TEST_INIT( CalPhysique::calculateVertex );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  CalCoreSubmesh::PhysicalProperty physicalProperty;
  physicalProperty.weight = 1;

  m_pCoreSubmesh->setPhysicalProperty(0, physicalProperty);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  m_pCalPhysique->calculateVertex( m_pSubmesh , 0 );

  TEST_RES( calculateVertex failed );  
}

TEST( CalPhysique_calculateVertices )
{

  TEST_INIT( CalPhysique::calculateVertices );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  float vertexBuf[32];
  m_pCalPhysique->calculateVertices( m_pSubmesh , vertexBuf , 0 );

  TEST_RES( calculateVertices failed );  
}

TEST( CalPhysique_calculateVerticesAndNormals )
{

  TEST_INIT( CalPhysique::calculateVerticesAndNormals );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  float vertexBuf[32];
  m_pCalPhysique->calculateVerticesAndNormals( m_pSubmesh , vertexBuf , 0 );

  TEST_RES( calculateVerticesAndNormals failed );  
}

TEST( CalPhysique_calculateVerticesNormalsAndTexCoords )
{

  TEST_INIT( CalPhysique::calculateVerticesNormalsAndTexCoords );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  float vertexBuf[32];
  m_pCalPhysique->calculateVerticesNormalsAndTexCoords( m_pSubmesh , vertexBuf , 0 );

  TEST_RES( calculateVerticesNormalsAndTexCoords failed );  
}

TEST( CalPhysique_calculateTangentSpaces )
{

  TEST_INIT( CalPhysique::calculateVerticesNormalsAndTexCoords );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 1, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  CalCoreSubmesh::TextureCoordinate textureCoordinate;
  textureCoordinate.u = 0.0;
  textureCoordinate.v = 0.0;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);
  m_pCoreSubmesh->setTextureCoordinate(0, 0, textureCoordinate);

  m_pCoreSubmesh->setTangentSpace(0, 0, CalVector(), 0.5);
  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh); 

  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  float pTangentSpaceBuf[32];
  m_pCalPhysique->calculateTangentSpaces( m_pSubmesh, 0, pTangentSpaceBuf , 0 );

  TEST_RES( calculateTangentSpaces failed );  
}

TEST( CalPhysique_calculateNormals )
{

  TEST_INIT( CalPhysique::calculateNormals );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Influence influence;
  influence.boneId = 0;
  influence.weight = 0.5; 

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;
  vertex.vectorInfluence.reserve(1);
  vertex.vectorInfluence.resize(1);

  vertex.vectorInfluence[0] = influence;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  CalCoreSubmesh::PhysicalProperty physicalProperty;
  physicalProperty.weight = 1;

  m_pCoreSubmesh->setPhysicalProperty(0, physicalProperty);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalPhysique *m_pCalPhysique = new CalPhysique(m_pModel);

  CalSubmesh *m_pSubmesh = new CalSubmesh(m_pCoreSubmesh);

  float pNormalBuffer[32];
  m_pCalPhysique->calculateNormals( m_pSubmesh, pNormalBuffer , 0 );

  TEST_RES( calculateNormals failed );  
}

//****************************************************************************//
// springsystem.cpp                                                           //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
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

#include "error.h"
#include "springsystem.h"
#include "coremodel.h"
#include "model.h"
#include "mesh.h"
#include "submesh.h"
#include "coresubmesh.h"
#include "vector.h"

 /*****************************************************************************/
/** Constructs the spring system instance.
  *
  * This function is the default constructor of the spring system instance.
  *****************************************************************************/

CalSpringSystem::CalSpringSystem()
{
}

 /*****************************************************************************/
/** Destructs the spring system instance.
  *
  * This function is the destructor of the spring system instance.
  *****************************************************************************/

CalSpringSystem::~CalSpringSystem()
{
}

 /*****************************************************************************/
/** Calculates the forces on each unbound vertex.
  *
  * This function calculates the forces on each unbound vertex of a specific
  * submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the forces should be
  *                 calculated.
  * @param deltaTime The elapsed time in seconds since the last calculation.
  *****************************************************************************/

void CalSpringSystem::calculateForces(CalSubmesh *pSubmesh, float deltaTime)
{
  // get the spring vector of the core submesh
  std::vector<CalCoreSubmesh::Spring>& vectorSpring = pSubmesh->getCoreSubmesh()->getVectorSpring();

  // get the vertex vector of the submesh
  std::vector<CalVector>& vectorVertex = pSubmesh->getVectorVertex();

  // get the vertex vector of the submesh
  std::vector<CalSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getVectorPhysicalProperty();

  // get the physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorCorePhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // loop through all the springs
  std::vector<CalCoreSubmesh::Spring>::iterator iteratorSpring;
  for(iteratorSpring = vectorSpring.begin(); iteratorSpring != vectorSpring.end(); ++iteratorSpring)
  {
    // get the spring
    CalCoreSubmesh::Spring& spring = *iteratorSpring;

    // compute the difference between the two spring vertices
    CalVector distance;
    distance = vectorVertex[spring.vertexId[1]] - vectorVertex[spring.vertexId[0]];

    // get the current length of the spring and normalize the force vector
    float length;
    length = distance.normalize();

    // calculate the current amount of force of the spring
    float factor;
    factor = (length - spring.idleLength) * spring.springCoefficient;

    // scale the force vector by the calculated amount
    CalVector force;
    force = distance * factor;

    // compute relative speed for damping
    CalVector damping;
    damping = vectorPhysicalProperty[spring.vertexId[1]].velocity - vectorPhysicalProperty[spring.vertexId[0]].velocity;

    // calculate the damping force
    damping *= spring.springCoefficient * deltaTime;

    // apply the force and damping to both vertices
    vectorPhysicalProperty[spring.vertexId[0]].force += force;
    vectorPhysicalProperty[spring.vertexId[0]].force += damping;
    vectorPhysicalProperty[spring.vertexId[1]].force -= force;
    vectorPhysicalProperty[spring.vertexId[1]].force -= damping;

    // adjust the neighbour count in both vertices
    vectorPhysicalProperty[spring.vertexId[0]].neighbourCount++;
    vectorPhysicalProperty[spring.vertexId[1]].neighbourCount++;
  }

  // loop through all the springs
  for(iteratorSpring = vectorSpring.begin(); iteratorSpring != vectorSpring.end(); ++iteratorSpring)
  {
    // get the spring
    CalCoreSubmesh::Spring& spring = *iteratorSpring;

    float factor;

    factor = deltaTime / (vectorCorePhysicalProperty[spring.vertexId[1]].weight + deltaTime * deltaTime * spring.springCoefficient * vectorPhysicalProperty[spring.vertexId[1]].neighbourCount);
    vectorPhysicalProperty[spring.vertexId[0]].neighbourTerm += vectorPhysicalProperty[spring.vertexId[1]].force * factor;

    factor = deltaTime / (vectorCorePhysicalProperty[spring.vertexId[0]].weight + deltaTime * deltaTime * spring.springCoefficient * vectorPhysicalProperty[spring.vertexId[0]].neighbourCount);
    vectorPhysicalProperty[spring.vertexId[1]].neighbourTerm += vectorPhysicalProperty[spring.vertexId[0]].force * factor;
  }
}

 /*****************************************************************************/
/** Calculates the vertices influenced by the spring system instance.
  *
  * This function calculates the vertices influenced by the spring system
  * instance.
  *
  * @param pSubmesh A pointer to the submesh from which the vertices should be
  *                 calculated.
  * @param deltaTime The elapsed time in seconds since the last calculation.
  *****************************************************************************/

void CalSpringSystem::calculateVertices(CalSubmesh *pSubmesh, float deltaTime)
{
  // get the vertex vector of the submesh
  std::vector<CalVector>& vectorVertex = pSubmesh->getVectorVertex();

  // get the physical property vector of the submesh
  std::vector<CalSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getVectorPhysicalProperty();

  // get the physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorCorePhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // loop through all the vertices
  int vertexId;
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); vertexId++)
  {
    // get the vertex
    CalVector& vertex = vectorVertex[vertexId];

    // get the physical property of the vertex
    CalSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

    // get the physical property of the core vertex
    CalCoreSubmesh::PhysicalProperty& corePhysicalProperty = vectorCorePhysicalProperty[vertexId];

    // only take vertices with a weight > 0 into account
    if(corePhysicalProperty.weight > 0.0f)
    {
      CalVector deltaVelocity;
      deltaVelocity = deltaTime * physicalProperty.force + deltaTime * deltaTime * physicalProperty.neighbourTerm;

      float factor;
      factor = corePhysicalProperty.weight + deltaTime * deltaTime * 1000.0f * physicalProperty.neighbourCount;
      deltaVelocity /= factor;

      // TODO: external gravity setting
      const CalVector gravity(0.0f, 5.0f, -9.81f);

      // adjust the velocity of the vertex
      physicalProperty.velocity += deltaVelocity + gravity * deltaTime;

      // adjust the position of the vertex
      vertex += physicalProperty.velocity * deltaTime;
    }
    else
    {
      CalVector acceleration;
      acceleration = 2.0f  / (3.0f * deltaTime * deltaTime) * ((vertex - physicalProperty.position) - physicalProperty.velocity * deltaTime);

      // adjust the velocity of the vertex
      physicalProperty.velocity += acceleration * deltaTime;
      physicalProperty.velocity *= 0.99f;

      physicalProperty.force = acceleration * corePhysicalProperty.weight;
    }

    // store vertex position for possible use in next simulation step
    physicalProperty.position = vertex;

    // clear the force and neighbour term
    physicalProperty.force.clear();
    physicalProperty.neighbourTerm.clear();

    // clear the neighbour count
    physicalProperty.neighbourCount = 0;
  }

  // get the spring vector of the core submesh
  std::vector<CalCoreSubmesh::Spring>& vectorSpring = pSubmesh->getCoreSubmesh()->getVectorSpring();

  // loop through all the springs
  std::vector<CalCoreSubmesh::Spring>::iterator iteratorSpring;
  for(iteratorSpring = vectorSpring.begin(); iteratorSpring != vectorSpring.end(); ++iteratorSpring)
  {
    // get the spring
    CalCoreSubmesh::Spring& spring = *iteratorSpring;

    // compute the difference between the two spring vertices
    CalVector distance;
    distance = vectorVertex[spring.vertexId[1]] - vectorVertex[spring.vertexId[0]];

    // get the current length of the spring and normalize the force vector
    float length;
    length = distance.normalize();

const float LIMIT = 1.1f;

    if((length > LIMIT * spring.idleLength) || (length < (2.0f - LIMIT) * spring.idleLength))
    {
      if((vectorCorePhysicalProperty[spring.vertexId[0]].weight > 0.0f) && (vectorCorePhysicalProperty[spring.vertexId[1]].weight > 0.0f))
      {
        vectorVertex[spring.vertexId[0]] += 0.5f * (length - LIMIT * spring.idleLength) * distance;
        vectorVertex[spring.vertexId[1]] -= 0.5f * (length - LIMIT * spring.idleLength) * distance;
      }
      else if(vectorCorePhysicalProperty[spring.vertexId[0]].weight > 0.0f)
      {
        vectorVertex[spring.vertexId[0]] += (length - LIMIT * spring.idleLength) * distance;
      }
      else if(vectorCorePhysicalProperty[spring.vertexId[1]].weight > 0.0f)
      {
        vectorVertex[spring.vertexId[1]] -= (length - LIMIT * spring.idleLength) * distance;
      }
    }
  }
}

 /*****************************************************************************/
/** Creates the spring system instance.
  *
  * This function creates the spring system instance.
  *
  * @param pModel A pointer to the model that should be managed with this
  *               spring system instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSpringSystem::create(CalModel *pModel)
{
  if(pModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pModel = pModel;

  CalCoreModel *pCoreModel;
  pCoreModel = m_pModel->getCoreModel();
  if(pCoreModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the spring system instance.
  *
  * This function destroys all data stored in the spring system instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalSpringSystem::destroy()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Updates all the spring systems in the attached meshes.
  *
  * This functon updates all the spring systems in the attached meshes.
  *****************************************************************************/

void CalSpringSystem::update(float deltaTime)
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // loop through all the attached meshes
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = vectorMesh.begin(); iteratorMesh != vectorMesh.end(); ++iteratorMesh)
  {
    // get the ssubmesh vector of the mesh
    std::vector<CalSubmesh *>& vectorSubmesh = (*iteratorMesh)->getVectorSubmesh();

    // loop through all the submeshes of the mesh
    std::vector<CalSubmesh *>::iterator iteratorSubmesh;
    for(iteratorSubmesh = vectorSubmesh.begin(); iteratorSubmesh != vectorSubmesh.end(); ++iteratorSubmesh)
    {
      // check if the submesh contains a spring system
      if((*iteratorSubmesh)->getCoreSubmesh()->getSpringCount() > 0)
      {
int I = 10;
int i;
for(i = 0; i < I; i++)
{
        // calculate the new forces on each unbound vertex
        calculateForces(*iteratorSubmesh, deltaTime / (float)I);

        // calculate the vertices influenced by the spring system
        calculateVertices(*iteratorSubmesh, deltaTime / (float)I);
      }
}
    }
  }
}

//****************************************************************************//

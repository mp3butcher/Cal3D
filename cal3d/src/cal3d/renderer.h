//****************************************************************************//
// renderer.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_RENDERER_H
#define CAL_RENDERER_H


#include "cal3d/global.h"


class CalModel;
class CalSubmesh;


class CAL3D_API CalRenderer
{
public:
  CalRenderer(CalModel* pModel);
  CalRenderer(CalRenderer* pRenderer); 
  ~CalRenderer() { }

  bool beginRendering();
  void endRendering();
  void getAmbientColor(unsigned char *pColorBuffer);
  void getDiffuseColor(unsigned char *pColorBuffer);
  int getFaceCount() const;
  int getFaces(CalIndex *pFaceBuffer) const;
  int getMapCount() const;
  Cal::UserData getMapUserData(int mapId);
  int getMeshCount() const;
  int getNormals(float *pNormalBuffer, int stride=0);
  float getShininess() const;
  void getSpecularColor(unsigned char *pColorBuffer);
  int getSubmeshCount(int meshId) const;
  int getTextureCoordinates(int mapId, float *pTextureCoordinateBuffer, int stride=0);
  int getVertexCount() const;
  int getVertices(float *pVertexBuffer, int stride=0);
  int getTangentSpaces(int mapId, float *pTangentSpaceBuffer, int stride=0);
  int getVerticesAndNormals(float *pVertexBuffer, int stride=0);
  int getVerticesNormalsAndTexCoords(float *pVertexBuffer,int NumTexCoords=1);
  bool isTangentsEnabled(int mapId) const;
  bool selectMeshSubmesh(int meshId, int submeshId);
  void setNormalization(bool normalize);

private:
  CalModel *m_pModel;
  CalSubmesh *m_pSelectedSubmesh;
};

#endif

//****************************************************************************//

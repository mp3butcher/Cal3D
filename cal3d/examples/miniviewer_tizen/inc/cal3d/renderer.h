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
  CalRenderer(CalModel *pModel);
  CalRenderer(CalRenderer *pRenderer);
  ~CalRenderer() { }

  bool beginRendering();
  void endRendering();
  void getAmbientColor(unsigned char *pColorBuffer) const;
  void getDiffuseColor(unsigned char *pColorBuffer) const;
  int getFaceCount() const;
  int getFaces(CalIndex *pFaceBuffer) const;
  int getMapCount() const;
  Cal::UserData getMapUserData(int mapId);
  const Cal::UserData getMapUserData(int mapId) const;
  int getMeshCount() const;
  int getNormals(float *pNormalBuffer, int stride=0) const;
  float getShininess() const;
  void getSpecularColor(unsigned char *pColorBuffer) const;
  int getSubmeshCount(int meshId) const;
  int getTextureCoordinates(int mapId, float *pTextureCoordinateBuffer, int stride=0) const;
  int getVertexCount() const;
  int getVertices(float *pVertexBuffer, int stride=0) const;
  int getTangentSpaces(int mapId, float *pTangentSpaceBuffer, int stride=0) const;
  int getVertColors(float *pVertexBuffer);
  int getVertColorsAsStandardPixels( unsigned int *pVertexBuffer);
  int getVerticesAndNormals(float *pVertexBuffer, int stride=0) const;
  int getVerticesNormalsAndTexCoords(float *pVertexBuffer,int NumTexCoords=1) const;
  bool isTangentsEnabled(int mapId) const;
  bool selectMeshSubmesh(int meshId, int submeshId);
  void setNormalization(bool normalize);
  bool textureCoordinatesForMapValid( int mapId );
  bool hasNonWhiteVertexColors();

private:
  CalModel   *m_pModel;
  CalSubmesh *m_pSelectedSubmesh;
};

#endif

//****************************************************************************//

//----------------------------------------------------------------------------//
// viewer.cpp                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//            Adapted by Laurent Desmecht for D3D version                     //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef VIEWER_H
#define VIEWER_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class Viewer
{
// member variables
protected:
  int m_width;
  int m_height;
  bool m_bFullscreen;
  int m_mouseX;
  int m_mouseY;
  float m_tiltAngle;
  float m_twistAngle;
  float m_distance;
  bool m_bLeftMouseButtonDown;
  bool m_bRightMouseButtonDown;
  unsigned int m_lastTick;
  bool m_bPaused;
  bool m_bBump;
  bool m_bCanPS;
  CalCoreModel* m_calCoreModel;
  CalModel* m_calModel;
  float m_scale;
  int m_currentAnimationId;
  float m_leftAnimationTime;
  float m_blendTime;
  float m_lodLevel;
  int m_vertexCount;
  int m_faceCount;

  float m_fpsDuration;
  int m_fpsFrames;
  int m_fps;


// constructors/destructor
public:
	Viewer();
	virtual ~Viewer();
	
// member functions
public:
  std::string getCaption();
  bool getFullscreen();
  int getHeight();
  int getWidth();
  bool onCreate(LPSTR lpCmdLine);
  void onIdle();
  bool onInit();
  void onKey(unsigned char key, int x, int y);
  void onMouseButtonDown(int button, int x, int y);
  void onMouseButtonUp(int button, int x, int y);
  void onMouseMove(int x, int y);
  void onRender();
  void onShutdown();
  void setDimension(int width, int height);
  
protected:
  LPDIRECT3DTEXTURE9 loadTexture(const std::string& strFilename);
  bool parseModelConfiguration(const std::string& strFilename);
  void renderCursor();
  void renderModel();
/*
  // Create the normalization cube map
  void CreateNormalizationCubeMap();
  // Function used by D3DXFillCubeTexture to generate the normalization cube map
  static void WINAPI  genNormalizeCubeMap(D3DXVECTOR4* pOut, CONST D3DXVECTOR3* pTexCoord, CONST D3DXVECTOR3* pTexelSize, LPVOID pData);
*/

  bool loadVertexShader();
  bool loadVertexBuffer();
  void loadFont();


private:
	// D3D vertex & index buffers

	// Main vertex buffer : position, normal and first texture coords
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	int m_VBCursor;
	// index buffer
	LPDIRECT3DINDEXBUFFER9  m_pIB;
	int m_IBCursor;
	// vertex buffer : Tangent spaces
	LPDIRECT3DVERTEXBUFFER9 m_pTB;	
	int m_TBCursor;
	// vertex buffer : Second texture coords
	LPDIRECT3DVERTEXBUFFER9 m_pTCB;	
	int m_TCBCursor;

    // Vertex shader
    LPDIRECT3DVERTEXSHADER9 m_pVS;
	
    LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	// Nomalization cube map

	//LPDIRECT3DCUBETEXTURE9 m_pNormalizationCubeMap;

	LPD3DXFONT  m_pFont;
};

extern Viewer theViewer;

#endif

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// viewer.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
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
  float m_camheight;
  bool m_bLeftMouseButtonDown;
  bool m_bRightMouseButtonDown;
  unsigned int m_lastTick;
  bool m_bPaused;
  bool m_drawSkeleton;
  bool m_drawGrid;
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

  float m_timeScale;


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
  bool onCreate(int argc, char *argv[]);
  void onIdle();
  bool onInit();
  void onKey(unsigned char key, int x, int y);
  void onSpecial(int key, int x, int y);
  void onMouseButtonDown(int button, int x, int y);
  void onMouseButtonUp(int button, int x, int y);
  void onMouseMove(int x, int y);
  void onRender();
  void onShutdown();
  void setDimension(int width, int height);

protected:
  GLuint loadTexture(const std::string& strFilename);
  bool parseModelConfiguration(const std::string& strFilename);
  void renderCursor();
  void renderModel();
  void renderSkeleton();
  void drawGrid();
};

extern Viewer theViewer;

#endif

//----------------------------------------------------------------------------//

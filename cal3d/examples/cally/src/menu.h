//----------------------------------------------------------------------------//
// menu.h                                                                     //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MENU_H
#define MENU_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class Menu
{
// misc
protected:
  static const int MENUITEM_Y[5];
  static const int MENUITEM_HEIGHT[5];
  static const int MENUITEM_MOTION_X[3];
  static const int MENUITEM_MOTION_Y[3];

// member variables
protected:
  int m_menuX;
  int m_menuY;
  int m_lodX;
  int m_lodY;
  GLuint m_textureId;
  GLuint m_lodTextureId;
  bool m_bMotionMovement;
  bool m_bLodMovement;
  int m_bSkeleton;
  bool m_bWireframe;
  bool m_bLight;
  float m_actionTimespan[2];
  float m_nextTimespan;

// constructors/destructor
public:
  Menu();
  virtual ~Menu();

// member functions
public:
  int getMenuItem(int x, int y);
  bool isInside(int x, int y);
  bool isLight();
  int isSkeleton();
  bool isWireframe();
  bool onInit(int width, int height);
  bool onKey(unsigned char key, int x, int y);
  bool onMouseButtonDown(int button, int x, int y);
  bool onMouseButtonUp(int button, int x, int y);
  bool onMouseMove(int x, int y);
  void onRender();
  void onResize(int width, int height);
  void onShutdown();
  void onUpdate(float elapsedSeconds);

protected:
  void calculateLodLevel(int x, int y);
  void calculateMotionBlend(int x, int y);
};

extern Menu theMenu;

#endif

//----------------------------------------------------------------------------//

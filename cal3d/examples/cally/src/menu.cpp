//----------------------------------------------------------------------------//
// menu.cpp                                                                   //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "menu.h"
#include "demo.h"
#include "model.h"

//----------------------------------------------------------------------------//
// The one and only Menu instance                                             //
//----------------------------------------------------------------------------//

Menu theMenu;

//----------------------------------------------------------------------------//
// Static member variables initialization                                     //
//----------------------------------------------------------------------------//

const int Menu::MENUITEM_Y[5] = { 228, 200, 94, 66, 38 };
const int Menu::MENUITEM_HEIGHT[5] = { 28, 28, 106, 28, 28 };
const int Menu::MENUITEM_MOTION_X[3] = { 42, 80, 118 };
const int Menu::MENUITEM_MOTION_Y[3] = { 168, 102, 168 };

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Menu::Menu()
{
  m_bMotionMovement = false;
  m_menuX = 4;
  m_menuY = 4;
  m_bSkeleton = 0;
  m_bWireframe = false;
  m_bLight = true;
  m_actionTimespan[0] = 0.0f;
  m_actionTimespan[1] = 0.0f;
  m_nextTimespan = 0.0f;
  m_lodX = 4;
  m_lodY = 4;
  m_bLodMovement = false;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Menu::~Menu()
{
}

//----------------------------------------------------------------------------//
// Calculate new motion blend factors for a given position                    //
//----------------------------------------------------------------------------//

void Menu::calculateLodLevel(int x, int y)
{
  // convert to local coordinates
  x -= m_lodX;
  y -= m_lodY;

  // calculate the new lod level from the local coordinates
  float lodLevel;
  lodLevel = (float)(247 - x) / 200.0f;

  // clamp the value to [0.0, 1.0]
  if(lodLevel < 0.0) lodLevel = 0.0f;
  if(lodLevel > 1.0f) lodLevel = 1.0f;

  // set new motion blend factors
  theDemo.getModel()->setLodLevel(lodLevel);
}

//----------------------------------------------------------------------------//
// Calculate new motion blend factors for a given position                    //
//----------------------------------------------------------------------------//

void Menu::calculateMotionBlend(int x, int y)
{
  // convert to local coordinates
  x -= m_menuX;
  y -= m_menuY;

  // check if point is inside motion area
  if((y >= MENUITEM_Y[Model::STATE_MOTION]) && (y < MENUITEM_Y[Model::STATE_MOTION] + MENUITEM_HEIGHT[Model::STATE_MOTION]))
  {
    // calculate baryzentric coordinates inside motion triangle
    float motionBlend[3];
    motionBlend[0] = 1.0f - ((float)(x - MENUITEM_MOTION_X[0]) + (float)(MENUITEM_MOTION_Y[0] - y) / 1.732f) / 76.0f;

    // clamp first to range [0.0 - 1.0]
    if(motionBlend[0] < 0.0f) motionBlend[0] = 0.0f;
    if(motionBlend[0] > 1.0f) motionBlend[0] = 1.0f;

    motionBlend[1] = 1.0f - (float)(y - MENUITEM_MOTION_Y[1]) / 66.0f;

    // clamp second to range [0.0 - 1.0]
    if(motionBlend[1] < 0.0f) motionBlend[1] = 0.0f;
    if(motionBlend[1] > 1.0f) motionBlend[1] = 1.0f;

    // clamp sum of first and second to range [0.0 - 1.0]
    if(motionBlend[0] + motionBlend[1] > 1.0f)
    {
      float factor;
      factor = motionBlend[0] + motionBlend[1];
      motionBlend[0] /= factor;
      motionBlend[1] /= factor;
    }

    motionBlend[2] = 1.0f - motionBlend[0] - motionBlend[1];

    // clamp third to range [0.0 - 1.0]
    if(motionBlend[2] < 0.0f) motionBlend[2] = 0.0f;

    // set new motion blend factors
    theDemo.getModel()->setMotionBlend(motionBlend, 0.1f);
  }
}

//----------------------------------------------------------------------------//
// Get the menu item at a given position                                      //
//----------------------------------------------------------------------------//

int Menu::getMenuItem(int x, int y)
{
  // check if the point is inside the menu
  if(!isInside(x, y)) return -1;

  // check for the lod bar
  if((x - m_lodX >= 0) && (x - m_lodX < 256) && (y - m_lodY >= 0) && (y - m_lodY < 32))
  {
    return 9;
  }

  // check for each menu item
  int itemId;
  for(itemId = 0; itemId < 5; itemId++)
  {
    if((y - m_menuY >= MENUITEM_Y[itemId]) && (y - m_menuY < MENUITEM_Y[itemId] + MENUITEM_HEIGHT[itemId])) return itemId;
  }

  // test for flag menu items
  if((y - m_menuY >= 0) && (y - m_menuY < 35))
  {
    return 5 + (x - m_menuX) / 32;
  }

  return -1;
}

//----------------------------------------------------------------------------//
// Check if point is inside the menu                                          //
//----------------------------------------------------------------------------//

bool Menu::isInside(int x, int y)
{
  if((x - m_menuX >= 0) && (x - m_menuX < 128) && (y - m_menuY >= 0) && (y - m_menuY < 256)) return true;
  if((x - m_lodX >= 0) && (x - m_lodX < 256) && (y - m_lodY >= 0) && (y - m_lodY < 32)) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Get light flag                                                             //
//----------------------------------------------------------------------------//

bool Menu::isLight()
{
  return m_bLight;
}

//----------------------------------------------------------------------------//
// Get skeleton flag                                                          //
//----------------------------------------------------------------------------//

int Menu::isSkeleton()
{
  return m_bSkeleton;
}

//----------------------------------------------------------------------------//
// Get wirefrmae flag                                                         //
//----------------------------------------------------------------------------//

bool Menu::isWireframe()
{
  return m_bWireframe;
}

//----------------------------------------------------------------------------//
// Initialize the menu                                                        //
//----------------------------------------------------------------------------//

bool Menu::onInit(int width, int height)
{
  // load the menu texture
  std::string strFilename;
  strFilename = theDemo.getDatapath() + "menu.raw";

  if(!theDemo.loadTexture(strFilename, m_textureId)) return false;

  // load the lodxture
  strFilename = theDemo.getDatapath() + "lod.raw";

  if(!theDemo.loadTexture(strFilename, m_lodTextureId)) return false;

  return true;
}

//----------------------------------------------------------------------------//
// Handle a key event                                                         //
//----------------------------------------------------------------------------//

bool Menu::onKey(unsigned char key, int x, int y)
{
  return false;
}

//----------------------------------------------------------------------------//
// Handle a mouse button down event                                           //
//----------------------------------------------------------------------------//

bool Menu::onMouseButtonDown(int button, int x, int y)
{
  // get activated menu item
  int menuItem;
  menuItem = getMenuItem(x, y);

  // handle 'idle' button
  if(menuItem == Model::STATE_IDLE)
  {
    theDemo.getModel()->setState(Model::STATE_IDLE, 0.3f);
    return true;
  }

  // handle 'fancy' button
  if(menuItem == Model::STATE_FANCY)
  {
    theDemo.getModel()->setState(Model::STATE_FANCY, 0.3f);
    return true;
  }

  // handle 'motion' button/controller
  if(menuItem == Model::STATE_MOTION)
  {
    theDemo.getModel()->setState(Model::STATE_MOTION, 0.3f);
    calculateMotionBlend(x, y);
    m_bMotionMovement = true;
    return true;
  }

  // handle 'f/x 1' button
  if(menuItem == 3)
  {
    theDemo.getModel()->executeAction(0);
    m_actionTimespan[0] = 1.0f;
  }

  // handle 'f/x 2' button
  if(menuItem == 4)
  {
    theDemo.getModel()->executeAction(1);
    m_actionTimespan[1] = 1.0f;
  }

  // handle 'skeleton' button
  if(menuItem == 5)
  {
    m_bSkeleton = (m_bSkeleton + 1) % 3;
  }

  // handle 'wireframe' button
  if(menuItem == 6)
  {
    m_bWireframe = !m_bWireframe;
  }

  // handle 'light' button
  if(menuItem == 7)
  {
    m_bLight = !m_bLight;
  }

  // handle 'next model' button
  if(menuItem == 8)
  {
    theDemo.nextModel();
    m_nextTimespan = 0.3f;
  }

  // handle lod bar
  if(menuItem == 9)
  {
    calculateLodLevel(x, y);
    m_bLodMovement = true;
    return true;
  }

  return isInside(x, y);
}

//----------------------------------------------------------------------------//
// Handle a mouse button up event                                             //
//----------------------------------------------------------------------------//

bool Menu::onMouseButtonUp(int button, int x, int y)
{
  if(m_bMotionMovement)
  {
    m_bMotionMovement = false;
    return true;
  }

  if(m_bLodMovement)
  {
    m_bLodMovement = false;
    return true;
  }

  return false;
}

//----------------------------------------------------------------------------//
// Handle a mouse move event                                                  //
//----------------------------------------------------------------------------//

bool Menu::onMouseMove(int x, int y)
{
  if(m_bMotionMovement)
  {
    // update motion blend factors
    calculateMotionBlend(x, y);

    return true;
  }

  if(m_bLodMovement)
  {
    // update lod level
    calculateLodLevel(x, y);

    return true;
  }

  return false;
}

//----------------------------------------------------------------------------//
// Render the menu                                                            //
//----------------------------------------------------------------------------//

void Menu::onRender()
{
  // get the current animation state of the model
  int state;
  state = theDemo.getModel()->getState();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_textureId);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
    // render the base menu
    glTexCoord2f(0.5f, 1.0f);
    glVertex2i(m_menuX, m_menuY);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(m_menuX + 128, m_menuY);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(m_menuX + 128, m_menuY + 256);
    glTexCoord2f(0.5f, 0.0f);
    glVertex2i(m_menuX, m_menuY + 256);

    // render all active states
    int startY, endY;
    startY = MENUITEM_Y[state];
    endY = startY + MENUITEM_HEIGHT[state];

    glTexCoord2f(0.0f, 1.0f - (float)startY / 256.0f);
    glVertex2i(m_menuX, m_menuY + startY);
    glTexCoord2f(0.5f, 1.0f - (float)startY / 256.0f);
    glVertex2i(m_menuX + 128, m_menuY + startY);
    glTexCoord2f(0.5f, 1.0f - (float)endY / 256.0f);
    glVertex2i(m_menuX + 128, m_menuY + endY);
    glTexCoord2f(0.0f, 1.0f - (float)endY / 256.0f);
    glVertex2i(m_menuX, m_menuY + endY);

    if(m_actionTimespan[0] > 0.0f)
    {
      startY = MENUITEM_Y[3];
      endY = startY + MENUITEM_HEIGHT[3];

      glTexCoord2f(0.0f, 1.0f - (float)startY / 256.0f);
      glVertex2i(m_menuX, m_menuY + startY);
      glTexCoord2f(0.5f, 1.0f - (float)startY / 256.0f);
      glVertex2i(m_menuX + 128, m_menuY + startY);
      glTexCoord2f(0.5f, 1.0f - (float)endY / 256.0f);
      glVertex2i(m_menuX + 128, m_menuY + endY);
      glTexCoord2f(0.0f, 1.0f - (float)endY / 256.0f);
      glVertex2i(m_menuX, m_menuY + endY);
    }

    if(m_actionTimespan[1] > 0.0f)
    {
      startY = MENUITEM_Y[4];
      endY = startY + MENUITEM_HEIGHT[4];

      glTexCoord2f(0.0f, 1.0f - (float)startY / 256.0f);
      glVertex2i(m_menuX, m_menuY + startY);
      glTexCoord2f(0.5f, 1.0f - (float)startY / 256.0f);
      glVertex2i(m_menuX + 128, m_menuY + startY);
      glTexCoord2f(0.5f, 1.0f - (float)endY / 256.0f);
      glVertex2i(m_menuX + 128, m_menuY + endY);
      glTexCoord2f(0.0f, 1.0f - (float)endY / 256.0f);
      glVertex2i(m_menuX, m_menuY + endY);
    }

    if(m_bSkeleton!=0)
    {
      glTexCoord2f(0.0f, 1.0f);
      glVertex2i(m_menuX, m_menuY);
      glTexCoord2f(0.125f, 1.0f);
      glVertex2i(m_menuX + 32, m_menuY);
      glTexCoord2f(0.125f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 32, m_menuY + 35);
      glTexCoord2f(0.0f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX, m_menuY + 35);
    }

    if(m_bWireframe)
    {
      glTexCoord2f(0.125f, 1.0f);
      glVertex2i(m_menuX + 32, m_menuY);
      glTexCoord2f(0.25f, 1.0f);
      glVertex2i(m_menuX + 64, m_menuY);
      glTexCoord2f(0.25f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 64, m_menuY + 35);
      glTexCoord2f(0.125f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 32, m_menuY + 35);
    }

    if(m_bLight)
    {
      glTexCoord2f(0.25f, 1.0f);
      glVertex2i(m_menuX + 64, m_menuY);
      glTexCoord2f(0.375f, 1.0f);
      glVertex2i(m_menuX + 96, m_menuY);
      glTexCoord2f(0.375f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 96, m_menuY + 35);
      glTexCoord2f(0.25f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 64, m_menuY + 35);
    }

    if(m_nextTimespan > 0.0f)
    {
      glTexCoord2f(0.375f, 1.0f);
      glVertex2i(m_menuX + 96, m_menuY);
      glTexCoord2f(0.5f, 1.0f);
      glVertex2i(m_menuX + 128, m_menuY);
      glTexCoord2f(0.5f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 128, m_menuY + 35);
      glTexCoord2f(0.375f, 1.0f - 35.0f / 256.0f);
      glVertex2i(m_menuX + 96, m_menuY + 35);
    }

  glEnd();

  // get the current lod level of the model
  float lodLevel;
  lodLevel = theDemo.getModel()->getLodLevel();

  glBindTexture(GL_TEXTURE_2D, m_lodTextureId);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
    // render the base lod
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(m_lodX, m_lodY);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(m_lodX + 256, m_lodY);
    glTexCoord2f(1.0f, 0.5f);
    glVertex2i(m_lodX + 256, m_lodY + 32);
    glTexCoord2f(0.0f, 0.5f);
    glVertex2i(m_lodX, m_lodY + 32);

    // render the current lod level
    glTexCoord2f((247 - lodLevel * 200) / 256.0f, 0.5f);
    glVertex2i(m_lodX + 247 - (int)(lodLevel * 200), m_lodY);
    glTexCoord2f(1.0f, 0.5f);
    glVertex2i(m_lodX + 256, m_lodY);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(m_lodX + 256, m_lodY + 32);
    glTexCoord2f((247 - lodLevel * 200) / 256.0f, 0.0f);
    glVertex2i(m_lodX + 247 - (int)(lodLevel * 200), m_lodY + 32);

  glEnd();

  glDisable(GL_TEXTURE_2D);

  // render motion triangle
  if(state == Model::STATE_MOTION)
  {
    // get current blending factors
    float motionBlend[3];
    theDemo.getModel()->getMotionBlend(motionBlend);

    // calculate the current motion point
    int motionX, motionY;
    motionX = (int)(motionBlend[0] * MENUITEM_MOTION_X[0] + motionBlend[1] * MENUITEM_MOTION_X[1] + motionBlend[2] * MENUITEM_MOTION_X[2]);
    motionY = (int)(motionBlend[0] * MENUITEM_MOTION_Y[0] + motionBlend[1] * MENUITEM_MOTION_Y[1] + motionBlend[2] * MENUITEM_MOTION_Y[2]);

    glLineWidth(2.0f);

    glBegin(GL_LINES);
      glColor3f(1.0f, 1.0f, 0.0f);
      glVertex2i(m_menuX + MENUITEM_MOTION_X[0], m_menuY + MENUITEM_MOTION_Y[0]);
      glVertex2i(m_menuX + motionX, m_menuY + motionY);
      glColor3f(0.0f, 1.0f, 1.0f);
      glVertex2i(m_menuX + MENUITEM_MOTION_X[1], m_menuY + MENUITEM_MOTION_Y[1]);
      glVertex2i(m_menuX + motionX, m_menuY + motionY);
      glColor3f(1.0f, 0.0f, 1.0f);
      glVertex2i(m_menuX + MENUITEM_MOTION_X[2], m_menuY + MENUITEM_MOTION_Y[2]);
      glVertex2i(m_menuX + motionX, m_menuY + motionY);
    glEnd();

    glLineWidth(1.0f);
  }
}

//----------------------------------------------------------------------------//
// Handle window resize event                                                 //
//----------------------------------------------------------------------------//

void Menu::onResize(int width, int height)
{
  // adjust menu position
  m_menuX = width - 132;

  // adjust lod position
  m_lodX = width / 2 - 128;
}

//----------------------------------------------------------------------------//
// Shut the menu down                                                         //
//----------------------------------------------------------------------------//

void Menu::onShutdown()
{
}

//----------------------------------------------------------------------------//
// Update the menu                                                            //
//----------------------------------------------------------------------------//

void Menu::onUpdate(float elapsedSeconds)
{
  // calculate new timespan for f/x 1
  if(m_actionTimespan[0] > 0.0f)
  {
    m_actionTimespan[0] -= elapsedSeconds;
    if(m_actionTimespan[0] < 0.0f) m_actionTimespan[0] = 0.0f;
  }

  // calculate new timespan for f/x 2
  if(m_actionTimespan[1] > 0.0f)
  {
    m_actionTimespan[1] -= elapsedSeconds;
    if(m_actionTimespan[1] < 0.0f) m_actionTimespan[1] = 0.0f;
  }
  // calculate new timespan for 'next model'
  if(m_nextTimespan > 0.0f)
  {
    m_nextTimespan -= elapsedSeconds;
    if(m_nextTimespan < 0.0f) m_nextTimespan = 0.0f;
  }
}

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// main.cpp                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"
#include "viewer.h"

//----------------------------------------------------------------------------//
// GLUT callback functions                                                    //
//----------------------------------------------------------------------------//

void displayFunc()
{
  // render the scene
  theViewer.onRender();
}

void exitFunc()
{
  // shut the viewer instance down
  theViewer.onShutdown();
}

void idleFunc()
{
  // redirect to the viewer instance
  theViewer.onIdle();
}

void keyboardFunc(unsigned char key, int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.onKey(key, x, theViewer.getHeight() - y - 1);
}

void specialFunc(int key, int x, int y)
{
  theViewer.onSpecial(key, x, theViewer.getHeight() - y - 1);
}

void motionFunc(int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.onMouseMove(x, theViewer.getHeight() - y - 1);
}

void mouseFunc(int button, int state, int x, int y)
{
  // redirect the message to the viewer instance
  if(state == GLUT_DOWN)
  {
    theViewer.onMouseButtonDown(button, x, theViewer.getHeight() - y - 1);
  }
  else if(state == GLUT_UP)
  {
    theViewer.onMouseButtonUp(button, x, theViewer.getHeight() - y - 1);
  }
}

void reshapeFunc(int width, int height)
{
  // set the new width/height values
  theViewer.setDimension(width, height);
}

//----------------------------------------------------------------------------//
// Main entry point of the application                                        //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
  // initialize the GLUT system
  glutInit(&argc, argv);

  // create our view instance
  if(!theViewer.onCreate(argc, argv))
  {
    std::cerr << "Creation of the viewer failed." << std::endl;
    return -1;
  }

  // register our own exit callback
  atexit(exitFunc);

  // set all GLUT modes
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(theViewer.getWidth(), theViewer.getHeight());
  glutCreateWindow(theViewer.getCaption().c_str());
  if(theViewer.getFullscreen()) glutFullScreen();
  glutSetCursor(GLUT_CURSOR_NONE);

  // register all GLUT callback functions
  glutIdleFunc(idleFunc);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutPassiveMotionFunc(motionFunc);
  glutReshapeFunc(reshapeFunc);
  glutDisplayFunc(displayFunc);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(specialFunc);

  // initialize our viewer instance
  if(!theViewer.onInit())
  {
    std::cerr << "Initialization of the viewer failed." << std::endl;
    return -1;
  }

  // run the GLUT message loop
  glutMainLoop();

  return 0;
}

//----------------------------------------------------------------------------//

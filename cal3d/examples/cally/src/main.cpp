//----------------------------------------------------------------------------//
// main.cpp                                                                   //
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

#include "demo.h"

//----------------------------------------------------------------------------//
// GLUT callback functions                                                    //
//----------------------------------------------------------------------------//

void displayFunc()
{
  // render the scene
  theDemo.onRender();
}

void exitFunc()
{
  // shut the demo instance down
  theDemo.onShutdown();
}

void idleFunc()
{
  // redirect to the demo instance
  theDemo.onIdle();
}

void keyboardFunc(unsigned char key, int x, int y)
{
  // redirect the message to the demo instance
  theDemo.onKey(key, x, theDemo.getHeight() - y - 1);
}

void motionFunc(int x, int y)
{
  // redirect the message to the demo instance
  theDemo.onMouseMove(x, theDemo.getHeight() - y - 1);
}

void mouseFunc(int button, int state, int x, int y)
{
  // redirect the message to the demo instance
  if(state == GLUT_DOWN)
  {
    theDemo.onMouseButtonDown(button, x, theDemo.getHeight() - y - 1);
  }
  else if(state == GLUT_UP)
  {
    theDemo.onMouseButtonUp(button, x, theDemo.getHeight() - y - 1);
  }
}

void reshapeFunc(int width, int height)
{
  // set the new width/height values
  theDemo.setDimension(width, height);
}

//----------------------------------------------------------------------------//
// Main entry point of the application                                        //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
  // initialize the GLUT system
  glutInit(&argc, argv);

  // create our demo instance
  if(!theDemo.onCreate(argc, argv))
  {
    std::cerr << "Creation of the demo failed." << std::endl;
    return -1;
  }

  // register our own exit callback
  atexit(exitFunc);

  // set all GLUT modes
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(theDemo.getWidth(), theDemo.getHeight());
  glutCreateWindow(theDemo.getCaption().c_str());
  if(theDemo.getFullscreen()) glutFullScreen();
  glutSetCursor(GLUT_CURSOR_NONE);

  // register all GLUT callback functions
  glutIdleFunc(idleFunc);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutPassiveMotionFunc(motionFunc);
  glutReshapeFunc(reshapeFunc);
  glutDisplayFunc(displayFunc);
  glutKeyboardFunc(keyboardFunc);

  // initialize our demo instance
  if(!theDemo.onInit())
  {
    std::cerr << "Initialization of the demo failed." << std::endl;
    return -1;
  }

  // run the GLUT message loop
  glutMainLoop();

  return 0;
}

//----------------------------------------------------------------------------//

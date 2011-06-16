//----------------------------------------------------------------------------//
// viewer.cpp                                                                 //
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

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "viewer.h"
#include "tick.h"
#include "tga.h"

//----------------------------------------------------------------------------//
// The one and only Viewer instance                                           //
//----------------------------------------------------------------------------//

Viewer theViewer;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Viewer::Viewer()
{
  m_calCoreModel = new CalCoreModel("dummy");

  m_width = 640;
  m_height = 480;
  m_bFullscreen = false;
  m_mouseX = 0;
  m_mouseY = 0;
  m_tiltAngle = -70.0f;
  m_twistAngle = -45.0f;
  m_distance = 250.0f;
  m_camheight = 0.0f;
  m_bLeftMouseButtonDown = false;
  m_bRightMouseButtonDown = false;
  m_lastTick = 0;
  m_bPaused = false;
  m_drawGrid = false;
  m_drawSkeleton = false;
  m_scale = 1.0f;
  m_blendTime = 0.3f;
  m_lodLevel = 1.0f;
  m_vertexCount = 0;
  m_faceCount = 0;

  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;

  m_timeScale = 1;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Viewer::~Viewer()
{
}

//----------------------------------------------------------------------------//
// Get the viewer caption                                                     //
//----------------------------------------------------------------------------//

std::string Viewer::getCaption()
{
  return "- Cal3D MiniViewer -";
}

//----------------------------------------------------------------------------//
// Get the fullscreen flag                                                    //
//----------------------------------------------------------------------------//

bool Viewer::getFullscreen()
{
  return m_bFullscreen;
}

//----------------------------------------------------------------------------//
// Get the window height                                                      //
//----------------------------------------------------------------------------//

int Viewer::getHeight()
{
  return m_height;
}

//----------------------------------------------------------------------------//
// Get the window width                                                       //
//----------------------------------------------------------------------------//

int Viewer::getWidth()
{
  return m_width;
}

//----------------------------------------------------------------------------//
// Read a int from file stream (to avoid Little/Big endian issue)
//----------------------------------------------------------------------------//

int readInt( std::ifstream *file ) 
{
	int x = 0;
	for ( int i = 0; i < 32; i+=8 ) 
	{
		char c;
		file->read ( &c, 1 );
		x += c << i;
 	}
 	return x;
 }

//----------------------------------------------------------------------------//
// Load and create a texture from a given file                                //
//----------------------------------------------------------------------------//

GLuint Viewer::loadTexture(const std::string& strFilename)
{
  GLuint textureId=0;
  if(stricmp(strrchr(strFilename.c_str(),'.'),".raw")==0)
  {

     // open the texture file
     std::ifstream file;
     file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
     if(!file)
     {
       std::cerr << "Texture file '" << strFilename << "' not found." << std::endl;
       return 0;
     }

     // load the dimension of the texture
     int width = readInt(&file);     
     int height = readInt(&file);     
     int depth = readInt(&file);     

     // allocate a temporary buffer to load the texture to
     unsigned char *pBuffer;
     pBuffer = new unsigned char[2 * width * height * depth];
     if(pBuffer == 0)
     {
       std::cerr << "Memory allocation for texture '" << strFilename << "' failed." << std::endl;
       return 0;
     }

     // load the texture
     file.read((char *)pBuffer, width * height * depth);

     // explicitely close the file
     file.close();

     // flip texture around y-axis (-> opengl-style)
     int y;
     for(y = 0; y < height; y++)
     {
       memcpy(&pBuffer[(height + y) * width * depth], &pBuffer[(height - y - 1) * width * depth], width * depth);
     }
     
     // generate texture
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     glGenTextures(1, &textureId);
     glBindTexture(GL_TEXTURE_2D, textureId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     
	 glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, &pBuffer[width * height * depth]);
     // free the allocated memory

     delete [] pBuffer;  
  }
  else if(stricmp(strrchr(strFilename.c_str(),'.'),".tga")==0)  
  {
	  CTga *Tga;
	  Tga = new CTga();
	  if(Tga->ReadFile(strFilename.c_str())==0)
	  {
		  Tga->Release();
		  return 0;
	  }

	  if(Tga->Bpp()!=32)
	  {
		  Tga->Release();
		  return 0;
	  }

     //Flip texture
     int width = Tga->GetSizeX();
     int height = Tga->GetSizeY();
     //int depth = Tga->Bpp() / 8;    

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     glGenTextures(1, &textureId);

     glBindTexture(GL_TEXTURE_2D, textureId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA , GL_UNSIGNED_BYTE, (char*)Tga->GetPointer() );

	  Tga->Release();


  }


  return textureId;
}

//----------------------------------------------------------------------------//
// Create the viewer                                                          //
//----------------------------------------------------------------------------//

bool Viewer::onCreate(int argc, char *argv[])
{
  // show some information
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "|                    Cal3D MiniViewer 0.10.0                     |" << std::endl;
  std::cout << "|     (C) 2001, 2002, 2003 Bruno 'Beosil' Heidelberger           |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "| This program is free software; you can redistribute it and/or  |" << std::endl;
  std::cout << "| modify it under the terms of the GNU General Public License as |" << std::endl;
  std::cout << "| published by the Free Software Foundation; either version 2 of |" << std::endl;
  std::cout << "| the License, or (at your option) any later version.            |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << std::endl;

  bool bModelConfiguration;
  bModelConfiguration = false;

  // parse the command line arguments
  int arg;
  for(arg = 1; arg < argc; arg++)
  {
    // check for fullscreen flag
    if(strcmp(argv[arg], "--fullscreen") == 0) m_bFullscreen = true;
    // check for window flag
    else if(strcmp(argv[arg], "--window") == 0) m_bFullscreen = false;
    // check for dimension flag
    else if((strcmp(argv[arg], "--dimension") == 0) && (argc - arg > 2))
    {
      m_width = atoi(argv[++arg]);
      m_height = atoi(argv[++arg]);
      if((m_width <= 0) || (m_height <= 0))
      {
        std::cerr << "Invalid dimension!" << std::endl;
        return false;
      }
    }
    // check for help flag
    else if(strcmp(argv[arg], "--help") == 0)
    {
      std::cerr << "Usage: " << argv[0] << " [--fullscreen] [--window] [--dimension width height] [--help] model-configuration-file" << std::endl;
      return false;
    }
    // must be the model configuration file then
    else
    {
      // parse the model configuration file
      if(!parseModelConfiguration(argv[arg])) return false;

      // set model configuration flag
      bModelConfiguration = true;
    }
  }

  // check if we have successfully loaded a model configuration
  if(!bModelConfiguration)
  {
    std::cerr << "No model configuration file given." << std::endl;
    return false;
  }

  // make one material thread for each material
  // NOTE: this is not the right way to do it, but this viewer can't do the right
  // mapping without further information on the model etc., so this is the only
  // thing we can do here.
  int materialId;
  for(materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // create the a material thread
    m_calCoreModel->createCoreMaterialThread(materialId);

    // initialize the material thread
    m_calCoreModel->setCoreMaterialId(materialId, 0, materialId);
  }

  m_calModel = new CalModel(m_calCoreModel);

  return true;
}

//----------------------------------------------------------------------------//
// Handle an idle event                                                       //
//----------------------------------------------------------------------------//

void Viewer::onIdle()
{
  // get the current tick value
  unsigned int tick = Tick::getTick();

  // calculate the amount of elapsed seconds
  float elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

  // adjust fps counter
  m_fpsDuration += elapsedSeconds;
  if(m_fpsDuration >= 1.0f)
  {
    m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
	printf("%d\n",m_fps);
    m_fpsDuration = 0.0f;
    m_fpsFrames = 0;
  }

  elapsedSeconds *= m_timeScale;

  // update the model if not paused
  if(!m_bPaused)
  {
    // check if the time has come to blend to the next animation
    if(m_calCoreModel->getCoreAnimationCount() > 1)
    {
      m_leftAnimationTime -= elapsedSeconds;
      if(m_leftAnimationTime <= m_blendTime)
      {
        // get the next animation
        m_currentAnimationId = (m_currentAnimationId + 1) % m_calCoreModel->getCoreAnimationCount();

        // fade in the new animation
        m_calModel->getMixer()->executeAction(m_currentAnimationId, m_leftAnimationTime, m_blendTime);

        // adjust the animation time left until next animation flip
        m_leftAnimationTime = m_calCoreModel->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
      }
    }

    m_calModel->update(elapsedSeconds);
  }

  // current tick will be last tick next round
  m_lastTick = tick;

  // update the screen
  glutPostRedisplay();
}

//----------------------------------------------------------------------------//
// Initialize the demo                                                        //
//----------------------------------------------------------------------------//

bool Viewer::onInit()
{
  // load all textures and store the opengl texture id in the corresponding map in the material
  for(int materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // get the core material
    CalCoreMaterial *pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);

    // loop through all maps of the core material
    for(int mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      // get the filename of the texture
      std::string strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      GLuint textureId = loadTexture(strFilename);

      // store the opengl texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
    }
  }

  // attach all meshes to the model
  for(int meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
  {
    m_calModel->attachMesh(meshId);
  }

  // set the material set of the whole model
  m_calModel->setMaterialSet(0);

  // set initial animation state
  if(m_calCoreModel->getCoreAnimationCount() > 0)
  {
    m_currentAnimationId = 0;
    m_leftAnimationTime = m_calCoreModel->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
    if(m_calCoreModel->getCoreAnimationCount() > 1)
    {
      m_calModel->getMixer()->executeAction(m_currentAnimationId, 0.0f, m_blendTime);
    }
    else
    {
      m_calModel->getMixer()->blendCycle(m_currentAnimationId, 1.0f, 0.0f);
    }
  }
  else
  {
    m_currentAnimationId = -1;
    m_leftAnimationTime = -1.0f;
  }

  // we're done
  std::cout << "Initialization done." << std::endl;
  std::cout << std::endl;
  std::cout << "Quit the viewer by pressing 'q' or ESC" << std::endl;
  std::cout << std::endl;

  m_lastTick = Tick::getTick();

  return true;
}

//----------------------------------------------------------------------------//
// Handle a key event                                                         //
//----------------------------------------------------------------------------//

void Viewer::onKey(unsigned char key, int x, int y)
{
  switch(key)
  {
    // test for quit event
    case 27:
    case 'q':
    case 'Q':
      exit(0);
      break;
    case '-':
      m_lodLevel -= 0.002f;
      if(m_lodLevel < 0.0f) m_lodLevel = 0.0f;
      break;
    case '+':
      m_lodLevel += 0.002f;
      if(m_lodLevel > 1.0f) m_lodLevel = 1.0f;
      break;
    case 'g':
      m_drawGrid = !m_drawGrid;
      break;
    case 's':
      m_drawSkeleton = !m_drawSkeleton;
      break;
    // test for pause event
    case ' ':
      m_bPaused = !m_bPaused;
      break;
    case '*':
      m_timeScale *= 1.1f;
      break;
    case '/':
      m_timeScale /= 1.1f;
      break;
    // test for the lod keys
    default:
      if((key >= '0') && (key <= '9'))
      {
        m_lodLevel = (key == '0') ? 1.0f : (key - '0') * 0.1f;
      }
      break;
  }

  // set the (possible) new lod level
  m_calModel->setLodLevel(m_lodLevel);
}

//----------------------------------------------------------------------------//
// Handle special keys (F1, F2, UP, DOWN, etc.)                               //
//----------------------------------------------------------------------------//
void Viewer::onSpecial(int key, int x, int y)
{
  switch(key) 
  {
    case GLUT_KEY_UP:
      m_distance -= 3.0f;
      break;
    case GLUT_KEY_DOWN:
      m_distance += 3.0f;
      break;
    case GLUT_KEY_RIGHT:
      m_camheight += 3.0f;
      break;
    case GLUT_KEY_LEFT:
      m_camheight -= 3.0f;
      break;
    default:
      break;	
  }
}

//----------------------------------------------------------------------------//
// Handle a mouse button down event                                           //
//----------------------------------------------------------------------------//

void Viewer::onMouseButtonDown(int button, int x, int y)
{
  // update mouse button states
  if(button == GLUT_LEFT_BUTTON)
  {
    m_bLeftMouseButtonDown = true;
  }

  if(button == GLUT_RIGHT_BUTTON)
  {
    m_bRightMouseButtonDown = true;
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse button up event                                             //
//----------------------------------------------------------------------------//

void Viewer::onMouseButtonUp(int button, int x, int y)
{
  // update mouse button states
  if(button == GLUT_LEFT_BUTTON)
  {
    m_bLeftMouseButtonDown = false;
  }

  if(button == GLUT_RIGHT_BUTTON)
  {
    m_bRightMouseButtonDown = false;
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse move event                                                  //
//----------------------------------------------------------------------------//

void Viewer::onMouseMove(int x, int y)
{
  // update twist/tilt angles
  if(m_bLeftMouseButtonDown)
  {
    // calculate new angles
    m_twistAngle += (float)(x - m_mouseX);
    m_tiltAngle -= (float)(y - m_mouseY);
  }

  // update distance
  if(m_bRightMouseButtonDown)
  {
    // calculate new distance
    m_distance -= (float)(y - m_mouseY) / 3.0f;
    if(m_distance < 0.0f) m_distance = 0.0f;
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Render the current scene                                                   //
//----------------------------------------------------------------------------//

void Viewer::onRender()
{
  // clear the vertex and face counters
  m_vertexCount = 0;
  m_faceCount = 0;

  // clear all the buffers
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, 0.01, 10000);

  // set the model transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // light attributes
  const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
  const GLfloat light_diffuse[]  = { 0.52f, 0.5f, 0.5f, 1.0f };
  const GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

  // setup the light attributes
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  // set the light position
  GLfloat lightPosition[] = { 0.0f, -1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  // set camera position
  glTranslatef(0.0f, 0.0f, -m_distance * m_scale);
  glRotatef(m_tiltAngle, 1.0f, 0.0f, 0.0f);
  glRotatef(m_twistAngle, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f, 0.0f, -90.0f * m_scale);
  glTranslatef(0.0f, 0.0f, m_camheight);

  // render the model
  renderModel();

  // render the cursor
  renderCursor();

  // swap the front- and back-buffer
  glutSwapBuffers();

  // increase frame counter
  m_fpsFrames++;  

  //printf("%d\n",m_fps);
}

//----------------------------------------------------------------------------//
// Shut the viewer down                                                       //
//----------------------------------------------------------------------------//

void Viewer::onShutdown()
{
  delete m_calModel;
}

//----------------------------------------------------------------------------//
// Render the cursor                                                          //
//----------------------------------------------------------------------------//

void Viewer::renderCursor()
{
  // switch to orthogonal projection for the cursor
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // render the cursor
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_TRIANGLES);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2i(m_mouseX, m_mouseY);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glVertex2i(m_mouseX + 16, m_mouseY - 32);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glVertex2i(m_mouseX + 32, m_mouseY - 16);
  glEnd();

  glDisable(GL_BLEND);
}

//----------------------------------------------------------------------------//
// Parse the configuration file and load the whole model                      //
//----------------------------------------------------------------------------//

bool Viewer::parseModelConfiguration(const std::string& strFilename)
{
  // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
    return false;
  }

  // parse all lines from the model configuration file
  int line;
  for(line = 1; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof()) break;

    // check if an error happened while reading from the file
    if(!file)
    {
      std::cerr << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') continue;

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

    // handle the model creation
    if(strKey == "scale")
    {
      // set rendering scale factor
      m_scale = atof(strData.c_str());
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel->loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMesh(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMaterial(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      // everything else triggers an error message, but is ignored
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
    }
  }

  // explicitely close the file
  file.close();

  return true;
}

//----------------------------------------------------------------------------//
// Render the model                                                           //
//----------------------------------------------------------------------------//

void Viewer::renderModel()
{
  if (m_drawGrid)
    drawGrid();

  // get the renderer of the model
  CalRenderer *pCalRenderer = m_calModel->getRenderer();

  // begin the rendering loop
  if(pCalRenderer->beginRendering())
  {
    // set global OpenGL states
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // we will use vertex arrays, so enable them
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // get the number of meshes
    int meshCount = pCalRenderer->getMeshCount();

    // render all meshes of the model
    for(int meshId = 0; meshId < meshCount; meshId++)
    {
      // get the number of submeshes
      int submeshCount = pCalRenderer->getSubmeshCount(meshId);

      // render all submeshes of the mesh
      for(int submeshId = 0; submeshId < submeshCount; submeshId++)
      {
        // select mesh and submesh for further data access
        if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
        {
          unsigned char meshColor[4];
          GLfloat materialColor[4];

          // set the material ambient color
          pCalRenderer->getAmbientColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

          // set the material diffuse color
          pCalRenderer->getDiffuseColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

          // set the material specular color
          pCalRenderer->getSpecularColor(&meshColor[0]);
          materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

          // set the material shininess factor
          float shininess;
          shininess = 50.0f; //pCalRenderer->getShininess();
          glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

          // get the transformed vertices of the submesh
          static float meshVertices[30000][3];
          int vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

          // get the transformed normals of the submesh
          static float meshNormals[30000][3];
          pCalRenderer->getNormals(&meshNormals[0][0]);

          // get the texture coordinates of the submesh
          static float meshTextureCoordinates[30000][2];
          int textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

          // get the faces of the submesh
          static CalIndex meshFaces[50000][3];
          int faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

          // set the vertex and normal buffers
          glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
          glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

          // set the texture coordinate buffer and state if necessary
          if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_COLOR_MATERIAL);

            // set the texture id we stored in the map user data
            glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)pCalRenderer->getMapUserData(0));

            // set the texture coordinate buffer
            glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
            glColor3f(1.0f, 1.0f, 1.0f);
          }

          // draw the submesh
		  
          if(sizeof(CalIndex)==2)
            glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
          else
            glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);

          // disable the texture coordinate state if necessary
          if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          {
            glDisable(GL_COLOR_MATERIAL);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
          }

          // adjust the vertex and face counter
          m_vertexCount += vertexCount;
          m_faceCount += faceCount;

        }
      }
    }

    // clear vertex array state
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // clear light
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);

    // end the rendering
    pCalRenderer->endRendering();
  }

  if (m_drawSkeleton)
    renderSkeleton();
}

inline void glVertex(const CalVector& v) {
  glVertex3f(v.x, v.y, v.z);
}

void Viewer::renderSkeleton()
{
  // draw the bone lines
  float lines[1024][2][3];
  int nrLines = m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);

  glLineWidth(3.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  for(int currLine = 0; currLine < nrLines; currLine++)
  {
    glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
    glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
  }
  glEnd();
  glLineWidth(1.0f);

  // draw the bone points
  float points[1024][3];
  int nrPoints = m_calModel->getSkeleton()->getBonePoints(&points[0][0]);

  glPointSize(4.0f);
  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 1.0f);
  for(int currPoint = 0; currPoint < nrPoints; currPoint++)
  {
    glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
  }
  glEnd();
  glPointSize(1.0f);
}

void Viewer::drawGrid()
{
  glColor3f(0.3f, 0.3f, 0.3f);
  glBegin(GL_LINES);

  // Draw grid.
  for (int i = -100; i <= 100; i += 10) {
    glVertex2f(-100, i);
    glVertex2f( 100, i);

    glVertex2f(i, -100);
    glVertex2f(i,  100);
  }

  // Draw axis lines.
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(100, 0, 0);

  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 100, 0);

  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 100);

  glEnd();
}

//----------------------------------------------------------------------------//
// Set the dimension                                                          //
//----------------------------------------------------------------------------//

void Viewer::setDimension(int width, int height)
{
  // store new width and height values
  m_width = width;
  m_height = height;

  // set new viewport dimension
  glViewport(0, 0, m_width, m_height);
}

//----------------------------------------------------------------------------//

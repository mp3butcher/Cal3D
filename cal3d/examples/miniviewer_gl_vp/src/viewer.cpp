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



char vertexProgramStr[]= 
"!!ARBvp1.0\n"\
"PARAM constant = { 1, 3, 0, 0 };\n"\
"TEMP R0, R1, R2, R3, R4, R5;\n"\
"ADDRESS A0;\n"\
"ATTRIB texCoord = vertex.attrib[8];\n"\
"ATTRIB normal = vertex.attrib[2];\n"\
"ATTRIB index = vertex.attrib[3];\n"\
"ATTRIB weight = vertex.attrib[1];\n"\
"ATTRIB position = vertex.attrib[0];\n"\
"PARAM worldViewProjMatrix[4] = { state.matrix.mvp };\n"\
"PARAM diffuse = state.material.diffuse;\n"\
"PARAM ambient = state.material.ambient;\n"\
"PARAM lightDir = state.light[0].position;\n"\
"PARAM matrix[87] = { program.local[0..86] };\n"\
"\n"\
"MOV result.texcoord[0].xy, texCoord.xyxx;	\n"\
"\n"\
"MUL R4, index, constant.y;	\n"\
"\n"\
"ARL A0.x, R4.y;\n"\
"DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;\n"\
"DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;\n"\
"DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;\n"\
"MUL R1.yzw, R0.xxyz, weight.y;\n"\
"\n"\
"ARL A0.x, R4.x;\n"\
"DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;\n"\
"DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;\n"\
"DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;\n"\
"MAD R1.yzw, R0.xxyz, weight.x, R1.yyzw;\n"\
"\n"\
"DP3 R0.x, R1.yzwy, R1.yzwy;\n"\
"RSQ R0.x, R0.x;\n"\
"MUL R0.xyz, R0.x, R1.yzwy;\n"\
"DP3 R1.x, lightDir.xyzx, lightDir.xyzx;\n"\
"RSQ R1.x, R1.x;\n"\
"MUL R2.xyz, R1.x, lightDir.xyzx;\n"\
"DP3 R0.x, R0.xyzx, R2.xyzx;\n"\
"MAX R0.x, R0.x, constant.z;\n"\
"ADD R0, R0.x, ambient;\n"\
"MUL result.color.front.primary, R0, diffuse;\n"\
"\n"\
"ARL A0.x, R4.w;\n"\
"DPH R0.x, position.xyzx, matrix[A0.x];\n"\
"DPH R0.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R0.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"ARL A0.x, R4.z;\n"\
"DPH R3.x, position.xyzx, matrix[A0.x];\n"\
"DPH R3.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R3.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"ARL A0.x, R4.y;\n"\
"DPH R1.y, position.xyzx, matrix[A0.x];\n"\
"DPH R1.z, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R1.w, position.xyzx, matrix[A0.x + 2];\n"\
"MUL R2.xyz, R1.yzwy, weight.y;\n"\
"\n"\
"ARL A0.x, R4.x;\n"\
"DPH R1.x, position.xyzx, matrix[A0.x];\n"\
"DPH R1.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R1.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"MAD R1.xyz, R1.xyzx, weight.x, R2.xyzx;\n"\
"MAD R1.xyz, R3.xyzx, weight.z, R1.xyzx;\n"\
"MAD R0.xyz, R0.xyzx, weight.w, R1.xyzx;\n"\
"\n"\
"DPH result.position.x, R0.xyzx, worldViewProjMatrix[0];\n"\
"DPH result.position.y, R0.xyzx, worldViewProjMatrix[1];\n"\
"DPH result.position.z, R0.xyzx, worldViewProjMatrix[2];\n"\
"DPH result.position.w, R0.xyzx, worldViewProjMatrix[3];\n"\
"END\n";



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
  m_scale = 1.0f;
  m_blendTime = 0.3f;
  m_lodLevel = 1.0f;
  m_vertexCount = 0;
  m_faceCount = 0;
  m_vertexProgramId=0;

  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;
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
  std::cout << "|                    Cal3D MiniViewer 0.9                        |" << std::endl;
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
  unsigned int tick;
  tick = Tick::getTick();

  // calculate the amount of elapsed seconds
  float elapsedSeconds;
  elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

  // adjust fps counter
  m_fpsDuration += elapsedSeconds;
  if(m_fpsDuration >= 1.0f)
  {
    m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
	printf("%d\n",m_fps);
    m_fpsDuration = 0.0f;
    m_fpsFrames = 0;
  }

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
  int materialId;
  for(materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // get the core material
    CalCoreMaterial *pCoreMaterial;
    pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);

    // loop through all maps of the core material
    int mapId;
    for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      // get the filename of the texture
      std::string strFilename;
      strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      GLuint textureId;
      textureId = loadTexture(strFilename);

      // store the opengl texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
    }
  }

  // attach all meshes to the model
  int meshId;
  for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
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


  // Disable internal data
  // this disable spring system


  std::cout << "Disable internal." << std::endl;
  m_calModel->disableInternalData();

  m_lastTick = Tick::getTick();

  glewInit();

  if (!GLEW_ARB_vertex_program)
  {
      std::cerr << "Error ARB_vertex_program OpenGL extension not found." << std::endl;
	  return false;
  }

  if (!GLEW_ARB_vertex_buffer_object)
  {
      std::cerr << "Error ARB_vertex_buffer_object OpenGL extension not found." << std::endl;
	  return false;
  }


  if(!loadBufferObject())
  {
      std::cerr << "Error loading vertex buffer object." << std::endl;
	  return false;
  }


  if(!loadVertexProgram())
  {
      std::cerr << "Error loading vertex program." << std::endl;
	  return false;
  }
  


  // we're done
  std::cout << "Initialization done." << std::endl;
  std::cout << std::endl;
  std::cout << "Quit the viewer by pressing 'q' or ESC" << std::endl;
  std::cout << std::endl;

  

  return true;
}


bool Viewer::loadBufferObject()
{

  float *pVertexBuffer = (float*)malloc(30000*3*sizeof(float));
  float *pWeightBuffer = (float*)malloc(30000*4*sizeof(float));
  float *pMatrixIndexBuffer = (float*)malloc(30000*4*sizeof(float));
  float *pNormalBuffer = (float*)malloc(30000*3*sizeof(float));
  float *pTexCoordBuffer = (float*)malloc(30000*2*sizeof(float));
  CalIndex *pIndexBuffer = (CalIndex*)malloc(50000*3*sizeof(CalIndex));

  if(pVertexBuffer==NULL || pWeightBuffer == NULL
	 || pMatrixIndexBuffer==NULL || pNormalBuffer == NULL
	 || pTexCoordBuffer==NULL || pIndexBuffer == NULL)
  {
	  return false;
  }	  


  m_calHardwareModel = new CalHardwareModel(m_calCoreModel);
  m_calHardwareModel->setVertexBuffer((char*)pVertexBuffer,3*sizeof(float));
  m_calHardwareModel->setNormalBuffer((char*)pNormalBuffer,3*sizeof(float));
  m_calHardwareModel->setWeightBuffer((char*)pWeightBuffer,4*sizeof(float));
  m_calHardwareModel->setMatrixIndexBuffer((char*)pMatrixIndexBuffer,4*sizeof(float));
  m_calHardwareModel->setTextureCoordNum(1);
  m_calHardwareModel->setTextureCoordBuffer(0,(char*)pTexCoordBuffer,2*sizeof(float));
  m_calHardwareModel->setIndexBuffer(pIndexBuffer);

  m_calHardwareModel->load( 0, 0, MAXBONESPERMESH);



  // the index index in pIndexBuffer are relative to the begining of the hardware mesh,
  // we make them relative to the begining of the buffer.

  int meshId;
  for(meshId = 0; meshId < m_calHardwareModel->getHardwareMeshCount(); meshId++)
  {
	  m_calHardwareModel->selectHardwareMesh(meshId);

	  int faceId;
	  for(faceId = 0; faceId < m_calHardwareModel->getFaceCount(); faceId++)
	  {
		  pIndexBuffer[faceId*3+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
		  pIndexBuffer[faceId*3+1+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
		  pIndexBuffer[faceId*3+2+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
	  }

  }

  // We use ARB_vertex_buffer_object extension,
  // it provide better performance

  glGenBuffersARB(6, m_bufferObject);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[0]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*3*sizeof(float),(const void*)pVertexBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[1]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*4*sizeof(float),(const void*)pWeightBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[2]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*3*sizeof(float),(const void*)pNormalBuffer, GL_STATIC_DRAW_ARB);
  
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[3]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*4*sizeof(float),(const void*)pMatrixIndexBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[4]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*2*sizeof(float),(const void*)pTexCoordBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferObject[5]);

  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalFaceCount()*3*sizeof(CalIndex),(const void*)pIndexBuffer, GL_STATIC_DRAW_ARB);

  free(pVertexBuffer);
  free(pWeightBuffer);
  free(pNormalBuffer);
  free(pMatrixIndexBuffer);
  free(pIndexBuffer);

  return true;

}


bool Viewer::loadVertexProgram()
{
	glGenProgramsARB( 1, &m_vertexProgramId );
	
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vertexProgramId );
	
	glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		strlen(vertexProgramStr), vertexProgramStr );
	
	if ( GL_INVALID_OPERATION == glGetError() )
	{
		// Find the error position
		GLint errPos;
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB,
			&errPos );
		// Print implementation-dependent program
		// errors and warnings string.
		const unsigned char *errString = glGetString( GL_PROGRAM_ERROR_STRING_ARB);
		fprintf( stderr, "error at position: %d\n%s\n",
			errPos, errString );
		return false;
	}
	
	
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );

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
    // test for pause event
    case ' ':
      m_bPaused = !m_bPaused;
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
  gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, m_scale * 50.0, m_scale * 1000.0);

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
  delete m_calHardwareModel;
  delete m_calModel;
  delete m_calCoreModel;

  glDeleteProgramsARB(1, &m_vertexProgramId);
  glDeleteBuffersARB(6, m_bufferObject);

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
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vertexProgramId );

	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
    glEnableVertexAttribArrayARB(2);
	glEnableVertexAttribArrayARB(3);
    glEnableVertexAttribArrayARB(8);
	
	glEnable(GL_TEXTURE_2D);
	// set global OpenGL states
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);		
	glEnable(GL_VERTEX_PROGRAM_ARB);
	

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[0]);
	glVertexAttribPointerARB(0, 3 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[1]);
	glVertexAttribPointerARB(1, 4 , GL_FLOAT, false, 0, NULL);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[2]);
    glVertexAttribPointerARB(2, 3 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[3]);

	glVertexAttribPointerARB(3, 4 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[4]);
	glVertexAttribPointerARB(8, 2 , GL_FLOAT, false, 0, NULL);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferObject[5]);
	
		
	int hardwareMeshId;
	
	for(hardwareMeshId=0;hardwareMeshId<m_calHardwareModel->getHardwareMeshCount() ; hardwareMeshId++)
	{
		m_calHardwareModel->selectHardwareMesh(hardwareMeshId);

		unsigned char meshColor[4];	
		float materialColor[4];
		// set the material ambient color
		m_calHardwareModel->getAmbientColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
		
		// set the material diffuse color
		m_calHardwareModel->getDiffuseColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
		
		// set the material specular color
		m_calHardwareModel->getSpecularColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
		
		// set the material shininess factor
		float shininess;
		shininess = 50.0f; //m_calHardwareModel->getShininess();
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

		int boneId;
		for(boneId = 0; boneId < m_calHardwareModel->getBoneCount(); boneId++)
		{
			CalQuaternion rotationBoneSpace = m_calHardwareModel->getRotationBoneSpace(boneId, m_calModel->getSkeleton());
			CalVector translationBoneSpace = m_calHardwareModel->getTranslationBoneSpace(boneId, m_calModel->getSkeleton());

			CalMatrix rotationMatrix = rotationBoneSpace;

			float transformation[12];

			transformation[0]=rotationMatrix.dxdx;transformation[1]=rotationMatrix.dxdy;transformation[2]=rotationMatrix.dxdz;transformation[3]=translationBoneSpace.x;
			transformation[4]=rotationMatrix.dydx;transformation[5]=rotationMatrix.dydy;transformation[6]=rotationMatrix.dydz;transformation[7]=translationBoneSpace.y;
			transformation[8]=rotationMatrix.dzdx;transformation[9]=rotationMatrix.dzdy;transformation[10]=rotationMatrix.dzdz;transformation[11]=translationBoneSpace.z;

			
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3,&transformation[0]);
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3+1,&transformation[4]);
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3+2,&transformation[8]);			
			
            // set the texture id we stored in the map user data
            glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)m_calHardwareModel->getMapUserData(0));
		}

		if(sizeof(CalIndex)==2)
			glDrawElements(GL_TRIANGLES, m_calHardwareModel->getFaceCount() * 3, GL_UNSIGNED_SHORT, (((CalIndex *)NULL)+ m_calHardwareModel->getStartIndex()));
		else
			glDrawElements(GL_TRIANGLES, m_calHardwareModel->getFaceCount() * 3, GL_UNSIGNED_INT, (((CalIndex *)NULL)+ m_calHardwareModel->getStartIndex()));
		

	}

    // clear vertex array state    

	glDisableVertexAttribArrayARB(0);
	glDisableVertexAttribArrayARB(1);
    glDisableVertexAttribArrayARB(2);
	glDisableVertexAttribArrayARB(3);
    glDisableVertexAttribArrayARB(8);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    // clear light
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_VERTEX_PROGRAM_ARB);


	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );

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

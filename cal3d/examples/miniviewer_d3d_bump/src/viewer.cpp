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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "viewer.h"
#include "tick.h"

//----------------------------------------------------------------------------//
// The one and only Viewer instance                                           //
//----------------------------------------------------------------------------//

Viewer theViewer;

//----------------------------------------------------------------------------//
// The Direct3D Vertex                                                        //
//----------------------------------------------------------------------------//

struct VERTEX
{
    D3DXVECTOR3    pos;
    D3DXVECTOR3	normal;
    FLOAT tu,tv;
};

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


struct VERTEX2 //Use for the cursor
{
    D3DXVECTOR3    pos;
	DWORD	   d;
};

#define D3DFVF_VERTEX2 (D3DFVF_XYZ|D3DFVF_DIFFUSE)

// It isn't the best way but it work and the performance are ok
// In future version of Cal3D, more fonction will be add so
// We can use only one stream

D3DVERTEXELEMENT9 decl[] = 
  {  
	  { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	  { 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	  { 1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	  { 2, 0,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	  D3DDECL_END()
  };

/*

    The vertex shader are generate from this hlsl, you
	can compile to a vertex shader with the command:

	fxc /Tvs_1_1 VertexShader.fx /Fc vs.vsa /Zpr

	
struct appdata  {
	float4 Position: POSITION;
  	float4 Tangent: TANGENT;
  	float3 Normal: NORMAL;
    float2 TexCoord0:TEXCOORD0;    
    float2 TexCoord1:TEXCOORD1;
};

struct vpconn  {
	float4 HPosition: POSITION;
	float2 TexCoord0: TEXCOORD0;
	float2 TexCoord1: TEXCOORD1;
	float3 Color: COLOR0;
	float3 Specular: COLOR1;
};


vpconn main(appdata IN,
    uniform float4x4 matWorld: register (c0),	
	uniform float4x4 matWorldViewProj: register (c4),
	uniform float3 LightVec: register (c16),
	uniform float4 Diffuse: register (c9)
	)
{
	vpconn OUT;
	
	OUT.HPosition = mul(matWorldViewProj,IN.Position);
	
	float3x3 worldToTangentSpace;
	

      worldToTangentSpace[0] = mul(matWorld, IN.Tangent.xyz);
      worldToTangentSpace[2] = mul(matWorld, IN.Normal);

      worldToTangentSpace[1] = cross(worldToTangentSpace[0], worldToTangentSpace[2]) * IN.Tangent.w;
      
      float3 LightVecTangentSpace;
      LightVecTangentSpace = mul(worldToTangentSpace,LightVec);      
      
      OUT.TexCoord0=IN.TexCoord1;
      OUT.TexCoord1=IN.TexCoord0;   
      OUT.Specular = Diffuse;
	  OUT.Color = LightVecTangentSpace.xyz /2.0 + 0.5;
	  return OUT;
}

*/


char vertexShaderStr[] =
"    vs_1_1                                \n"\
"    def c3, 0.5, 0, 0, 0                  \n"\
"    dcl_position v0                       \n"\
"    dcl_tangent v1                        \n"\
"    dcl_normal v2                         \n"\
"    dcl_texcoord v3                       \n"\
"    dcl_texcoord1 v4                      \n"\
"    m4x4 oPos, v0, c4                     \n"\
"    m3x3 r0.xyz, v1, c0                   \n"\
"    m3x3 r2.xyz, v2, c0                   \n"\
"    mul r7.xyz, r0.zxyw, r2.yzxw          \n"\
"    mad r6.xyz, r0.yzxw, r2.zxyw, -r7     \n"\
"    mul r1.xyz, r6, v1.w                  \n"\
"    m3x3 r11.xyz, c16, r0                 \n"\
"    mad oD0.xyz, r11, c3.x, c3.x          \n"\
"    mov oT0.xy, v4                        \n"\
"    mov oT1.xy, v3                        \n"\
"    mov oD1.xyz, c9                       \n";


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
  m_distance = 270.0f;
  m_bLeftMouseButtonDown = false;
  m_bRightMouseButtonDown = false;
  m_lastTick = 0;
  m_bPaused = false;
  m_scale = 1.0f;
  m_blendTime = 0.3f;
  m_lodLevel = 1.0f;
  m_vertexCount = 0;
  m_faceCount = 0;
  m_pVB=NULL;
  m_pIB=NULL;
  m_pTB=NULL;
  m_pTCB=NULL; 
  m_pVS=NULL;
  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;
  m_bBump=true;
  m_bCanPS=false;
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
  return "- Cal3D MiniViewer D3D9 version -";
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
// Load and create a texture from a given file                                //
//----------------------------------------------------------------------------//

LPDIRECT3DTEXTURE9 Viewer::loadTexture(const std::string& strFilename)
{
	LPDIRECT3DTEXTURE9 pTex=NULL;
	
	
	if(strcmpi(strchr(strFilename.c_str(),'.'),".raw")==0)
	{
		std::ifstream file;

		file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

		if(!file)
		{
			std::cout << "Texture file '" << strFilename << "' not found." << std::endl;
			return NULL;
		}

		 // load the dimension of the texture
		int width;

		file.read((char *)&width, 4);
		
		int height;
		file.read((char *)&height, 4);
		int depth;

		file.read((char *)&depth, 4);

		// allocate a temporary buffer to load the texture to

		unsigned char *pBuffer;

		pBuffer = new unsigned char[2 * width * height * depth];

		if(pBuffer == 0)
		{
			std::cout << "Memory allocation for texture '" << strFilename << "' failed." << std::endl;
			return 0;
		}

		// load the texture
		
		file.read((char *)pBuffer, width * height * depth);
		// explicitely close the file

		file.close();

		// flip texture around y-axis (-> opengl/d3d-style)
		int y;

		for(y = 0; y < height; y++)
		{
			memcpy(&pBuffer[(height + y) * width * depth], &pBuffer[(height - y - 1) * width * depth], width * depth);
		}


		D3DXCreateTexture(g_pD3DDevice, width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex);
		
		D3DLOCKED_RECT Locked;

		pTex->LockRect(0, &Locked, NULL, 0);
		
		for (y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				DWORD* pBits = (DWORD*)((BYTE*)Locked.pBits + (y * Locked.Pitch));
				pBits += x;
				DWORD p;
				p=pBuffer[height * width * depth + (y*width+x)*depth] << 16;
				p+=pBuffer[height * width * depth + (y*width+x)*depth+1] << 8;
				p+=pBuffer[height * width * depth + (y*width+x)*depth+2] ;
				if(depth==4)
					p+=pBuffer[height * width * depth + (y*width+x)*depth+3]<<24;
				
				*pBits = p;
			}
		}

		pTex->UnlockRect(0);
		
		D3DXFilterTexture(pTex, NULL, 0, D3DX_FILTER_LINEAR);

		delete [] pBuffer;
	}	
	else 
		D3DXCreateTextureFromFile(g_pD3DDevice,strFilename.c_str(),&pTex);	
	
	
	g_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,     D3DTEXF_LINEAR  );
    g_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,     D3DTEXF_LINEAR  );
	g_pD3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,     D3DTEXF_LINEAR  );	

	return pTex;
}

//----------------------------------------------------------------------------//
// Create the viewer                                                          //
//----------------------------------------------------------------------------//


bool Viewer::onCreate(LPSTR lpCmdLine)
{
  // show some information
  
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "|           Cal3D D3D MiniViewer with bumpmapping 0.10.0         |" << std::endl;
  std::cout << "|           (C) 2001, 2002 Bruno 'Beosil' Heidelberger           |" << std::endl;
  std::cout << "|           (C) 2002, 2003  Laurent Desmecht                     |" << std::endl;
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

  //check for bumpmapping flag
  if(strstr(lpCmdLine, "--nobump") != NULL) g_bCanDot3 = false;
  
  // check for fullscreen flag
  
  if(strstr(lpCmdLine, "--fullscreen") != NULL) m_bFullscreen = true;
  // check for window flag
  if(strstr(lpCmdLine, "--window") != NULL) m_bFullscreen = false;
    // check for dimension flag
  if((strstr(lpCmdLine, "--dimension") != NULL))
  {
	sscanf(strstr(lpCmdLine,"--dimension"),"--dimension %d %d",&m_width,&m_height);
	
	if((m_width <= 0) || (m_height <= 0))
    {
      std::cout << "Invalid dimension!" << std::endl;
      return false;
    }
  }
    // check for help flag
  if(strstr(lpCmdLine, "--help") != NULL)
  {
    std::cout << "Usage: " << "cal3d_miniviewer_d3d.exe" << " [--fullscreen] [--window] [--dimension width height] [--help] model-configuration-file" << std::endl;
	MessageBox(g_hWnd,
		"cal3d_miniviewer_d3d.exe [--fullscreen] [--nobump] [--window] [--dimension width height] [--help] model-configuration-file [> output-file]",
		"Usage",MB_OK);    
    
	return false;
  }

  char *str;
  int i=0;
  int fileincmd=0;
  while(lpCmdLine[i]!=0)
  {
	  if(i==0)
	  {
		  if(lpCmdLine[i]!='-' && lpCmdLine[i]!=' ')
		  {
			  fileincmd=1;
			  str=&lpCmdLine[i];
		  }
	  }
	else
	  if(lpCmdLine[i]!='-' && lpCmdLine[i-1]==' ')
	  {
		  str=&lpCmdLine[i];
		  fileincmd=1;
	  }
	i++;
  }
  if(fileincmd)
  {
	  i=0;
	  while(str[i]!=0)
	  {
	  if(str[i]==' ')
		  str[i]=0;
	  i++;
	  }

	  // parse the model configuration file
	  if(!parseModelConfiguration(str)) return false;
      // set model configuration flag
      bModelConfiguration = true; 
  }
  

  // check if we have successfully loaded a model configuration
  if(!bModelConfiguration)
  {
	MessageBox(g_hWnd,
		"cal3d_miniviewer_d3d.exe [--fullscreen] [--nobump] [--window] [--dimension width height] [--help] model-configuration-file [> output-file]",
		"Usage",MB_OK);
    std::cout << "No model configuration file given." << std::endl;
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

}
/*
void  WINAPI Viewer::genNormalizeCubeMap(D3DXVECTOR4* pOut, CONST D3DXVECTOR3* pTexCoord, CONST D3DXVECTOR3* pTexelSize, LPVOID pData)
{
	D3DXVECTOR3 p = *pTexCoord;

	D3DXVec3Normalize(&p, &p);
    p /= 2;
    p += D3DXVECTOR3(.5, .5, .5);    
	*pOut = D3DXVECTOR4(p.x,p.y,p.z,1);
}

void Viewer::CreateNormalizationCubeMap()
{
    g_pD3DDevice->CreateCubeTexture( 128,
       0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pNormalizationCubeMap,NULL);
  
    D3DXFillCubeTexture(m_pNormalizationCubeMap,genNormalizeCubeMap,NULL);
}
*/

bool Viewer::loadVertexShader()
{
  g_pD3DDevice->CreateVertexDeclaration( decl, &m_pVertexDeclaration );

  // Load bumpmapping vertex shader

  LPD3DXBUFFER pCode;
  LPD3DXBUFFER pError;

  if(FAILED(D3DXAssembleShader(vertexShaderStr,sizeof(vertexShaderStr),NULL, NULL, 0,  
                          &pCode, &pError)))
  {
    	  std::cout << "Error while assembling default vertex shader:\n " << (char *)pError->GetBufferPointer();
	      pError->Release();
		  return false;	  
  }	    

  if(FAILED(g_pD3DDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(),
                                  &m_pVS)))
  {
      pCode->Release();
	  return false;
  }
  pCode->Release();
   
  return true;
}

bool Viewer::loadVertexBuffer()
{
	// Create vertex buffer
  if(FAILED(g_pD3DDevice->CreateVertexBuffer(30000*sizeof(VERTEX),
	  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_VERTEX, 
	  D3DPOOL_DEFAULT , &m_pVB, NULL
	  )))
	  return false;

  // Create vertex buffer for tangent spaces
  if(FAILED(g_pD3DDevice->CreateVertexBuffer(30000*sizeof(D3DXVECTOR4),
	  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 0, 
	  D3DPOOL_DEFAULT , &m_pTB, NULL
	  )))
	  return false;

  // Create vertex buffer for the second texture coord (normal map)
  if(FAILED(g_pD3DDevice->CreateVertexBuffer(30000*sizeof(D3DXVECTOR2),
	  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 0, 
	  D3DPOOL_DEFAULT , &m_pTCB, NULL
	  )))
	  return false;


  // Create index buffer
  if(sizeof(CalIndex)==2)
  {
  if(FAILED(g_pD3DDevice->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX16,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }
  else
  {
  if(FAILED(g_pD3DDevice->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }

  return true;
}

void Viewer::loadFont()
{
  // 
  //  There are changes here between Direct3D SDK 9 and Direct3D SDK 9 summer 2003 update
  //

  /* Original SDK

  LOGFONT LogFont = {24,0,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,"Arial"};

  D3DXCreateFontIndirect(g_pD3DDevice,&LogFont ,&m_pFont);

  */

  ///* Update
  
  D3DXCreateFont(g_pD3DDevice,24,0,0,0,false,DEFAULT_CHARSET,CLIP_DEFAULT_PRECIS,
	  PROOF_QUALITY,DEFAULT_PITCH, "Arial", &m_pFont);

  //*/
}


bool Viewer::onInit()
{
  // load all textures and store the D3D texture object id in the corresponding map in the material
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
      LPDIRECT3DTEXTURE9 texture;
      texture = loadTexture(strFilename);

      // store the d3d texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)texture);
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

  // Enable tangents

  for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
  {
    int submeshId;
    for(submeshId = 0; submeshId < m_calModel->getMesh(meshId)->getSubmeshCount() ; submeshId++)
    {
      CalSubmesh *Submesh = m_calModel->getMesh(meshId)->getSubmesh(submeshId);
      CalCoreMaterial *pCoreMaterial= m_calCoreModel->getCoreMaterial(Submesh->getCoreMaterialId());

      if(pCoreMaterial->getMapCount()==2)
      {
        if(strstr(pCoreMaterial->getMapFilename(1).c_str(),"bump")!=NULL)
        {
          Submesh->enableTangents(1,true);
        }
      }
    }
  }

  m_bBump=g_bCanDot3;

  
  // we're done
  std::cout << "Initialization done." << std::endl;
  std::cout << std::endl;
  std::cout << "Quit the viewer by pressing 'q' or ESC" << std::endl;
  std::cout << std::endl;

  m_lastTick = Tick::getTick();

  if(!loadVertexBuffer())
	  return false;

  if(!loadVertexShader())
	  return false;

  loadFont();

  // CreateNormalizationCubeMap();
     
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
    case 'b':
	case 'B':
		if(g_bCanDot3)
            m_bBump= !m_bBump;
		else m_bBump=false;
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
// Handle a mouse button down event                                           //
//----------------------------------------------------------------------------//

void Viewer::onMouseButtonDown(int button, int x, int y)
{
  // update mouse button states
  if(button == 0)
  {
    m_bLeftMouseButtonDown = true;
  }

  if(button == 1)
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
  if(button == 0)
  {
    m_bLeftMouseButtonDown = false;
  }

  if(button == 1)
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
  static char s[40];

  // clear the vertex and face counters
  m_vertexCount = 0;
  m_faceCount = 0;


  // clear all the buffers
  g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
						D3DCOLOR_XRGB(0,0,77), 1.0f, 0 );
  // Setup Prespective matrix
  g_pD3DDevice->BeginScene();

  D3DXMATRIX matProj,matWorld,matView, matViewInv;
  D3DXMatrixPerspectiveFovRH( &matProj, D3DX_PI/4, (float)m_width / (float)m_height, m_scale * 50.0f, m_scale * 1000.0f );
  
  
  g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
  
  D3DXMATRIX Mat1,Mat2;                // tmp matrix

  D3DXMatrixIdentity(&Mat1);

  

  D3DXMatrixTranslation(&Mat2,0.0f,0.0f,-m_distance * m_scale);
  
  D3DXMatrixMultiply(&Mat1,&Mat2,&Mat1);

  
  D3DXMatrixRotationX(&Mat2, m_tiltAngle/180.0f*3.14159f);
  D3DXMatrixMultiply(&Mat1,&Mat2,&Mat1);

  D3DXMatrixRotationZ(&Mat2, m_twistAngle/180.0f*3.14159f);
  D3DXMatrixMultiply(&Mat1,&Mat2,&Mat1);


  D3DXMatrixTranslation(&Mat2,0.0f,0.0f,-90.0f * m_scale);			//I have done à 1/1 translation
  D3DXMatrixMultiply(&matView,&Mat2,&Mat1);							//of the OpenGL version

  g_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

  D3DXMatrixIdentity(&matWorld);
  g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld ); 

  // setup the light attributes
  D3DLIGHT9 light;
  ZeroMemory( &light, sizeof(D3DLIGHT9) );
  light.Type       = D3DLIGHT_DIRECTIONAL;
  light.Ambient.r=0.2f;light.Ambient.g=0.2f;light.Ambient.b=0.2f;light.Ambient.a=1.0f;
  light.Diffuse.r=1.0f;light.Diffuse.g=1.0f;light.Diffuse.b=1.0f;light.Diffuse.a=1.0f;
  light.Specular.r=0.1f;light.Specular.g=0.1f;light.Specular.b=0.1f;light.Specular.a=1.0f;
    
  //light.Direction= D3DXVECTOR3(0.0f,0.70f,0.70f);
  light.Direction= D3DXVECTOR3(sinf(Tick::getTick()/1000.0f),cosf(Tick::getTick()/1000.0f),-0.5f);

  float r=light.Direction.x*light.Direction.x
	  +light.Direction.y*light.Direction.y
	  +light.Direction.z*light.Direction.z;

  light.Direction.x/=r;
  light.Direction.y/=r;
  light.Direction.z/=r;  

  g_pD3DDevice->SetLight( 0, &light );
  g_pD3DDevice->LightEnable( 0, TRUE );  
  g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

  if(m_bBump)
  {
      g_pD3DDevice->SetVertexShaderConstantF(13, (float*)&light.Diffuse, 1 );
      g_pD3DDevice->SetVertexShaderConstantF(14, (float*)&light.Specular, 1 );
      g_pD3DDevice->SetVertexShaderConstantF(15, (float*)&light.Ambient, 1 );
      g_pD3DDevice->SetVertexShaderConstantF(16, (float*)&(-((D3DXVECTOR3)light.Direction)), 1 );

	  D3DXMatrixTranspose( &Mat1, &matWorld );
      g_pD3DDevice->SetVertexShaderConstantF( 0, (float*)&Mat1, 4 );

      D3DXMatrixMultiply( &Mat1, &matView, &matProj );
      D3DXMatrixMultiply( &Mat1, &matWorld, &Mat1 );
      D3DXMatrixTranspose( &Mat1, &Mat1 );
      g_pD3DDevice->SetVertexShaderConstantF( 4, (float*)&Mat1, 4 );

	  float Const[4] = {0.5f,0.5f,0.5f,0.5f};

      g_pD3DDevice->SetVertexShaderConstantF( 20,Const, 1 );
  }
  
  // render the model
  renderModel();

  g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

  // render the cursor
  renderCursor();

  RECT FontPosition;

  FontPosition.top = 0;
  FontPosition.left = 0;
  FontPosition.right = m_width;
  FontPosition.bottom = 100;

  sprintf(s,"%d fps, press 'b' to toggle bump mapping",m_fps);

  // 
  //  There are changes here between Direct3D SDK 9 and Direct3D SDK 9 summer 2003 update
  //

  /* Original SDK

  m_pFont->Begin();										
  m_pFont->DrawText(s,-1,&FontPosition,DT_CENTER,0xffffffff);
  m_pFont->End();

  */


  ///* Update

  m_pFont->DrawText(NULL,s,-1,&FontPosition,DT_CENTER,0xffffffff);

  //*/


  g_pD3DDevice->EndScene();


  // swap the front- and back-buffer
  g_pD3DDevice->Present( NULL, NULL, NULL, NULL );  

  // increase frame counter
  m_fpsFrames++;

}


//----------------------------------------------------------------------------//
// Shut the viewer down                                                       //
//----------------------------------------------------------------------------//

void Viewer::onShutdown()
{
  LPDIRECT3DTEXTURE9 texture;      
  
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
		
      
      // get the d3d texture id in the user data of the map
      texture = (LPDIRECT3DTEXTURE9)pCoreMaterial->getMapUserData(mapId);
	  if(texture!=NULL)
		  texture->Release();
    }
  }


  // destroy
  delete m_calModel;
  delete m_calCoreModel;

  if(m_pVB!=NULL)
  	  m_pVB->Release();  
  if(m_pIB!=NULL)
		m_pIB->Release();
  if(m_pTB!=NULL)
		m_pTB->Release();
  if(m_pTCB!=NULL)
		m_pTCB->Release();
  if(m_pVS!=NULL)
		m_pVS->Release();
  if(m_pVertexDeclaration!=NULL)
		m_pVertexDeclaration->Release();

}

void Viewer::renderCursor()
{
	D3DXMATRIX mat;
	D3DXMatrixOrthoRH(&mat,(float)m_width,(float)m_height,-1.0f,1.0f);
    g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &mat );

	D3DXMatrixIdentity( &mat );
	g_pD3DDevice->SetTransform( D3DTS_VIEW, &mat);
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &mat);

	VERTEX2 Vertex[3];
	
	Vertex[0].pos=D3DXVECTOR3((float)(m_mouseX-m_width/2),(float)(m_mouseY-m_height/2),0.0f);
	Vertex[0].d=0xFFFFFFFF;

	Vertex[2].pos=D3DXVECTOR3((float)(m_mouseX+16-m_width/2),(float)( m_mouseY-32-m_height/2),0.0f);
	Vertex[2].d=0x20FFFFFF;

	Vertex[1].pos=D3DXVECTOR3((float)(m_mouseX+32-m_width/2),(float)( m_mouseY-16-m_height/2),0.0f);
	Vertex[1].d=0x20FFFFFF;

    g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetVertexDeclaration(NULL);
	g_pD3DDevice->SetFVF(D3DFVF_VERTEX2);

	g_pD3DDevice->SetTexture(0,NULL);
	g_pD3DDevice->SetTexture(1,NULL);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND ,D3DBLEND_SRCALPHA   );
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );
    g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST,1, (void*) Vertex,sizeof(VERTEX2));

	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);


  
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
    std::cout << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
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
      std::cout << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
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
      std::cout << strFilename << "(" << line << "): Invalid syntax." << std::endl;
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
      m_scale = (float) atof(strData.c_str());
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
      std::cout << strFilename << "(" << line << "): Invalid syntax." << std::endl;
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
  	
  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_calModel->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering()) return;

  m_VBCursor=0;
  m_IBCursor=0;
  m_TBCursor=0;
  m_TCBCursor=0;

  DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwTBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwTCBLockFlags=D3DLOCK_NOOVERWRITE;

  g_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) );
  g_pD3DDevice->SetStreamSource( 1, NULL , 0,  0 );
  g_pD3DDevice->SetStreamSource( 2, NULL , 0,  0 );

  g_pD3DDevice->SetFVF(D3DFVF_VERTEX);
  
      
  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  // render all meshes of the model
  int meshId;
  for(meshId = 0; meshId < meshCount; meshId++)
  {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // render all submeshes of the mesh
    int submeshId;
    for(submeshId = 0; submeshId < submeshCount; submeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {

		  D3DMATERIAL9 mat;
          unsigned char meshColor[4];
          pCalRenderer->getAmbientColor(&meshColor[0]);
          mat.Ambient.r=meshColor[0]/255.0f;mat.Ambient.g=meshColor[1]/255.0f;
          mat.Ambient.b=meshColor[2]/255.0f;mat.Ambient.a=meshColor[3]/255.0f;

          pCalRenderer->getDiffuseColor(&meshColor[0]);
          mat.Diffuse.r=meshColor[0]/255.0f;mat.Diffuse.g=meshColor[1]/255.0f;
          mat.Diffuse.b=meshColor[2]/255.0f;mat.Diffuse.a=meshColor[3]/255.0f;

          pCalRenderer->getSpecularColor(&meshColor[0]);
          mat.Specular.r=meshColor[0]/255.0f;mat.Specular.g=meshColor[1]/255.0f;
          mat.Specular.b=meshColor[2]/255.0f;mat.Specular.a=meshColor[3]/255.0f;

		  mat.Power=0.0f;
          mat.Emissive.r=0.0f;mat.Emissive.g=0.0f;mat.Emissive.b=0.0f;mat.Emissive.a=0.0f;
				
          g_pD3DDevice->SetMaterial(&mat);


		  if(m_VBCursor+ pCalRenderer->getVertexCount()>=30000)
		  {
		  	  m_VBCursor=0;
			  dwVBLockFlags=D3DLOCK_DISCARD;
		  }

		  if(m_TBCursor+ pCalRenderer->getVertexCount()>=30000)
		  {
		  	  m_TBCursor=0;
			  dwTBLockFlags=D3DLOCK_DISCARD;
		  }

		  if(m_TCBCursor+ pCalRenderer->getVertexCount()>=30000)
		  {
		  	  m_TCBCursor=0;
			  dwTCBLockFlags=D3DLOCK_DISCARD;
		  }

		  if(m_IBCursor + pCalRenderer->getFaceCount()>=50000)
		  {
		  	  m_IBCursor=0;
			  dwIBLockFlags=D3DLOCK_DISCARD;

		  }
		  

		  // Get vertexbuffer from the model		  		  
		  VERTEX *pVertices;

		  m_pVB->Lock(m_VBCursor*sizeof(VERTEX), pCalRenderer->getVertexCount()*sizeof(VERTEX), (void**)&pVertices, dwVBLockFlags);
		  int vertexCount = pCalRenderer->getVerticesNormalsAndTexCoords(&pVertices->pos.x);
		  m_pVB->Unlock();
		  
		  CalIndex *meshFaces;

		  m_pIB->Lock(m_IBCursor* 3*sizeof(CalIndex), pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&meshFaces,dwIBLockFlags);
		  int faceCount = pCalRenderer->getFaces(meshFaces);
		  m_pIB->Unlock();

		  
  		  if(pCalRenderer->getMapCount()==2 && m_bBump  && pCalRenderer->isTangentsEnabled(1))
		  {
  			  D3DXVECTOR4 *pTangents;
		      m_pTB->Lock(m_TBCursor*sizeof(D3DXVECTOR4), pCalRenderer->getVertexCount()*sizeof(D3DXVECTOR4), (void**)&pTangents, dwTBLockFlags);
	    	  pCalRenderer->getTangentSpaces(1,(float*)pTangents);
			  m_pTB->Unlock();

			  D3DXVECTOR2 *pTexCoords;
			  m_pTCB->Lock(m_TCBCursor*sizeof(D3DXVECTOR2), pCalRenderer->getVertexCount()*sizeof(D3DXVECTOR2), (void**)&pTexCoords, dwTCBLockFlags);
	    	  pCalRenderer->getTextureCoordinates(1,(float*)pTexCoords);
			  m_pTCB->Unlock();

			  // Rem:
			  // We need to swap the map, texture coord are also swapped in the vertex shader
              
  			  g_pD3DDevice->SetTexture(0,(LPDIRECT3DTEXTURE9)pCalRenderer->getMapUserData(1));
			  g_pD3DDevice->SetTexture(1,(LPDIRECT3DTEXTURE9)pCalRenderer->getMapUserData(0));

			  			  
			  g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 ,D3DTA_TEXTURE);
		      g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_DOTPRODUCT3);
		      g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2 ,D3DTA_DIFFUSE);
			  
			  g_pD3DDevice->SetTextureStageState(1,D3DTSS_COLORARG1 ,D3DTA_TEXTURE);
		      g_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_MODULATE);
		      g_pD3DDevice->SetTextureStageState(1,D3DTSS_COLORARG2 ,D3DTA_CURRENT);	

			  // Uncomment this if you want to use the diffuse color of
			  // The material, but I will not work on a geforce-class hardware

			  /*		      
			  g_pD3DDevice->SetTextureStageState(2,D3DTSS_COLORARG1 ,D3DTA_CURRENT);
    	      g_pD3DDevice->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_MODULATE);
			  g_pD3DDevice->SetTextureStageState(2,D3DTSS_COLORARG2 ,D3DTA_SPECULAR);
			  */

              g_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) );
	  		  g_pD3DDevice->SetStreamSource( 1, m_pTB,0, sizeof(D3DXVECTOR4) );
			  g_pD3DDevice->SetStreamSource( 2, m_pTCB,0, sizeof(D3DXVECTOR2) );

              g_pD3DDevice->SetVertexShader(m_pVS);
			  g_pD3DDevice->SetVertexDeclaration(m_pVertexDeclaration);
			  g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			
			  g_pD3DDevice->SetVertexShaderConstantF(9, (float*)&mat.Diffuse, 1 );
    	      g_pD3DDevice->SetVertexShaderConstantF(10, (float*)&mat.Specular, 1 );
	    	  g_pD3DDevice->SetVertexShaderConstantF(11, (float*)&mat.Ambient, 1 );
		  }
  		  
		  if(pCalRenderer->getMapCount()==1 || ( (!m_bBump || !pCalRenderer->isTangentsEnabled(1))  && pCalRenderer->getMapCount()==2))
		  {
  			  g_pD3DDevice->SetTexture(0,(LPDIRECT3DTEXTURE9)pCalRenderer->getMapUserData(0));
			  g_pD3DDevice->SetTexture(1,NULL);

			  g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 ,D3DTA_CURRENT);
		      g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
		      g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2 ,D3DTA_TEXTURE);			  
			  g_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);          

              g_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) );
			  g_pD3DDevice->SetStreamSource( 1, NULL,0, 0 );
			  g_pD3DDevice->SetStreamSource( 2, NULL,0, 0 );

			  g_pD3DDevice->SetVertexShader(NULL);
			  g_pD3DDevice->SetVertexDeclaration(NULL);
  			  g_pD3DDevice->SetFVF(D3DFVF_VERTEX);

  			  g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );


		  }
		  if(pCalRenderer->getMapCount()==0)
		  {
  			  g_pD3DDevice->SetTexture(0,NULL);			  
  			  g_pD3DDevice->SetTexture(1,NULL);

			  g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 ,D3DTA_CURRENT);
		      g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);		      
			  g_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);

  			  g_pD3DDevice->SetStreamSource( 1, NULL,0, 0 );
			  g_pD3DDevice->SetStreamSource( 2, NULL,0, 0 );

              g_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) );
			  g_pD3DDevice->SetVertexShader(NULL);
			  g_pD3DDevice->SetVertexDeclaration(NULL);
  			  g_pD3DDevice->SetFVF(D3DFVF_VERTEX);

  			  g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		  }

          g_pD3DDevice->SetIndices(m_pIB);
          g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
          
		  
		  g_pD3DDevice->DrawIndexedPrimitive(
			  D3DPT_TRIANGLELIST,
			  m_VBCursor,
			  0,
			  vertexCount,
			  m_IBCursor*3,
			  faceCount
			);


		  m_VBCursor+=vertexCount;
		  m_TBCursor+=vertexCount;
		  m_TCBCursor+=vertexCount;
		  m_IBCursor+=faceCount;		  

		  dwIBLockFlags=D3DLOCK_NOOVERWRITE;
		  dwTBLockFlags=D3DLOCK_NOOVERWRITE;
		  dwTCBLockFlags=D3DLOCK_NOOVERWRITE;
		  dwVBLockFlags=D3DLOCK_NOOVERWRITE;

      }
    }
  }
  // end the rendering
  pCalRenderer->endRendering();
  

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
//  glViewport(0, 0, m_width, m_height);
}

//----------------------------------------------------------------------------//








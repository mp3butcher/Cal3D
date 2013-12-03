//----------------------------------------------------------------------------//
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (c) 2012 Samsung Electronics Co., Ltd.                           //
// http://www.samsung.com                     								  //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "Cal3DRenderer.h"

const GLfloat ONEP = GLfloat(+1.0f);
const GLfloat ONEN = GLfloat(-1.0f);
const GLfloat ZERO = GLfloat( 0.0f);

void SetPerspective(GLfloat fovDegree, GLfloat aspect, GLfloat zNear,  GLfloat zFar)
{
	// tan(double(degree) * 3.1415962 / 180.0 / 2.0);
	static const float HALF_TAN_TABLE[91] =
	{
		0.00000f, 0.00873f, 0.01746f, 0.02619f, 0.03492f, 0.04366f, 0.05241f, 0.06116f, 0.06993f,
		0.07870f, 0.08749f, 0.09629f, 0.10510f, 0.11394f, 0.12278f, 0.13165f, 0.14054f, 0.14945f,
		0.15838f, 0.16734f, 0.17633f, 0.18534f, 0.19438f, 0.20345f, 0.21256f, 0.22169f, 0.23087f,
		0.24008f, 0.24933f, 0.25862f, 0.26795f, 0.27732f, 0.28675f, 0.29621f, 0.30573f, 0.31530f,
		0.32492f, 0.33460f, 0.34433f, 0.35412f, 0.36397f, 0.37389f, 0.38386f, 0.39391f, 0.40403f,
		0.41421f, 0.42448f, 0.43481f, 0.44523f, 0.45573f, 0.46631f, 0.47698f, 0.48773f, 0.49858f,
		0.50953f, 0.52057f, 0.53171f, 0.54296f, 0.55431f, 0.56577f, 0.57735f, 0.58905f, 0.60086f,
		0.61280f, 0.62487f, 0.63707f, 0.64941f, 0.66189f, 0.67451f, 0.68728f, 0.70021f, 0.71329f,
		0.72654f, 0.73996f, 0.75356f, 0.76733f, 0.78129f, 0.79544f, 0.80979f, 0.82434f, 0.83910f,
		0.85408f, 0.86929f, 0.88473f, 0.90041f, 0.91633f, 0.93252f, 0.94897f, 0.96569f, 0.98270f,
		1.00000f
	};

	int degree = int(fovDegree + 0.5f);

	degree = (degree >=  0) ? degree :  0;
	degree = (degree <= 90) ? degree : 90;

	GLfloat fxdYMax  = GLfloat(zNear * HALF_TAN_TABLE[degree]);
	GLfloat fxdYMin  = -fxdYMax;

	GLfloat fxdXMax  = GLfloat(GLfloat(fxdYMax) * aspect);
	GLfloat fxdXMin  = -fxdXMax;

	glFrustumf(fxdXMin, fxdXMax, fxdYMin, fxdYMax, GLfloat(zNear), GLfloat(zFar));
}

Cal3DRenderer::Cal3DRenderer(Cal3DLoader* cal3dLoader)
	: __controlWidth(0)
	, __controlHeight(0)
	, __angle(0)
	, __eglDisplay(EGL_DEFAULT_DISPLAY)
	, __eglSurface(EGL_NO_SURFACE)
	, __eglConfig(null)
	, __eglContext(EGL_NO_CONTEXT)
	, __pbuffer_surface(EGL_NO_SURFACE)
	, __pixmap_surface(EGL_NO_SURFACE)

	, __tiltAngle(-70.0f)
	, __twistAngle(-45.0f)
	, __distance(270.0f)
	, __lastTick(0)
	, __fpsDuration(0.0f)
	, __fpsFrames(0)
	, __fps(0)
	, __timeScale(1.0f)

	, __vertexCount(0)
	, __faceCount(0)

	, __cal3dLoader(cal3dLoader)
{
}

Cal3DRenderer::~Cal3DRenderer(void)
{

}

bool
Cal3DRenderer::InitializeGl(void)
{
	// Initialize GL status. 

	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, GetTargetControlWidth(), GetTargetControlHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	SetPerspective(60.0f, 1.0f * GetTargetControlWidth() / GetTargetControlHeight(), 1.0f, 400.0f);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

bool
Cal3DRenderer::TerminateGl(void)
{
	return true;
}

bool
Cal3DRenderer::Draw(void)
{
	{
		__cal3dLoader->UpdateModel(UpdateTick());
		RenderCal3d();

		glFlush();
		glFinish();
	}

	if (__pixmap_surface == EGL_NO_SURFACE)
	{
		eglSwapBuffers(__eglDisplay, __eglSurface);
	}
	else
	{
		eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext);
	}

#if defined(DISPLAY_FPS)
	static float     fps = 0.0f;
	static float     updateInterval = 1000.0f;
	static float     timeSinceLastUpdate = 0.0f;
	static float     frameCount = 0;
	static long long currentTick;
	static long long lastTick;
	static bool      isFirst = true;

	if (isFirst)
	{
		SystemTime::GetTicks(currentTick);
		lastTick = currentTick;
		isFirst = false;
	}

	frameCount++;
	SystemTime::GetTicks(currentTick);

	float elapsed = currentTick - lastTick;

	lastTick = currentTick;
	timeSinceLastUpdate += elapsed;

	if (timeSinceLastUpdate > updateInterval)
	{
		if (timeSinceLastUpdate)
		{
			fps = (frameCount / timeSinceLastUpdate) * 1000.f;
			AppLog("[Cal3DViewer] FPS: %f frames/sec\n", fps);

			frameCount = 0;
			timeSinceLastUpdate -= updateInterval;
		}
	}
#endif

	return true;
}

bool
Cal3DRenderer::Pause(void)
{
	// TODO:
	// Do something necessary when Player is paused.

	return true;
}

bool
Cal3DRenderer::Resume(void)
{
	// TODO:
	// Do something necessary when Player is resumed.

	return true;
}

int
Cal3DRenderer::GetTargetControlWidth(void)
{
	// TODO:
	// Return target control width

	return __controlWidth;
}

int
Cal3DRenderer::GetTargetControlHeight(void)
{
	// TODO:
	// Return target control height

	return __controlHeight;
}

void
Cal3DRenderer::SetTargetControlWidth(int width)
{
	// TODO:
	// Assign target control width

	__controlWidth = width;
}

void
Cal3DRenderer::SetTargetControlHeight(int height)
{
	// TODO:
	// Assign target control height

	__controlHeight = height;
}

void
Cal3DRenderer::IncreaseTiltAngle(float addTiltAngle)
{
	__tiltAngle += addTiltAngle;
}

void
Cal3DRenderer::IncreaseTwistAngle(float addTwistAngle)
{
	__twistAngle += addTwistAngle;
}

void
Cal3DRenderer::IncreaseDistance(float addDistance)
{
	__distance -= addDistance;
	if(__distance < 0.0f)
		__distance = 0.0f;
}

void
Cal3DRenderer::ChangeModel()
{
	__cal3dLoader->NextModel();
}

float
Cal3DRenderer::UpdateTick()
{
	unsigned int tick;
	long long time;
	Tizen::System::SystemTime::GetTicks(time);
	tick = (unsigned int) (time);

	// calculate the amount of elapsed seconds
	float elapsedSeconds = (float) (tick - __lastTick) / 1000.0f;

	// adjust fps counter
	__fpsDuration += elapsedSeconds;
	if (__fpsDuration >= 1.0f) {
		__fps = (int) ((float) __fpsFrames / __fpsDuration);
		__fpsDuration = 0.0f;
		__fpsFrames = 0;
	}

	elapsedSeconds *= __timeScale;

	// current tick will be last tick next round
	__lastTick = tick;

	return elapsedSeconds;
}

void
Cal3DRenderer::RenderCal3d()
{
	// clear the vertex and face counters
	__vertexCount = 0;
	__faceCount = 0;

	// clear all the buffers
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	SetPerspective(45.0f, ((double) GetTargetControlWidth()) / ((double) GetTargetControlHeight()), 0.01, 10000);

	// set the model transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// light attributes
	const GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	const GLfloat light_diffuse[] = { 0.52f, 0.5f, 0.5f, 1.0f };
	const GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	// setup the light attributes
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// set the light position
	GLfloat lightPosition[] = { 0.0f, -1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// set camera position
	glTranslatef(0.0f, 0.0f, (-__distance * __cal3dLoader->getScale()));
	glRotatef(__tiltAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(__twistAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -90.0f * __cal3dLoader->getScale());

	// render the model
	RenderModel();

	// increase frame counter
	__fpsFrames++;

}

//----------------------------------------------------------------------------//
// Render the model                                                           //
//----------------------------------------------------------------------------//

void
Cal3DRenderer::RenderModel()
{

	// get the renderer of the model
	CalRenderer *pCalRenderer = __cal3dLoader->getCurrentCalModel()->getRenderer();

	// begin the rendering loop
	if (pCalRenderer->beginRendering()) {
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
		for (int meshId = 0; meshId < meshCount; meshId++) {
			// get the number of submeshes
			int submeshCount = pCalRenderer->getSubmeshCount(meshId);

			// render all submeshes of the mesh
			for (int submeshId = 0; submeshId < submeshCount; submeshId++) {
				// select mesh and submesh for further data access
				if (pCalRenderer->selectMeshSubmesh(meshId, submeshId)) {
					unsigned char meshColor[4];
					GLfloat materialColor[4];

					// set the material ambient color
					pCalRenderer->getAmbientColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

					// set the material diffuse color
					pCalRenderer->getDiffuseColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

					// set the material specular color
					pCalRenderer->getSpecularColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

					// set the material shininess factor
					float shininess;
					shininess = 50.0f; //pCalRenderer->getShininess();
					glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

					// get the transformed vertices of the submesh
					static float meshVertices[30000][3];
					int vertexCount = pCalRenderer->getVertices(
							&meshVertices[0][0]);

					// get the transformed normals of the submesh
					static float meshNormals[30000][3];
					pCalRenderer->getNormals(&meshNormals[0][0]);

					// get the texture coordinates of the submesh
					static float meshTextureCoordinates[30000][2];
					int textureCoordinateCount =
							pCalRenderer->getTextureCoordinates(0,
									&meshTextureCoordinates[0][0]);

					// get the faces of the submesh
					static CalIndex meshFaces[50000][3];
					int faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

					// set the vertex and normal buffers
					glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
					glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

					// set the texture coordinate buffer and state if necessary
					if ((pCalRenderer->getMapCount() > 0)
							&& (textureCoordinateCount > 0)) {
						glEnable(GL_TEXTURE_2D);
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glEnable(GL_COLOR_MATERIAL);

						// set the texture id we stored in the map user data
						glBindTexture(GL_TEXTURE_2D,
								(GLuint) pCalRenderer->getMapUserData(0));

						// set the texture coordinate buffer
						glTexCoordPointer(2, GL_FLOAT, 0,
								&meshTextureCoordinates[0][0]);
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					}

					// draw the submesh
					if (sizeof(CalIndex) == 2)
						glDrawElements(GL_TRIANGLES, faceCount * 3,
								GL_UNSIGNED_SHORT, &meshFaces[0][0]);
					else
						glDrawElements(GL_TRIANGLES, faceCount * 3,
								GL_UNSIGNED_SHORT, &meshFaces[0][0]);

					// disable the texture coordinate state if necessary
					if ((pCalRenderer->getMapCount() > 0)
							&& (textureCoordinateCount > 0)) {
						glDisable(GL_COLOR_MATERIAL);
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						glDisable(GL_TEXTURE_2D);
					}

					// adjust the vertex and face counter
					__vertexCount += vertexCount;
					__faceCount += faceCount;

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
}



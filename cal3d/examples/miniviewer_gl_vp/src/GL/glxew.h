/*
** The OpenGL Extension Wrangler Library
** Copyright (C) 2003, 2002, Milan Ikits <milan.ikits@ieee.org>
** Copyright (C) 2003, 2002, Marcelo E. Magallon <mmagallo@debian.org>
** Copyright (C) 2002, Lev Povalahev <levp@gmx.net>
** All rights reserved.
** 
** Redistribution and use in source and binary forms, with or without 
** modification, are permitted provided that the following conditions are met:
** 
** * Redistributions of source code must retain the above copyright notice, 
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice, 
**   this list of conditions and the following disclaimer in the documentation 
**   and/or other materials provided with the distribution.
** * The name of the author may be used to endorse or promote products 
**   derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
** THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** The contents of this file are subject to the GLX Public License Version 1.0
** (the "License"). You may not use this file except in compliance with the
** License. You may obtain a copy of the License at Silicon Graphics, Inc.,
** attn: Legal Services, 2011 N. Shoreline Blvd., Mountain View, CA 94043
** or at http://www.sgi.com/software/opensource/glx/license.html.
**
** Software distributed under the License is distributed on an "AS IS"
** basis. ALL WARRANTIES ARE DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY
** IMPLIED WARRANTIES OF MERCHANTABILITY, OF FITNESS FOR A PARTICULAR
** PURPOSE OR OF NON- INFRINGEMENT. See the License for the specific
** language governing rights and limitations under the License.
**
** The Original Software is GLX version 1.2 source code, released February,
** 1999. The developer of the Original Software is Silicon Graphics, Inc.
** Those portions of the Subject Software created by Silicon Graphics, Inc.
** are Copyright (c) 1991-9 Silicon Graphics, Inc. All Rights Reserved.
*/

#ifndef __glxew_h__
#define __glxew_h__
#define __GLXEW_H__

#ifdef __glxext_h_
#error glxext.h included before glxew.h
#endif

#define __glxext_h_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------- GLX_VERSION_1_0 --------------------------- */

#ifndef GLX_VERSION_1_0
#define GLX_VERSION_1_0 1

#define GLX_USE_GL 1
#define GLX_BUFFER_SIZE 2
#define GLX_LEVEL 3
#define GLX_RGBA 4
#define GLX_DOUBLEBUFFER 5
#define GLX_STEREO 6
#define GLX_AUX_BUFFERS 7
#define GLX_RED_SIZE 8
#define GLX_GREEN_SIZE 9
#define GLX_BLUE_SIZE 10
#define GLX_ALPHA_SIZE 11
#define GLX_DEPTH_SIZE 12
#define GLX_STENCIL_SIZE 13
#define GLX_ACCUM_RED_SIZE 14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE 16
#define GLX_ACCUM_ALPHA_SIZE 17
#define GLX_BAD_SCREEN 1
#define GLX_BAD_ATTRIBUTE 2
#define GLX_NO_EXTENSION 3
#define GLX_BAD_VISUAL 4
#define GLX_BAD_CONTEXT 5
#define GLX_BAD_VALUE 6
#define GLX_BAD_ENUM 7

typedef XID GLXDrawable;
typedef XID GLXPixmap;
typedef struct __GLXcontextRec *GLXContext;

extern Bool glXQueryExtension (Display *dpy, int *errorBase, int *eventBase);
extern Bool glXQueryVersion (Display *dpy, int *major, int *minor);
extern int glXGetConfig (Display *dpy, XVisualInfo *vis, int attrib, int *value);
extern XVisualInfo* glXChooseVisual (Display *dpy, int screen, int *attribList);
extern GLXPixmap glXCreateGLXPixmap (Display *dpy, XVisualInfo *vis, Pixmap pixmap);
extern void glXDestroyGLXPixmap (Display *dpy, GLXPixmap pix);
extern GLXContext glXCreateContext (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern void glXDestroyContext (Display *dpy, GLXContext ctx);
extern Bool glXIsDirect (Display *dpy, GLXContext ctx);
extern void glXCopyContext (Display *dpy, GLXContext src, GLXContext dst, GLuint mask);
extern Bool glXMakeCurrent (Display *dpy, GLXDrawable drawable, GLXContext ctx);
extern GLXContext glXGetCurrentContext (void);
extern GLXDrawable glXGetCurrentDrawable (void);
extern void glXWaitGL (void);
extern void glXWaitX (void);
extern void glXSwapBuffers (Display *dpy, GLXDrawable drawable);
extern void glXUseXFont (Font font, int first, int count, int listBase);

GLEWAPI GLboolean GLXEW_VERSION_1_0;
#define glxew_VERSION_1_0 GLXEW_VERSION_1_0

#endif /* GLX_VERSION_1_0 */

/* ---------------------------- GLX_VERSION_1_1 --------------------------- */

#ifndef GLX_VERSION_1_1
#define GLX_VERSION_1_1

#define GLX_VENDOR 0x1
#define GLX_VERSION 0x2
#define GLX_EXTENSIONS 0x3

extern const char* glXQueryExtensionsString (Display *dpy, int screen);
extern const char* glXGetClientString (Display *dpy, int name);
extern const char* glXQueryServerString (Display *dpy, int screen, int name);

GLEWAPI GLboolean GLXEW_VERSION_1_1;
#define glxew_VERSION_1_1 GLXEW_VERSION_1_1

#endif /* GLX_VERSION_1_1 */

/* ---------------------------- GLX_VERSION_1_2 ---------------------------- */

#ifndef GLX_VERSION_1_2
#define GLX_VERSION_1_2 1

typedef Display* ( * PFNGLXGETCURRENTDISPLAYPROC) (void);

GLEWAPI PFNGLXGETCURRENTDISPLAYPROC glewXGetCurrentDisplay;

#define glXGetCurrentDisplay glewXGetCurrentDisplay

GLEWAPI GLboolean GLXEW_VERSION_1_2;

#endif /* GLX_VERSION_1_2 */

/* ---------------------------- GLX_VERSION_1_3 ---------------------------- */

#ifndef GLX_VERSION_1_3
#define GLX_VERSION_1_3 1

#define GLX_RGBA_BIT 0x00000001
#define GLX_FRONT_LEFT_BUFFER_BIT 0x00000001
#define GLX_WINDOW_BIT 0x00000001
#define GLX_COLOR_INDEX_BIT 0x00000002
#define GLX_PIXMAP_BIT 0x00000002
#define GLX_FRONT_RIGHT_BUFFER_BIT 0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT 0x00000004
#define GLX_PBUFFER_BIT 0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT 0x00000008
#define GLX_AUX_BUFFERS_BIT 0x00000010
#define GLX_CONFIG_CAVEAT 0x20
#define GLX_DEPTH_BUFFER_BIT 0x00000020
#define GLX_X_VISUAL_TYPE 0x22
#define GLX_TRANSPARENT_TYPE 0x23
#define GLX_TRANSPARENT_INDEX_VALUE 0x24
#define GLX_TRANSPARENT_RED_VALUE 0x25
#define GLX_TRANSPARENT_GREEN_VALUE 0x26
#define GLX_TRANSPARENT_BLUE_VALUE 0x27
#define GLX_TRANSPARENT_ALPHA_VALUE 0x28
#define GLX_STENCIL_BUFFER_BIT 0x00000040
#define GLX_ACCUM_BUFFER_BIT 0x00000080
#define GLX_NONE 0x8000
#define GLX_SLOW_CONFIG 0x8001
#define GLX_TRUE_COLOR 0x8002
#define GLX_DIRECT_COLOR 0x8003
#define GLX_PSEUDO_COLOR 0x8004
#define GLX_STATIC_COLOR 0x8005
#define GLX_GRAY_SCALE 0x8006
#define GLX_STATIC_GRAY 0x8007
#define GLX_TRANSPARENT_RGB 0x8008
#define GLX_TRANSPARENT_INDEX 0x8009
#define GLX_VISUAL_ID 0x800B
#define GLX_SCREEN 0x800C
#define GLX_NON_CONFORMANT_CONFIG 0x800D
#define GLX_DRAWABLE_TYPE 0x8010
#define GLX_RENDER_TYPE 0x8011
#define GLX_X_RENDERABLE 0x8012
#define GLX_FBCONFIG_ID 0x8013
#define GLX_RGBA_TYPE 0x8014
#define GLX_COLOR_INDEX_TYPE 0x8015
#define GLX_MAX_PBUFFER_WIDTH 0x8016
#define GLX_MAX_PBUFFER_HEIGHT 0x8017
#define GLX_MAX_PBUFFER_PIXELS 0x8018
#define GLX_PRESERVED_CONTENTS 0x801B
#define GLX_LARGEST_PBUFFER 0x801C
#define GLX_WIDTH 0x801D
#define GLX_HEIGHT 0x801E
#define GLX_EVENT_MASK 0x801F
#define GLX_DAMAGED 0x8020
#define GLX_SAVED 0x8021
#define GLX_WINDOW 0x8022
#define GLX_PBUFFER 0x8023
#define GLX_PBUFFER_HEIGHT 0x8040
#define GLX_PBUFFER_WIDTH 0x8041
#define GLX_PBUFFER_CLOBBER_MASK 0x08000000
#define GLX_DONT_CARE 0xFFFFFFFF

typedef XID GLXFBConfigID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef struct { int event_type; int draw_type; unsigned long serial; Bool send_event; Display *display; GLXDrawable drawable; unsigned int buffer_mask; unsigned int aux_buffer; int x, y; int width, height; int count; } GLXPbufferClobberEvent;
typedef union __GLXEvent { GLXPbufferClobberEvent glxpbufferclobber; long pad[24]; } GLXEvent;

typedef GLXFBConfig* ( * PFNGLXCHOOSEFBCONFIGPROC) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef GLXContext ( * PFNGLXCREATENEWCONTEXTPROC) (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
typedef GLXPbuffer ( * PFNGLXCREATEPBUFFERPROC) (Display *dpy, GLXFBConfig config, const int *attrib_list);
typedef GLXPixmap ( * PFNGLXCREATEPIXMAPPROC) (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
typedef GLXWindow ( * PFNGLXCREATEWINDOWPROC) (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
typedef void ( * PFNGLXDESTROYPBUFFERPROC) (Display *dpy, GLXPbuffer pbuf);
typedef void ( * PFNGLXDESTROYPIXMAPPROC) (Display *dpy, GLXPixmap pixmap);
typedef void ( * PFNGLXDESTROYWINDOWPROC) (Display *dpy, GLXWindow win);
typedef GLXDrawable ( * PFNGLXGETCURRENTREADDRAWABLEPROC) (void);
typedef int ( * PFNGLXGETFBCONFIGATTRIBPROC) (Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef GLXFBConfig* ( * PFNGLXGETFBCONFIGSPROC) (Display *dpy, int screen, int *nelements);
typedef void ( * PFNGLXGETSELECTEDEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long *event_mask);
typedef XVisualInfo* ( * PFNGLXGETVISUALFROMFBCONFIGPROC) (Display *dpy, GLXFBConfig config);
typedef Bool ( * PFNGLXMAKECONTEXTCURRENTPROC) (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
typedef int ( * PFNGLXQUERYCONTEXTPROC) (Display *dpy, GLXContext ctx, int attribute, int *value);
typedef void ( * PFNGLXQUERYDRAWABLEPROC) (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
typedef void ( * PFNGLXSELECTEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long event_mask);

GLEWAPI PFNGLXCHOOSEFBCONFIGPROC glewXChooseFBConfig;
GLEWAPI PFNGLXCREATENEWCONTEXTPROC glewXCreateNewContext;
GLEWAPI PFNGLXCREATEPBUFFERPROC glewXCreatePbuffer;
GLEWAPI PFNGLXCREATEPIXMAPPROC glewXCreatePixmap;
GLEWAPI PFNGLXCREATEWINDOWPROC glewXCreateWindow;
GLEWAPI PFNGLXDESTROYPBUFFERPROC glewXDestroyPbuffer;
GLEWAPI PFNGLXDESTROYPIXMAPPROC glewXDestroyPixmap;
GLEWAPI PFNGLXDESTROYWINDOWPROC glewXDestroyWindow;
GLEWAPI PFNGLXGETCURRENTREADDRAWABLEPROC glewXGetCurrentReadDrawable;
GLEWAPI PFNGLXGETFBCONFIGATTRIBPROC glewXGetFBConfigAttrib;
GLEWAPI PFNGLXGETFBCONFIGSPROC glewXGetFBConfigs;
GLEWAPI PFNGLXGETSELECTEDEVENTPROC glewXGetSelectedEvent;
GLEWAPI PFNGLXGETVISUALFROMFBCONFIGPROC glewXGetVisualFromFBConfig;
GLEWAPI PFNGLXMAKECONTEXTCURRENTPROC glewXMakeContextCurrent;
GLEWAPI PFNGLXQUERYCONTEXTPROC glewXQueryContext;
GLEWAPI PFNGLXQUERYDRAWABLEPROC glewXQueryDrawable;
GLEWAPI PFNGLXSELECTEVENTPROC glewXSelectEvent;

#define glXChooseFBConfig glewXChooseFBConfig
#define glXCreateNewContext glewXCreateNewContext
#define glXCreatePbuffer glewXCreatePbuffer
#define glXCreatePixmap glewXCreatePixmap
#define glXCreateWindow glewXCreateWindow
#define glXDestroyPbuffer glewXDestroyPbuffer
#define glXDestroyPixmap glewXDestroyPixmap
#define glXDestroyWindow glewXDestroyWindow
#define glXGetCurrentReadDrawable glewXGetCurrentReadDrawable
#define glXGetFBConfigAttrib glewXGetFBConfigAttrib
#define glXGetFBConfigs glewXGetFBConfigs
#define glXGetSelectedEvent glewXGetSelectedEvent
#define glXGetVisualFromFBConfig glewXGetVisualFromFBConfig
#define glXMakeContextCurrent glewXMakeContextCurrent
#define glXQueryContext glewXQueryContext
#define glXQueryDrawable glewXQueryDrawable
#define glXSelectEvent glewXSelectEvent

GLEWAPI GLboolean GLXEW_VERSION_1_3;

#endif /* GLX_VERSION_1_3 */

/* ---------------------------- GLX_VERSION_1_4 ---------------------------- */

#ifndef GLX_VERSION_1_4
#define GLX_VERSION_1_4 1

#define GLX_SAMPLE_BUFFERS 100000
#define GLX_SAMPLES 100001

extern void ( * glXGetProcAddress (const GLubyte *procName)) (void);

GLEWAPI GLboolean GLXEW_VERSION_1_4;

#endif /* GLX_VERSION_1_4 */

/* -------------------------- GLX_3DFX_multisample ------------------------- */

#ifndef GLX_3DFX_multisample
#define GLX_3DFX_multisample 1

#define GLX_SAMPLE_BUFFERS_3DFX 0x8050
#define GLX_SAMPLES_3DFX 0x8051

GLEWAPI GLboolean GLXEW_3DFX_multisample;

#endif /* GLX_3DFX_multisample */

/* ------------------------ GLX_ARB_get_proc_address ----------------------- */

#ifndef GLX_ARB_get_proc_address
#define GLX_ARB_get_proc_address 1

extern void ( * glXGetProcAddressARB (const GLubyte *procName)) (void);

GLEWAPI GLboolean GLXEW_ARB_get_proc_address;

#endif /* GLX_ARB_get_proc_address */

/* -------------------------- GLX_ARB_multisample -------------------------- */

#ifndef GLX_ARB_multisample
#define GLX_ARB_multisample 1

#define GLX_SAMPLE_BUFFERS_ARB 100000
#define GLX_SAMPLES_ARB 100001

GLEWAPI GLboolean GLXEW_ARB_multisample;

#endif /* GLX_ARB_multisample */

/* ----------------------- GLX_ATI_pixel_format_float ---------------------- */

#ifndef GLX_ATI_pixel_format_float
#define GLX_ATI_pixel_format_float 1

#define GLX_RGBA_FLOAT_ATI_BIT 0x00000100

GLEWAPI GLboolean GLXEW_ATI_pixel_format_float;

#endif /* GLX_ATI_pixel_format_float */

/* ------------------------- GLX_ATI_render_texture ------------------------ */

#ifndef GLX_ATI_render_texture
#define GLX_ATI_render_texture 1

#define GLX_BIND_TO_TEXTURE_RGB_ATI 0x9800
#define GLX_BIND_TO_TEXTURE_RGBA_ATI 0x9801
#define GLX_TEXTURE_FORMAT_ATI 0x9802
#define GLX_TEXTURE_TARGET_ATI 0x9803
#define GLX_MIPMAP_TEXTURE_ATI 0x9804
#define GLX_TEXTURE_RGB_ATI 0x9805
#define GLX_TEXTURE_RGBA_ATI 0x9806
#define GLX_NO_TEXTURE_ATI 0x9807
#define GLX_TEXTURE_CUBE_MAP_ATI 0x9808
#define GLX_TEXTURE_1D_ATI 0x9809
#define GLX_TEXTURE_2D_ATI 0x980A
#define GLX_MIPMAP_LEVEL_ATI 0x980B
#define GLX_CUBE_MAP_FACE_ATI 0x980C
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_X_ATI 0x980D
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_X_ATI 0x980E
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_Y_ATI 0x980F
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_Y_ATI 0x9810
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_Z_ATI 0x9811
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_Z_ATI 0x9812
#define GLX_FRONT_LEFT_ATI 0x9813
#define GLX_FRONT_RIGHT_ATI 0x9814
#define GLX_BACK_LEFT_ATI 0x9815
#define GLX_BACK_RIGHT_ATI 0x9816
#define GLX_AUX0_ATI 0x9817
#define GLX_AUX1_ATI 0x9818
#define GLX_AUX2_ATI 0x9819
#define GLX_AUX3_ATI 0x981A
#define GLX_AUX4_ATI 0x981B
#define GLX_AUX5_ATI 0x981C
#define GLX_AUX6_ATI 0x981D
#define GLX_AUX7_ATI 0x981E
#define GLX_AUX8_ATI 0x981F
#define GLX_AUX9_ATI 0x9820
#define GLX_BIND_TO_TEXTURE_LUMINANCE_ATI 0x9821
#define GLX_BIND_TO_TEXTURE_INTENSITY_ATI 0x9822

typedef void ( * PFNGLXBINDTEXIMAGEATIPROC) (Display *dpy, GLXPbuffer pbuf, int buffer);
typedef void ( * PFNGLXDRAWABLEATTRIBATIPROC) (Display *dpy, GLXDrawable draw, const int *attrib_list);
typedef void ( * PFNGLXRELEASETEXIMAGEATIPROC) (Display *dpy, GLXPbuffer pbuf, int buffer);

GLEWAPI PFNGLXBINDTEXIMAGEATIPROC glewXBindTexImageATI;
GLEWAPI PFNGLXDRAWABLEATTRIBATIPROC glewXDrawableAttribATI;
GLEWAPI PFNGLXRELEASETEXIMAGEATIPROC glewXReleaseTexImageATI;

#define glXBindTexImageATI glewXBindTexImageATI
#define glXDrawableAttribATI glewXDrawableAttribATI
#define glXReleaseTexImageATI glewXReleaseTexImageATI

GLEWAPI GLboolean GLXEW_ATI_render_texture;

#endif /* GLX_ATI_render_texture */

/* ------------------------- GLX_EXT_import_context ------------------------ */

#ifndef GLX_EXT_import_context
#define GLX_EXT_import_context 1

#define GLX_SHARE_CONTEXT_EXT 0x800A
#define GLX_VISUAL_ID_EXT 0x800B
#define GLX_SCREEN_EXT 0x800C

typedef XID GLXContextID;

typedef void ( * PFNGLXFREECONTEXTEXTPROC) (Display* dpy, GLXContext context);
typedef GLXContextID ( * PFNGLXGETCONTEXTIDEXTPROC) (const GLXContext context);
typedef GLXContext ( * PFNGLXIMPORTCONTEXTEXTPROC) (Display* dpy, GLXContextID contextID);
typedef int ( * PFNGLXQUERYCONTEXTINFOEXTPROC) (Display* dpy, GLXContext context, int attribute,int *value);

GLEWAPI PFNGLXFREECONTEXTEXTPROC glewXFreeContextEXT;
GLEWAPI PFNGLXGETCONTEXTIDEXTPROC glewXGetContextIDEXT;
GLEWAPI PFNGLXIMPORTCONTEXTEXTPROC glewXImportContextEXT;
GLEWAPI PFNGLXQUERYCONTEXTINFOEXTPROC glewXQueryContextInfoEXT;

#define glXFreeContextEXT glewXFreeContextEXT
#define glXGetContextIDEXT glewXGetContextIDEXT
#define glXImportContextEXT glewXImportContextEXT
#define glXQueryContextInfoEXT glewXQueryContextInfoEXT

GLEWAPI GLboolean GLXEW_EXT_import_context;

#endif /* GLX_EXT_import_context */

/* -------------------------- GLX_EXT_scene_marker ------------------------- */

#ifndef GLX_EXT_scene_marker
#define GLX_EXT_scene_marker 1

GLEWAPI GLboolean GLXEW_EXT_scene_marker;

#endif /* GLX_EXT_scene_marker */

/* -------------------------- GLX_EXT_visual_info -------------------------- */

#ifndef GLX_EXT_visual_info
#define GLX_EXT_visual_info 1

#define GLX_X_VISUAL_TYPE_EXT 0x22
#define GLX_TRANSPARENT_TYPE_EXT 0x23
#define GLX_TRANSPARENT_INDEX_VALUE_EXT 0x24
#define GLX_TRANSPARENT_RED_VALUE_EXT 0x25
#define GLX_TRANSPARENT_GREEN_VALUE_EXT 0x26
#define GLX_TRANSPARENT_BLUE_VALUE_EXT 0x27
#define GLX_TRANSPARENT_ALPHA_VALUE_EXT 0x28
#define GLX_NONE_EXT 0x8000
#define GLX_TRUE_COLOR_EXT 0x8002
#define GLX_DIRECT_COLOR_EXT 0x8003
#define GLX_PSEUDO_COLOR_EXT 0x8004
#define GLX_STATIC_COLOR_EXT 0x8005
#define GLX_GRAY_SCALE_EXT 0x8006
#define GLX_STATIC_GRAY_EXT 0x8007
#define GLX_TRANSPARENT_RGB_EXT 0x8008
#define GLX_TRANSPARENT_INDEX_EXT 0x8009

GLEWAPI GLboolean GLXEW_EXT_visual_info;

#endif /* GLX_EXT_visual_info */

/* ------------------------- GLX_EXT_visual_rating ------------------------- */

#ifndef GLX_EXT_visual_rating
#define GLX_EXT_visual_rating 1

#define GLX_VISUAL_CAVEAT_EXT 0x20
#define GLX_SLOW_VISUAL_EXT 0x8001
#define GLX_NON_CONFORMANT_VISUAL_EXT 0x800D

GLEWAPI GLboolean GLXEW_EXT_visual_rating;

#endif /* GLX_EXT_visual_rating */

/* ------------------------ GLX_MESA_copy_sub_buffer ----------------------- */

#ifndef GLX_MESA_copy_sub_buffer
#define GLX_MESA_copy_sub_buffer 1

typedef void ( * PFNGLXCOPYSUBBUFFERMESAPROC) (Display* dpy, GLXDrawable drawable, int x, int y, int width, int height);

GLEWAPI PFNGLXCOPYSUBBUFFERMESAPROC glewXCopySubBufferMESA;

#define glXCopySubBufferMESA glewXCopySubBufferMESA

GLEWAPI GLboolean GLXEW_MESA_copy_sub_buffer;

#endif /* GLX_MESA_copy_sub_buffer */

/* ------------------------ GLX_MESA_pixmap_colormap ----------------------- */

#ifndef GLX_MESA_pixmap_colormap
#define GLX_MESA_pixmap_colormap 1

typedef GLXPixmap ( * PFNGLXCREATEGLXPIXMAPMESAPROC) (Display* dpy, XVisualInfo *visual, Pixmap pixmap, Colormap cmap);

GLEWAPI PFNGLXCREATEGLXPIXMAPMESAPROC glewXCreateGLXPixmapMESA;

#define glXCreateGLXPixmapMESA glewXCreateGLXPixmapMESA

GLEWAPI GLboolean GLXEW_MESA_pixmap_colormap;

#endif /* GLX_MESA_pixmap_colormap */

/* ------------------------ GLX_MESA_release_buffers ----------------------- */

#ifndef GLX_MESA_release_buffers
#define GLX_MESA_release_buffers 1

typedef Bool ( * PFNGLXRELEASEBUFFERSMESAPROC) (Display* dpy, GLXDrawable d);

GLEWAPI PFNGLXRELEASEBUFFERSMESAPROC glewXReleaseBuffersMESA;

#define glXReleaseBuffersMESA glewXReleaseBuffersMESA

GLEWAPI GLboolean GLXEW_MESA_release_buffers;

#endif /* GLX_MESA_release_buffers */

/* ------------------------- GLX_MESA_set_3dfx_mode ------------------------ */

#ifndef GLX_MESA_set_3dfx_mode
#define GLX_MESA_set_3dfx_mode 1

#define GLX_3DFX_WINDOW_MODE_MESA 0x1
#define GLX_3DFX_FULLSCREEN_MODE_MESA 0x2

typedef GLboolean ( * PFNGLXSET3DFXMODEMESAPROC) (GLint mode);

GLEWAPI PFNGLXSET3DFXMODEMESAPROC glewXSet3DfxModeMESA;

#define glXSet3DfxModeMESA glewXSet3DfxModeMESA

GLEWAPI GLboolean GLXEW_MESA_set_3dfx_mode;

#endif /* GLX_MESA_set_3dfx_mode */

/* -------------------------- GLX_NV_float_buffer -------------------------- */

#ifndef GLX_NV_float_buffer
#define GLX_NV_float_buffer 1

#define GLX_FLOAT_COMPONENTS_NV 0x20B0

GLEWAPI GLboolean GLXEW_NV_float_buffer;

#endif /* GLX_NV_float_buffer */

/* ----------------------- GLX_NV_vertex_array_range ----------------------- */

#ifndef GLX_NV_vertex_array_range
#define GLX_NV_vertex_array_range 1

typedef void * ( * PFNGLXALLOCATEMEMORYNVPROC) (GLsizei size, GLfloat readFrequency, GLfloat writeFrequency, GLfloat priority);
typedef void ( * PFNGLXFREEMEMORYNVPROC) (void *pointer);

GLEWAPI PFNGLXALLOCATEMEMORYNVPROC glewXAllocateMemoryNV;
GLEWAPI PFNGLXFREEMEMORYNVPROC glewXFreeMemoryNV;

#define glXAllocateMemoryNV glewXAllocateMemoryNV
#define glXFreeMemoryNV glewXFreeMemoryNV

GLEWAPI GLboolean GLXEW_NV_vertex_array_range;

#endif /* GLX_NV_vertex_array_range */

/* -------------------------- GLX_OML_swap_method -------------------------- */

#ifndef GLX_OML_swap_method
#define GLX_OML_swap_method 1

#define GLX_SWAP_METHOD_OML 0x8060
#define GLX_SWAP_EXCHANGE_OML 0x8061
#define GLX_SWAP_COPY_OML 0x8062
#define GLX_SWAP_UNDEFINED_OML 0x8063

GLEWAPI GLboolean GLXEW_OML_swap_method;

#endif /* GLX_OML_swap_method */

/* -------------------------- GLX_OML_sync_control ------------------------- */

#if !defined(GLX_OML_sync_control) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <inttypes.h>
#define GLX_OML_sync_control 1

typedef Bool ( * PFNGLXGETMSCRATEOMLPROC) (Display* dpy, GLXDrawable drawable, int32_t* numerator, int32_t* denominator);
typedef Bool ( * PFNGLXGETSYNCVALUESOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t* ust, int64_t* msc, int64_t* sbc);
typedef int64_t ( * PFNGLXSWAPBUFFERSMSCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder);
typedef Bool ( * PFNGLXWAITFORMSCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder, int64_t* ust, int64_t* msc, int64_t* sbc);
typedef Bool ( * PFNGLXWAITFORSBCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_sbc, int64_t* ust, int64_t* msc, int64_t* sbc);

GLEWAPI PFNGLXGETMSCRATEOMLPROC glewXGetMscRateOML;
GLEWAPI PFNGLXGETSYNCVALUESOMLPROC glewXGetSyncValuesOML;
GLEWAPI PFNGLXSWAPBUFFERSMSCOMLPROC glewXSwapBuffersMscOML;
GLEWAPI PFNGLXWAITFORMSCOMLPROC glewXWaitForMscOML;
GLEWAPI PFNGLXWAITFORSBCOMLPROC glewXWaitForSbcOML;

#define glXGetMscRateOML glewXGetMscRateOML
#define glXGetSyncValuesOML glewXGetSyncValuesOML
#define glXSwapBuffersMscOML glewXSwapBuffersMscOML
#define glXWaitForMscOML glewXWaitForMscOML
#define glXWaitForSbcOML glewXWaitForSbcOML

GLEWAPI GLboolean GLXEW_OML_sync_control;

#endif /* GLX_OML_sync_control */

/* ------------------------ GLX_SGIS_blended_overlay ----------------------- */

#ifndef GLX_SGIS_blended_overlay
#define GLX_SGIS_blended_overlay 1

#define GLX_BLENDED_RGBA_SGIS 0x8025

GLEWAPI GLboolean GLXEW_SGIS_blended_overlay;

#endif /* GLX_SGIS_blended_overlay */

/* -------------------------- GLX_SGIS_color_range ------------------------- */

#ifndef GLX_SGIS_color_range
#define GLX_SGIS_color_range 1

#define GLX_MIN_RED_SGIS 0
#define GLX_MAX_GREEN_SGIS 0
#define GLX_MIN_BLUE_SGIS 0
#define GLX_MAX_ALPHA_SGIS 0
#define GLX_MIN_GREEN_SGIS 0
#define GLX_MIN_ALPHA_SGIS 0
#define GLX_MAX_RED_SGIS 0
#define GLX_EXTENDED_RANGE_SGIS 0
#define GLX_MAX_BLUE_SGIS 0

GLEWAPI GLboolean GLXEW_SGIS_color_range;

#endif /* GLX_SGIS_color_range */

/* -------------------------- GLX_SGIS_multisample ------------------------- */

#ifndef GLX_SGIS_multisample
#define GLX_SGIS_multisample 1

#define GLX_SAMPLE_BUFFERS_SGIS 100000
#define GLX_SAMPLES_SGIS 100001

GLEWAPI GLboolean GLXEW_SGIS_multisample;

#endif /* GLX_SGIS_multisample */

/* ---------------------- GLX_SGIS_shared_multisample ---------------------- */

#ifndef GLX_SGIS_shared_multisample
#define GLX_SGIS_shared_multisample 1

#define GLX_MULTISAMPLE_SUB_RECT_WIDTH_SGIS 0x8026
#define GLX_MULTISAMPLE_SUB_RECT_HEIGHT_SGIS 0x8027

GLEWAPI GLboolean GLXEW_SGIS_shared_multisample;

#endif /* GLX_SGIS_shared_multisample */

/* --------------------------- GLX_SGIX_fbconfig --------------------------- */

#ifndef GLX_SGIX_fbconfig
#define GLX_SGIX_fbconfig 1

#define GLX_WINDOW_BIT_SGIX 0x00000001
#define GLX_RGBA_BIT_SGIX 0x00000001
#define GLX_PIXMAP_BIT_SGIX 0x00000002
#define GLX_COLOR_INDEX_BIT_SGIX 0x00000002
#define GLX_SCREEN_EXT 0x800C
#define GLX_DRAWABLE_TYPE_SGIX 0x8010
#define GLX_RENDER_TYPE_SGIX 0x8011
#define GLX_X_RENDERABLE_SGIX 0x8012
#define GLX_FBCONFIG_ID_SGIX 0x8013
#define GLX_RGBA_TYPE_SGIX 0x8014
#define GLX_COLOR_INDEX_TYPE_SGIX 0x8015

typedef XID GLXFBConfigIDSGIX;
typedef struct __GLXFBConfigRec *GLXFBConfigSGIX;

typedef GLXFBConfigSGIX* ( * PFNGLXCHOOSEFBCONFIGSGIXPROC) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef GLXContext ( * PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC) (Display* dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
typedef GLXPixmap ( * PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC) (Display* dpy, GLXFBConfig config, Pixmap pixmap);
typedef int ( * PFNGLXGETFBCONFIGATTRIBSGIXPROC) (Display* dpy, GLXFBConfigSGIX config, int attribute, int *value);
typedef GLXFBConfigSGIX ( * PFNGLXGETFBCONFIGFROMVISUALSGIXPROC) (Display* dpy, XVisualInfo *vis);
typedef XVisualInfo* ( * PFNGLXGETVISUALFROMFBCONFIGSGIXPROC) (Display *dpy, GLXFBConfig config);

GLEWAPI PFNGLXCHOOSEFBCONFIGSGIXPROC glewXChooseFBConfigSGIX;
GLEWAPI PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC glewXCreateContextWithConfigSGIX;
GLEWAPI PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC glewXCreateGLXPixmapWithConfigSGIX;
GLEWAPI PFNGLXGETFBCONFIGATTRIBSGIXPROC glewXGetFBConfigAttribSGIX;
GLEWAPI PFNGLXGETFBCONFIGFROMVISUALSGIXPROC glewXGetFBConfigFromVisualSGIX;
GLEWAPI PFNGLXGETVISUALFROMFBCONFIGSGIXPROC glewXGetVisualFromFBConfigSGIX;

#define glXChooseFBConfigSGIX glewXChooseFBConfigSGIX
#define glXCreateContextWithConfigSGIX glewXCreateContextWithConfigSGIX
#define glXCreateGLXPixmapWithConfigSGIX glewXCreateGLXPixmapWithConfigSGIX
#define glXGetFBConfigAttribSGIX glewXGetFBConfigAttribSGIX
#define glXGetFBConfigFromVisualSGIX glewXGetFBConfigFromVisualSGIX
#define glXGetVisualFromFBConfigSGIX glewXGetVisualFromFBConfigSGIX

GLEWAPI GLboolean GLXEW_SGIX_fbconfig;

#endif /* GLX_SGIX_fbconfig */

/* ---------------------------- GLX_SGIX_pbuffer --------------------------- */

#ifndef GLX_SGIX_pbuffer
#define GLX_SGIX_pbuffer 1

#define GLX_FRONT_LEFT_BUFFER_BIT_SGIX 0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT_SGIX 0x00000002
#define GLX_PBUFFER_BIT_SGIX 0x00000004
#define GLX_BACK_LEFT_BUFFER_BIT_SGIX 0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT_SGIX 0x00000008
#define GLX_AUX_BUFFERS_BIT_SGIX 0x00000010
#define GLX_DEPTH_BUFFER_BIT_SGIX 0x00000020
#define GLX_STENCIL_BUFFER_BIT_SGIX 0x00000040
#define GLX_ACCUM_BUFFER_BIT_SGIX 0x00000080
#define GLX_SAMPLE_BUFFERS_BIT_SGIX 0x00000100
#define GLX_MAX_PBUFFER_WIDTH_SGIX 0x8016
#define GLX_MAX_PBUFFER_HEIGHT_SGIX 0x8017
#define GLX_MAX_PBUFFER_PIXELS_SGIX 0x8018
#define GLX_OPTIMAL_PBUFFER_WIDTH_SGIX 0x8019
#define GLX_OPTIMAL_PBUFFER_HEIGHT_SGIX 0x801A
#define GLX_PRESERVED_CONTENTS_SGIX 0x801B
#define GLX_LARGEST_PBUFFER_SGIX 0x801C
#define GLX_WIDTH_SGIX 0x801D
#define GLX_HEIGHT_SGIX 0x801E
#define GLX_EVENT_MASK_SGIX 0x801F
#define GLX_DAMAGED_SGIX 0x8020
#define GLX_SAVED_SGIX 0x8021
#define GLX_WINDOW_SGIX 0x8022
#define GLX_PBUFFER_SGIX 0x8023
#define GLX_BUFFER_CLOBBER_MASK_SGIX 0x08000000

typedef XID GLXPbufferSGIX;
typedef struct { int type; unsigned long serial; Bool send_event; Display *display; GLXDrawable drawable; int event_type; int draw_type; unsigned int mask; int x, y; int width, height; int count; } GLXBufferClobberEventSGIX;

typedef GLXPbuffer ( * PFNGLXCREATEGLXPBUFFERSGIXPROC) (Display* dpy, GLXFBConfig config, unsigned int width, unsigned int height, int *attrib_list);
typedef void ( * PFNGLXDESTROYGLXPBUFFERSGIXPROC) (Display* dpy, GLXPbuffer pbuf);
typedef void ( * PFNGLXGETSELECTEDEVENTSGIXPROC) (Display* dpy, GLXDrawable drawable, unsigned long *mask);
typedef void ( * PFNGLXQUERYGLXPBUFFERSGIXPROC) (Display* dpy, GLXPbuffer pbuf, int attribute, unsigned int *value);
typedef void ( * PFNGLXSELECTEVENTSGIXPROC) (Display* dpy, GLXDrawable drawable, unsigned long mask);

GLEWAPI PFNGLXCREATEGLXPBUFFERSGIXPROC glewXCreateGLXPbufferSGIX;
GLEWAPI PFNGLXDESTROYGLXPBUFFERSGIXPROC glewXDestroyGLXPbufferSGIX;
GLEWAPI PFNGLXGETSELECTEDEVENTSGIXPROC glewXGetSelectedEventSGIX;
GLEWAPI PFNGLXQUERYGLXPBUFFERSGIXPROC glewXQueryGLXPbufferSGIX;
GLEWAPI PFNGLXSELECTEVENTSGIXPROC glewXSelectEventSGIX;

#define glXCreateGLXPbufferSGIX glewXCreateGLXPbufferSGIX
#define glXDestroyGLXPbufferSGIX glewXDestroyGLXPbufferSGIX
#define glXGetSelectedEventSGIX glewXGetSelectedEventSGIX
#define glXQueryGLXPbufferSGIX glewXQueryGLXPbufferSGIX
#define glXSelectEventSGIX glewXSelectEventSGIX

GLEWAPI GLboolean GLXEW_SGIX_pbuffer;

#endif /* GLX_SGIX_pbuffer */

/* ------------------------- GLX_SGIX_swap_barrier ------------------------- */

#ifndef GLX_SGIX_swap_barrier
#define GLX_SGIX_swap_barrier 1

typedef void ( * PFNGLXBINDSWAPBARRIERSGIXPROC) (Display *dpy, GLXDrawable drawable, int barrier);
typedef Bool ( * PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC) (Display *dpy, int screen, int *max);

GLEWAPI PFNGLXBINDSWAPBARRIERSGIXPROC glewXBindSwapBarrierSGIX;
GLEWAPI PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC glewXQueryMaxSwapBarriersSGIX;

#define glXBindSwapBarrierSGIX glewXBindSwapBarrierSGIX
#define glXQueryMaxSwapBarriersSGIX glewXQueryMaxSwapBarriersSGIX

GLEWAPI GLboolean GLXEW_SGIX_swap_barrier;

#endif /* GLX_SGIX_swap_barrier */

/* -------------------------- GLX_SGIX_swap_group -------------------------- */

#ifndef GLX_SGIX_swap_group
#define GLX_SGIX_swap_group 1

typedef void ( * PFNGLXJOINSWAPGROUPSGIXPROC) (Display *dpy, GLXDrawable drawable, GLXDrawable member);

GLEWAPI PFNGLXJOINSWAPGROUPSGIXPROC glewXJoinSwapGroupSGIX;

#define glXJoinSwapGroupSGIX glewXJoinSwapGroupSGIX

GLEWAPI GLboolean GLXEW_SGIX_swap_group;

#endif /* GLX_SGIX_swap_group */

/* ------------------------- GLX_SGIX_video_resize ------------------------- */

#ifndef GLX_SGIX_video_resize
#define GLX_SGIX_video_resize 1

#define GLX_SYNC_FRAME_SGIX 0x00000000
#define GLX_SYNC_SWAP_SGIX 0x00000001

typedef int ( * PFNGLXBINDCHANNELTOWINDOWSGIXPROC) (Display* display, int screen, int channel, Window window);
typedef int ( * PFNGLXCHANNELRECTSGIXPROC) (Display* display, int screen, int channel, int x, int y, int w, int h);
typedef int ( * PFNGLXCHANNELRECTSYNCSGIXPROC) (Display* display, int screen, int channel, GLenum synctype);
typedef int ( * PFNGLXQUERYCHANNELDELTASSGIXPROC) (Display* display, int screen, int channel, int *x, int *y, int *w, int *h);
typedef int ( * PFNGLXQUERYCHANNELRECTSGIXPROC) (Display* display, int screen, int channel, int *dx, int *dy, int *dw, int *dh);

GLEWAPI PFNGLXBINDCHANNELTOWINDOWSGIXPROC glewXBindChannelToWindowSGIX;
GLEWAPI PFNGLXCHANNELRECTSGIXPROC glewXChannelRectSGIX;
GLEWAPI PFNGLXCHANNELRECTSYNCSGIXPROC glewXChannelRectSyncSGIX;
GLEWAPI PFNGLXQUERYCHANNELDELTASSGIXPROC glewXQueryChannelDeltasSGIX;
GLEWAPI PFNGLXQUERYCHANNELRECTSGIXPROC glewXQueryChannelRectSGIX;

#define glXBindChannelToWindowSGIX glewXBindChannelToWindowSGIX
#define glXChannelRectSGIX glewXChannelRectSGIX
#define glXChannelRectSyncSGIX glewXChannelRectSyncSGIX
#define glXQueryChannelDeltasSGIX glewXQueryChannelDeltasSGIX
#define glXQueryChannelRectSGIX glewXQueryChannelRectSGIX

GLEWAPI GLboolean GLXEW_SGIX_video_resize;

#endif /* GLX_SGIX_video_resize */

/* ---------------------- GLX_SGIX_visual_select_group --------------------- */

#ifndef GLX_SGIX_visual_select_group
#define GLX_SGIX_visual_select_group 1

#define GLX_VISUAL_SELECT_GROUP_SGIX 0x8028

GLEWAPI GLboolean GLXEW_SGIX_visual_select_group;

#endif /* GLX_SGIX_visual_select_group */

/* ---------------------------- GLX_SGI_cushion ---------------------------- */

#ifndef GLX_SGI_cushion
#define GLX_SGI_cushion 1

typedef void ( * PFNGLXCUSHIONSGIPROC) (Display* dpy, Window window, float cushion);

GLEWAPI PFNGLXCUSHIONSGIPROC glewXCushionSGI;

#define glXCushionSGI glewXCushionSGI

GLEWAPI GLboolean GLXEW_SGI_cushion;

#endif /* GLX_SGI_cushion */

/* ----------------------- GLX_SGI_make_current_read ----------------------- */

#ifndef GLX_SGI_make_current_read
#define GLX_SGI_make_current_read 1

typedef GLXDrawable ( * PFNGLXGETCURRENTREADDRAWABLESGIPROC) (void);
typedef Bool ( * PFNGLXMAKECURRENTREADSGIPROC) (Display* dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);

GLEWAPI PFNGLXGETCURRENTREADDRAWABLESGIPROC glewXGetCurrentReadDrawableSGI;
GLEWAPI PFNGLXMAKECURRENTREADSGIPROC glewXMakeCurrentReadSGI;

#define glXGetCurrentReadDrawableSGI glewXGetCurrentReadDrawableSGI
#define glXMakeCurrentReadSGI glewXMakeCurrentReadSGI

GLEWAPI GLboolean GLXEW_SGI_make_current_read;

#endif /* GLX_SGI_make_current_read */

/* -------------------------- GLX_SGI_swap_control ------------------------- */

#ifndef GLX_SGI_swap_control
#define GLX_SGI_swap_control 1

typedef int ( * PFNGLXSWAPINTERVALSGIPROC) (int interval);

GLEWAPI PFNGLXSWAPINTERVALSGIPROC glewXSwapIntervalSGI;

#define glXSwapIntervalSGI glewXSwapIntervalSGI

GLEWAPI GLboolean GLXEW_SGI_swap_control;

#endif /* GLX_SGI_swap_control */

/* --------------------------- GLX_SGI_video_sync -------------------------- */

#ifndef GLX_SGI_video_sync
#define GLX_SGI_video_sync 1

typedef int ( * PFNGLXGETVIDEOSYNCSGIPROC) (uint* count);
typedef int ( * PFNGLXWAITVIDEOSYNCSGIPROC) (int divisor, int remainder, unsigned int* count);

GLEWAPI PFNGLXGETVIDEOSYNCSGIPROC glewXGetVideoSyncSGI;
GLEWAPI PFNGLXWAITVIDEOSYNCSGIPROC glewXWaitVideoSyncSGI;

#define glXGetVideoSyncSGI glewXGetVideoSyncSGI
#define glXWaitVideoSyncSGI glewXWaitVideoSyncSGI

GLEWAPI GLboolean GLXEW_SGI_video_sync;

#endif /* GLX_SGI_video_sync */

/* --------------------- GLX_SUN_get_transparent_index --------------------- */

#ifndef GLX_SUN_get_transparent_index
#define GLX_SUN_get_transparent_index 1

typedef Status ( * PFNGLXGETTRANSPARENTINDEXSUNPROC) (Display* dpy, Window overlay, Window underlay, unsigned long *pTransparentIndex);

GLEWAPI PFNGLXGETTRANSPARENTINDEXSUNPROC glewXGetTransparentIndexSUN;

#define glXGetTransparentIndexSUN glewXGetTransparentIndexSUN

GLEWAPI GLboolean GLXEW_SUN_get_transparent_index;

#endif /* GLX_SUN_get_transparent_index */

/* ------------------------------------------------------------------------ */

extern GLboolean glxewGetExtension (const GLubyte* name);

#ifdef __cplusplus
}
#endif

#endif /* __glxew_h__ */

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

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"
#include "viewer.h"

//----------------------------------------------------------------------------//
//    Windows and D3D vars															  //
//----------------------------------------------------------------------------//

HWND	g_hWnd;

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;


//----------------------------------------------------------------------------//
// D3D initialation                                                           //
//----------------------------------------------------------------------------//


LRESULT InitD3D(HWND hWnd)
{
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;
	D3DDISPLAYMODE d3ddm;
	if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		return E_FAIL;
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed   = TRUE;

	d3dpp.BackBufferCount        = 2;
	d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality     =0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(theViewer.getFullscreen())
	{
		d3dpp.Windowed   = FALSE;
		d3dpp.BackBufferWidth=theViewer.getWidth();
		d3dpp.BackBufferHeight=theViewer.getHeight();

	}

	D3DCAPS9 Caps;
	g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&Caps);

	if(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
        if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pD3DDevice ) ) )
			return E_FAIL;
	}
	else
	{
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pD3DDevice ) ) )
			return E_FAIL;
	}

	
	return S_OK;
}

//----------------------------------------------------------------------------//
// Windows function                                                           //
//----------------------------------------------------------------------------//

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x=0,y=0;
	

    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
		case WM_KEYDOWN:
			theViewer.onKey((int) wParam, x, theViewer.getHeight() - y - 1);			
			break;
		case WM_MOUSEMOVE:
			x = LOWORD(lParam);  // horizontal position of cursor 
			y = HIWORD(lParam);  // vertical position of cursor
			theViewer.onMouseMove(x, theViewer.getHeight() - y - 1);
			break;
		case WM_LBUTTONDOWN:
			x = LOWORD(lParam);  // horizontal position of cursor 
			y = HIWORD(lParam);  // vertical position of cursor			
			theViewer.onMouseButtonDown(0, x, theViewer.getHeight() - y - 1);
			break;
		case WM_LBUTTONUP:
			x = LOWORD(lParam);  // horizontal position of cursor 
			y = HIWORD(lParam);  // vertical position of cursor			
			theViewer.onMouseButtonUp(0, x, theViewer.getHeight() - y - 1);
			break;
		case WM_RBUTTONDOWN:
			x = LOWORD(lParam);  // horizontal position of cursor 
			y = HIWORD(lParam);  // vertical position of cursor			
			theViewer.onMouseButtonDown(1, x, theViewer.getHeight() - y - 1);			
			break;
		case WM_RBUTTONUP:
			x = LOWORD(lParam);  // horizontal position of cursor 
			y = HIWORD(lParam);  // vertical position of cursor			
			theViewer.onMouseButtonUp(1, x, theViewer.getHeight() - y - 1);
			break;
		case WM_SIZING:
			LPRECT lprc = (LPRECT) lParam;
			theViewer.setDimension(lprc->left-lprc->right, lprc->bottom-lprc->top );
			break;

    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{	
	
	if(!theViewer.onCreate(lpCmdLine))
	{
		std::cerr << "Creation of the viewer failed." << std::endl;
		return -1;
	}

    // Register the window class.
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "cal3d_miniviewer", NULL };
    RegisterClassEx( &wc );

    // Create the application's window.
    g_hWnd = CreateWindow( "cal3d_miniviewer", "cal3d_miniviewer", 
                              WS_OVERLAPPEDWINDOW, 0, 0, theViewer.getWidth() , theViewer.getHeight(),
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	
	if( SUCCEEDED( InitD3D( g_hWnd ) ) )
    { 
        // Show the window
        ShowWindow( g_hWnd, SW_SHOWDEFAULT );
        UpdateWindow( g_hWnd );
		theViewer.onInit();

        // Enter the message loop
        MSG msg; 
        ZeroMemory( &msg, sizeof(msg) );
        while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
				{
					theViewer.onIdle();
                    theViewer.onRender();
				}
            }
    }

	theViewer.onShutdown();
    UnregisterClass( "cal3d_miniviewer", wc.hInstance );
	return 0;
}


subdirs(
cal3d_converter
cally/src    
#miniviewer_bada 
miniviewer_gl/src 
miniviewer_gl_vp/src 
#miniviewer_tizen/src
)
if(WIN32)
 
# DirectX SDK
	if (CMAKE_CL_64)
		set (DirectX_ARCHITECTURE x64)
	else ()
		set (DirectX_ARCHITECTURE x86)
	endif ()
	find_path (DirectX_ROOT_DIR
		Include/d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2010)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (June 2010)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (February 2010)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (February 2010)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (March 2009)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (March 2009)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (August 2008)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (August 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2008)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (June 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (March 2008)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (March 2008)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (November 2007)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (November 2007)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (August 2007)"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK (August 2007)"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK"
			"$ENV{ProgramFilesx86}/Microsoft DirectX SDK"
		DOC "DirectX SDK root directory"
	)
	if (DirectX_ROOT_DIR)
		set (DirectX_INC_SEARCH_PATH "${DirectX_ROOT_DIR}/Include")
		set (DirectX_LIB_SEARCH_PATH "${DirectX_ROOT_DIR}/Lib/${DirectX_ARCHITECTURE}")
		set (DirectX_BIN_SEARCH_PATH "${DirectX_ROOT_DIR}/Utilities/bin/x86")
	endif ()
find_path (DirectX_D3D9_INCLUDE_DIR d3d9.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "The directory where d3d9.h resides")

	find_path (DirectX_D3DX9_INCLUDE_DIR d3dx9.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "The directory where d3dx9.h resides")

	find_library (DirectX_D3D9_LIBRARY d3d9
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "The directory where d3d9 resides")

	find_library (DirectX_D3DX9_LIBRARY d3dx9
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "The directory where d3dx9 resides")

	if (DirectX_D3D9_INCLUDE_DIR AND DirectX_D3D9_LIBRARY)
		set (DirectX_D3D9_FOUND 1)
		if (DirectX_D3DX9_INCLUDE_DIR AND DirectX_D3DX9_LIBRARY)
			set (DirectX_D3DX9_FOUND 1)
		endif ()
	endif ()
endif(WIN32)
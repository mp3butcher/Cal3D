#include "cal3d_max_importer.h"


CMaxImporterApp theDll;


CMaxAnimationImportDesc CMaxImporterApp::m_maxAnimationImportDesc;
CMaxSkeletonImportDesc  CMaxImporterApp::m_maxSkeletonImportDesc;


//----------------------------------------------------------------------------//
// Exported functions                                                         //
//----------------------------------------------------------------------------//

__declspec(dllexport) ClassDesc *LibClassDesc(int id)
{
	return theDll.GetClassDesc(id);
}

__declspec(dllexport) const TCHAR *LibDescription()
{
	return "Cal3D Importer";
}

__declspec(dllexport) int LibNumberClasses()
{
	return theDll.GetClassDescCount();
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

//----------------------------------------------------------------------------//
// Get the class description handle for a given id                            //
//----------------------------------------------------------------------------//

ClassDesc* CMaxImporterApp::GetClassDesc(int id)
{
	switch(id)
	{
		case 0:
			m_maxAnimationImportDesc.SetInstance(m_hInstance);
			return &m_maxAnimationImportDesc;
    case 1:
      m_maxSkeletonImportDesc.SetInstance(m_hInstance);
      return &m_maxSkeletonImportDesc;
		default:
			return 0;
	}
}

//----------------------------------------------------------------------------//
// Get the number of class description handles                                //
//----------------------------------------------------------------------------//

int CMaxImporterApp::GetClassDescCount()
{
	return 2;
}

//----------------------------------------------------------------------------//
// Initialize the DLL instance                                                //
//----------------------------------------------------------------------------//

BOOL CMaxImporterApp::InitInstance() 
{
	static bool bInitialized = false;

	// initialize controls once
	if(!bInitialized)
	{
		DisableThreadLibraryCalls(m_hInstance);
		InitCustomControls(m_hInstance);
		InitCommonControls();
		bInitialized = true;
	}

	return TRUE;
}

//----------------------------------------------------------------------------//


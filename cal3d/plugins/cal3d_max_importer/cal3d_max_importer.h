#ifndef CAL3D_MAX_IMPORTER_H
#define CAL3D_MAX_IMPORTER_H


#include <afxwin.h>
#include <max.h>
#include "MaxAnimationImportDesc.h"
#include "MaxSkeletonImportDesc.h"

class CMaxImporterApp : public CWinApp {
public:
  ClassDesc* GetClassDesc(int id);
  int GetClassDescCount();

  BOOL InitInstance();

protected:
  static CMaxAnimationImportDesc m_maxAnimationImportDesc;
  static CMaxSkeletonImportDesc  m_maxSkeletonImportDesc;
};


extern CMaxImporterApp theDLL;


#endif

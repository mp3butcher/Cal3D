#ifndef MAX_SKELETON_IMPORT_DESC_H
#define MAX_SKELETON_IMPORT_DESC_H


#include <max.h>
#include <iparamb2.h>


#define MAX_SKELETON_IMPORT_ID Class_ID(0x630f18fc, 0x3cec6ce7)


class CMaxSkeletonImportDesc : public ClassDesc2
{
public:
  void SetInstance(HINSTANCE instance);

  const TCHAR* Category();
  Class_ID ClassID();
  const TCHAR* ClassName();
  void* Create(BOOL loading = FALSE);
  HINSTANCE HInstance();
  const TCHAR* InternalName(); 
  int IsPublic();
  SClass_ID SuperClassID();
  char * GetRsrcString(long n);

private:
  HINSTANCE m_instance;
};


#endif

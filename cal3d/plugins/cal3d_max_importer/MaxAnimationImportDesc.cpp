#include "MaxAnimationImportDesc.h"
#include "MaxAnimationImport.h"


void
CMaxAnimationImportDesc::SetInstance(HINSTANCE instance) {
  m_instance = instance;
}


const TCHAR*
CMaxAnimationImportDesc::Category() {
  return _T("Game Utilities");
}


Class_ID
CMaxAnimationImportDesc::ClassID() {
  return MAX_ANIMATION_IMPORT_ID;
}


const TCHAR*
CMaxAnimationImportDesc::ClassName() {
  return _T("Cal3D Animation Import");
}


void*
CMaxAnimationImportDesc::Create(BOOL loading) {
  return new CMaxAnimationImport();
}


HINSTANCE
CMaxAnimationImportDesc::HInstance() {
  return m_instance;
}


const TCHAR*
CMaxAnimationImportDesc::InternalName()
{
  return _T("Cal3D_Animation_Import");
} 


int
CMaxAnimationImportDesc::IsPublic()
{
  return 1;
}


SClass_ID
CMaxAnimationImportDesc::SuperClassID()
{
  return SCENE_IMPORT_CLASS_ID;
}

char * CMaxAnimationImportDesc::GetRsrcString(long n)
{
	return NULL;
}
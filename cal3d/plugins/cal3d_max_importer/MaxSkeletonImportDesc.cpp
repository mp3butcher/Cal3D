#include "MaxSkeletonImportDesc.h"
#include "MaxSkeletonImport.h"


void
CMaxSkeletonImportDesc::SetInstance(HINSTANCE instance) {
  m_instance = instance;
}


const TCHAR*
CMaxSkeletonImportDesc::Category() {
  return _T("Game Utilities");
}


Class_ID
CMaxSkeletonImportDesc::ClassID() {
  return MAX_SKELETON_IMPORT_ID;
}


const TCHAR*
CMaxSkeletonImportDesc::ClassName() {
  return _T("Cal3D Skeleton Import");
}


void*
CMaxSkeletonImportDesc::Create(BOOL loading) {
  return new CMaxSkeletonImport();
}


HINSTANCE
CMaxSkeletonImportDesc::HInstance() {
  return m_instance;
}


const TCHAR*
CMaxSkeletonImportDesc::InternalName()
{
  return _T("Cal3D_Skeleton_Import");
} 


int
CMaxSkeletonImportDesc::IsPublic()
{
  return 1;
}


SClass_ID
CMaxSkeletonImportDesc::SuperClassID()
{
  return SCENE_IMPORT_CLASS_ID;
}

char * CMaxSkeletonImportDesc::GetRsrcString(long n)
{
	return NULL;
}

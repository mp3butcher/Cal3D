#include <afxdlgs.h>
#include <cal3d/cal3d.h>
#include "bones.h"
#include "MaxSkeletonImport.h"


const TCHAR*
CMaxSkeletonImport::AuthorName() {
  return _T("Chad Austin");
}


const TCHAR*
CMaxSkeletonImport::CopyrightMessage() {
  return _T("(c) Chad Austin 2003");
}


int
CMaxSkeletonImport::DoImport(
  const TCHAR* name,
  ImpInterface* ii,
  Interface* i,
  BOOL suppressPrompts)
{
  HWND window = i->GetMAXHWnd();

  ImpNode* parent = ii->CreateNode();
  if (parent) {
    void* object = CreateInstance(HELPER_CLASS_ID, Class_ID(BONE_CLASS_ID, 0));
    if (!object) {
      delete parent;
      MessageBox(window, "Error2", "Import", MB_OK);
      return IMPEXP_FAIL;
    }

    BoneObj* bone = (BoneObj*)object;

	  parent->Reference(bone);
	  Matrix3 tm;
	  tm.IdentityMatrix();		// Reset initial matrix to identity
	  tm.SetTrans(Point3(5,6,7));			// Add in the center point
	  parent->SetTransform(0, tm);
	  ii->AddNodeToScene(parent);
	  parent->SetName("PARENT");
	}

  ImpNode* child = ii->CreateNode();
  if (child) {
    void* object = CreateInstance(HELPER_CLASS_ID, Class_ID(BONE_CLASS_ID, 0));
    if (!object) {
      delete child;
      MessageBox(window, "Error3", "Import", MB_OK);
      return IMPEXP_FAIL;
    }

    BoneObj* bone = (BoneObj*)object;

    child->Reference(bone);
    Matrix3 tm;
    tm.IdentityMatrix();
    tm.SetTrans(Point3(-1, -2, -3));
    child->SetTransform(0, tm);
    ii->AddNodeToScene(child);
    child->SetName("CHILD");
  }

  parent->GetINode()->AttachChild(child->GetINode(), 0);

  ImpNode* grandchild = ii->CreateNode();
  if (grandchild) {
    void* object = CreateInstance(HELPER_CLASS_ID, Class_ID(BONE_CLASS_ID, 0));
    if (!object) {
      delete grandchild;
      MessageBox(window, "Error3", "Import", MB_OK);
      return IMPEXP_FAIL;
    }

    BoneObj* bone = (BoneObj*)object;

    grandchild->Reference(bone);
    Matrix3 tm;
    tm.IdentityMatrix();
    tm.SetTrans(Point3(10, -5, 15));
    grandchild->SetTransform(0, tm);
    ii->AddNodeToScene(child);
    grandchild->SetName("CHILD");
  }

  child->GetINode()->AttachChild(grandchild->GetINode(), 0);
  
  parent->GetINode()->ShowBone(1);
  child->GetINode()->ShowBone(1);
  grandchild->GetINode()->ShowBone(1);


  return IMPEXP_SUCCESS;



#if 0
  ImpNode* node = ii->CreateNode();
  if (!node) {
    return IMPEXP_FAIL;
  }

/*
  INode* inode = node->GetINode();
  inode->SetBoneNodeOnOff(TRUE, 0);

  Matrix3 m;
  m.IdentityMatrix();
  m.SetTrans(Point3(9, 4, 1));
  node->SetTransform(0, m);

  node->SetName("NODE");
*/
  ii->AddNodeToScene(node);
  return IMPEXP_SUCCESS;
#endif

#if 0
  HWND window = i->GetMAXHWnd();
  CFileDialog fileDialog(TRUE, "xsf", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         0, CWnd::FromHandle(window));
  if (fileDialog.DoModal() != IDOK) {
    return IMPEXP_CANCEL;
  }

  CalLoader loader;
  CString skeleton = fileDialog.GetPathName();
  CalCoreSkeleton* skel = loader.loadCoreSkeleton(std::string(skeleton));
  if (!skel) {
    MessageBox(
      window, "Loading skeleton file failed",
      "Import Cal3D Skeleton", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  


  skel->destroy();
  delete skel;

  return IMPEXP_FAIL;
#endif


#if 0
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  HWND window = i->GetMAXHWnd();

  CFileDialog fileDialog(TRUE, "xsf", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         0, CWnd::FromHandle(window));
  if (fileDialog.DoModal() != IDOK) {
    return IMPEXP_CANCEL;
  }

  CString skeleton = fileDialog.GetPathName();
  
  CalLoader loader;
  CalCoreSkeleton* skel = loader.loadCoreSkeleton(std::string(skeleton));
  if (!skel) {
    MessageBox(
      window, "Loading skeleton file failed",
      "Import Cal3D Skeleton", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  CalCoreSkeleton* anim = loader.loadCoreSkeleton(name);
  if (!anim) {
    MessageBox(
      window, "Loading animation file failed",
      "Import Cal3D Skeleton", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  typedef std::list<CalCoreTrack*> CoreTrackList;
  CoreTrackList& trackList = anim->getListCoreTrack();
  for (CoreTrackList::iterator itr = trackList.begin(); itr != trackList.end(); ++itr) {
    CalCoreTrack* track = *itr;

    int boneId = track->getCoreBoneId();
    CalCoreBone* bone = skel->getCoreBone(boneId);
    if (!bone) continue;

    INode* node = i->GetINodeByName(bone->getName().c_str());
    if (!node) continue;

    unsigned kfCount = track->getCoreKeyframeCount();

    SuspendAnimate();
    AnimateOn();
    for (unsigned i = 0; i < kfCount; ++i) {
      CalCoreKeyframe* kf = track->getCoreKeyframe(i);
      CalQuaternion kf_q = kf->getRotation();
      CalVector     kf_v = kf->getTranslation();
      TimeValue     time = SecToTicks(kf->getTime());

      Matrix3 tm;
      tm.IdentityMatrix();
      Quat(kf_q.x, kf_q.y, kf_q.z, kf_q.w).MakeMatrix(tm);
      tm.SetTrans(Point3(kf_v.x, kf_v.y, kf_v.z));

      INode* parent = node->GetParentNode();
      if (parent) {
        tm *= parent->GetNodeTM(time);
      }

      node->SetNodeTM(time, tm);
    }
    ResumeAnimate();

/*
    typedef std::map<float, CalCoreKeyframe*> KeyMap;
    KeyMap& keys = track->getMapCoreKeyframe();

    int mapsize = sizeof(keys);

    int size = keys.size();

    int idx = 0;
    for (KeyMap::iterator mi = keys.begin(); mi != keys.end(); ++mi) {
      Point3 p;
      CalCoreKeyframe* kf = mi->second;
      p.x = kf->getTranslation().x;
      p.y = kf->getTranslation().y;
      p.z = kf->getTranslation().z;
      pos->SetValue(SecToTicks(mi->first), &p);
    }
*/

/*
    IKeyControl* kc = GetKeyControlInterface(pos);
    if (!kc) continue;

    typedef std::map<float, CalCoreKeyframe*> KeyMap;
    KeyMap& keys = track->getMapCoreKeyframe();
    kc->SetNumKeys(keys.size());

    int idx = 0;
    for (KeyMap::iterator mi = keys.begin(); mi != keys.end(); ++mi) {
      ITCBPoint3Key key;
      key.time = SecToTicks(mi->first);
      key.tens = 0;
      key.cont = 0;
      key.bias = 0;
      key.easeIn = 25.0;
      key.easeOut = 25.0;
      key.val.x = mi->second->getTranslation().x;
      key.val.y = mi->second->getTranslation().y;
      key.val.z = mi->second->getTranslation().z;

      kc->SetKey(idx++, &key);
    }

    kc->SortKeys();
*/
  }
  

  anim->destroy();
  delete anim;

  skel->destroy();
  delete skel;
  
  return IMPEXP_SUCCESS;
#endif
}


const TCHAR*
CMaxSkeletonImport::Ext(int n) {
  switch (n) {
    case 0:  return _T("csf");
    case 1:  return _T("xsf");
    default: return _T("");
  }
}


int
CMaxSkeletonImport::ExtCount() {
  return 2;
}


const TCHAR*
CMaxSkeletonImport::LongDesc() {
  return _T("Cal3D Skeleton File");
}
	

const TCHAR*
CMaxSkeletonImport::ShortDesc() {
  return LongDesc();
}


const TCHAR*
CMaxSkeletonImport::OtherMessage1() {
  return _T("");
}


const TCHAR*
CMaxSkeletonImport::OtherMessage2() {
  return _T("");
}


void
CMaxSkeletonImport::ShowAbout(HWND window) {
  MessageBox(
    window,
    _T("Cal3D Skeleton Import Plug-In\n"
       "(c) Chad Austin 2003"),
    _T("About Cal3D Skeleton Importer"),
    MB_OK);
}


unsigned int
CMaxSkeletonImport::Version() {
  return 100;
}

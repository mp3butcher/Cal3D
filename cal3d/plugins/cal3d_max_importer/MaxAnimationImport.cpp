#include <afxdlgs.h>
#include <cal3d/cal3d.h>
#include "MaxAnimationImport.h"


const TCHAR*
CMaxAnimationImport::AuthorName() {
  return _T("Chad Austin");
}


const TCHAR*
CMaxAnimationImport::CopyrightMessage() {
  return _T("(c) Chad Austin 2003");
}


int
CMaxAnimationImport::DoImport(
  const TCHAR* name,
  ImpInterface* ii,
  Interface* i,
  BOOL suppressPrompts)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  HWND window = i->GetMAXHWnd();

  CFileDialog fileDialog(TRUE, "xsf", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         0, CWnd::FromHandle(window));
  if (fileDialog.DoModal() != IDOK) {
    return IMPEXP_CANCEL;
  }

  CString skeleton = fileDialog.GetPathName();
  
  CalCoreSkeleton* skel = CalLoader::loadCoreSkeleton(std::string(skeleton));
  if (!skel) {
    MessageBox(
      window, "Loading skeleton file failed",
      "Import Cal3D Animation", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  CalCoreAnimation* anim = CalLoader::loadCoreAnimation(name);
  if (!anim) {
    MessageBox(
      window, "Loading animation file failed",
      "Import Cal3D Animation", MB_OK | MB_ICONERROR);
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
  

  delete anim;

  delete skel;
  
  return IMPEXP_SUCCESS;
}


const TCHAR*
CMaxAnimationImport::Ext(int n) {
  switch (n) {
    case 0:  return _T("caf");
    case 1:  return _T("xaf");
    default: return _T("");
  }
}


int
CMaxAnimationImport::ExtCount() {
  return 2;
}


const TCHAR*
CMaxAnimationImport::LongDesc() {
  return _T("Cal3D Animation File");
}
	

const TCHAR*
CMaxAnimationImport::ShortDesc() {
  return LongDesc();
}


const TCHAR*
CMaxAnimationImport::OtherMessage1() {
  return _T("");
}


const TCHAR*
CMaxAnimationImport::OtherMessage2() {
  return _T("");
}


void
CMaxAnimationImport::ShowAbout(HWND window) {
  MessageBox(
    window,
    _T("Cal3D Animation Import Plug-In\n"
       "(c) Chad Austin 2003"),
    _T("About Cal3D Animation Importer"),
    MB_OK);
}


unsigned int
CMaxAnimationImport::Version() {
  return 100;
}

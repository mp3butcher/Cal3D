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
  
  CalCoreSkeletonPtr skel = CalLoader::loadCoreSkeleton(std::string(skeleton));
  if (!skel) {
    MessageBox(
      window, "Loading skeleton file failed",
      "Import Cal3D Animation", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  CalCoreAnimationPtr anim = CalLoader::loadCoreAnimation(name);
  if (!anim) {
    MessageBox(
      window, "Loading animation file failed",
      "Import Cal3D Animation", MB_OK | MB_ICONERROR);
    return IMPEXP_FAIL;
  }

  // Get the pose information in the animation
  const std::vector<CalTransform>& poses = anim->getPoses();
  unsigned int num_poses = poses.size() / anim->getTrackCount();

  // Calculate the time_per_frame incorrectly since the duration for animations
  // is stored incorrectly.
  float time_per_frame = anim->getDuration() / num_poses;

  // Import each track
  for (unsigned track_id = 0; track_id < anim->getTrackCount(); ++track_id)
  {
    // Get the core bone mapped to the animation
    int bone_id = anim->getBoneAssignment(track_id);
    CalCoreBone* bone = skel->getCoreBone(bone_id);
    if (!bone) continue;

    // Get the max node for the bone
    INode* node = i->GetINodeByName(bone->getName().c_str());
    if (!node) continue;

    SuspendAnimate();
    AnimateOn();

    // Add each pose keyframe in the track
    float keyframe_time = 0.0f;
    for (unsigned keyframe_index = 0; keyframe_index < num_poses; ++keyframe_index)
    {
      // Get the keyframe data
      const CalTransform& pose_coord_sys = poses[(keyframe_index * anim->getTrackCount()) + track_id];
      const CalVector     &kf_v = pose_coord_sys.getTranslation();
      const CalQuaternion &kf_q = pose_coord_sys.getRotation();
      TimeValue time = SecToTicks(keyframe_time);

      // Convert to Max math
      Matrix3 tm;
      tm.IdentityMatrix();
      Quat(kf_q.x, kf_q.y, kf_q.z, kf_q.w).MakeMatrix(tm);
      tm.SetTrans(Point3(kf_v.x, kf_v.y, kf_v.z));

      // Convert the transform to world space
      INode* parent = node->GetParentNode();
      if (parent)
      {
        tm *= parent->GetNodeTM(time);
      }

      // Set the new transform on the node
      node->SetNodeTM(time, tm);

      keyframe_time += time_per_frame;
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

//****************************************************************************//
// cal3d_wrapper.h                                                            //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//           (C) 2002 Laurent 'Maxun' Desmecht                                //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CAL3D_WRAPPER_H
#define CAL_CAL3D_WRAPPER_H

//****************************************************************************//
// Defines for Win32 and MingW32                                              //
//****************************************************************************//

#ifdef _WIN32

#ifdef __MINGW32__

#define CAL3D_WRAPPER_API

#else

#pragma warning(disable : 4251)
#pragma warning(disable : 4786)

#ifdef CAL3D_WRAPPER_EXPORTS
#define CAL3D_WRAPPER_API __declspec(dllexport)
#else
#define CAL3D_WRAPPER_API __declspec(dllimport)
#endif

#endif

#endif

//****************************************************************************//
// Defines for Linux, Cygwin, Sun and Mips                                    //
//****************************************************************************//

#if defined(__linux__) || (__CYGWIN__) || (sun) || (mips)

#define CAL3D_WRAPPER_API

#endif

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

#ifndef CAL3D_WRAPPER_EXPORTS

struct CalAnimation;
struct CalAnimationAction;
struct CalAnimationCycle;
struct CalBone;
struct CalCoreAnimation;
struct CalCoreBone;
struct CalCoreKeyframe;
struct CalCoreMaterial;
struct CalCoreMesh;
struct CalCoreModel;
struct CalCoreSkeleton;
struct CalCoreSubmesh;
struct CalCoreTrack;
struct CalLoader;
struct CalMesh;
struct CalMixer;
struct CalModel;
struct CalPhysique;
struct CalPlatform;
struct CalQuaternion;
struct CalRenderer;
struct CalSaver;
struct CalSkeleton;
struct CalSpringSystem;
struct CalSubmesh;
struct CalVector;

#endif

typedef void *CalUserData;

//****************************************************************************//
// Substitute for the C++ 'bool' type                                         //
//****************************************************************************//

enum Boolean
{
  False = 0,
  True = 1
};

//****************************************************************************//
// "C" wrapper functions declaration                                          //
//****************************************************************************//

#ifdef __cplusplus
extern "C"
{
#endif

//****************************************************************************//
// CalAnimation wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API enum CalAnimationType
  {
    ANIMATION_TYPE_NONE = 0,
    ANIMATION_TYPE_CYCLE,
    ANIMATION_TYPE_POSE,
    ANIMATION_TYPE_ACTION
  };

  CAL3D_WRAPPER_API enum CalAnimationState
  {
    ANIMATION_STATE_NONE = 0,
    ANIMATION_STATE_SYNC,
    ANIMATION_STATE_ASYNC,
    ANIMATION_STATE_IN,
    ANIMATION_STATE_STEADY,
    ANIMATION_STATE_OUT
  };

  CAL3D_WRAPPER_API Boolean CalAnimation_Create(CalAnimation *self, CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimation_Delete(CalAnimation *self);
  CAL3D_WRAPPER_API void CalAnimation_Destroy(CalAnimation *self);
  CAL3D_WRAPPER_API CalCoreAnimation *CalAnimation_GetCoreAnimation(CalAnimation *self);
  CAL3D_WRAPPER_API CalAnimationState CalAnimation_GetState(CalAnimation *self);
  CAL3D_WRAPPER_API float CalAnimation_GetTime(CalAnimation *self);
  CAL3D_WRAPPER_API CalAnimationType CalAnimation_GetType(CalAnimation *self);
  CAL3D_WRAPPER_API float CalAnimation_GetWeight(CalAnimation *self);

//****************************************************************************//
// CalAnimationAction wrapper functions declaration                           //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalAnimationAction_Create(CalAnimationAction *self, CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimationAction_Delete(CalAnimationAction *self);
  CAL3D_WRAPPER_API void CalAnimationAction_Destroy(CalAnimationAction *self);
  CAL3D_WRAPPER_API Boolean CalAnimationAction_Execute(CalAnimationAction *self, float delayIn, float delayOut);
  CAL3D_WRAPPER_API CalAnimationAction *CalAnimationAction_New();
  CAL3D_WRAPPER_API Boolean CalAnimationAction_Update(CalAnimationAction *self, float deltaTime);

//****************************************************************************//
// CalAnimationCycle wrapper functions declaration                            //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalAnimationCycle_Blend(CalAnimationCycle *self, float weight, float delay);
  CAL3D_WRAPPER_API Boolean CalAnimationCycle_Create(CalAnimationCycle *self, CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimationCycle_Delete(CalAnimationCycle *self);
  CAL3D_WRAPPER_API void CalAnimationCycle_Destroy(CalAnimationCycle *self);
  CAL3D_WRAPPER_API CalAnimationCycle *CalAnimationCycle_New();
  CAL3D_WRAPPER_API void CalAnimationCycle_SetAsync(CalAnimationCycle *self, float time, float duration);
  CAL3D_WRAPPER_API Boolean CalAnimationCycle_Update(CalAnimationCycle *self, float deltaTime);

//****************************************************************************//
// CalBone wrapper functions declaration                                      //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalBone_BlendState(CalBone *self, float weight, CalVector *pTranslation, CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalBone_CalculateState(CalBone *self);
  CAL3D_WRAPPER_API void CalBone_ClearState(CalBone *self);
  CAL3D_WRAPPER_API Boolean CalBone_Create(CalBone *self, CalCoreBone *pCoreBone);
  CAL3D_WRAPPER_API void CalBone_Delete(CalBone *self);
  CAL3D_WRAPPER_API void CalBone_Destroy(CalBone *self);
  CAL3D_WRAPPER_API CalCoreBone *CalBone_GetCoreBone(CalBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalBone_GetRotation(CalBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalBone_GetRotationAbsolute(CalBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalBone_GetRotationBoneSpace(CalBone *self);
  CAL3D_WRAPPER_API CalVector *CalBone_GetTranslation(CalBone *self);
  CAL3D_WRAPPER_API CalVector *CalBone_GetTranslationAbsolute(CalBone *self);
  CAL3D_WRAPPER_API CalVector *CalBone_GetTranslationBoneSpace(CalBone *self);
  CAL3D_WRAPPER_API void CalBone_LockState(CalBone *self);
  CAL3D_WRAPPER_API CalBone *CalBone_New();
  CAL3D_WRAPPER_API void CalBone_SetSkeleton(CalBone *self, CalSkeleton *pSkeleton);

//****************************************************************************//
// CalCoreAnimation wrapper functions declaration                             //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreAnimation_AddCoreTrack(CalCoreAnimation *self, CalCoreTrack *pCoreTrack);
  CAL3D_WRAPPER_API Boolean CalCoreAnimation_Create(CalCoreAnimation *self);
  CAL3D_WRAPPER_API void CalCoreAnimation_Delete(CalCoreAnimation *self);
  CAL3D_WRAPPER_API void CalCoreAnimation_Destroy(CalCoreAnimation *self);
  CAL3D_WRAPPER_API CalCoreTrack *CalCoreAnimation_GetCoreTrack(CalCoreAnimation *self, int coreBoneId);
  CAL3D_WRAPPER_API float CalCoreAnimation_GetDuration(CalCoreAnimation *self);
//  CAL3D_WRAPPER_API std::list<CalCoreTrack *>& CalCoreAnimation_GetListCoreTrack(CalCoreAnimation *self);
  CAL3D_WRAPPER_API CalCoreAnimation *CalCoreAnimation_New();
  CAL3D_WRAPPER_API void CalCoreAnimation_SetDuration(CalCoreAnimation *self, float duration);

//****************************************************************************//
// CalCoreBone wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreBone_AddChildId(CalCoreBone *self, int childId);
  CAL3D_WRAPPER_API void CalCoreBone_CalculateState(CalCoreBone *self);
  CAL3D_WRAPPER_API Boolean CalCoreBone_Create(CalCoreBone *self, char *strName);
  CAL3D_WRAPPER_API void CalCoreBone_Delete(CalCoreBone *self);
  CAL3D_WRAPPER_API void CalCoreBone_Destroy(CalCoreBone *self);
//  CAL3D_WRAPPER_API std::list<int>& CalCoreBone_GetListChildId(CalCoreBone *self);
  CAL3D_WRAPPER_API char *CalCoreBone_GetName(CalCoreBone *self);
  CAL3D_WRAPPER_API int CalCoreBone_GetParentId(CalCoreBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalCoreBone_GetRotation(CalCoreBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalCoreBone_GetRotationAbsolute(CalCoreBone *self);
  CAL3D_WRAPPER_API CalQuaternion *CalCoreBone_GetRotationBoneSpace(CalCoreBone *self);
  CAL3D_WRAPPER_API CalVector *CalCoreBone_GetTranslation(CalCoreBone *self);
  CAL3D_WRAPPER_API CalVector *CalCoreBone_GetTranslationAbsolute(CalCoreBone *self);
  CAL3D_WRAPPER_API CalVector *CalCoreBone_GetTranslationBoneSpace(CalCoreBone *self);
  CAL3D_WRAPPER_API CalUserData CalCoreBone_GetUserData(CalCoreBone *self);
  CAL3D_WRAPPER_API CalCoreBone *CalCoreBone_New();
  CAL3D_WRAPPER_API void CalCoreBone_SetCoreSkeleton(CalCoreBone *self, CalCoreSkeleton *pCoreSkeleton);
  CAL3D_WRAPPER_API void CalCoreBone_SetParentId(CalCoreBone *self, int parentId);
  CAL3D_WRAPPER_API void CalCoreBone_SetRotation(CalCoreBone *self, CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreBone_SetRotationBoneSpace(CalCoreBone *self, CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreBone_SetTranslation(CalCoreBone *self, CalVector *pTranslation);
  CAL3D_WRAPPER_API void CalCoreBone_SetTranslationBoneSpace(CalCoreBone *self, CalVector *pTranslation);
  CAL3D_WRAPPER_API void CalCoreBone_SetUserData(CalCoreBone *self, CalUserData userData);

//****************************************************************************//
// CalCoreKeyframe wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreKeyframe_Create(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API void CalCoreKeyframe_Delete(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API void CalCoreKeyframe_Destroy(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API CalQuaternion *CalCoreKeyframe_GetRotation(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API float CalCoreKeyframe_GetTime(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API CalVector *CalCoreKeyframe_GetTranslation(CalCoreKeyframe *self);
  CAL3D_WRAPPER_API CalCoreKeyframe *CalCoreKeyframe_New();
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetRotation(CalCoreKeyframe *self, CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetTime(CalCoreKeyframe *self, float time);
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetTranslation(CalCoreKeyframe *self, CalVector *pTranslation);

//****************************************************************************//
// CalCoreMaterial wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreMaterial_Create(CalCoreMaterial *self);
  CAL3D_WRAPPER_API void CalCoreMaterial_Delete(CalCoreMaterial *self);
  CAL3D_WRAPPER_API void CalCoreMaterial_Destroy(CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetAmbientColor(CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetDiffuseColor(CalCoreMaterial *self);
  CAL3D_WRAPPER_API int CalCoreMaterial_GetMapCount(CalCoreMaterial *self);
  CAL3D_WRAPPER_API char *CalCoreMaterial_GetMapFilename(CalCoreMaterial *self, int mapId);
  CAL3D_WRAPPER_API CalUserData CalCoreMaterial_GetMapUserData(CalCoreMaterial *self, int mapId);
  CAL3D_WRAPPER_API float CalCoreMaterial_GetShininess(CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetSpecularColor(CalCoreMaterial *self);
  CAL3D_WRAPPER_API CalUserData CalCoreMaterial_GetUserData(CalCoreMaterial *self);
//  CAL3D_WRAPPER_API std::vector<Map>& CalCoreMaterial_GetVectorMap(CalCoreMaterial *self);
  CAL3D_WRAPPER_API CalCoreMaterial *CalCoreMaterial_New();
  CAL3D_WRAPPER_API Boolean CalCoreMaterial_Reserve(CalCoreMaterial *self, int mapCount);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetAmbientColor(CalCoreMaterial *self, CalCoreMaterial::Color *pAmbientColor);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetDiffuseColor(CalCoreMaterial *self, CalCoreMaterial::Color *pDiffuseColor);
//  CAL3D_WRAPPER_API Boolean CalCoreMaterial_SetMap(CalCoreMaterial *self, int mapId, CalCoreMaterial::Map *pMap);
  CAL3D_WRAPPER_API Boolean CalCoreMaterial_SetMapUserData(CalCoreMaterial *self, int mapId, CalUserData userData);
  CAL3D_WRAPPER_API void CalCoreMaterial_SetShininess(CalCoreMaterial *self, float shininess);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetSpecularColor(CalCoreMaterial *self, CalCoreMaterial::Color *pSpecularColor);
  CAL3D_WRAPPER_API void CalCoreMaterial_SetUserData(CalCoreMaterial *self, CalUserData userData);

//****************************************************************************//
// CalCoreMesh wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreMesh_AddCoreSubmesh(CalCoreMesh *self, CalCoreSubmesh *pCoreSubmesh);
  CAL3D_WRAPPER_API Boolean CalCoreMesh_Create(CalCoreMesh *self);
  CAL3D_WRAPPER_API void CalCoreMesh_Delete(CalCoreMesh *self);
  CAL3D_WRAPPER_API void CalCoreMesh_Destroy(CalCoreMesh *self);
  CAL3D_WRAPPER_API CalCoreSubmesh *CalCoreMesh_GetCoreSubmesh(CalCoreMesh *self, int id);
  CAL3D_WRAPPER_API int CalCoreMesh_GetCoreSubmeshCount(CalCoreMesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh *>& CalCoreMesh_GetVectorCoreSubmesh(CalCoreMesh *self);
  CAL3D_WRAPPER_API CalCoreMesh *CalCoreMesh_New();

//****************************************************************************//
// CalCoreModel wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreModel_AddCoreAnimation(CalCoreModel *self, CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API int CalCoreModel_AddCoreMaterial(CalCoreModel *self, CalCoreMaterial *pCoreMaterial);
  CAL3D_WRAPPER_API int CalCoreModel_AddCoreMesh(CalCoreModel *self, CalCoreMesh *pCoreMesh);
  CAL3D_WRAPPER_API Boolean CalCoreModel_Create(CalCoreModel *self, char *strName);
  CAL3D_WRAPPER_API Boolean CalCoreModel_CreateCoreMaterialThread(CalCoreModel *self, int coreMaterialThreadId);
  CAL3D_WRAPPER_API void CalCoreModel_Delete(CalCoreModel *self);
  CAL3D_WRAPPER_API void CalCoreModel_Destroy(CalCoreModel *self);
  CAL3D_WRAPPER_API CalCoreAnimation *CalCoreModel_GetCoreAnimation(CalCoreModel *self, int coreAnimationId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreAnimationCount(CalCoreModel *self);
  CAL3D_WRAPPER_API CalCoreMaterial *CalCoreModel_GetCoreMaterial(CalCoreModel *self, int coreMaterialId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMaterialCount(CalCoreModel *self);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId);
  CAL3D_WRAPPER_API CalCoreMesh *CalCoreModel_GetCoreMesh(CalCoreModel *self, int coreMeshId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMeshCount(CalCoreModel *self);
  CAL3D_WRAPPER_API CalCoreSkeleton *CalCoreModel_GetCoreSkeleton(CalCoreModel *self);
  CAL3D_WRAPPER_API CalUserData CalCoreModel_GetUserData(CalCoreModel *self);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreAnimation(CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreMaterial(CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreMesh(CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API Boolean CalCoreModel_LoadCoreSkeleton(CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API CalCoreModel *CalCoreModel_New();
  CAL3D_WRAPPER_API Boolean CalCoreModel_SaveCoreAnimation(CalCoreModel *self, char *strFilename, int coreAnimtionId);
  CAL3D_WRAPPER_API Boolean CalCoreModel_SaveCoreMaterial(CalCoreModel *self, char *strFilename, int coreMaterialId);
  CAL3D_WRAPPER_API Boolean CalCoreModel_SaveCoreMesh(CalCoreModel *self, char *strFilename, int coreMeshId);
  CAL3D_WRAPPER_API Boolean CalCoreModel_SaveCoreSkeleton(CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API Boolean CalCoreModel_SetCoreMaterialId(CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId);
  CAL3D_WRAPPER_API void CalCoreModel_SetCoreSkeleton(CalCoreModel *self, CalCoreSkeleton *pCoreSkeleton);
  CAL3D_WRAPPER_API void CalCoreModel_SetUserData(CalCoreModel *self, CalUserData userData);

//****************************************************************************//
// CalCoreSkeleton wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreSkeleton_AddCoreBone(CalCoreSkeleton *self, CalCoreBone *pCoreBone);
  CAL3D_WRAPPER_API void CalCoreSkeleton_CalculateState(CalCoreSkeleton *self);
  CAL3D_WRAPPER_API Boolean CalCoreSkeleton_Create(CalCoreSkeleton *self);
  CAL3D_WRAPPER_API void CalCoreSkeleton_Delete(CalCoreSkeleton *self);
  CAL3D_WRAPPER_API void CalCoreSkeleton_Destroy(CalCoreSkeleton *self);
  CAL3D_WRAPPER_API CalCoreBone *CalCoreSkeleton_GetCoreBone(CalCoreSkeleton *self, int coreBoneId);
  CAL3D_WRAPPER_API int CalCoreSkeleton_GetCoreBoneId(CalCoreSkeleton *self, char *strName);
//  CAL3D_WRAPPER_API std::list<int>& CalCoreSkeleton_GetListRootCoreBoneId(CalCoreSkeleton *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreBone *>& CalCoreSkeleton_GetVectorCoreBone(CalCoreSkeleton *self);
  CAL3D_WRAPPER_API CalCoreSkeleton *CalCoreSkeleton_New();

//****************************************************************************//
// CalCoreSubmesh wrapper functions declaration                               //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_Create(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API void CalCoreSubmesh_Delete(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API void CalCoreSubmesh_Destroy(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetCoreMaterialThreadId(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetFaceCount(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetLodCount(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetSpringCount(CalCoreSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh::Face>& CalCoreSubmesh_GetVectorFace(CalCoreSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh::PhysicalProperty>& CalCoreSubmesh_GetVectorPhysicalProperty(CalCoreSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh::Spring>& CalCoreSubmesh_GetVectorSpring(CalCoreSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& CalCoreSubmesh_GetVectorVectorTextureCoordinate(CalCoreSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh::Vertex>& CalCoreSubmesh_GetVectorVertex(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetVertexCount(CalCoreSubmesh *self);
  CAL3D_WRAPPER_API CalCoreSubmesh *CalCoreSubmesh_New();
  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_Reserve(CalCoreSubmesh *self, int vertexCount, int textureCoordinateCount, int faceCount, int springCount);
  CAL3D_WRAPPER_API void CalCoreSubmesh_SetCoreMaterialThreadId(CalCoreSubmesh *self, int coreMaterialThreadId);
//  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_SetFace(CalCoreSubmesh *self, int faceId, CalCoreSubmesh::Face *pFace);
  CAL3D_WRAPPER_API void CalCoreSubmesh_SetLodCount(CalCoreSubmesh *self, int lodCount);
//  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_SetPhysicalProperty(CalCoreSubmesh *self, int vertexId, CalCoreSubmesh::PhysicalProperty *pPhysicalProperty);
//  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_SetSpring(CalCoreSubmesh *self, int springId, CalCoreSubmesh::Spring *pSpring);
//  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_SetTextureCoordinate(CalCoreSubmesh *self, int vertexId, int textureCoordinateId, CalCoreSubmesh::TextureCoordinate *pTextureCoordinate);
//  CAL3D_WRAPPER_API Boolean CalCoreSubmesh_SetVertex(CalCoreSubmesh *self, int vertexId, CalCoreSubmesh::Vertex *pVertex);

//****************************************************************************//
// CalCoreTrack wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalCoreTrack_AddCoreKeyframe(CalCoreTrack *self, CalCoreKeyframe *pCoreKeyframe);
  CAL3D_WRAPPER_API Boolean CalCoreTrack_Create(CalCoreTrack *self);
  CAL3D_WRAPPER_API void CalCoreTrack_Delete(CalCoreTrack *self);
  CAL3D_WRAPPER_API void CalCoreTrack_Destroy(CalCoreTrack *self);
  CAL3D_WRAPPER_API int CalCoreTrack_GetCoreBoneId(CalCoreTrack *self);
//  CAL3D_WRAPPER_API std::map<float, CalCoreKeyframe *>& CalCoreTrack_GetMapCoreKeyframe(CalCoreTrack *self);
  CAL3D_WRAPPER_API Boolean CalCoreTrack_GetState(CalCoreTrack *self, float time, CalVector *pTranslation, CalQuaternion *pRotation);
  CAL3D_WRAPPER_API CalCoreTrack *CalCoreTrack_New();
  CAL3D_WRAPPER_API Boolean CalCoreTrack_SetCoreBoneId(CalCoreTrack *self, int coreBoneId);

//****************************************************************************//
// CalError wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API enum CalErrorCode
  {
    ERROR_CODE_OK = 0,
    ERROR_CODE_INTERNAL,
    ERROR_CODE_INVALID_HANDLE,
    ERROR_CODE_MEMORY_ALLOCATION_FAILED,
    ERROR_CODE_FILE_NOT_FOUND,
    ERROR_CODE_INVALID_FILE_FORMAT,
    ERROR_CODE_FILE_PARSER_FAILED,
    ERROR_CODE_INDEX_BUILD_FAILED,
    ERROR_CODE_NO_PARSER_DOCUMENT,
    ERROR_CODE_INVALID_ANIMATION_DURATION,
    ERROR_CODE_BONE_NOT_FOUND,
    ERROR_CODE_INVALID_ATTRIBUTE_VALUE,
    ERROR_CODE_INVALID_KEYFRAME_COUNT,
    ERROR_CODE_INVALID_ANIMATION_TYPE,
    ERROR_CODE_FILE_CREATION_FAILED,
    ERROR_CODE_FILE_WRITING_FAILED,
    ERROR_CODE_INCOMPATIBLE_FILE_VERSION,
    ERROR_CODE_NO_MESH_IN_MODEL,
    ERROR_CODE_MAX_ERROR_CODE
  };

  CAL3D_WRAPPER_API CalErrorCode CalError_GetLastErrorCode();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorDescription();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorFile();
  CAL3D_WRAPPER_API int CalError_GetLastErrorLine();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorText();
  CAL3D_WRAPPER_API void CalError_PrintLastError();
  CAL3D_WRAPPER_API void CalError_SetLastError(CalErrorCode code, char *strFile, int line, char *strText);

//****************************************************************************//
// CalLoader wrapper functions declaration                                    //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalLoader_Delete(CalLoader *self);
  CAL3D_WRAPPER_API CalCoreAnimation *CalLoader_LoadCoreAnimation(CalLoader *self, char *strFilename);
  CAL3D_WRAPPER_API CalCoreMaterial *CalLoader_LoadCoreMaterial(CalLoader *self, char *strFilename);
  CAL3D_WRAPPER_API CalCoreMesh *CalLoader_LoadCoreMesh(CalLoader *self, char *strFilename);
  CAL3D_WRAPPER_API CalCoreSkeleton *CalLoader_LoadCoreSkeleton(CalLoader *self, char *strFilename);
  CAL3D_WRAPPER_API CalLoader *CalLoader_New();

//****************************************************************************//
// CalMesh wrapper functions declaration                                      //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalMesh_Create(CalMesh *self, CalCoreMesh *pCoreMesh);
  CAL3D_WRAPPER_API void CalMesh_Delete(CalMesh *self);
  CAL3D_WRAPPER_API void CalMesh_Destroy(CalMesh *self);
  CAL3D_WRAPPER_API CalCoreMesh *CalMesh_GetCoreMesh(CalMesh *self);
  CAL3D_WRAPPER_API CalSubmesh *CalMesh_GetSubmesh(CalMesh *self, int id);
  CAL3D_WRAPPER_API int CalMesh_GetSubmeshCount(CalMesh *self);
//  CAL3D_WRAPPER_API std::vector<CalSubmesh *>& CalMesh_GetVectorSubmesh(CalMesh *self);
  CAL3D_WRAPPER_API CalMesh *CalMesh_New();
  CAL3D_WRAPPER_API void CalMesh_SetLodLevel(CalMesh *self, float lodLevel);
  CAL3D_WRAPPER_API void CalMesh_SetMaterialSet(CalMesh *self, int setId);
  CAL3D_WRAPPER_API void CalMesh_SetModel(CalMesh *self, CalModel *pModel);

//****************************************************************************//
// CalMixer wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalMixer_BlendCycle(CalMixer *self, int id, float weight, float delay);
  CAL3D_WRAPPER_API Boolean CalMixer_ClearCycle(CalMixer *self, int id, float delay);
  CAL3D_WRAPPER_API Boolean CalMixer_Create(CalMixer *self, CalModel *pModel);
  CAL3D_WRAPPER_API void CalMixer_Delete(CalMixer *self);
  CAL3D_WRAPPER_API void CalMixer_Destroy(CalMixer *self);
  CAL3D_WRAPPER_API Boolean CalMixer_ExecuteAction(CalMixer *self, int id, float delayIn, float delayOut);
  CAL3D_WRAPPER_API CalMixer *CalMixer_New();
  CAL3D_WRAPPER_API void CalMixer_UpdateAnimation(CalMixer *self, float deltaTime);
  CAL3D_WRAPPER_API void CalMixer_UpdateSkeleton(CalMixer *self);

//****************************************************************************//
// CalModel wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalModel_AttachMesh(CalModel *self, int coreMeshId);
  CAL3D_WRAPPER_API Boolean CalModel_Create(CalModel *self, CalCoreModel *pCoreModel);
  CAL3D_WRAPPER_API void CalModel_Delete(CalModel *self);
  CAL3D_WRAPPER_API void CalModel_Destroy(CalModel *self);
  CAL3D_WRAPPER_API Boolean CalModel_DetachMesh(CalModel *self, int coreMeshId);
  CAL3D_WRAPPER_API CalCoreModel *CalModel_GetCoreModel(CalModel *self);
  CAL3D_WRAPPER_API CalMesh *CalModel_GetMesh(CalModel *self, int coreMeshId);
  CAL3D_WRAPPER_API CalMixer *CalModel_GetMixer(CalModel *self);
  CAL3D_WRAPPER_API CalPhysique *CalModel_GetPhysique(CalModel *self);
  CAL3D_WRAPPER_API CalRenderer *CalModel_GetRenderer(CalModel *self);
  CAL3D_WRAPPER_API CalSkeleton *CalModel_GetSkeleton(CalModel *self);
  CAL3D_WRAPPER_API CalSpringSystem *CalModel_GetSpringSystem(CalModel *self);
  CAL3D_WRAPPER_API CalUserData CalModel_GetUserData(CalModel *self);
//  CAL3D_WRAPPER_API std::vector<CalMesh *>& CalModel_GetVectorMesh(CalModel *self);
  CAL3D_WRAPPER_API CalModel *CalModel_New();
  CAL3D_WRAPPER_API void CalModel_SetLodLevel(CalModel *self, float lodLevel);
  CAL3D_WRAPPER_API void CalModel_SetMaterialSet(CalModel *self, int setId);
  CAL3D_WRAPPER_API void CalModel_SetUserData(CalModel *self, CalUserData userData);
  CAL3D_WRAPPER_API void CalModel_Update(CalModel *self, float deltaTime);

//****************************************************************************//
// CalPhysique wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalPhysique_CalculateNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pNormalBuffer);
  CAL3D_WRAPPER_API int CalPhysique_CalculateVertices(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer);
  CAL3D_WRAPPER_API int CalPhysique_CalculateVerticesAndNormals(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer);
  CAL3D_WRAPPER_API int CalPhysique_CalculateVerticesNormalsAndTexCoords(CalPhysique *self, CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords);  
  CAL3D_WRAPPER_API Boolean CalPhysique_Create(CalPhysique *self, CalModel *pModel);
  CAL3D_WRAPPER_API void CalPhysique_Delete(CalPhysique *self);
  CAL3D_WRAPPER_API void CalPhysique_Destroy(CalPhysique *self);
  CAL3D_WRAPPER_API CalPhysique *CalPhysique_New();
  CAL3D_WRAPPER_API void CalPhysique_Update(CalPhysique *self);

//****************************************************************************//
// CalPlatform wrapper functions declaration                                  //
//****************************************************************************//

//****************************************************************************//
// CalQuaternion wrapper functions declaration                                //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalQuaternion_Blend(CalQuaternion *self, float d, CalQuaternion *pQ);
  CAL3D_WRAPPER_API void CalQuaternion_Clear(CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Conjugate(CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Delete(CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Equal(CalQuaternion *self, CalQuaternion *pQ);
  CAL3D_WRAPPER_API float *CalQuaternion_Get(CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Multiply(CalQuaternion *self, CalQuaternion *pQ);
  CAL3D_WRAPPER_API void CalQuaternion_MultiplyVector(CalQuaternion *self, CalVector *pV);
  CAL3D_WRAPPER_API CalQuaternion *CalQuaternion_New();
  CAL3D_WRAPPER_API void CalQuaternion_Op_Multiply(CalQuaternion *pResult, CalQuaternion *pQ, CalQuaternion *pR);
  CAL3D_WRAPPER_API void CalQuaternion_Set(CalQuaternion *self, float qx, float qy, float qz, float qw);

//****************************************************************************//
// CalRenderer wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalRenderer_BeginRendering(CalRenderer *self);
  CAL3D_WRAPPER_API Boolean CalRenderer_Create(CalRenderer *self, CalModel *pModel);
  CAL3D_WRAPPER_API void CalRenderer_Delete(CalRenderer *self);
  CAL3D_WRAPPER_API void CalRenderer_Destroy(CalRenderer *self);
  CAL3D_WRAPPER_API void CalRenderer_EndRendering(CalRenderer *self);
  CAL3D_WRAPPER_API void CalRenderer_GetAmbientColor(CalRenderer *self, unsigned char *pColorBuffer);
  CAL3D_WRAPPER_API void CalRenderer_GetDiffuseColor(CalRenderer *self, unsigned char *pColorBuffer);
  CAL3D_WRAPPER_API int CalRenderer_GetFaceCount(CalRenderer *self);
#ifdef CAL_16BIT_INDICES
  CAL3D_WRAPPER_API int CalRenderer_GetFaces(CalRenderer *self, unsigned short *pFaceBuffer);
#else
  CAL3D_WRAPPER_API int CalRenderer_GetFaces(CalRenderer *self, int *pFaceBuffer);
#endif
  CAL3D_WRAPPER_API int CalRenderer_GetMapCount(CalRenderer *self);
  CAL3D_WRAPPER_API CalUserData CalRenderer_GetMapUserData(CalRenderer *self, int mapId);
  CAL3D_WRAPPER_API int CalRenderer_GetMeshCount(CalRenderer *self);
  CAL3D_WRAPPER_API int CalRenderer_GetNormals(CalRenderer *self, float *pNormalBuffer);
  CAL3D_WRAPPER_API float CalRenderer_GetShininess(CalRenderer *self);
  CAL3D_WRAPPER_API void CalRenderer_GetSpecularColor(CalRenderer *self, unsigned char *pColorBuffer);
  CAL3D_WRAPPER_API int CalRenderer_GetSubmeshCount(CalRenderer *self, int meshId);
  CAL3D_WRAPPER_API int CalRenderer_GetTextureCoordinates(CalRenderer *self, int mapId, float *pTextureCoordinateBuffer);
  CAL3D_WRAPPER_API int CalRenderer_GetVertexCount(CalRenderer *self);
  CAL3D_WRAPPER_API int CalRenderer_GetVertices(CalRenderer *self, float *pVertexBuffer);
  CAL3D_WRAPPER_API int CalRenderer_GetVerticesAndNormals(CalRenderer *self, float *pVertexBuffer);
  CAL3D_WRAPPER_API int CalRenderer_GetVerticesNormalsAndTexCoords(CalRenderer *self, float *pVertexBuffer, int NumTexCoords);
  CAL3D_WRAPPER_API CalRenderer *CalRenderer_New();
  CAL3D_WRAPPER_API Boolean CalRenderer_SelectMeshSubmesh(CalRenderer *self, int meshId, int submeshId);

//****************************************************************************//
// CalSaver wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSaver_Delete(CalSaver *self);
  CAL3D_WRAPPER_API CalSaver *CalSaver_New();
  CAL3D_WRAPPER_API Boolean CalSaver_SaveCoreAnimation(CalSaver *self, char *strFilename, CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API Boolean CalSaver_SaveCoreMaterial(CalSaver *self, char *strFilename, CalCoreMaterial *pCoreMaterial);
  CAL3D_WRAPPER_API Boolean CalSaver_SaveCoreMesh(CalSaver *self, char *strFilename, CalCoreMesh *pCoreMesh);
  CAL3D_WRAPPER_API Boolean CalSaver_SaveCoreSkeleton(CalSaver *self, char *strFilename, CalCoreSkeleton *pCoreSkeleton);

//****************************************************************************//
// CalSkeleton wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSkeleton_CalculateState(CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_ClearState(CalSkeleton *self);
  CAL3D_WRAPPER_API Boolean CalSkeleton_Create(CalSkeleton *self, CalCoreSkeleton *pCoreSkeleton);
  CAL3D_WRAPPER_API void CalSkeleton_Delete(CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_Destroy(CalSkeleton *self);
  CAL3D_WRAPPER_API CalBone *CalSkeleton_GetBone(CalSkeleton *self, int boneId);
  CAL3D_WRAPPER_API CalCoreSkeleton *CalSkeleton_GetCoreSkeleton(CalSkeleton *self);
//  CAL3D_WRAPPER_API std::vector<CalBone *>& CalSkeleton_GetVectorBone(CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_LockState(CalSkeleton *self);
  CAL3D_WRAPPER_API CalSkeleton *CalSkeleton_New();

  // DEBUG-CODE
  CAL3D_WRAPPER_API int CalSkeleton_GetBonePoints(CalSkeleton *self, float *pPoints);
  CAL3D_WRAPPER_API int CalSkeleton_GetBonePointsStatic(CalSkeleton *self, float *pPoints);
  CAL3D_WRAPPER_API int CalSkeleton_GetBoneLines(CalSkeleton *self, float *pLines);
  CAL3D_WRAPPER_API int CalSkeleton_GetBoneLinesStatic(CalSkeleton *self, float *pLines);

//****************************************************************************//
// CalSpringSystem wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSpringSystem_CalculateForces(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime);
  CAL3D_WRAPPER_API void CalSpringSystem_CalculateVertices(CalSpringSystem *self, CalSubmesh *pSubmesh, float deltaTime);
  CAL3D_WRAPPER_API Boolean CalSpringSystem_Create(CalSpringSystem *self, CalModel *pModel);
  CAL3D_WRAPPER_API void CalSpringSystem_Delete(CalSpringSystem *self);
  CAL3D_WRAPPER_API void CalSpringSystem_Destroy(CalSpringSystem *self);
  CAL3D_WRAPPER_API CalSpringSystem *CalSpringSystem_New();
  CAL3D_WRAPPER_API void CalSpringSystem_Update(CalSpringSystem *self, float deltaTime);

//****************************************************************************//
// CalSubmesh wrapper functions declaration                                   //
//****************************************************************************//

  CAL3D_WRAPPER_API Boolean CalSubmesh_Create(CalSubmesh *self, CalCoreSubmesh *pCoreSubmesh);
  CAL3D_WRAPPER_API void CalSubmesh_Delete(CalSubmesh *self);
  CAL3D_WRAPPER_API void CalSubmesh_Destroy(CalSubmesh *self);
  CAL3D_WRAPPER_API CalCoreSubmesh *CalSubmesh_GetCoreSubmesh(CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetCoreMaterialId(CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetFaceCount(CalSubmesh *self);
#ifdef CAL_16BIT_INDICES
  CAL3D_WRAPPER_API int CalSubmesh_GetFaces(CalSubmesh *self, unsigned short *pFaceBuffer);
#else
  CAL3D_WRAPPER_API int CalSubmesh_GetFaces(CalSubmesh *self, int *pFaceBuffer);
#endif

//  CAL3D_WRAPPER_API std::vector<CalVector>& CalSubmesh_GetVectorNormal(CalSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalSubmesh::PhysicalProperty>& CalSubmesh_GetVectorPhysicalProperty(CalSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalVector>& CalSubmesh_GetVectorVertex(CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetVertexCount(CalSubmesh *self);
  CAL3D_WRAPPER_API Boolean CalSubmesh_HasInternalData(CalSubmesh *self);
  CAL3D_WRAPPER_API CalSubmesh *CalSubmesh_New();
  CAL3D_WRAPPER_API void CalSubmesh_SetCoreMaterialId(CalSubmesh *self, int coreMaterialId);
  CAL3D_WRAPPER_API void CalSubmesh_SetLodLevel(CalSubmesh *self, float lodLevel);

//****************************************************************************//
// CalVector wrapper functions declaration                                    //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalVector_Add(CalVector *self, CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Blend(CalVector *self, float d, CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Clear(CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Delete(CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Equal(CalVector *self, CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_InverseScale(CalVector *self, float d);
  CAL3D_WRAPPER_API float *CalVector_Get(CalVector *self);
  CAL3D_WRAPPER_API float CalVector_Length(CalVector *self);
  CAL3D_WRAPPER_API CalVector *CalVector_New();
  CAL3D_WRAPPER_API float CalVector_Normalize(CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Op_Add(CalVector *pResult, CalVector *pV, CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Op_Subtract(CalVector *pResult, CalVector *pV, CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_CalVector_Op_Scale(CalVector *pResult, CalVector *pV, float d);
  CAL3D_WRAPPER_API void CalVector_CalVector_Op_InverseScale(CalVector *pResult, CalVector *pV, float d);
  CAL3D_WRAPPER_API float CalVector_Op_Scalar(CalVector *pV, CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Op_Cross(CalVector *pResult, CalVector *pV, CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Scale(CalVector *self, float d);
  CAL3D_WRAPPER_API void CalVector_Set(CalVector *self, float vx, float vy, float vz);
  CAL3D_WRAPPER_API void CalVector_Subtract(CalVector *self, CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Transform(CalVector *self, CalQuaternion *pQ);

#ifdef __cplusplus
}
#endif

#endif

//****************************************************************************//

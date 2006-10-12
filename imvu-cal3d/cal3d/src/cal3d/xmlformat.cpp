#include "cal3d/loader.h"
#include "cal3d/error.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coretrack.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/coremesh.h"
#include "cal3d/coremorphtrack.h"
#include "cal3d/coremorphkeyframe.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"
#include "cal3d/corematerial.h"
#include "cal3d/tinyxml.h"
#include "cal3d/streamsource.h"
#include "cal3d/buffersource.h"
#include "cal3d/xmlformat.h"


#ifdef WIN32
#include "cal3d/calxmlbindings.h"
#else
#include "cal3d/calxmlbindings.cpp"
#endif


static inline void ReadPair( char const * buffer, float * f1, float * f2 )
{
#if CAL3D_USE_STL_INSTEAD_OF_SSCANF
  std::stringstream str;
  str << buffer;
  str >> *f1 >> *f2;
#else
  sscanf( buffer, "%f %f", f1, f2 );
#endif
}

static inline void ReadPair( char const * buffer, int * f1, int * f2 )
{

#if CAL3D_USE_STL_INSTEAD_OF_SSCANF
  std::stringstream str;
  str << buffer;
  str >> *f1 >> *f2;
#else
  sscanf( buffer, "%d %d", f1, f2 );
#endif
}



#define CAL3D_VALIDATE_XML_TAGS ( 1 )


template<class T>
static inline bool _ValidateTag( T * element, char const * requiredTag,
  CalCoreMesh * pCoreMesh, CalCoreSubmesh * pCoreSubmesh,
  char const * file, int line )
{
  bool ok = true;
#if CAL3D_VALIDATE_XML_TAGS
  if(!element) {
    ok = false;
  }
  
  if(ok && requiredTag && stricmp(element->Value(),requiredTag)!=0)
  {
    ok = false;
  }

  if( ! ok ) {
    if( pCoreMesh ) {
      pCoreMesh->destroy();
      delete pCoreMesh;
    }
    if( pCoreSubmesh ) {
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
    }
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, file, line);
  }
#endif

  return ok;
}

#define ValidateTag(a,b,c,d) _ValidateTag(a,b,c,d,__FILE__,__LINE__)


static inline bool TexCoordFromXml( TiXmlElement * texcoord, char const * tag,
  CalCoreSubmesh::TextureCoordinate * texCoord, int loadingMode,
  CalCoreMesh * pCoreMesh, CalCoreSubmesh * pCoreSubmesh)
{
  if( !ValidateTag(texcoord, tag, pCoreMesh, pCoreSubmesh) ) {
    return false;
  }
  TiXmlNode * node = texcoord->FirstChild();
  if( !ValidateTag(node, NULL, pCoreMesh, pCoreSubmesh) ) {
    return false;
  }
  TiXmlText* texcoorddata = node->ToText();
  if(!ValidateTag(texcoorddata , NULL, pCoreMesh, pCoreSubmesh))
  {
    return false;
  }
  
  ReadPair( texcoorddata->Value(), &texCoord->u, &texCoord->v );

  if (loadingMode & LOADER_INVERT_V_COORD)
  {
    texCoord->v = 1.0f - texCoord->v;
  }
  return true;
}



static inline void ReadTripleFloat( char const * buffer, float * f1, float * f2, float * f3 )
{
#if CAL3D_USE_STL_INSTEAD_OF_SSCANF
  std::stringstream str;
  str << buffer;
  str >> *f1 >> *f2 >> *f3;
#else
  sscanf( buffer, "%f %f %f", f1, f2, f3 );
#endif
}



static inline bool CalVectorFromXml( TiXmlElement * pos, char const * tag, CalVector * calVec,
  CalCoreMesh * pCoreMesh, CalCoreSubmesh * pCoreSubmesh)
{
  if( !ValidateTag(pos, tag, pCoreMesh, pCoreSubmesh) ) {
    return false;
  }
  TiXmlNode * node = pos->FirstChild();
  if( !ValidateTag(node, NULL, pCoreMesh, pCoreSubmesh) ) {
    return false;
  }
  TiXmlText* posdata = node->ToText();
  if(!ValidateTag(posdata, NULL, pCoreMesh, pCoreSubmesh))
  {
    return false;
  }
  ReadTripleFloat( posdata->Value(), &calVec->x, &calVec->y, &calVec->z );
  return true;
}


static inline void ReadQuadFloat( char const * buffer, float * f1, float * f2, float * f3, float * f4 )
{
#if CAL3D_USE_STL_INSTEAD_OF_SSCANF
  std::stringstream str;
  str << buffer;
  str >> *f1 >> *f2 >> *f3 >> *f4;
#else
  sscanf( buffer, "%f %f %f %f", f1, f2, f3, f4 );
#endif
}

 /*****************************************************************************/
/** Loads a core skeleton instance from a XML file.
  *
  * This function loads a core skeleton instance from a XML file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeleton *CalLoader::loadXmlCoreSkeletonFromFile(const std::string& strFilename)
{ 
  
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  return loadXmlCoreSkeleton(doc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance from a XML file.
  *
  * This function loads a core skeleton instance from a XML file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeleton *CalLoader::loadXmlCoreSkeleton(const void *dataSrc)
{ 
  
  TiXmlDocument doc;


  doc.Parse(static_cast<const char*>(dataSrc));
  if(doc.Error())
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  return loadXmlCoreSkeleton(doc);
}


 /*****************************************************************************/
/** Loads a core Mesh instance from a XML file.
  *
  * This function loads a core Mesh instance from a XML file.
  *
  * @param strFilename The name of the file to load the core Mesh instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core Mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMesh *CalLoader::loadXmlCoreMesh(const void *dataSrc)
{ 
  
  TiXmlDocument doc;
  doc.Clear();


  doc.Parse(static_cast<const char*>(dataSrc));
  if(doc.Error())
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  return loadXmlCoreMesh(doc);
}

 /******************************  ***********************************************/
/** Loads a core Material instance from a XML file.
  *
  * This function loads a core Material instance from a XML file.
  *
  * @param strFilename The name of the file to load the core Material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core Material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterial *CalLoader::loadXmlCoreMaterial(const void *dataSrc)
{ 
  
  TiXmlDocument doc;


  doc.Parse(static_cast<char const *>(dataSrc));
  if(doc.Error())
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  return loadXmlCoreMaterial(doc);
}

 /*****************************************************************************/
/** Loads a core animation instance from a XML file.
  *
  * This function loads a core animation instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimation*CalLoader::loadXmlCoreAnimation(const char* dataSrc, CalCoreSkeleton *skel)
{ 
  TiXmlDocument doc;
  doc.Clear();

  doc.Parse(dataSrc);
  if(doc.Error())
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  return loadXmlCoreAnimation(doc, skel);
}


 /*****************************************************************************/
/** Loads a core animatedMorph instance from a XML file.
  *
  * This function loads a core animatedMorph instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animatedMorph instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animatedMorph
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimatedMorph*CalLoader::loadXmlCoreAnimatedMorph(const void *dataSrc)
{ 
  TiXmlDocument doc;
  doc.Clear();

  doc.Parse(static_cast<const char*>(dataSrc));
  if(doc.Error())
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  return loadXmlCoreAnimatedMorph(doc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance from a XML file.
  *
  * This function loads a core skeleton instance from a XML file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/
CalCoreSkeleton *CalLoader::loadXmlCoreSkeleton(TiXmlDocument & doc)
{ 
  std::stringstream str;

  std::string strFilename = "";

  TiXmlNode* node;
  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
  }  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_EXTENSION)!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return false;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return false;
  }

  TiXmlElement*skeleton = header->NextSiblingElement();
  if(!skeleton || stricmp(skeleton->Value(),"SKELETON")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return false;
  }  
  
  // allocate a new core skeleton instance
  CalCoreSkeleton *pCoreSkeleton;
  pCoreSkeleton = new CalCoreSkeleton();
  if(pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core skeleton instance
  if(!pCoreSkeleton->create())
  {
    delete pCoreSkeleton;
    return 0;
  }

  char const * attrStr = skeleton->Attribute("SCENEAMBIENTCOLOR");
  if( attrStr ) {
    CalVector sceneColor;
    ReadTripleFloat( attrStr, &sceneColor.x, &sceneColor.y, &sceneColor.z );
    pCoreSkeleton->setSceneAmbientColor( sceneColor );
  }
  
  TiXmlElement* bone;
  for( bone = skeleton->FirstChildElement();bone;bone = bone->NextSiblingElement() )
  {
    if(stricmp(bone->Value(),"BONE")!=0)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }   
    
    std::string strName=bone->Attribute("NAME");

          CalLightType lightType = LIGHT_TYPE_NONE;
          CalVector lightColor;
          
          char const * attrStr = bone->Attribute("LIGHTTYPE");
          if( attrStr ) {
            lightType = (CalLightType)atoi(attrStr);
          }

          attrStr = bone->Attribute("LIGHTCOLOR");
          if( attrStr ) {
            ReadTripleFloat(attrStr, &lightColor.x, &lightColor.y, &lightColor.z);
          }
          

    // get the translation of the bone

    TiXmlElement* translation = bone->FirstChildElement();
    if(!translation || stricmp( translation->Value(),"TRANSLATION")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
      return false;
    }

    float tx, ty, tz;
    
    node = translation->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }   
    TiXmlText* translationdata = node->ToText();
    if(!translationdata)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }   
    str.clear();
    str << translationdata->Value();
    str >> tx >> ty >> tz;

      // get the rotation of the bone

    TiXmlElement* rotation = translation->NextSiblingElement();
    if(!rotation || stricmp(rotation->Value(),"ROTATION")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
      return false;
    }

    float rx, ry, rz, rw;
    
    node = rotation->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    TiXmlText* rotationdata = node->ToText();
    if(!rotationdata)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    str.clear();
    str << rotationdata->Value();
    str >> rx >> ry >> rz >> rw;    
    
    // get the bone space translation of the bone
      
    
    TiXmlElement* translationBoneSpace = rotation->NextSiblingElement();
    if(!rotation || stricmp(translationBoneSpace->Value(),"LOCALTRANSLATION")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;     
      return false;
    }

    float txBoneSpace, tyBoneSpace, tzBoneSpace;
    
    node = translationBoneSpace->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    TiXmlText* translationBoneSpacedata = node->ToText();
    if(!translationBoneSpacedata)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    str.clear();
    str << translationBoneSpacedata->Value();
    str >> txBoneSpace >> tyBoneSpace >> tzBoneSpace;

      // get the bone space rotation of the bone

    TiXmlElement* rotationBoneSpace = translationBoneSpace->NextSiblingElement();
    if(!rotationBoneSpace || stricmp(rotationBoneSpace->Value(),"LOCALROTATION")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;      
      return false;
    }

    float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
    
    node = rotationBoneSpace->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    TiXmlText* rotationBoneSpacedata = node->ToText();
    if(!rotationBoneSpacedata)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    str.clear();
    str << rotationBoneSpacedata->Value();
    str >> rxBoneSpace >> ryBoneSpace >> rzBoneSpace >> rwBoneSpace;
    
    // get the parent bone id

    TiXmlElement* parent = rotationBoneSpace->NextSiblingElement();
    if(!parent ||stricmp(parent->Value(),"PARENTID")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;       
      return false;
    }
    

    int parentId;
    
    node = parent->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    TiXmlText* parentid = node->ToText();
      if(!parentid)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreSkeleton->destroy();
        delete pCoreSkeleton;    
        return false;
    }
    parentId = atoi(parentid->Value());

    // allocate a new core bone instance
    CalCoreBone *pCoreBone;
    pCoreBone = new CalCoreBone();
    if(pCoreBone == 0)
    {
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return 0;
    }

    // create the core bone instance
    if(!pCoreBone->createWithName(strName.c_str()))
    {
      pCoreSkeleton->destroy();
          delete pCoreSkeleton;    
      delete pCoreBone;
      return 0;
    }

    // set the parent of the bone
    pCoreBone->setParentId(parentId);

    // set all attributes of the bone

    CalVector trans = CalVector(tx, ty, tz);
    CalQuaternion rot = CalQuaternion(rx, ry, rz, rw);

    if (loadingMode & LOADER_ROTATE_X_AXIS)
    {
      if (parentId == -1) // only root bone necessary
      {
        // Root bone must have quaternion rotated
        CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
        rot *= x_axis_90;
        // Root bone must have translation rotated also
        trans *= x_axis_90;
      }
    }   
    

    pCoreBone->setTranslation(trans);
    pCoreBone->setRotation(rot);
    pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
    pCoreBone->setRotationBoneSpace(CalQuaternion(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace));

          pCoreBone->setLightType( lightType );
          pCoreBone->setLightColor( lightColor );

    TiXmlElement* child;
    for( child = parent->NextSiblingElement();child;child = child->NextSiblingElement() )
    {
      if(stricmp(child->Value(),"CHILDID")!=0)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSkeleton->destroy();
        delete pCoreSkeleton;
        pCoreBone->destroy();
        delete pCoreBone;
        return false;
      }

      TiXmlNode *node= child->FirstChild();
      if(!node)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSkeleton->destroy();
        delete pCoreSkeleton;
        pCoreBone->destroy();
        delete pCoreBone;
        return false;
      }
      TiXmlText* childid = node->ToText();
      if(!childid)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSkeleton->destroy();
        delete pCoreSkeleton;
        pCoreBone->destroy();
        delete pCoreBone;
        return false;
      }
      
      int childId = atoi(childid->Value());
      
      pCoreBone->addChildId(childId);
    }

    // set the core skeleton of the core bone instance
    pCoreBone->setCoreSkeleton(pCoreSkeleton);

    // add the core bone to the core skeleton instance
    pCoreSkeleton->addCoreBone(pCoreBone);

  }

  doc.Clear();

  pCoreSkeleton->calculateState();

    return pCoreSkeleton;
}

 /*****************************************************************************/
/** Loads a core animation instance from a XML file.
  *
  * This function loads a core animation instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimation *CalLoader::loadXmlCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel)
{
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }
  return loadXmlCoreAnimation(doc, skel);
}

 /*****************************************************************************/
/** Loads a core animatedMorph instance from a XML file.
  *
  * This function loads a core animatedMorph instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animatedMorph instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animatedMorph
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimatedMorph *CalLoader::loadXmlCoreAnimatedMorph(const std::string& strFilename)
{
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }
  return loadXmlCoreAnimatedMorph(doc);
}

 /*****************************************************************************/
/** Loads a core animation instance from a XML file.
  *
  * This function loads a core animation instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimation *CalLoader::loadXmlCoreAnimation(TiXmlDocument &doc, CalCoreSkeleton *skel)
{
  std::stringstream str;

  const std::string strFilename = "";
  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
    str.clear();
    str << "Header element is " << (header ? header->Value() : "<unknown>");
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, str.str());
    return 0;
  }
  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_EXTENSION)!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return 0;
  }

  TiXmlElement*animation = header->NextSiblingElement();
  if(!animation || stricmp(animation->Value(),"ANIMATION")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
  }  

  int trackCount= atoi(animation->Attribute("NUMTRACKS"));
  float duration= (float) atof(animation->Attribute("DURATION"));
    
  // allocate a new core animation instance
  CalCoreAnimation *pCoreAnimation = new CalCoreAnimation();

  // create the core animation instance
  if(!pCoreAnimation->create())
  {
    delete pCoreAnimation;
    return 0;
  }


  // check for a valid duration
  if(duration <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__, strFilename);
    pCoreAnimation->destroy();
    delete pCoreAnimation;
    return 0;
  }

  // set the duration in the core animation instance
  pCoreAnimation->setDuration(duration);
  TiXmlElement* track=animation->FirstChildElement();

  // load all core bones
  for(int trackId = 0; trackId < trackCount; ++trackId)
  {
    if(!track || stricmp(track->Value(),"TRACK")!=0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      pCoreAnimation->destroy();
      delete pCoreAnimation;
      return 0;
    }

    CalCoreTrack *pCoreTrack = new CalCoreTrack();

    // create the core track instance
    pCoreTrack->create();

    int coreBoneId = atoi(track->Attribute("BONEID"));
    const char * trstr = track->Attribute("TRANSLATIONREQUIRED");
    bool translationRequired = true; // Default value if flag is not supplied (for backwards compatibility).
    if( trstr ) {
      translationRequired = atoi( trstr ) ? true : false;
    }

    const char * trstr2 = track->Attribute("HIGHRANGEREQUIRED");
    bool highRangeRequired = true; // Default value if flag is not supplied (for backwards compatibility).
    if( trstr2 ) {
      highRangeRequired = atoi( trstr2 ) ? true : false;
    }

    const char * trstr3 = track->Attribute("TRANSLATIONISDYNAMIC");
    bool translationIsDynamic = true; // Default value if flag is not supplied (for backwards compatibility).
    if( trstr3 ) {
      translationIsDynamic = atoi( trstr3 ) ? true : false;
    }

    // XML files may or may not have a translationRequired flag.  The default value is true if it is not supplied.
    // XML files may or may not have the translation in keyframes, but if they don't, then either (a) they do
    // have a translationRequired flag and it is false, or (b) they have a false translationIsDynamic flag AND
    // the keyframe is not the first.
    // If XML files do not have the flag, then we assume translation is required until we decide otherwise.
    // If the caller has passed in a skeleton (not NULL for the skeleton), then we will re-derive 
    // whether translation is required, and we will update the translationRequired flag.

    // link the core track to the appropriate core bone instance
    pCoreTrack->setCoreBoneId(coreBoneId);

    // read the number of keyframes
    int keyframeCount= atoi(track->Attribute("NUMKEYFRAMES"));

    if(keyframeCount <= 0)
    {
      pCoreAnimation->destroy();
      delete pCoreAnimation;
      pCoreTrack->destroy();
      delete pCoreTrack;
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
    }

    TiXmlElement* keyframe= track->FirstChildElement();

    // load all core keyframes
    CalCoreKeyframe * prevCoreKeyframe = NULL;
    int keyframeId;
    for(keyframeId = 0; keyframeId < keyframeCount; ++keyframeId)
    {
      // load the core keyframe
      if(!keyframe|| stricmp(keyframe->Value(),"KEYFRAME")!=0)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
        return 0;
      }

      float time= (float) atof(keyframe->Attribute("TIME"));

      // Translation component in the XML is now optional.
      // I first fill the translation with zero.
      // Then if I have a skeleton, I fill it with the values from the skeleton.
      // Then if I have an XML translation entry, I fill it with the value from that entry.
      TiXmlElement * translation = keyframe->FirstChildElement();
      TiXmlElement * rotation = translation;
      float tx, ty, tz;
      SetTranslationInvalid( & tx, & ty, & tz );
      if( skel ) {
        CalCoreBone * cb = skel->getCoreBone( coreBoneId );
        CalVector const & cbtrans = cb->getTranslation();
        tx = cbtrans.x;
        ty = cbtrans.y;
        tz = cbtrans.z;
      }

      // If translation is required but not dynamic, then I may elide the translation
      // values for all but the first frame, and for each frame's translation I will
      // copy the translation from the previous frame.
      if( prevCoreKeyframe && !translationIsDynamic && translationRequired ) {
        CalVector const & vec = prevCoreKeyframe->getTranslation();
        tx = vec.x;
        ty = vec.y;
        tz = vec.z;
      }

      if(!translation )
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreAnimation->destroy();
        delete pCoreAnimation;
        pCoreTrack->destroy();
        delete pCoreTrack;
        return 0;
      }
      if( stricmp(translation->Value(),"TRANSLATION") ==0) {
        node = translation->FirstChild();
        if(!node)
        {
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
          return 0;
        }
        
        TiXmlText* translationdata = node->ToText();
        if(!translationdata)
        {
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
          return 0;
        }
        ReadTripleFloat( translationdata->Value(), &tx, &ty, &tz );
        rotation = rotation->NextSiblingElement();
      }

      if(!rotation || stricmp(rotation->Value(),"ROTATION")!=0)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
        return 0;
      }

      float rx, ry, rz, rw;

      node = rotation->FirstChild();
      if(!node)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
        return 0;
      }
      TiXmlText* rotationdata = node->ToText();
      if(!rotationdata)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
        return 0;
      }
      ReadQuadFloat( rotationdata->Value(), &rx, &ry, &rz, &rw );  
      
      // allocate a new core keyframe instance

      CalCoreKeyframe *pCoreKeyframe;
      pCoreKeyframe = new CalCoreKeyframe();
      if(pCoreKeyframe == 0)
      {
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;        
        CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
        return 0;
      }
 
      // create the core keyframe instance
      if(!pCoreKeyframe->create())
      {
        pCoreAnimation->destroy();
          delete pCoreAnimation;
          pCoreTrack->destroy();
          delete pCoreTrack;
        delete pCoreKeyframe;
        return 0;       
      }
      // set all attributes of the keyframe
      pCoreKeyframe->setTime(time);
      pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
      pCoreKeyframe->setRotation(CalQuaternion(rx, ry, rz, rw));
      prevCoreKeyframe = pCoreKeyframe;
      
      if (loadingMode & LOADER_ROTATE_X_AXIS)
      {
        // Check for anim rotation
        if (skel && skel->getCoreBone(coreBoneId)->getParentId() == -1)  // root bone
        {
          // rotate root bone quaternion
          CalQuaternion rot = pCoreKeyframe->getRotation();
          CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
          rot *= x_axis_90;
          pCoreKeyframe->setRotation(rot);
          // rotate root bone displacement
          CalVector trans = pCoreKeyframe->getTranslation();
          trans *= x_axis_90;
          pCoreKeyframe->setTranslation(trans);
        }
      }    
      
      // add the core keyframe to the core track instance
         pCoreTrack->addCoreKeyframe(pCoreKeyframe);

     keyframe = keyframe->NextSiblingElement();

    }
    pCoreTrack->setTranslationRequired( translationRequired );
    pCoreTrack->setHighRangeRequired( highRangeRequired );
    pCoreTrack->setTranslationIsDynamic( translationIsDynamic );
    if( loadingCompressionOn ) {

      // This function MIGHT call setTranslationRequired() on the track.
      // Alas, you may be passing me NULL for skel, in which case compress() won't update the 
      // translationRequired flag; instead it will leave it, as above.
      pCoreTrack->compress( translationTolerance, rotationToleranceDegrees, skel );
    }
    pCoreAnimation->addCoreTrack(pCoreTrack);   
    track=track->NextSiblingElement();
  }

  // explicitly close the file
  doc.Clear();

  return pCoreAnimation;
}





 /*****************************************************************************/
/** Loads a core animatedMorph instance from a XML file.
  *
  * This function loads a core animatedMorph instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animatedMorph instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animatedMorph
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimatedMorph *CalLoader::loadXmlCoreAnimatedMorph(TiXmlDocument &doc)
{
  //std::stringstream str;

  std::string strFilename = "";

  TiXmlElement*header = doc.FirstChildElement();

  CalHeader headerData;
  if( !header || !BindFromXml( *header, &headerData ) ) {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }
  
  
  if(stricmp(headerData.magic,Cal::ANIMATEDMORPH_XMLFILE_EXTENSION)!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }    
  
  if(headerData.version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // allocate a new core animatedMorph instance
  CalCoreAnimatedMorph *pCoreAnimatedMorph;
  pCoreAnimatedMorph = new CalCoreAnimatedMorph();
  if(pCoreAnimatedMorph == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core animatedMorph instance
  if(!pCoreAnimatedMorph->create())
  {
    delete pCoreAnimatedMorph;
    return 0;
  }


  TiXmlElement*animatedMorph = header->NextSiblingElement();
  if(!animatedMorph || stricmp(animatedMorph->Value(),"ANIMATION")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreAnimatedMorph->destroy();
    delete pCoreAnimatedMorph;
    return 0;
  }  

  if( !BindFromXml( *animatedMorph, pCoreAnimatedMorph ) ) {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreAnimatedMorph->destroy();
    delete pCoreAnimatedMorph;
    return 0;
  }
  
  // check for a valid duration
  if(pCoreAnimatedMorph->getDuration() <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__, strFilename);
    pCoreAnimatedMorph->destroy();
    delete pCoreAnimatedMorph;
    return 0;
  }

  // explicitly close the file
  doc.Clear();

  return pCoreAnimatedMorph;
}

 /*****************************************************************************/
/** Loads a core mesh instance from a Xml file.
  *
  * This function loads a core mesh instance from a Xml file.
  *
  * @param strFilename The name of the file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMesh *CalLoader::loadXmlCoreMesh(const std::string& strFilename)
{

  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }
  return loadXmlCoreMesh(doc);
}

 /*****************************************************************************/
/** Loads a core mesh instance from a Xml file.
  *
  * This function loads a core mesh instance from a Xml file.
  *
  * @param strFilename The name of the file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMesh *CalLoader::loadXmlCoreMesh(TiXmlDocument & doc)
{
  std::string strFilename = "";

  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
  }
   
  
  if(stricmp(header->Attribute("MAGIC"),Cal::MESH_XMLFILE_EXTENSION)!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
  }    

  int version = atoi(header->Attribute("VERSION"));
  if(version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return 0;
  }

  bool hasVertexColors = (version >= Cal::FIRST_FILE_VERSION_WITH_VERTEX_COLORS);

  TiXmlElement*mesh = header->NextSiblingElement();
  if(!mesh || stricmp(mesh->Value(),"MESH")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
  }

  // get the number of submeshes
  int submeshCount = atoi(mesh->Attribute("NUMSUBMESH"));
  
  // allocate a new core mesh instance
  CalCoreMesh *pCoreMesh;
  pCoreMesh = new CalCoreMesh();
  if(pCoreMesh == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core mesh instance
  if(!pCoreMesh->create())
  {
    delete pCoreMesh;
    return 0;
  }

  TiXmlElement*submesh = mesh->FirstChildElement();

  // load all core submeshes
  int submeshId;
  for(submeshId = 0; submeshId < submeshCount; ++submeshId)
  {
    if(!submesh || stricmp(submesh->Value(),"SUBMESH")!=0)
    {
      pCoreMesh->destroy();
      delete pCoreMesh;
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
    }

    // get the material thread id of the submesh
    int coreMaterialThreadId = atoi(submesh->Attribute("MATERIAL"));

    // get the number of vertices, faces, level-of-details and springs
    int vertexCount = atoi(submesh->Attribute("NUMVERTICES"));

    int faceCount = atoi(submesh->Attribute("NUMFACES"));

    int lodCount = atoi(submesh->Attribute("NUMLODSTEPS"));

    int springCount = atoi(submesh->Attribute("NUMSPRINGS"));

    int textureCoordinateCount = atoi(submesh->Attribute("NUMTEXCOORDS"));

          char const * numMorphStr = submesh->Attribute("NUMMORPHS");
          int morphCount = 0;
          if( numMorphStr ) {
            morphCount = atoi(numMorphStr);
          }
          
    // allocate a new core submesh instance
    CalCoreSubmesh *pCoreSubmesh;
    pCoreSubmesh = new CalCoreSubmesh();
    if(pCoreSubmesh == 0)
    {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      pCoreMesh->destroy();
      delete pCoreMesh;
      return 0;
    }

    // create the core submesh instance
    if(!pCoreSubmesh->create())
    {
      pCoreMesh->destroy();
      delete pCoreMesh;
      delete pCoreSubmesh;
      return 0;
    }
    pCoreSubmesh->setHasNonWhiteVertexColors( false );

    // set the LOD step count
    pCoreSubmesh->setLodCount(lodCount);

    // set the core material id
    pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

    // reserve memory for all the submesh data
    if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
    {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__, strFilename);
      pCoreMesh->destroy();
      delete pCoreMesh;
      pCoreSubmesh->destroy();
      delete pCoreSubmesh;
      return 0;
    }

    TiXmlElement *vertex = submesh->FirstChildElement();
    
    // load all vertices and their influences
          std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();

    int vertexId;
    for(vertexId = 0; vertexId < vertexCount; ++vertexId)
    {
            if( !ValidateTag(vertex, "VERTEX", pCoreMesh, pCoreSubmesh) ) {
              return 0;
            }
            CalCoreSubmesh::Vertex & Vertex = vectorVertex[vertexId];

            TiXmlElement *pos= vertex->FirstChildElement();
            if( !ValidateTag(pos, "POS", pCoreMesh, pCoreSubmesh) ) {
              return 0;
            }
            node = pos->FirstChild();
            if( !ValidateTag(node, NULL, pCoreMesh, pCoreSubmesh) ) {
              return 0;
            }
      TiXmlText* posdata = node->ToText();
      if(!ValidateTag(posdata, NULL, pCoreMesh, pCoreSubmesh))
      {
                    return 0;
      }
                  ReadTripleFloat( posdata->Value(), &Vertex.position.x, &Vertex.position.y, &Vertex.position.z );

      TiXmlElement *norm= pos->NextSiblingElement();
      if(!ValidateTag(norm, "NORM", pCoreMesh, pCoreSubmesh))
      {
                    return 0;
      }
      node = norm->FirstChild();
      if(!node)
      {
          pCoreMesh->destroy();
        delete pCoreMesh;
        pCoreSubmesh->destroy();
          delete pCoreSubmesh;
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
      }
      TiXmlText* normdata = node->ToText();
      if(!normdata)
      {
          pCoreMesh->destroy();
        delete pCoreMesh;
        pCoreSubmesh->destroy();
          delete pCoreSubmesh;
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
      }

                  ReadTripleFloat( normdata->Value(),  &Vertex.normal.x, &Vertex.normal.y, &Vertex.normal.z );

      TiXmlElement *vertColor = norm->NextSiblingElement();
      TiXmlElement *collapse = 0;
      if(!vertColor||stricmp(vertColor->Value(),"COLOR")!=0)
      {
        if( hasVertexColors ) {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        } else {
          collapse = vertColor;
        }
      } else {
        node = vertColor->FirstChild();
        if(!node)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        TiXmlText* vcdata = node->ToText();
        if(!vcdata)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        ReadTripleFloat( vcdata->Value(), &Vertex.vertexColor.x, &Vertex.vertexColor.y, &Vertex.vertexColor.z );
        if( Vertex.vertexColor.x != 1.0f
          || Vertex.vertexColor.y != 1.0f
          || Vertex.vertexColor.z != 1.0f ) {
          pCoreSubmesh->setHasNonWhiteVertexColors( true );
        }

        collapse= vertColor->NextSiblingElement();
      }        
      if(!collapse)
      {
          pCoreMesh->destroy();
        delete pCoreMesh;
        pCoreSubmesh->destroy();
          delete pCoreSubmesh;
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
      }
      if(stricmp(collapse->Value(),"COLLAPSEID")==0)
      {
        node = collapse->FirstChild();
        if(!node)
        {
              pCoreMesh->destroy();
            delete pCoreMesh;
            pCoreSubmesh->destroy();
              delete pCoreSubmesh;
            CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
            return 0;
        }
        TiXmlText* collapseid = node->ToText();
        if(!collapseid)
        {
              pCoreMesh->destroy();
            delete pCoreMesh;
            pCoreSubmesh->destroy();
              delete pCoreSubmesh;
            CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
            return 0;
        }
        Vertex.collapseId = atoi(collapseid->Value());

        TiXmlElement *collapseCount= collapse->NextSiblingElement();
        if(!collapseCount|| stricmp(collapseCount->Value(),"COLLAPSECOUNT")!=0)
        {
          pCoreMesh->destroy();
            delete pCoreMesh;
          pCoreSubmesh->destroy();
            delete pCoreSubmesh;
            CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
            return 0;
        }

        node = collapseCount->FirstChild();
        if(!node)
        {
              pCoreMesh->destroy();
            delete pCoreMesh;
            pCoreSubmesh->destroy();
              delete pCoreSubmesh;
            CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
            return 0;
        }
        TiXmlText* collapseCountdata = node->ToText();
        if(!collapseCountdata)
        {
              pCoreMesh->destroy();
            delete pCoreMesh;
            pCoreSubmesh->destroy();
              delete pCoreSubmesh;
            CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
            return 0;
        }
        Vertex.faceCollapseCount= atoi(collapseCountdata->Value());
        collapse = collapseCount->NextSiblingElement();       
      }
      else
      {
        Vertex.collapseId=-1;
        Vertex.faceCollapseCount=0;
      }


      TiXmlElement *texcoord = collapse;

      // load all texture coordinates of the vertex
      int textureCoordinateId;
      for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
      {
        CalCoreSubmesh::TextureCoordinate textureCoordinate;
        // load data of the influence
#if CAL3D_VALIDATE_XML_TAGS
        if(!texcoord || stricmp(texcoord->Value(),"TEXCOORD")!=0)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
#endif
        node = texcoord->FirstChild();
        if(!node)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        TiXmlText* texcoorddata = node->ToText();
        if(!texcoorddata)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }

                          ReadPair( texcoorddata->Value(), &textureCoordinate.u, &textureCoordinate.v );

        if (loadingMode & LOADER_INVERT_V_COORD)
        {
          textureCoordinate.v = 1.0f - textureCoordinate.v;
        }


        // set texture coordinate in the core submesh instance
        pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
        texcoord = texcoord->NextSiblingElement();
      }

      // get the number of influences
      int influenceCount= atoi(vertex->Attribute("NUMINFLUENCES"));

      if(influenceCount < 0)
      {
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        pCoreSubmesh->destroy();
        pCoreMesh->destroy();
        delete pCoreMesh;
        delete pCoreSubmesh;
        return 0;
      }

      // reserve memory for the influences in the vertex
      Vertex.vectorInfluence.reserve(influenceCount);
      Vertex.vectorInfluence.resize(influenceCount);

      TiXmlElement *influence = texcoord;
      
      // load all influences of the vertex
      int influenceId;
      for(influenceId = 0; influenceId < influenceCount; ++influenceId)
      {
#if CAL3D_VALIDATE_XML_TAGS
        if(!influence ||stricmp(influence->Value(),"INFLUENCE")!=0)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
#endif
        node = influence->FirstChild();
        if(!node)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        TiXmlText* influencedata = node->ToText();
        if(!influencedata)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }

        Vertex.vectorInfluence[influenceId].boneId = atoi(influence->Attribute("ID"));

        Vertex.vectorInfluence[influenceId].weight = (float) atof(influencedata->Value());

        influence=influence->NextSiblingElement();    
      }

      // set vertex in the core submesh instance
      // === pCoreSubmesh->setVertex(vertexId, Vertex);

      TiXmlElement *physique = influence;

        

      // load the physical property of the vertex if there are springs in the core submesh
      if(springCount > 0)
      {
        CalCoreSubmesh::PhysicalProperty physicalProperty;
        
        if(!physique || stricmp(physique->Value(),"PHYSIQUE")!=0)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        node = physique->FirstChild();
        if(!node)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }
        TiXmlText* physiquedata = node->ToText();
        if(!physiquedata)
        {
          pCoreMesh->destroy();
          delete pCoreMesh;
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          return 0;
        }

        physicalProperty.weight = (float) atof(physiquedata->Value());

        // set the physical property in the core submesh instance
        pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);          
        
      }


      vertex = vertex->NextSiblingElement();
   }

   TiXmlElement *spring= vertex;

   // load all springs
   int springId;
   for(springId = 0; springId < springCount; ++springId)
   {
      CalCoreSubmesh::Spring Spring;
      if(!spring ||stricmp(spring->Value(),"SPRING")!=0)
      {
        pCoreMesh->destroy();
        delete pCoreMesh;
        pCoreSubmesh->destroy();
        delete pCoreSubmesh;
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
      }
                  ReadPair( spring->Attribute("VERTEXID"), &Spring.vertexId[0], &Spring.vertexId[1] );
      Spring.springCoefficient = (float) atof(spring->Attribute("COEF"));
      Spring.idleLength = (float) atof(spring->Attribute("LENGTH"));

      // set spring in the core submesh instance
      pCoreSubmesh->setSpring(springId, Spring);
      spring = spring->NextSiblingElement();
   }

   TiXmlElement *face = spring;

         TiXmlElement * morph = face;
         for( int morphId = 0; morphId < morphCount; morphId++ ) {
           if( stricmp(morph->Value(), "MORPH") != 0 ) {
             pCoreMesh->destroy();
             delete pCoreMesh;
             pCoreSubmesh->destroy();
             delete pCoreSubmesh;
             CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
             return 0;
           }
           CalCoreSubMorphTarget * morphTarget = new CalCoreSubMorphTarget();
           if( !morphTarget->create() ) {
             pCoreMesh->destroy();
             delete pCoreMesh;
             pCoreSubmesh->destroy();
             delete pCoreSubmesh;
             CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
             return 0;
           }
           if( !morphTarget->reserve(vertexCount) ) {
             pCoreMesh->destroy();
             delete pCoreMesh;
             pCoreSubmesh->destroy();
             delete pCoreSubmesh;
             CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
             return 0;
           }

           morphTarget->setName(morph->Attribute("NAME"));
           
           TiXmlElement * blendVert = morph->FirstChildElement();
           for( int blendVertI = 0; blendVertI < vertexCount; blendVertI++ ) {
             CalCoreSubMorphTarget::BlendVertex Vertex;
             Vertex.textureCoords.clear();
             Vertex.textureCoords.reserve(textureCoordinateCount);

             bool copyOrig = true;
             if( blendVert && !stricmp(blendVert->Value(), "BLENDVERTEX") ) {
               int vertId = atoi(blendVert->Attribute("VERTEXID"));

               if( vertId == blendVertI ) {
                 copyOrig = false;
               }
             }

             if( !copyOrig ) {
               if( !ValidateTag(blendVert, "BLENDVERTEX", pCoreMesh, pCoreSubmesh) ) {
                 return 0;
               }

               TiXmlElement *pos= blendVert->FirstChildElement();
               if(!CalVectorFromXml(pos, "POSITION", &Vertex.position, pCoreMesh, pCoreSubmesh)) {
                 return 0;
               }
               
               TiXmlElement *norm = pos->NextSiblingElement();
               if(!CalVectorFromXml(norm, "NORMAL", &Vertex.normal, pCoreMesh, pCoreSubmesh)) {
                 return 0;
               }

               TiXmlElement * texcoord = norm->NextSiblingElement();
               int textureCoordinateId;
               for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
               {
                 CalCoreSubmesh::TextureCoordinate textureCoordinate;
                 if(!TexCoordFromXml(texcoord, "TEXCOORD", &textureCoordinate, 
                   loadingMode,
                   pCoreMesh, pCoreSubmesh)) {
                   return 0;
                 }
                 Vertex.textureCoords.push_back(textureCoordinate);
                 texcoord = texcoord->NextSiblingElement();
               }
               blendVert = blendVert->NextSiblingElement();
               morphTarget->setBlendVertex(blendVertI, Vertex);
             } else {
#if 0
               CalCoreSubmesh::Vertex & origVertex = vectorVertex[blendVertI];
               // use the origVertex
               Vertex.position = origVertex.position;
               Vertex.normal = origVertex.normal;
               std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorvectorTextureCoordinate =
                 pCoreSubmesh->getVectorVectorTextureCoordinate();

               for( int tcI = 0; tcI < textureCoordinateCount; tcI++ ) {
                 Vertex.textureCoords.push_back( vectorvectorTextureCoordinate[tcI][blendVertI] );
               }
#endif
             }

           }
           pCoreSubmesh->addCoreSubMorphTarget(morphTarget);

           morph = morph->NextSiblingElement();
         }

         face = morph;
   // load all faces
   int faceId;
   for(faceId = 0; faceId < faceCount; ++faceId)
   {
      CalCoreSubmesh::Face Face;

#if CAL3D_VALIDATE_XML_TAGS
      if(!face || stricmp(face->Value(),"FACE")!=0)
      {
        pCoreMesh->destroy();
        delete pCoreMesh;
        pCoreSubmesh->destroy();
        delete pCoreSubmesh;
        CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
        return 0;
      }
#endif
      int tmp[3];

      // load data of the face
#if CAL3D_USE_STL_INSTEAD_OF_SSCANF
      str.clear();
      str << face->Attribute("VERTEXID");
      str >> tmp[0] >> tmp [1] >> tmp[2];
#else
                  sscanf( face->Attribute("VERTEXID"), "%d %d %d", &tmp[0], &tmp[1], &tmp[2] );
#endif

      if(sizeof(CalIndex)==2)
      {
        if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
        {
          CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
          pCoreSubmesh->destroy();
          delete pCoreSubmesh;
          return 0;
        }
      }
      Face.vertexId[0]=tmp[0];
      Face.vertexId[1]=tmp[1];
      Face.vertexId[2]=tmp[2];

      pCoreSubmesh->setFace(faceId, Face);

      face=face->NextSiblingElement();
    }
    submesh=submesh->NextSiblingElement();
  
    // add the core submesh to the core mesh instance
      pCoreMesh->addCoreSubmesh(pCoreSubmesh);

  }


// explicitly close the file
doc.Clear();
return pCoreMesh;
}



 /*****************************************************************************/
/** Loads a core material instance from a XML file.
  *
  * This function loads a core material instance from a XML file.
  *
  * @param strFilename The name of the file to load the core material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/


CalCoreMaterial *CalLoader::loadXmlCoreMaterial(const std::string& strFilename)
{
  std::stringstream str;
  TiXmlDocument doc(strFilename);
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }
  return loadXmlCoreMaterial(doc);

}

 /*****************************************************************************/
/** Loads a core material instance from a XML file.
  *
  * This function loads a core material instance from a XML file.
  *
  * @param strFilename The name of the file to load the core material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/


CalCoreMaterial *CalLoader::loadXmlCoreMaterial(TiXmlDocument & doc)
{
  std::stringstream str;

  const std::string strFilename = "";
  TiXmlNode* node;

  TiXmlElement*header = doc.FirstChildElement();
  if(!header || stricmp(header->Value(),"HEADER")!=0)
  {
    str.clear();
    str << "Header element is " << (header ? header->Value() : "<unknown>");
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, str.str());
    return 0;
  }
  
  
  if(stricmp(header->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_EXTENSION)!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    return 0;
  }    
  
  if(atoi(header->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
        return 0;
  }

  TiXmlElement*material = header->NextSiblingElement();
  if(!material||stricmp(material->Value(),"MATERIAL")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
      return 0;
  }
  

  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = new CalCoreMaterial();
  if(pCoreMaterial == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core material instance
  if(!pCoreMaterial->create())
  {
    delete pCoreMaterial;
    return 0;
  }

  TiXmlElement* ambient = material->FirstChildElement();
  if(!ambient ||stricmp(ambient->Value(),"AMBIENT")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }

  CalCoreMaterial::Color ambientColor; 
  node = ambient->FirstChild();
  if(!node)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  TiXmlText* ambientdata = node->ToText();
  if(!ambientdata)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }

  int r,g,b,a;

  str << ambientdata->Value();
  str >> r >> g >> b >> a;
  ambientColor.red = (unsigned char)r;
  ambientColor.green = (unsigned char)g;
  ambientColor.blue = (unsigned char)b;
  ambientColor.alpha = (unsigned char)a; 

  TiXmlElement* diffuse = ambient->NextSiblingElement();
  if(!diffuse || stricmp(diffuse->Value(),"DIFFUSE")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }

  CalCoreMaterial::Color diffuseColor; 
  node = diffuse->FirstChild();
  if(!node)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  TiXmlText* diffusedata = node->ToText();
  if(!diffusedata)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  str.clear();
  str << diffusedata->Value();
  str >> r >> g >> b >> a;
  diffuseColor.red = (unsigned char)r;
  diffuseColor.green = (unsigned char)g;
  diffuseColor.blue = (unsigned char)b;
  diffuseColor.alpha = (unsigned char)a;
  

  TiXmlElement* specular = diffuse->NextSiblingElement();
  if(!specular||stricmp(specular->Value(),"SPECULAR")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }

  CalCoreMaterial::Color specularColor; 
  node = specular->FirstChild();
  if(!node)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  TiXmlText* speculardata = node->ToText();
  if(!speculardata)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  str.clear();
  str << speculardata->Value();
  str >> r >> g >> b >> a;
  specularColor.red = (unsigned char)r;
  specularColor.green = (unsigned char)g;
  specularColor.blue = (unsigned char)b;
  specularColor.alpha = (unsigned char)a;


  TiXmlElement* shininess = specular->NextSiblingElement();
  if(!shininess||stricmp(shininess->Value(),"SHININESS")!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }

  float fshininess;
  node = shininess->FirstChild();
  if(!node)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  TiXmlText* shininessdata = node->ToText();
  if(!shininessdata)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
  }
  fshininess = (float)atof(shininessdata->Value());

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(fshininess);
  
  std::vector<std::string> MatFileName;
  std::vector<std::string> MatTypes;

  TiXmlElement* map;

  for( map = shininess->NextSiblingElement();map;map = map->NextSiblingElement() )
  {
    if(!map||stricmp(map->Value(),"MAP")!=0)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
    }
    

    node= map->FirstChild();
    if(!node)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
    }

    TiXmlText* mapfile = node->ToText();
    if(!mapfile)
    {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
    pCoreMaterial->destroy();
        delete pCoreMaterial;    
        return 0;
    }

    MatFileName.push_back(mapfile->Value());
    std::string mapType = "Diffuse Color";

    if( map->Attribute("TYPE") ) {
      mapType = map->Attribute("TYPE");
    }
    MatTypes.push_back(mapType);
  }

  pCoreMaterial->reserve(MatFileName.size());



  for (unsigned int mapId=0; mapId < MatFileName.size(); ++mapId)
  {
    CalCoreMaterial::Map Map;
    // initialize the user data
    Map.userData = 0;

    Map.strFilename= MatFileName[mapId];    
    Map.mapType = MatTypes[mapId];

    // set map in the core material instance
    pCoreMaterial->setMap(mapId, Map);
  }

  doc.Clear();
  
  return pCoreMaterial;
}

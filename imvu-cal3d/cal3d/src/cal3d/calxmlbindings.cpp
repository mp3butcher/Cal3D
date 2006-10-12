#if defined(_MSC_VER) && _MSC_VER == 1200
#pragma warning(disable: 4786)
#endif

#include "cal3d/saver.h"
#include "cal3d/error.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coretrack.h"
#include "cal3d/coremorphtrack.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/coremorphkeyframe.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"
#include "cal3d/corematerial.h"
#include "cal3d/tinyxml.h"

#ifdef WIN32
#include "cal3d/tinybind.h"
#else
#include "cal3d/tinybind.cpp"
#endif

#include "calxmlbindings.h"

TiXmlBinding<CalHeader> const *
GetTiXmlBinding( CalHeader const &,  IdentityBase  )
{
  static MemberTiXmlBinding<CalHeader> binding;
  if( binding.empty() ) {
    binding.AddMember( "VERSION", MemberAttribute(&CalHeader::version) );
    binding.AddMember( "MAGIC", MemberAttribute(&CalHeader::magic) );
  }
  return &binding;
}


TiXmlBinding<CalCoreAnimatedMorph> const *
GetTiXmlBinding( CalCoreAnimatedMorph const &,  IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreAnimatedMorph> binding;
  if( binding.empty() ) {
    binding.AddMember( "DURATION", MemberAttribute(&CalCoreAnimatedMorph::getDuration,
        &CalCoreAnimatedMorph::setDuration) );
    binding.AddMember( "TRACK", MemberPeer(&CalCoreAnimatedMorph::getListCoreTrack) );
  }
  return &binding;
}

TiXmlBinding<CalCoreMorphTrack> const *
GetTiXmlBinding( CalCoreMorphTrack const &,  IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreMorphTrack> binding;
  if( binding.empty() ) {
    binding.AddMember( "MORPHNAME", MemberAttribute(&CalCoreMorphTrack::getMorphName,
        &CalCoreMorphTrack::setMorphName) );
    binding.AddMember( "NUMKEYFRAMES", MemberAttribute(&CalCoreMorphTrack::getCoreMorphKeyframeCount,
        &CalCoreMorphTrack::reserve) );
    binding.AddMember( "KEYFRAME", MemberPeer(&CalCoreMorphTrack::getVectorCoreMorphKeyframes) );
  }
  return &binding;
}

TiXmlBinding<CalCoreMorphKeyframe> const *
GetTiXmlBinding( CalCoreMorphKeyframe const &,  IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreMorphKeyframe> binding;
  if( binding.empty() ) {
    binding.AddMember( "TIME", MemberAttribute(&CalCoreMorphKeyframe::getTime,
        &CalCoreMorphKeyframe::setTime) );
    binding.AddMember( "WEIGHT", Member(&CalCoreMorphKeyframe::getWeight,
        &CalCoreMorphKeyframe::setWeight) );
  }
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::VectorFace> const *
GetTiXmlBinding( CalCoreSubmesh::VectorFace const &,  IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::Face,
    CalCoreSubmesh::VectorFace> binding(false, NULL, "NUMFACES");
  return &binding;
}
TiXmlBinding<CalCoreSubmesh::VectorPhysicalProperty> const *
GetTiXmlBinding( CalCoreSubmesh::VectorPhysicalProperty const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::PhysicalProperty,
    CalCoreSubmesh::VectorPhysicalProperty> binding(false);
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::VectorSpring> const *
GetTiXmlBinding( CalCoreSubmesh::VectorSpring const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::Spring,
    CalCoreSubmesh::VectorSpring> binding(false, NULL, "NUMSPRINGS");
  return &binding;
}

#if 0
TiXmlBinding<CalCoreSubmesh::VectorVectorTangentSpace> const *
GetTiXmlBinding( CalCoreSubmesh::VectorVectorTangentSpace const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::VectorTangentSpace, CalCoreSubmesh::VectorVectorTangentSpace> binding(false);
  return &binding;
}
#endif

TiXmlBinding<CalCoreSubmesh::VectorVectorTextureCoordinate> const *
GetTiXmlBinding( CalCoreSubmesh::VectorVectorTextureCoordinate const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::VectorTextureCoordinate,
    CalCoreSubmesh::VectorVectorTextureCoordinate> binding(false, NULL, "NUMTEXCOORDS");
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::VectorTextureCoordinate> const *
GetTiXmlBinding( CalCoreSubmesh::VectorTextureCoordinate const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::TextureCoordinate,
    CalCoreSubmesh::VectorTextureCoordinate> binding(false, NULL, "NUMTEXCOORDS");
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::VectorVertex> const *
GetTiXmlBinding( CalCoreSubmesh::VectorVertex const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::Vertex,
    CalCoreSubmesh::VectorVertex> binding(false, NULL, "NUMVERTICES");
  return &binding;
}


TiXmlBinding<CalCoreSubmesh::VectorInfluence> const *
GetTiXmlBinding( CalCoreSubmesh::VectorInfluence const &,IdentityBase  )
{
  static StlContainerTiXmlBinding<CalCoreSubmesh::Influence,
    CalCoreSubmesh::VectorInfluence> binding(false, NULL, "NUMINFLUENCES");
  return &binding;
}


TiXmlBinding<CalCoreMesh::CalCoreSubmeshVector> const *
GetTiXmlBinding( CalCoreMesh::CalCoreSubmeshVector const &, Identity<CalCoreMesh::CalCoreSubmeshVector>  )
{
  static StlContainerPtrBinding<CalCoreSubmesh, CalCoreMesh::CalCoreSubmeshVector> binding(false);
  return &binding;
}


TiXmlBinding<CalCoreSubmesh::CoreSubMorphTargetVector> const *
GetTiXmlBinding( CalCoreSubmesh::CoreSubMorphTargetVector const &,Identity<CalCoreSubmesh::CoreSubMorphTargetVector>  )
{
  static StlContainerPtrBinding<CalCoreSubMorphTarget,
    CalCoreSubmesh::CoreSubMorphTargetVector> binding(false, NULL, "NUMMORPHS" );
  return &binding;
}


TiXmlBinding<CalCoreSubMorphTarget::VectorBlendVertex> const *
GetTiXmlBinding( CalCoreSubMorphTarget::VectorBlendVertex const &,Identity<CalCoreSubMorphTarget::VectorBlendVertex>  )
{
  static StlContainerPtrBinding<CalCoreSubMorphTarget::BlendVertex,
    CalCoreSubMorphTarget::VectorBlendVertex> binding(false, NULL, "NUMBLENDVERTS");
  return &binding;
}



TiXmlBinding<CalCoreMesh> const *
GetTiXmlBinding( CalCoreMesh const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreMesh> binding;
  if( binding.empty() ) {
    binding.AddMember( "NUMSUBMESH", MemberAttribute(&CalCoreMesh::getCoreSubmeshCount,
        &CalCoreMesh::reserve) );
    binding.AddMember( "SUBMESH", MemberPeer(&CalCoreMesh::getVectorCoreSubmesh) );
  }
  return &binding;
}

TiXmlBinding<CalCoreSubmesh> const *
GetTiXmlBinding( CalCoreSubmesh const &, IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh> binding;
  if( binding.empty() ) {
    binding.AddMember( "MATERIAL", MemberAttribute(&CalCoreSubmesh::getCoreMaterialThreadId,
        &CalCoreSubmesh::setCoreMaterialThreadId ) );    
    binding.AddMember( "NUMLODSTEPS", MemberAttribute(&CalCoreSubmesh::getLodCount,
        &CalCoreSubmesh::setLodCount ) );    
    binding.AddMember( "VERTEX", MemberPeer(&CalCoreSubmesh::getVectorVertex) );
    binding.AddMember( "SPRING", MemberPeer(&CalCoreSubmesh::getVectorSpring) );
    binding.AddMember( "MORPH", MemberPeer(&CalCoreSubmesh::getVectorCoreSubMorphTarget) );
    binding.AddMember( "FACE", MemberPeer(&CalCoreSubmesh::getVectorFace) );
  }
  return &binding;
}

char const *
ConvertToString( CalCoreSubmesh::TextureCoordinate const & textureCoordinate )
{
  static std::stringstream str;
  static std::string outStr;
  str.str("");
  str << textureCoordinate.u << " "
      << textureCoordinate.v;
  outStr = str.str();
  return outStr.c_str();
  
}

void
ConvertFromString( char const * inStr, CalCoreSubmesh::TextureCoordinate * textureCoordinate )
{
  static std::stringstream str;
  str.str("");
  str << inStr;
  str >> textureCoordinate->u >>
    textureCoordinate->v;
}

char const *
ConvertToString( CalVector const & v )
{
  static std::stringstream str;
  static std::string outStr;
  str.str("");
  str << v.x << " " << v.y << " "
      << v.y;
  outStr = str.str();
  return outStr.c_str();
  
}

void
ConvertFromString( char const * inStr, CalVector * v )
{
  static std::stringstream str;
  str.str("");
  str << inStr;
  str >> v->x >> v->y >> v->z;
}


char const *
ConvertToString( CalIndex const vertexId[3] )
{
  static std::stringstream str;
  static std::string outStr;
  str.str("");
  str << vertexId[0]<< " "
      << vertexId[1]<< " "
      << vertexId[2];
  outStr = str.str();
  return outStr.c_str();
}

void
ConvertFromString( char const * inStr, CalIndex (*vertexId)[3] )
{
  static std::stringstream str;
  str.str("");
  str << inStr;
  str >> *vertexId[0] >> *vertexId[1] >> *vertexId[2];
}

void
ConvertFromString( char const * inStr, CalIndex (*vertexId)[2] )
{
  static std::stringstream str;
  str.str("");
  str << inStr;
  str >> *vertexId[0] >> *vertexId[1] ;
}

TiXmlBinding<CalCoreSubmesh::Influence> const *
GetTiXmlBinding( CalCoreSubmesh::Influence const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh::Influence> binding;
  if( binding.empty() ) {
    binding.AddMember( "ID", MemberAttribute(&CalCoreSubmesh::Influence::boneId) );
    binding.AddMember( "INFLUENCE", MemberPeer(&CalCoreSubmesh::Influence::weight) );
  }
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::PhysicalProperty> const *
GetTiXmlBinding( CalCoreSubmesh::PhysicalProperty const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh::PhysicalProperty> binding;
  if( binding.empty() ) {
    binding.AddMember( "PHYSIQUE", MemberPeer(&CalCoreSubmesh::PhysicalProperty::weight) );
  }
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::Vertex> const *
GetTiXmlBinding( CalCoreSubmesh::Vertex const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh::Vertex> binding;
  if( binding.empty() ) {
    binding.AddMember( "POS", Member(&CalCoreSubmesh::Vertex::position) );
    binding.AddMember( "NORM", Member(&CalCoreSubmesh::Vertex::normal) );
    binding.AddMember( "COLOR", Member(&CalCoreSubmesh::Vertex::vertexColor) );
    //binding.AddMember( "TEXCOORD", MemberPeer(&CalCoreSubmesh::Vertex::vectorTexCoord) );
    binding.AddMember( "INFLUENCE", MemberPeer(&CalCoreSubmesh::Vertex::vectorInfluence) );
    binding.AddMember( "COLLAPSEID", Member(&CalCoreSubmesh::Vertex::collapseId) )->setFlags(MemberOptional);
    binding.AddMember( "COLLAPSECOUNT", Member(&CalCoreSubmesh::Vertex::faceCollapseCount) )->setFlags(MemberOptional);
  }
  return &binding;
}



TiXmlBinding<CalCoreSubmesh::Face> const *
GetTiXmlBinding( CalCoreSubmesh::Face const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh::Face> binding;
  if( binding.empty() ) {
    binding.AddMember( "VERTEXID", MemberAttribute(&CalCoreSubmesh::Face::vertexId) );
  }
  return &binding;
}
  
TiXmlBinding<CalCoreSubmesh::Spring> const *
GetTiXmlBinding( CalCoreSubmesh::Spring const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubmesh::Spring> binding;
  if( binding.empty() ) {
    binding.AddMember( "VERTEXID", MemberAttribute(&CalCoreSubmesh::Spring::vertexId) );
    binding.AddMember( "COEF", MemberAttribute(&CalCoreSubmesh::Spring::springCoefficient) );
    binding.AddMember( "LENGTH", MemberAttribute(&CalCoreSubmesh::Spring::idleLength) );
  }
  return &binding;
}

  
TiXmlBinding<CalCoreSubMorphTarget> const *
GetTiXmlBinding( CalCoreSubMorphTarget const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubMorphTarget> binding;
  if( binding.empty() ) {
    binding.AddMember( "NAME", MemberAttribute(&CalCoreSubMorphTarget::name,
        &CalCoreSubMorphTarget::setName) );
    binding.AddMember( "BLENDVERTEX", MemberPeer(&CalCoreSubMorphTarget::getVectorBlendVertex) );
  }
  return &binding;
}

TiXmlBinding<CalCoreSubMorphTarget::BlendVertex> const *
GetTiXmlBinding( CalCoreSubMorphTarget::BlendVertex const &,IdentityBase  )
{
  static MemberTiXmlBinding<CalCoreSubMorphTarget::BlendVertex> binding;
  if( binding.empty() ) { 
    binding.AddMember( "POSITION", Member(&CalCoreSubMorphTarget::BlendVertex::position) );
    binding.AddMember( "NORMAL", Member(&CalCoreSubMorphTarget::BlendVertex::normal) );
    binding.AddMember( "TEXCOORD", MemberPeer(&CalCoreSubMorphTarget::BlendVertex::textureCoords) )->setFlags(MemberOptional);
  }
  return &binding;
}





TiXmlBinding<CalVector> const *
GetTiXmlBinding( CalVector const &, IdentityBase  )
{
  static GenericTiXmlBinding<CalVector> binding;
  return &binding;
}

TiXmlBinding<CalCoreSubmesh::TextureCoordinate> const *
GetTiXmlBinding( CalCoreSubmesh::TextureCoordinate const &, IdentityBase  )
{
  static GenericTiXmlBinding<CalCoreSubmesh::TextureCoordinate> binding;
  return &binding;
}

// -*-c++-*-

# ifndef __GLSL_CG_DATA_TYPES // the space after '#' is necessary to
                              // differentiate `sed' defines from GLSL one's
  // remove half float types on non-nVidia videocards
  # define half    float
  # define half2   vec2
  # define half3   vec3
  # define half4   vec4
  # define half3x3 mat3
  # define ivec4   vec4
# endif

#if BONES_COUNT >= 1
# define weight gl_MultiTexCoord2
# define index  gl_MultiTexCoord3

uniform mat3 rotationMatrices[31];
uniform vec3 translationVectors[31];
#endif

varying vec3 vNormal;

#if NORMAL_MAPPING == 1 || BUMP_MAPPING == 1
# define inputTangent     (gl_MultiTexCoord1.xyz /* / 32767.0 */)
# define inputHandedness   gl_MultiTexCoord1.w
varying vec3 tangent;
varying vec3 binormal;
#endif

#if FOG
varying vec3 eyeVec;
#endif

//uniform bool clipPlanesUsed;

void main()
{
#if TEXTURING == 1 || NORMAL_MAPPING == 1 || BUMP_MAPPING == 1
    gl_TexCoord[0].st = gl_MultiTexCoord0.st; // export texCoord to fragment shader
#endif

#if BONES_COUNT >= 1
    mat3 totalRotation = weight.x * rotationMatrices[int(index.x)];
    vec3 transformedPosition = weight.x * translationVectors[int(index.x)];

#if BONES_COUNT >= 2
    totalRotation += weight.y * rotationMatrices[int(index.y)];
    transformedPosition += weight.y * translationVectors[int(index.y)];

#if BONES_COUNT >= 3
    totalRotation += weight.z * rotationMatrices[int(index.z)];
    transformedPosition += weight.z * translationVectors[int(index.z)];

#if BONES_COUNT >= 4
    totalRotation += weight.w * rotationMatrices[int(index.w)];
    transformedPosition += weight.w * translationVectors[int(index.w)];
#endif // BONES_COUNT >= 4
#endif // BONES_COUNT >= 3
#endif // BONES_COUNT >= 2

    transformedPosition += totalRotation * gl_Vertex.xyz;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(transformedPosition, 1.0);
    # ifdef __GLSL_CG_DATA_TYPES
      gl_ClipVertex = gl_ModelViewMatrix * vec4(transformedPosition, 1.0);
    # endif
#if 0
//  8.5 -- no clip planes
// 10.2 -- gl_ClipVertex always set (20% slowdown, on both 6600 and 8600)
// 10.8 -- if ( clipPlanesUsed /* == true */  ) gl_ClipVertex = ...
//  9.2 -- if ( clipPlanesUsed /* == false */ ) gl_ClipVertex = ...
// i.e.:
//    if ( clipPlanesUsed )  ~ 0.6-0.7 ms (uniform brancing is not optimized!)
//    gl_ClipVertex = ...    ~ 1.6 ms
// For not so hi-poly scenes gl_ClipVertex slowdown is 2-4%.
// gl_ClipVertex slowdown is independent on whether glClipPlane is
// used or not.
//
// For performance it's better to make separate shaders and determine
// at the draw time which to use. But we can get too many shaders
// (count of shaders is doubled with another option in vertex shader).
// And also shader with clip planes support must be also compiled at
// the GLObjectsVisitor stage (but can be not used).

// ATI doesn't support programmable clipping, gl_ClipVertex assignment
// in shaders throw it into software mode. ATI only support fixed
// function clipping, when ftransform() is used. 
// But people say clipping on ati works w/o any ftransform() or gl_ClipVertex
#endif

#if FOG
    eyeVec = (gl_ModelViewMatrix * vec4(transformedPosition, 1.0)).xyz;
#endif // no fog

    vNormal = gl_NormalMatrix * (totalRotation * gl_Normal);
#if NORMAL_MAPPING == 1 || BUMP_MAPPING == 1
    tangent = gl_NormalMatrix * (totalRotation * inputTangent);
    binormal = cross( vNormal, tangent ) * inputHandedness;
#endif // no tangent space

#else // no bones

    // dont touch anything when no bones influence mesh
    gl_Position = ftransform();
    # ifdef __GLSL_CG_DATA_TYPES
      gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
    # endif
#if FOG
    eyeVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
#endif // no fog

    vNormal = gl_NormalMatrix * gl_Normal;
#if NORMAL_MAPPING == 1 || BUMP_MAPPING == 1
    tangent = gl_NormalMatrix * inputTangent;
    binormal = cross( vNormal, tangent ) * inputHandedness;
#endif // no tangent space

#endif // BONES_COUNT >= 1
}

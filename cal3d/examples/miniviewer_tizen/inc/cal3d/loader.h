//****************************************************************************//
// loader.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_LOADER_H
#define CAL_LOADER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//


#include <string>
#include <istream>
#include "cal3d/global.h"
#include "cal3d/datasource.h"
#include "cal3d/coreanimation.h"
#include "cal3d/corematerial.h"
#include "cal3d/coremesh.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/tinyxml.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreModel;
class CalCoreBone;
class CalCoreAnimation;
class CalCoreAnimatedMorph;
class CalCoreTrack;
class CalCoreKeyframe;
class CalCoreMorphTrack;
class CalCoreMorphKeyframe;
class CalCoreMesh;
class CalCoreSubmesh;
class CalCoreMaterial;
class CalVector;
class CalQuaternion;

namespace cal3d
{
	class TiXmlDocument;
}

enum
{
    LOADER_ROTATE_X_AXIS = 1,
    LOADER_INVERT_V_COORD = 2,
    LOADER_FLIP_WINDING = 4
};


bool CAL3D_API CalVectorFromDataSrc( CalDataSource & dataSrc, CalVector * calVec );


 /*****************************************************************************/
/** The loader class.
  *****************************************************************************/

class CAL3D_API CalLoader
{
// member functions
public:
  static unsigned int const keyframeBitsPerOriComponent;
  static unsigned int const keyframeBitsPerTime;

  static unsigned int const keyframeBitsPerUnsignedPosComponent;
  static unsigned int const keyframeBitsPerPosPadding;
  static float const keyframePosRange;
  static unsigned int const keyframePosBytes;

  static unsigned int const keyframeBitsPerUnsignedPosComponentSmall;
  static unsigned int const keyframeBitsPerPosPaddingSmall;
  static float const keyframePosRangeSmall;
  static unsigned int const keyframePosBytesSmall;
  static CalCoreAnimationPtr loadCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel=NULL);
  static CalCoreMaterialPtr  loadCoreMaterial(const std::string& strFilename);
  static CalCoreMeshPtr      loadCoreMesh(const std::string& strFilename);
  static CalCoreSkeletonPtr  loadCoreSkeleton(const std::string& strFilename);

  static CalCoreAnimatedMorph *loadCoreAnimatedMorph(const std::string& strFilename);

  static CalCoreAnimationPtr loadCoreAnimation(std::istream& inputStream, CalCoreSkeleton *skel=NULL);
  static CalCoreAnimatedMorph *loadCoreAnimatedMorph(std::istream& inputStream);
  static CalCoreMaterialPtr  loadCoreMaterial(std::istream& inputStream);
  static CalCoreMeshPtr      loadCoreMesh(std::istream& inputStream);
  static CalCoreSkeletonPtr  loadCoreSkeleton(std::istream& inputStream);

  static CalCoreAnimationPtr loadCoreAnimation(void* inputBuffer, CalCoreSkeleton *skel=NULL);
  static CalCoreAnimatedMorph *loadCoreAnimatedMorphFromBuffer(void* inputBuffer, unsigned int len);
  static CalCoreMaterialPtr  loadCoreMaterial(void* inputBuffer);
  static CalCoreMeshPtr      loadCoreMesh(void* inputBuffer);
  static CalCoreSkeletonPtr  loadCoreSkeleton(void* inputBuffer);

  static CalCoreAnimationPtr loadCoreAnimation(CalDataSource& inputSrc, CalCoreSkeleton *skel=NULL);
  static CalCoreAnimatedMorph *loadCoreAnimatedMorph(CalDataSource& inputSrc);
  static CalCoreMaterialPtr  loadCoreMaterial(CalDataSource& inputSrc);
  static CalCoreMeshPtr      loadCoreMesh(CalDataSource& inputSrc);
  static CalCoreSkeletonPtr  loadCoreSkeleton(CalDataSource& inputSrc);

  static void setLoadingMode(int flags);
  static void setAnimationCollapseSequencesOn( bool p );
  static void setAnimationLoadingCompressionOn( bool p );
  static void setAnimationTranslationTolerance( double p );
  static void setAnimationRotationToleranceDegrees( double p );

  static bool getAnimationLoadingCompressionOn() { return loadingCompressionOn; }
  static CalCoreKeyframe *loadCoreKeyframe(CalDataSource& dataSrc);
  static CalCoreKeyframe *loadCompressedCoreKeyframe(CalDataSource& dataSrc, const CalVector &trackMinPt, const CalVector &trackScale, float trackDuration);
  static double getAnimationTranslationTolerance() { return translationTolerance; }
  static double getAnimationRotationToleranceDegrees() { return rotationToleranceDegrees; }
  static int getAnimationNumEliminatedKeyframes() { return numEliminatedKeyframes; }
  static int getAnimationNumKeptKeyframes() { return numKeptKeyframes; }
  static int getAnimationNumRoundedKeyframes() { return numRoundedKeyframes; }
  static int getAnimationNumCompressedAnimations() { return numCompressedAnimations; }
  static void addAnimationCompressionStatistic( int totalKeyframes, int eliminatedKeyframes, int numRounded ) {
    numEliminatedKeyframes += eliminatedKeyframes;
    numKeptKeyframes += totalKeyframes - eliminatedKeyframes;
    numRoundedKeyframes += numRounded;
    numCompressedAnimations++;
  }
  static void resetCompressionStatistics() {
    numEliminatedKeyframes = 0;
    numKeptKeyframes = 0;
    numCompressedAnimations = 0;
  }
  static bool usesAnimationCompression( int version );
  static unsigned int compressedKeyframeRequiredBytes( CalCoreKeyframe * lastCoreKeyframe, bool translationRequired, bool highRangeRequired, bool translationIsDynamic );
  static unsigned int readCompressedKeyframe( unsigned char * buf, unsigned int bytes, CalCoreBone * coreboneOrNull,
    CalVector * vecResult, CalQuaternion * quatResult, float * timeResult,
    CalCoreKeyframe * lastCoreKeyframe,
    bool translationRequired, bool highRangeRequired, bool translationIsDynamic,
    bool useAnimationCompression);
  static unsigned int writeCompressedKeyframe( unsigned char * buf, unsigned int bufLen, const std::string& strFilename, 
    CalVector const & translation, CalQuaternion const & rotation, float caltime,
    int version, 
    bool needTranslation, bool highRangeRequired );
  static void compressCoreAnimation( CalCoreAnimation * anim, CalCoreSkeleton *skel );

  // xmlformat.cpp
  static CalCoreAnimationPtr loadXmlCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel=NULL);
  static CalCoreAnimationPtr loadXmlCoreAnimation(const char*, CalCoreSkeleton* skel);
  static CalCoreAnimationPtr loadXmlCoreAnimation(cal3d::TiXmlDocument & doc, CalCoreSkeleton* skel);
  static CalCoreSkeletonPtr loadXmlCoreSkeleton(const std::string& strFilename);
  static CalCoreMeshPtr loadXmlCoreMesh(const std::string& strFilename);
  static CalCoreMaterialPtr loadXmlCoreMaterial(const std::string& strFilename);
  static CalCoreAnimatedMorph *loadXmlCoreAnimatedMorph(const std::string& strFilename);
  static CalCoreAnimatedMorph *loadXmlCoreAnimatedMorph(const void *);
  static CalCoreAnimatedMorph *loadXmlCoreAnimatedMorph(cal3d::TiXmlDocument & doc);

  static CalCoreSkeletonPtr loadXmlCoreSkeletonFromFile(const std::string& strFilename);
  static CalCoreSkeletonPtr loadXmlCoreSkeleton(const void *);

  static CalCoreMeshPtr loadXmlCoreMesh(const void *);

  static CalCoreMaterialPtr loadXmlCoreMaterial(const void *);

private:
  static CalCoreBone *loadCoreBones(CalDataSource& dataSrc, int version);
  static CalCoreKeyframe *loadCoreKeyframe(CalDataSource& dataSrc, CalCoreBone * coreboneOrNull, 
                                             int version, CalCoreKeyframe * lastCoreKeyframe, 
                                             bool translationRequired, bool highRangeRequired, bool translationIsDynamic,
                                             bool useAnimationCompression);
  static CalCoreMorphKeyframe *loadCoreMorphKeyframe(CalDataSource& dataSrc);
  static CalCoreSubmesh *loadCoreSubmesh(CalDataSource& dataSrc, int version);
  static CalCoreTrack *loadCoreTrack(CalDataSource & dataSrc, CalCoreSkeleton * skel, int version, bool useAnimationCompresssion);
  static CalCoreMorphTrack *loadCoreMorphTrack(CalDataSource& dataSrc);

  static CalCoreSkeletonPtr loadXmlCoreSkeleton(cal3d::TiXmlDocument& doc);
  static CalCoreMeshPtr loadXmlCoreMesh(cal3d::TiXmlDocument& doc);
  static CalCoreMaterialPtr loadXmlCoreMaterial(cal3d::TiXmlDocument& doc);

  static int loadingMode;
  static double translationTolerance;
  static double rotationToleranceDegrees;
  static bool loadingCompressionOn;
  static bool collapseSequencesOn;

  static int numEliminatedKeyframes;
  static int numKeptKeyframes;
  static int numCompressedAnimations;
  static int numRoundedKeyframes;
};


class BitWriter {
public:
  BitWriter( unsigned char * dest ) {
    dest_ = dest;
    buf_ = 0;
    bitsInBuf_ = 0;
    bytesWritten_ = 0;
  }
  void write( unsigned int data, unsigned int numBits );
  void flush();
  inline unsigned int bytesWritten() { return bytesWritten_; }
private:
  unsigned int buf_;
  unsigned int bitsInBuf_;
  unsigned int bytesWritten_;
  unsigned char * dest_;
};


class BitReader {
public:
  BitReader( unsigned char const * source ) {
    source_ = source;
    bitsInBuf_ = 0;
    buf_ = 0;
    bytesRead_ = 0;
  }
  inline void read( unsigned int * data, unsigned int numBits );
  inline unsigned int bytesRead() { return bytesRead_; }
private:
  unsigned int buf_;
  unsigned int bitsInBuf_;
  unsigned int bytesRead_;
  unsigned char const * source_;
};

inline void
BitReader::read( unsigned int * data, unsigned int numBits )
{

  // Fill up the buffer with enough bits.  I load 8 at a time, only when
  // needed, so I can never have more than 7 left over.
  assert( bitsInBuf_ < 8 );
  while( bitsInBuf_ < numBits ) {
    buf_ |= ( source_[ bytesRead_ ] << bitsInBuf_ );
    bitsInBuf_ += 8;
    bytesRead_++;
  }

  // Read the data.
  * data = buf_ & ( ( 1 << numBits ) - 1 );
  buf_ >>= numBits;
  bitsInBuf_ -= numBits;
}

inline unsigned int
FloatZeroToOneToFixedPoint( float zeroToOne, unsigned int numBits )
{

  // Consider the case of numBits = 2.
  // Then maxVal = 3.
  // The fractional values convert to fixed point at these thresholds:
  //
  //    0/1 --> 0
  //    1/6 --> 1
  //    3/6 --> 2
  //    5/6 --> 3
  //
  // Then they convert back by:
  //
  //    0 --> 0/1
  //    1 --> 1/3
  //    2 --> 2/3
  //    3 --> 1/1
  //
  // Note that we can represent the endpoints inclusively.  However, the midway value
  // is right on a threshold, and will get rounded up:
  //
  //    1/2 --> 2 --> 2/3.
  //
  // Thus while endpoints are represented accurately in fixed point, the midpoint will not be.
  unsigned int maxVal = ( 1 << numBits ) - 1;
  return ( unsigned int ) ( maxVal * zeroToOne + 0.5f );
}

inline float
FixedPointToFloatZeroToOne( unsigned int fixedPoint, unsigned int numBits )
{
  unsigned int maxVal = ( 1 << numBits ) - 1;
  return ( float ) fixedPoint / maxVal;
}



// Return the number of bytes written.
inline unsigned int
WriteQuatAndExtra( unsigned char * dest, float const * vals, unsigned int extra,
                 unsigned int bitsPerComponent, unsigned int bitsPerExtra )
{
  float absVals[] = { fabsf( vals[ 0 ] ), fabsf( vals[ 1 ] ), fabsf( vals[ 2 ] ), fabsf( vals[ 3 ] ) };

  // Calculate largest magnitude component.
  unsigned int i;
  unsigned int bigi = 0;
  float biggest = absVals[ bigi ];
  for( i = 1; i < 4; i++ ) {
    if( absVals[ i ] > biggest ) {
      biggest = absVals[ i ];
      bigi = i;
    }
  }

  // If largest component is negative, reverse sign of all components including largest, so
  // that I can assume largest in result is non-negative.
  unsigned int signOne = ( vals[ bigi ] < 0 ) ? 0 : 1;
  unsigned int signZero = 1 - signOne;

  // Format: 
  // selection (2), 
  // asign (1), afixed (n), 
  // bsign (1), bfixed (n), 
  // csign (1), cfixed (n), 
  // extra
  BitWriter bw( dest );
  bw.write( bigi, 2 );
  for( i = 0; i < 4; i++ ) {
    if( i != bigi ) {

      // Add the sign bit for the component.
      if( vals[ i ] < 0 ) {
        bw.write( signOne, 1 );
      } else {
        bw.write( signZero, 1 );
      }

      // Add the fixed point bits.
      bw.write( FloatZeroToOneToFixedPoint( absVals[ i ], bitsPerComponent ), bitsPerComponent );
    }
  }
  bw.write( extra, bitsPerExtra );
  bw.flush();
  return bw.bytesWritten();
}



// Return the number of bytes read.
inline unsigned int
ReadQuatAndExtra( unsigned char const * data, float * valsResult, unsigned int * extraResult,
                 unsigned int bitsPerComponent, unsigned int bitsPerExtra )
{
  BitReader br( data );
  unsigned int bigi;
  br.read( & bigi, 2 ); 
  unsigned int i;
  float sum = 0.0f;
  for( i = 0; i < 4; i++ ) {
    if( i != bigi ) {
      unsigned int sign;
      br.read( & sign, 1 );
      unsigned int val;
      br.read( & val, bitsPerComponent );
      float fval = FixedPointToFloatZeroToOne( val, bitsPerComponent );
      valsResult[ i ] = sign ? - fval : fval;
      sum += valsResult[ i ] * valsResult[ i ];
    }
  }
  if( sum > 1.0f ) sum = 1.0f; // Safety for sqrt.
  valsResult[ bigi ] = sqrtf( 1.0f - sum );
  br.read( extraResult, bitsPerExtra );
  return br.bytesRead();
}

void
SetTranslationInvalid( float * xResult, float * yResult, float * zResult );
void
SetTranslationInvalid( CalVector * result );
bool
TranslationInvalid( CalVector const & result );



#endif

//****************************************************************************//

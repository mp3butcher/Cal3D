#include "tinybind.h"


// do this to support your STL container types, like list and vector
#if 0
typedef std::vector<int> StlIntVector;
TiXmlBinding<StlIntVector> const *
GetTiXmlBinding( StlIntVector const &,  StlIntVector const &  )
{
  static StlContainerTiXmlBinding<int, StlIntVector> binding(false);
  return &binding;
}
#endif

namespace cal3d{
#ifdef WIN32
#undef TIXML_USE_STL
#else
#define TIXML_USE_STL
#endif

#ifdef TIXML_USE_STL
template<class T>
char const *
ConvertToString( T const & t )
{
  std::stringstream str;
  static std::string strOut;
  str << t;
  strOut = str.str();
  return strOut.c_str();
}

template<class T>
void 
ConvertFromString( char const * strIn, T * dataOut )
{
  std::stringstream str;
  str << strIn;
  str >> *dataOut;
}

#endif


template<>
char const * 
ConvertToString<double>( double const & d )
{
  static char buffer[2048];
  sprintf(buffer, "%g", d);
  return buffer;
}

template<>
char const *
ConvertToString<float>(float const & f)
{
  return ConvertToString((double)f);
}

template<>
char const *
ConvertToString<int>(int const & d)
{
  static char buffer[2048];
  sprintf(buffer, "%d", d);
  return buffer;
}

template<>
char const *
ConvertToString<unsigned>(unsigned int const & d)
{
  static char buffer[2048];
  sprintf(buffer, "%u", d);
  return buffer;
}

template<>
char const *
ConvertToString<unsigned long>(unsigned long const & d)
{
  static char buffer[2048];
  sprintf(buffer, "%lu", d);
  return buffer;
}

template<>
char const *
ConvertToString<long>(long const & d)
{
  static char buffer[2048];
  sprintf(buffer, "%ld", d);
  return buffer;
}

#ifdef WIN64
template<>
char const *
ConvertToString<unsigned __int64>(unsigned __int64 const & d)
{
  static char buffer[2048];
  sprintf(buffer, "%I64u", d);
  return buffer;
}
#endif

template<>
char const *
ConvertToString<char const*>(char const * const & s)
{
  return s;
}

template<>
char const *
ConvertToString<std::string>(std::string const & s)
{
  return s.c_str();
}

template<>
void
ConvertFromString<char const *>(char const * strIn, const char * * dataOut)
{
  *dataOut = strIn;
}

template<>
void
ConvertFromString<std::string>(char const * strIn, std::string * dataOut)
{
  *dataOut = strIn;
}

template<>
void
ConvertFromString<int>(char const * strIn, int * dataOut)
{
  *dataOut = atoi(strIn);
}

template<>
void
ConvertFromString<unsigned>(char const * strIn, unsigned int * dataOut)
{
  *dataOut = (unsigned int) atoi(strIn);
}

template<>
void
ConvertFromString<double>(char const * strIn, double * dataOut)
{
  *dataOut = atof(strIn);
}

template<>
void
ConvertFromString<float>(char const * strIn, float * dataOut)
{
  *dataOut = (float)atof(strIn);
}
//#endif
}


using namespace cal3d;

template<class T>
TiXmlBinding<T> const *
GetTiXmlBinding( T const &, IdentityBase  )
{
  static GenericTiXmlBinding<T> binding;
  return &binding;
}


TiXmlBinding<float> const *
GetTiXmlBinding( float const &, IdentityBase  )
{
  static GenericTiXmlBinding<float> binding;
  return &binding;
}

TiXmlBinding<double> const *
GetTiXmlBinding( double const &, IdentityBase  )
{
  static GenericTiXmlBinding<double> binding;
  return &binding;
}

TiXmlBinding<int> const *
GetTiXmlBinding( int const &, IdentityBase  )
{
  static GenericTiXmlBinding<int> binding;
  return &binding;
}

TiXmlBinding<char const *> const *
GetTiXmlBinding( char const * const &, IdentityBase  )
{
  static GenericTiXmlBinding<char const *> binding;
  return &binding;
}

TiXmlBinding<std::string> const *
GetTiXmlBinding( std::string const &, IdentityBase  )
{
  static GenericTiXmlBinding<std::string> binding;
  return &binding;
}


namespace {

  struct POD {
    int x, y;
  };

  TiXmlBinding<POD> const *
  GetTiXmlBinding( POD const &, IdentityBase  )
  {
    static MemberTiXmlBinding<POD> binding;
    if( binding.empty() ) {
      binding.AddMember( "x", MemberAttribute(&POD::x) );
      binding.AddMember( "y", MemberAttribute(&POD::y) );
    }
    return &binding;
  }

  void testMemberBinding() {
    POD p = { 10, 20 };
    const TiXmlBinding<POD>* binding = GetTiXmlBinding(p, IdentityBase());
    cal3d::TiXmlElement* e = new cal3d::TiXmlElement("array");
    binding->intoXml(e, p, SerializeParams());
    // assert 
  }

  struct MemberArray {
    int array[3];
  };

  TiXmlBinding<MemberArray> const *
  GetTiXmlBinding( MemberArray const &, IdentityBase  )
  {
    static MemberTiXmlBinding<MemberArray> binding;
    if( binding.empty() ) {
      //binding.AddMember( "array", MemberAttribute(&MemberArray::array, 3) );
    }
    return &binding;
  }

  void testMemberArrays() {
    MemberArray ma = {{6, 8, 43}};
    const TiXmlBinding<MemberArray>* binding = GetTiXmlBinding(ma, IdentityBase());
    cal3d::TiXmlElement* e = new cal3d::TiXmlElement("array");
    binding->intoXml(e, ma, SerializeParams());
  }

  static struct Tester {
    Tester() {
      testMemberBinding();
      testMemberArrays();
    }
  } _;

}

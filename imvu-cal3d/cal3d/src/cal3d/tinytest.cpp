#include "tinybind.cpp"
#include <stdio.h>
#include <vector>
#include <list>

struct MyData
{
  int i;
  double d;
  char const * s;
  std::vector<int> vec;
  int iref;
  
  void setIntvalue( int in ) {
    i = in;
  }
  int intvalue() {
    return i;
  }

  int & getIRef() {
    return iref;
  }
};


TiXmlBinding<MyData> const *
GetTiXmlBinding<MyData>( MyData const &, Identity<MyData> )
{
  static MemberTiXmlBinding<MyData> binding;
  if( binding.empty() ) {
    binding.AddMember( "ITAG", Member(&MyData::i) );
    binding.AddMember( "ITAGGETSET", Member(&MyData::intvalue, &MyData::setIntvalue) );
    binding.AddMember( "DTAG", Member(&MyData::d) );
    binding.AddMember( "STAG", Member(&MyData::s) );
    binding.AddMember( "VEC", Member(&MyData::vec) );
    binding.AddMember( "IREF", Member(&MyData::getIRef) );
  }
  return &binding;
}


#if 0
typedef std::vector<int> VecT;
TiXmlBinding<VecT> const *
GetTiXmlBinding( VecT const & )
{
  static StlContainerTiXmlBinding<int, VecT> binding(true, "INT");
  return &binding;
}

typedef std::list<MyData> VecT2;
TiXmlBinding<VecT2> const *
GetTiXmlBinding( VecT2 const & )
{
  static StlContainerTiXmlBinding<MyData, VecT2> binding(false, "DATAVECENTRY");
  return &binding;
}

#endif


struct MyData2
{
  MyData dataOne;
  std::list<MyData> dataVec;
  int xyz;
};

TiXmlBinding<MyData2> const *
GetTiXmlBinding( MyData2 const &, Identity<MyData2> )
{
  static MemberTiXmlBinding<MyData2> binding;
  if( binding.empty() ) {
    binding.AddMember( "XYZ", MemberAttribute(&MyData2::xyz ))->setFlags(MemberOptional);
    binding.AddMember( "DATAONE", Member(&MyData2::dataOne) );
    binding.AddMember( "DATAVEC", MemberPeer(&MyData2::dataVec) );
  }
  return &binding;
}

int main()
{
  MyData testData;
  testData.i = 100;
  testData.iref = 52;
  testData.d = 42.3;
  testData.s = "hello world";
  testData.vec.push_back(1);
  testData.vec.push_back(1);
  testData.vec.push_back(2);
  testData.vec.push_back(3);
  testData.vec.push_back(5);
  printf( "\nTestData: %d %d %g %s %d\n", testData.i, testData.iref, testData.d, testData.s, testData.vec[0] );
  TiXmlElement test("TESTDATA");

  BindToXml( &test, testData );
  test.Print( stdout, 0 );
  MyData testData2;
  BindFromXml( test, &testData2 );
  printf( "\nTestData2: %d %d %g %s %d\n", testData2.i, testData2.iref, testData2.d, testData2.s, testData2.vec[0] );
  TiXmlElement testAgain("AGAIN");
  BindToXml( &testAgain, testData2 );
  testAgain.Print( stdout, 0 );

  MyData2 testData3;
  testData3.xyz = 10342;
  testData3.dataOne = testData2;
  testData3.dataVec.push_back(testData2);
  testData3.dataVec.push_back(testData2);
  printf( "\nTestData3: %i %i %g %s %s\n", testData3.xyz, testData3.dataOne.i,
    testData3.dataOne.d, testData3.dataOne.s, testData3.dataVec.front().s );
  
  TiXmlElement test2("TEST2");
  BindToXml( &test2, testData3 );
  test2.Print( stdout, 0 );
  MyData2 testData4;
  BindFromXml( test2, &testData4 );
  printf( "\nTestData4: %i %i %g %s %s\n", testData4.xyz, testData4.dataOne.i, testData4.dataOne.d,
    testData4.dataOne.s, testData4.dataVec.front().s );

  MyData2 testData5;
  test2.SetAttribute("XYZ", "");
  testData5.xyz = 0;
  BindFromXml( test2, &testData5 );
  printf( "\nTestData5: %i %i %g %s %s\n", testData5.xyz, testData5.dataOne.i, testData5.dataOne.d,
    testData5.dataOne.s, testData5.dataVec.front().s );
  return 0;
}

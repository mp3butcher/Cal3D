#include "TestPrologue.h"


TEST(printComment) {
  TiXmlComment c;
  c.SetValue("Comment");

  std::ostringstream os;
  c.Print(os, 0);

  CHECK_EQUAL(os.str(), "<!--Comment-->");
}


TEST(printAttribute) {
  TiXmlAttribute a("name", "value");

  std::ostringstream os;
  a.Print(os, 0);
  const char* c1 = "name=\"value\"";
  CHECK_EQUAL(os.str(), c1);


  TiXmlAttribute a2("name", "val\"ue");

  os.str("");
  a2.Print(os, 0);
  // Dunno where that &quot; came from...  looks like PutString does it.
  const char* c2 = "name='val&quot;ue'";
  CHECK_EQUAL(os.str(), c2);
}


TEST(printText) {
  const char* text = "text\"";
  TiXmlText t(text);

  std::ostringstream os;
  t.Print(os, 0);
  CHECK_EQUAL(os.str(), "text&quot;");
}


TEST(printDeclaration) {
  TiXmlDeclaration d("version", "encoding", "standalone");

  std::ostringstream os;
  d.Print(os, 0);
  const char* result = "<?xml version=\"version\" encoding=\"encoding\" standalone=\"standalone\" ?>";
  CHECK_EQUAL(os.str(), result);
}

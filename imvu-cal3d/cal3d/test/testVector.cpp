#include "TestPrologue.h"


inline std::ostream& operator<<(std::ostream& os, const CalVector& v) {
  return os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
}


TEST(Vector) {
  CalVector s, t;
  s = t = CalVector(0, 0, 0);
  CHECK_EQUAL(s, CalVector(0, 0, 0));
  CHECK_EQUAL(t, CalVector(0, 0, 0));
}

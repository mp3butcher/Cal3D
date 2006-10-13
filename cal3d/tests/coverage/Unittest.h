#ifndef CAL3D_UNITTEST
#define CAL3D_UNITTEST

#include <cstdio>

#define VERBOSE_ALL  2
#define VERBOSE_TEST 1
#define VERBOSE_NONE 0

#define TEST_INIT(testDesc) \
    if ( verbose & VERBOSE_TEST ) { std::printf("testing "#testDesc" ... "); } \
    error_count = testResults_.GetFailureCount()

#define TEST_RES(failMsg) \
    if ( testResults_.GetFailureCount() > error_count ) { printf("-> "#failMsg"\n"); } \
    else if (verbose & VERBOSE_TEST) { std::printf("successful\n"); }

extern int verbose;
static int error_count = 0;

#endif

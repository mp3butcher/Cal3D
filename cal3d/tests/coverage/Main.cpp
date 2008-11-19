#include "UnitTest++.h"
#include "Unittest.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

int verbose = VERBOSE_NONE;

static void usage()
{
  printf("Cal3d Unittest\n");
  printf("Usage: unittest [options...]\n");
  printf("Options:\n");
  printf("  -h    print option list\n");
  printf("  -v    verbose tests\n");
}

int main(int argc, char* argv[])
{
  
  int i;
  char* option;
  verbose = 0;

  for(i=1;i<argc;i++) {
    option = argv[i];
    if(*option != '-') {
      break;
    } else {
      option++;
    }
    if (strcmp(option, "h") == 0) {
      usage();
      exit(0);
    } else if (strcmp(option, "v") == 0) {
      verbose |= VERBOSE_TEST;
    }
  }

  return UnitTest::RunAllTests();

}

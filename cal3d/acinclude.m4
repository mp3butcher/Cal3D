dnl ************************************************************************
dnl
dnl This file is part of the Cal3D library/package
dnl Copyright (C) 2002 Bruno 'Beosil' Heidelberger (beosil@swileys.com)
dnl
dnl This library is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU Lesser General Public License as published by
dnl the Free Software Foundation; either version 2.1 of the License, or (at
dnl your option) any later version.
dnl
dnl ************************************************************************

AC_DEFUN([CAL3D_CHECK_USER_CXX_FLAGS],
[
  AC_MSG_CHECKING(whether user defined C++ flags overwrite the default Cal3D ones)
  if test -z "$CXXFLAGS"; then
    cal3d_user_cxx_flags="no"
  else
    cal3d_user_cxx_flags="yes"
  fi
  AC_MSG_RESULT($cal3d_user_cxx_flags)
])

dnl ************************************************************************

AC_DEFUN([CAL3D_CHECK_BUILD],
[
  AC_MSG_CHECKING(whether to build Cal3D in debug mode)
  AC_ARG_ENABLE(debug,[  --enable-debug          build Cal3D in debug mode [default=no]],
  [
    if test $enableval = "no"; then
      cal3d_debug_mode="no"
    else
      cal3d_debug_mode="yes"
    fi
  ], [
    cal3d_debug_mode="no"
  ])
  AC_MSG_RESULT($cal3d_debug_mode)

  if test "$cal3d_user_cxx_flags" = "no"; then
    if test "$cal3d_debug_mode" = "yes"; then
      if test $ac_cv_prog_cxx_g = yes; then
        CXXFLAGS="-g"
      fi
    else
      if test "$GXX" = "yes"; then
        CXXFLAGS="-O2"
      fi
    fi
  fi

    CAL_INDICES_SIZE=""
    AC_ARG_ENABLE(16bit-indices,[  --enable-16bit-indices       use 16-bit indices (default 32-bit)],
    [
	if test $enableval = "no"; then
      	   CAL_INDICES_SIZE=""
        else
      	   CAL_INDICES_SIZE=-DCAL_16BIT_INDICES
	   CXXFLAGS="$CXXFLAGS -DCAL_16BIT_INDICES"
        fi
    ],)
    AC_SUBST(CAL_INDICES_SIZE)

    AC_ARG_ENABLE(profile,[  --enable-profile        enable profile building (disabled by default)],
    [
      CXXFLAGS="$CXXFLAGS -pg"
    ],)

    AC_ARG_ENABLE(coverage,[  --enable-coverage       enable gcov coverage capabilities (disabled by default)],
    [
      COVERAGE_ENABLED="no"   
      if test $enableval = "yes" ; then
        if test $enable_shared = "no" ; then
          COVERAGE_ENABLED="yes"
          CXXFLAGS="$CXXFLAGS -ftest-coverage -fprofile-arcs"
        fi
      fi

    ], [
      COVERAGE_ENABLED="no"
    ])
    AC_SUBST(COVERAGE_ENABLED)    

    AC_ARG_ENABLE(rtti, [  --enable-rtti           allow runtime type identification (disabled by default)],
    ,
    [
      CAL3D_CHECK_CXX_FLAG(fno-rtti,
      [
        CXXFLAGS="$CXXFLAGS -fno-rtti"
      ])
    ])

    if test "x$CXX" = "xCC"; then
      CXXFLAGS="$CXXFLAGS -LANG:std -n32 -mips3"
    fi

    AC_ARG_ENABLE(unittest, [  --disable-unittest do not build coverage unittest], [
        unittest=false                       # for AM_CONDITION
    ], [

       PKG_CHECK_MODULES(UNITTESTCPP, unittest++ >= 0.1, [unittest_enabled="yes"], [unittest_enabled="no"] )
       if test "$unittest_enabled" = "yes" ; then
         CPPFLAGS="$CPPFLAGS $UNITTESTCPP_CFLAGS"
         LIBS="$LIBS $UNITTESTCPP_LIBS"
         AC_DEFINE(USE_UNITTESTCPP, 1, [activate unittest++])
       
         unittest=true                        # for AM_CONDITION
       fi
    ])

    AM_CONDITIONAL([COVERAGE], [test x$unittest = xtrue])     # For use in Makefile.am

])

dnl ************************************************************************

AC_DEFUN([CAL3D_CHECK_CXX_FLAG],
[
  AC_MSG_CHECKING(whether $CXX supports -$1)
  cal3d_cache=`echo $1 | sed 'y%.=/+-%___p_%'`
  AC_CACHE_VAL(cal3d_cv_prog_cxx_$cal3d_cache,
  [
    echo 'void f(){}' >conftest.cc
    if test -z "`$CXX -$1 -c conftest.cc 2>&1`"; then
      eval "cal3d_cv_prog_cxx_$cal3d_cache=yes"
    else
      eval "cal3d_cv_prog_cxx_$cal3d_cache=no"
    fi
    rm -f conftest*
  ])
  if eval "test \"`echo '$cal3d_cv_prog_cxx_'$cal3d_cache`\" = yes"; then
    AC_MSG_RESULT(yes)
    :
    $2
  else
    AC_MSG_RESULT(no)
    :
    $3
  fi
])

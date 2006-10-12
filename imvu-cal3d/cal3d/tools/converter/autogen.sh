#!/bin/sh
# Run this to generate all the initial makefiles, etc.

DIE=0


: ${AUTOCONF=autoconf}
: ${AUTOHEADER=autoheader}
: ${ACLOCAL=aclocal}
: ${AUTOMAKE=automake}

($AUTOHEADER --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`autoheader'. The version of \`autoconf'"
  echo "installed doesn't appear recent enough."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source (2.13 or newer) from:"
  echo "http://www.gnu.org/software/autoconf/autoconf.html"
  DIE=1
}

($ACLOCAL --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
  echo "installed doesn't appear recent enough."
  echo "See http://www.gnu.org/software/automake"
  echo "to get a recent version (1.4 or newer)"
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

aclocalinclude="$ACLOCAL_FLAGS"

echo "Running $ACLOCAL $aclocalinclude ..."
$ACLOCAL $aclocalinclude

echo "Running $AUTOHEADER..."
$AUTOHEADER

echo "Running $AUTOMAKE ..."
$AUTOMAKE

echo "Running $AUTOCONF ..."
$AUTOCONF

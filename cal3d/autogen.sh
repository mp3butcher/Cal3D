#!/bin/sh

PROJECT=cal3d
ACLOCAL_FLAGS=
LT_FLAGS='--force'
AC_FLAGS=
AM_FLAGS='--add-missing --force-missing --copy'
TEST='-d examples'


srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd $srcdir

DIE=0

have_libtool=false
if libtool --version < /dev/null > /dev/null 2>&1 ; then
	libtool_version=`libtoolize --version | sed 's/^[^0-9]*\([0-9].[0-9.]*\).*/\1/'`
	case $libtool_version in
		1.4*|1.5*)
			have_libtool=true
			;;
	esac
fi
if $have_libtool ; then : ; else
	echo "You need libtool >= 1.4 installed to compile $PROJECT."
	DIE=1
fi

autoconf=''
for at in autoconf autoconf-2.53 autoconf-2.54 autoconf-2.55 autoconf-2.56 autoconf-2.57 autoconf-2.58 autoconf-2.59; do
	if $at --version < /dev/null > /dev/null 2>&1 ; then
		at_version=`$at --version | grep -i 'autoconf' | sed 's/^[^0-9]*\(.*\)/\1/'`
		case $at_version in
			2.5?) 
				autoconf="$at"
				;;
		esac
	fi
done
if test -z $autoconf ; then
	echo "You need autoconf >= 2.53 installed to compile $PROJECT."
	DIE=1
fi

automake=''
for am in automake automake-1.7 automake-1.8 automake-1.9; do
	if $am --version < /dev/null > /dev/null 2>&1 ; then
		am_version=`$am --version | grep 'automake (GNU automake)' | sed 's/^[^0-9]*\(.*\)/\1/'`
		case $am_version in
			1.[789]*) 
				automake="$am"
				;;
		esac
	fi
done
if test -z $automake ; then
	echo "You need automake >= 1.7 installed to compile $PROJECT."
	DIE=1
fi

aclocal=''
for acl in aclocal aclocal-1.7 aclocal-1.8 aclocal-1.9 ; do
	if $acl --version < /dev/null > /dev/null 2>&1 ; then
		acl_version=`$acl --version | grep 'aclocal (GNU automake)' | sed 's/^[^0-9]*\(.*\)/\1/'`
		case $acl_version in
			1.[789]*) 
				aclocal="$acl"
				;;
		esac
	fi
done
if test -z $aclocal; then
	echo "You need aclocal >= 1.7 installed to compile $PROJECT."
	DIE=1
fi

if test "$DIE" -eq 1; then
	echo
	echo "You can fetch and install latest GNU tools from your distribution"
	echo "packages or from http://ftp.gnu.org/gnu/. Note that aclocal is"
	echo "part of the automake software package."
	echo
	exit 1
fi

test $TEST || {
	echo "You must run this script in the top-level $PROJECT directory"
	exit 1
}


echo "Running '$aclocal $ACLOCAL_FLAGS'"
$aclocal $ACLOCAL_FLAGS || exit $?

# optionally run autoheader
autoheader --version < /dev/null > /dev/null 2>&1
if test $? -eq 0; then
	echo "Running 'autoheader'"
	autoheader || exit $?
fi

echo "Running 'libtoolize $LT_FLAGS'"
libtoolize $LT_FLAGS || exit $?

echo "Running '$automake $AM_FLAGS'"
$automake $AM_FLAGS || exit $?

echo "Running 'autoconf $AC_FLAGS'"
autoconf $AC_FLAGS || exit $?

cd $ORIGDIR

echo
echo "Now run ./configure and make."
echo

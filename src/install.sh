#!/bin/sh
# install.sh -- $Id: install.sh,v 1.1 2009/11/19 23:44:46 dave Exp $
#
#  CHANGES:
#  01/23/02 llc Comment out commands involving yorick, i, i0, and doc
#               subdirectories.  Not relevant here.
#               Also remove yorick/*.h from include file list.

# set Y_SITE, Y_HOME environment variables
if test -r Make.cfg; then
  eval `grep '^Y_SITE=' Make.cfg`
  eval `grep '^Y_HOME=' Make.cfg`
else
  echo install.sh: Make.cfg missing -- cannot install before make config
  exit 1
fi

home_only=no
un_install=no
case "$1" in
  +home) home_only=yes ;;
  -home) home_only=yes; un_install=yes ;;
  +both) ;;
  -both) un_install=yes ;;
  *) echo "install.sh: FATAL, damaged Makefile"; exit 1 ;;
esac

if test -n "$2"; then
  Y_SITE="$2$Y_SITE"
  Y_HOME="$2$Y_HOME"
fi

if test -n "$3"; then
  Y_BINDIR="$3"
else
  Y_BINDIR=$Y_HOME/bin
fi

if test $un_install = yes; then
echo "********************* uninstalling architecture-dependent files from"
echo Y_HOME=$Y_HOME
rm -f $Y_HOME/junk.tst
touch ./junk.tst
if test -f $Y_HOME/junk.tst; then
  for sub in include lib bin; do rm -rf $Y_HOME/$sub; done
  rm -f $Y_HOME/Maketmpl $Y_HOME/Make.*
else
  rm -rf $Y_HOME
fi
rm -rf $Y_BINDIR/yorick
rm -rf $Y_BINDIR/gist
rm -f ./junk.tst

if test $home_only = yes; then exit 0; fi
echo "********************* uninstalling architecture-independent files from"
echo Y_SITE=$Y_SITE
rm -f $Y_SITE/junk.tst
touch ./junk.tst
if test -f $Y_SITE/junk.tst; then
  rm -rf $Y_SITE/man
else
  rm -rf $Y_SITE
fi
rm -f ./junk.tst

else
echo "********************* installing architecture-dependent files to"
echo Y_HOME=$Y_HOME
if test ! -d $Y_HOME; then mkdir -p $Y_HOME; fi
if test ! -d $Y_HOME/include; then mkdir $Y_HOME/include; fi
if test ! -d $Y_HOME/lib; then mkdir $Y_HOME/lib; fi
if test -n "$3"; then
  if test ! -d $Y_BINDIR; then mkdir -p $Y_BINDIR; fi
else
  if test ! -d $Y_HOME/bin; then mkdir $Y_HOME/bin; fi
fi
#cp -f play/unix/config.h play/*.h gist/*.h yorick/*.h $Y_HOME/include
cp -f play/unix/config.h play/*.h gist/*.h $Y_HOME/include
touch ./junk.tst
if test -f $Y_HOME/junk.tst; then
  :
else
  cp -f Make.cfg $Y_HOME
fi
rm -f ./junk.tst
#cp -f yorick/Maketmpl $Y_HOME
#cp -f yorick/libyor.a yorick/main.o yorick/codger $Y_HOME/lib
#cp -f yorick/yorick $Y_BINDIR
cp -f gist/gist $Y_BINDIR
#eval `grep RANLIB= Make.cfg`; $RANLIB $Y_HOME/lib/libyor.a

if test $home_only = yes; then exit 0; fi
echo "********************* installing architecture-independent files to"
echo Y_SITE=$Y_SITE

if test ! -d $Y_SITE; then mkdir -p $Y_SITE; fi
if test ! -d $Y_SITE/i; then mkdir $Y_SITE/i; fi
if test ! -d $Y_SITE/i0; then mkdir $Y_SITE/i0; fi
if test ! -d $Y_SITE/g; then mkdir $Y_SITE/g; fi
if test ! -d $Y_SITE/doc; then mkdir $Y_SITE/doc; fi
rm -f $Y_SITE/junk.tst
touch ./junk.tst
if test -f $Y_SITE/junk.tst; then
  :
else
#  cp -f i/*.i i/README $Y_SITE/i
#  cp -f i0/*.i i0/README $Y_SITE/i0
  cp -f g/*.gs g/*.gp g/ps.ps g/README $Y_SITE/g
#  cp -f doc/*.tex doc/*.ps doc/*.pdf doc/FILE_FORMATS doc/README doc/*.doc $Y_SITE/doc
fi
rm -f ./junk.tst

fi

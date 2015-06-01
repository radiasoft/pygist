#!/usr/bin/env python

"""Setup script for the pygist module distribution."""

#  ---------------------------------------------------------------------
#
#  NAME:     setup.py
#
#  PURPOSE:  Faciliate installation of pygist module.
#
#  EXECUTE LINE:
#     python setup.py config           (do the configuration - required)
#     python setup.py build            (optional)
#     python setup.py build -g install (build a debug version and install)
#     python setup.py install          (does both build and install)
#     python setup.py sdist            (make a distribution version)
#
#  CHANGES:
#  10/28/01 llc Originated.
#  12/06/01 llc Replace hardwired path for Numeric include directory.
#  02/21/02 llc Add additional libraries to load on AIX (readline and cur).
#  09/02/02 llc Additional path for lx cluster.
#  09/16/02 llc Add path for IRIX and Solaris.
#  09/18/02 llc Consolidate all platform-specific differences here.
#               Add cygwin.
#  11/01/02 llc Remove readline and cur (curses) libraries.
#  11/04/02 llc For sdist, omit making libpyg.a.
#  11/11/02 mdh This script was completely rewritten by Michiel de Hoon
#               to ensure dependency of gistCmodule.so on libpyg.a.
#           llc No need to remove pscom.ps (similar to ps.ps).
#               List gfiles rather than use listdir.
#               No need to include X11 library directories.
#  11/13/02 llc Add option to use USE_RL_GETC, but do not use it.
#               /usr/local/include needed on aix5.
#  11/15/02 llc Use a third implementation approach by default.
#               PyOS_InputHook/u_waiter approach is enabled by defining
#               USE_U_WAITER.
#  12/27/02 mdh Rework to include config option, which works on Cygwin,
#               Windows, and Mac OS X, and will work on Unix/Linux when
#               python distutils is fixed.
#  04/07/03 mdh Add src/gist/style.c and set log verbosity.
#  04/08/03 llc Add extra compile option -nodtk for osf1V5; needed on
#               some of these platforms.
#  11/08/04 mdh Add support for Mac OSX.
#  11/29/12 dpg Extensive clean up. Now, all configure is done by the
#               src/configure script (which is mostly copied from Yorick).
#               libplay and gistexe are built via Makefiles
#               The resulting object files are directly used instead of
#               recompiling everything.
#
#  ---------------------------------------------------------------------

__revision__ = "$Id: setup.py,v 1.9 2011/09/28 22:50:28 grote Exp $"

import os
import os.path
import sys
import site
import glob

from distutils.core import setup, Extension
#from setuptools import setup, Extension
from distutils.command.config import config
from distutils.command.install import INSTALL_SCHEMES
from distutils import sysconfig

try:
    from distutils.command.build_py import build_py_2to3 as build_py
except ImportError:
    from distutils.command.build_py import build_py

pygist_name = "pygist"
pygist_version = "2.2.0"
pygist_maintainer = "Dave Grote"
pygist_maintainer_email = "dpgrote@lbl.gov"

cygwin = (sys.platform == 'cygwin')
windows = (sys.platform == 'win32')
darwin = (sys.platform == 'darwin')

macwithcocoa = False
# --- MAC now defaults to X11 (instead of cocoa)
#    macwithcocoa = (sys.platform=='darwin')

run_config = 'config' in sys.argv
run_build = 'build' in sys.argv
run_install = 'install' in sys.argv

if darwin:
    # --- Machines running csh/tcsh seem to have MACHTYPE defined and this is the safest way to set -arch.
    if 'MACHTYPE' in os.environ:
        MACHTYPE = os.environ['MACHTYPE']
        if os.environ['MACHTYPE'] == 'i386':
            os.environ['ARCHFLAGS'] = '-arch i386'
        elif os.environ['MACHTYPE'] == 'x86_64':
            os.environ['ARCHFLAGS'] = '-arch x86_64'
        elif os.environ['MACHTYPE'] == 'powerpc':
            os.environ['ARCHFLAGS'] = '-arch ppc'
    else:
        # --- If the shell is bash, MACHTYPE is undefined. So get what we can
        # --- from uname. We will assume that if we are running Snow Leopard
        # --- we are -arch x86-64 and if running Leopard on intel we are
        # --- -arch i386. This can be over-ridden by defining MACHTYPE.
        archtype = os.uname()[-1]
        if archtype in ['Power Macintosh','ppc']:
            MACHTYPE = 'ppc'
            os.environ['ARCHFLAGS'] = '-arch ppc'
        elif archtype in ['i386','x86_64']:
            MACHTYPE = 'i386'
            kernel_major = eval(os.uname()[2].split('.')[0])
            if kernel_major < 10 :
                os.environ['ARCHFLAGS'] = '-arch i386'  # Leopard or earlier
            else:
                os.environ['ARCHFLAGS'] = '-arch x86_64'  # Snow Leopard

for keyword in sys.argv:
    if keyword=='--x11':
        sys.argv.remove(keyword)
        cygwin = False
        macwithcocoa = False
    if keyword=='--cocoa':
        sys.argv.remove(keyword)
        if darwin:
          macwithcocoa = True
        else:
          raise "cocoa can only be specified on darwin"

if sys.platform=='linux2' and os.uname()[-1]=='x86_64':
    # RedHat puts the 64 bit libraries in the strange location of /usr/lib64.
    linux64 = os.access('/usr/lib64',os.F_OK)
else:
    linux64 = False

#------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------

class config_pygist (config):
    def run (self):
        # --- Get the C compiler and flags that was used to build python
        cc = sysconfig.get_config_var('CC')
        ccshared = sysconfig.get_config_var('CCSHARED')
        if darwin:
            # --- Remove any -arch arguments since it mucks things up on Darwin
            while 1:
                try:
                    ccsharedlist = ccshared.split()
                    index = ccsharedlist.index('-arch')
                    del ccsharedlist[index:index+2]
                    ccshared = ' '.join(ccsharedlist)
                except ValueError:
                    break
        flags = ''
        if cc: flags += 'CC="%s" '%cc
        if ccshared: flags += 'CFLAGS="%s" '%ccshared

        # --- Allow the standard configure script to do all of the work.
        os.system('cd src;%s ./configure'%flags)

#------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------

gfiles = ["src/g/README",
          "src/g/axes.gs",
          "src/g/boxed.gs",
          "src/g/boxed2.gs",
          "src/g/earth.gp",
          "src/g/gray.gp",
          "src/g/heat.gp",
          "src/g/l_nobox.gs",
          "src/g/ncar.gp",
          "src/g/nobox.gs",
          "src/g/ps.ps",
          "src/g/pscom.ps",
          "src/g/rainbow.gp",
          "src/g/stern.gp",
          "src/g/vg.gs",
          "src/g/vgbox.gs",
          "src/g/work.gs",
          "src/g/work2.gs",
          "src/g/yarg.gp"]

macsource = ["src/play/mac/pscr.m",
             "src/play/mac/pals.m",
             "src/play/mac/text.m",
             "src/play/mac/cell.m",
             "src/play/mac/bitblt.m",
             "src/play/mac/points.m",
             "src/play/mac/cursors.m",
             "src/play/mac/pwin.m",
             "src/play/mac/clips.m",
             "src/play/mac/pen.m",
             "src/play/mac/color.m",
             "src/play/mac/font.m"]

gistobjects = ['src/gist/gist.o',
               'src/gist/tick.o',
               'src/gist/tick60.o',
               'src/gist/engine.o',
               'src/gist/gtext.o',
               'src/gist/draw.o',
               'src/gist/draw0.o',
               'src/gist/clip.o',
               'src/gist/gread.o',
               'src/gist/gcntr.o',
               'src/gist/hlevel.o',
               'src/gist/ps.o',
               'src/gist/cgm.o',
               'src/gist/xfancy.o',
               'src/gist/xbasic.o']

unixobjects = ['src/play/unix/dir.o',
               'src/play/unix/files.o',
               'src/play/unix/fpuset.o',
               'src/play/unix/pathnm.o',
               'src/play/unix/slinks.o',
               'src/play/unix/timeu.o',
               'src/play/unix/timew.o',
               'src/play/unix/udl.o',
               'src/play/unix/uevent.o',
               'src/play/unix/ugetc.o',
               'src/play/unix/usernm.o',
               'src/play/unix/umain.o',
               'src/play/unix/uspawn.o']

x11objects = ['src/play/x11/colors.o',
              'src/play/x11/connect.o',
              'src/play/x11/cursors.o',
              'src/play/x11/errors.o',
              'src/play/x11/events.o',
              'src/play/x11/feep.o',
              'src/play/x11/fills.o',
              'src/play/x11/fonts.o',
              'src/play/x11/images.o',
              'src/play/x11/lines.o',
              'src/play/x11/pals.o',
              'src/play/x11/pwin.o',
              'src/play/x11/resource.o',
              'src/play/x11/rgbread.o',
              'src/play/x11/textout.o',
              'src/play/x11/rect.o',
              'src/play/x11/ellipse.o',
              'src/play/x11/clips.o',
              'src/play/x11/points.o']

winobjects = ['src/play/win/dir.o',
              'src/play/win/files.o',
              'src/play/win/handler.o',
              'src/play/win/sigansi.o',
              'src/play/win/pathnm.o',
              'src/play/win/conterm.o',
              'src/play/win/timeu.o',
              'src/play/win/timew.o',
              'src/play/win/usernm.o',
              'src/play/win/wpoll.o',
              'src/play/win/wdl.o',
              'src/play/win/wstdio.o',
              'src/play/win/cygapp.o',
              'src/play/win/cygmain.o',
              'src/play/win/clips.o',
              'src/play/win/cursors.o',
              'src/play/win/ellipse.o',
              'src/play/win/feep.o',
              'src/play/win/getdc.o',
              'src/play/win/pals.o',
              'src/play/win/pcell.o',
              'src/play/win/pfill.o',
              'src/play/win/plines.o',
              'src/play/win/points.o',
              'src/play/win/prect.o',
              'src/play/win/pscr.o',
              'src/play/win/ptext.o',
              'src/play/win/pwin.o',
              'src/play/win/wspawn.o']

anyobjects = ['src/play/any/hash.o',
              'src/play/any/hash0.o',
              'src/play/any/hashctx.o',
              'src/play/any/hashid.o',
              'src/play/any/mm.o',
              'src/play/any/mminit.o',
              'src/play/any/alarms.o',
              'src/play/any/pmemcpy.o',
              'src/play/any/pstrcpy.o',
              'src/play/any/pstrncat.o',
              'src/play/any/p595.o',
              'src/play/any/bitrev.o',
              'src/play/any/bitlrot.o',
              'src/play/any/bitmrot.o',
              'src/play/any/pstdio.o']

if windows:
    playobjects = winobjects + anyobjects + gistobjects
elif cygwin:
    playobjects = unixobjects + winobjects + anyobjects + gistobjects
elif macwithcocoa:
    unixobjects.remove('src/play/unix/fpuset.o')
    unixobjects.remove('src/play/unix/ugetc.o')
    unixobjects.remove('src/play/unix/umain.o')
    unixobjects.append('src/play/unix/stdinit.o')
    unixobjects.append('src/play/unix/uinbg.o')
    playobjects = unixobjects + anyobjects + gistobjects
else:
    playobjects = unixobjects + x11objects + anyobjects + gistobjects

gistpath = os.path.join(sys.prefix,"g")
gistpath = gistpath.replace('\\','\\\\\\\\')

# It is not clear which version is needed. Some fiddling is likely
# needed if these tests don't work.
if windows:
    extra_compile_args = ['-DGISTPATH="\\"' + gistpath + '\\""' ]
elif sys.hexversion < 33882608: # version 2.5.1
    extra_compile_args = ['''-DGISTPATH='"''' + gistpath + '''"' ''' ]
else:
    extra_compile_args = ['-DGISTPATH="' + gistpath + '"' ]

extra_link_args = []
if windows or cygwin:
    extra_compile_args.append("-DWINDOWS")
    extra_compile_args.append("-mwindows")
    extra_link_args.append("-mwindows")
if macwithcocoa:
    extra_link_args.append('-framework')
    extra_link_args.append('Cocoa')

include_dirs = []

if windows or cygwin or macwithcocoa:
    libraries = []
else:
    libraries = ['X11']

# setup the directories in which we will look for libraries
# X11 directories are not included because they are found during 'make config'.

if sys.platform == 'osf1V5':
    library_dirs=['.','src']
    extra_compile_args.append ( '-nodtk' )
#   include_dirs.append ( '/usr/local/include' )
#   extra_compile_args.append ( '-DUSE_RL_GETC' )
#   libraries.append ( 'readline' )

elif sys.platform in ['aix4', 'aix5']:
    library_dirs=['.','src', '/usr/local/lib']
#   include_dirs.append ( '/usr/local/include' )
#   extra_compile_args.append ( '-DUSE_RL_GETC' )
#   libraries.append ( 'readline' )
#   libraries.append ( 'cur' )

#  .. PC Linux (storm, fire, emperor) and alpha Linux (lx and furnace clusters)
elif sys.platform == 'linux2':
    if linux64:
        library_dirs=['.','src', '/usr/lib64']
    else:
        library_dirs=['.','src', '/usr/lib']

elif cygwin:
    library_dirs=['.','src', '/usr/lib']

elif sys.platform == "sunos5":
    library_dirs=['.','src', '/usr/local/lib']

elif sys.platform == 'irix646':
#   library_dirs=['.','src', '/usr/local/lib', '/usr/lib']
#   /usr/lib32 has the n32 libraries, whereas /usr/lib has o32 ones.
    library_dirs=['.','src', '/usr/lib32', '/home/chase/irix_6.5_64/lib']

else:
#    .. for another platform not in the list, try this first.
    library_dirs=['.','src']

if not run_config:
    #  read Make.cfg to add system-specific compile arguments, include
    #  directories, libraries, and library search paths
    inputfile = open(os.path.join("src","Make.cfg"))
    lines = inputfile.readlines()
    inputfile.close()
    for line in lines:
        if line[:8]=="MATHLIB=":
            mathlib = line[8:-1] #removing the \n
            # remove the -l
            mathlib = mathlib[2:]
            libraries.append(mathlib)
        if line[:5]=="XINC=":
            xinc = line[5:-1] # removing \n
            if xinc and not (windows or cygwin or macwithcocoa):
                # remove the -I
                xinc = xinc[2:]
                if xinc: include_dirs.append(xinc)
        if line[:5]=="XLIB=":
            xlib = line[5:-1] # removing \n
            if xlib and not (windows or cygwin or macwithcocoa):
                # remove the -L
                xlib = xlib[2:]
                library_dirs.append(xlib)

# Add the numpy library
import numpy
include_dirs.append(numpy.get_include())

# --- With this, the data_files listed in setup will be installed in
# --- the usual place in site-packages.
for scheme in INSTALL_SCHEMES.values():
    if darwin and sys.prefix == os.path.join('/System/Library/Frameworks/Python.framework/Versions',sys.version[:3]):
        # --- A special hack is needed for darwin. In dist_utils/command/install.py, install_purelib
        # --- is modified to the form below, but install_data is not. Without this hack, the data files
        # --- would be installed in the Python directory in /System/Library, which is not by default
        # --- user accessible.
        scheme['data'] = os.path.join('/Library/Python', sys.version[:3], 'site-packages')
    else:
        scheme['data'] = scheme['platlib']

# --- Build the gist and play objects by directly using the nice Makefile
# --- developed in Yorick.
if run_build or run_install:
    os.system('cd src;make libplay')
    os.system('cd src;make gistexe')

source = ['src/gistCmodule.c']
include_dirs.append('src/play')
include_dirs.append('src/gist')

if macwithcocoa:
    source += macsource
    include_dirs.append('src/play/mac')

# Now we know everything needed to define the extension module

extension = Extension ( 'gist.gistC',
                        source,
                        include_dirs=include_dirs,
                        library_dirs=library_dirs,
                        libraries=libraries,
                        extra_objects=playobjects,
                        extra_compile_args=extra_compile_args,
                        extra_link_args=extra_link_args)

# Now that we know how to build the extension, we can call setup

setup (
          name = pygist_name,
          version = pygist_version,
          description = "Python Wrapped Gist Graphics Package from Yorick",
          author = "Lee Busby, Zane Motteler, Dave Munro",
          maintainer = pygist_maintainer + "; Michiel de Hoon for the Windows version",
          maintainer_email = "dpgrote@lbl.gov",
          url = "http://www.llnl.gov",
          cmdclass = {'config': config_pygist,
                      'build_py':build_py},
          packages = ['gist'],
          package_dir = {'gist': 'gist'},
          #extra_path = 'gist',
          data_files = [('gist',gfiles),(os.path.dirname(sys.executable),['src/gist/gist'])],
          ext_modules = [extension],
   )

if run_install:
    # --- Fix permissions
    # --- Give the gist executable the same permissions as python
    os.chmod(os.path.join(os.path.dirname(sys.executable),'gist'),os.stat(sys.executable).st_mode)

    # --- Give the gistC.so shared object the same permissions as python
    # --- getsitepackages is only supported in version 2.7 and newer.
    if hasattr(site, 'getsitepackages'):
        for d in site.getsitepackages():
            dg = os.path.join(d,'gist')
            if os.access(dg,os.F_OK):
                # --- This is needed for python3, since extra stuff is
                # --- added to the suffix.
                # --- Should maybe do some error checking in case no gistC
                # --- is found.
                for gist in glob.iglob(os.path.join(dg,'gistC*.so')):
                    os.chmod(gist,os.stat(sys.executable).st_mode)

# Finished.

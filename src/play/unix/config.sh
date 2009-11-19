#! /bin/sh

debug=no

echo ""
echo "  ============= begin play/unix configuration ============="
echo ""

rm -f cfg* config.h

curdate=`date`
cursystem=`uname -a`
fatality=0
cat >config.h <<EOF
/* config.h used during config.sh script */
#ifndef CONFIG_SCRIPT
# error destroy this config.h and rerun configure script
#endif
EOF
cat >config.0h <<EOF
/* config.h from config.sh script $curdate
 * $cursystem
 */
EOF
# should LD_LIBRARY_PATH, LIBPATH (AIX), LPATH, SHLIB_PATH (HPUX) be saved?

commonargs="-DCONFIG_SCRIPT $CFLAGS -I. -I.. $LDFLAGS -o cfg config.c"

# find CPU time function (getrusage is best if present)
args="-DTEST_UTIME $commonargs"
if $CC -DUSE_GETRUSAGE $args >cfg.01a 2>&1; then
  echo "using getrusage() (CPU timer)"
  echo '#define USE_GETRUSAGE' >>config.0h
elif $CC -DUSE_TIMES $args >cfg.01b 2>&1; then
  echo "using times() (CPU timer)"
  echo '#define USE_TIMES' >>config.0h
  if test $debug = no; then rm -f cfg.01a; fi
elif $CC $args >cfg.01c 2>&1; then
  echo "fallback to clock(), getrusage() and times() missing (CPU timer)"
else
  echo "FATAL getrusage(), times(), and clock() all missing (timeu.c)"
  fatality=1
fi

# find wall time function (gettimeofday is best if present)
args="-DTEST_WTIME $commonargs"
if $CC -DUSE_GETTIMEOFDAY $args >cfg.02a 2>&1; then
  echo "using gettimeofday() (wall timer)"
  echo '#define USE_GETTIMEOFDAY' >>config.0h
elif $CC $args >cfg.02b 2>&1; then
  echo "fallback to time()+difftime(), gettimeofday() missing (wall timer)"
else
  echo "FATAL gettimeofday(), and time() or difftime() missing (timew.c)"
  fatality=1
fi

# find function to get user name
args="-DTEST_USERNM $commonargs"
if $CC $args >cfg.03a 2>&1; then
  echo "using POSIX getlogin(), getpwuid(), getuid() functions"
elif $CC -DUSE_PASSWD $args >cfg.03b 2>&1; then
  echo "fallback to cuserid(), POSIX getlogin() family missing"
  echo '#define NO_PASSWD' >>config.0h
else
  echo "FATAL cuserid(), POSIX getlogin() family both missing (usernm.c)"
  fatality=1
fi

# find function to get controlling terminal process group
args="-DTEST_TIOCGPGRP $commonargs"
cargs="-DTEST_TIOCGPGRP -DCONFIG_SCRIPT $CFLAGS -I. -I.. $LDFLAGS -c config.c"
if $CC $cargs >cfg.04a 2>&1; then
  :
elif $CC -DUSE_POSIX_GETPGRP $cargs >cfg.04a 2>&1; then
  echo "using strict POSIX getpgrp prototype"
  args="-DUSE_POSIX_GETPGRP -DTEST_TIOCGPGRP $commonargs"
  echo '#define USE_POSIX_GETPGRP' >>config.0h
fi
if $CC $args >cfg.04a 2>&1; then
  echo "using POSIX tcgetpgrp() function"
elif $CC '-DUSE_TIOCGPGRP_IOCTL=<sys/termios.h>' $args >cfg.04b 2>&1; then
  echo "fallback to TIOCGPGRP in sys/termios.h, POSIX tcgetpgrp() missing"
  echo '#define USE_TIOCGPGRP_IOCTL <sys/termios.h>' >>config.0h
elif $CC '-DUSE_TIOCGPGRP_IOCTL=<sgtty.h>' $args >cfg.04c 2>&1; then
  echo "fallback to TIOCGPGRP in sgtty.h, POSIX tcgetpgrp() missing"
  echo '#define USE_TIOCGPGRP_IOCTL <sgtty.h>' >>config.0h
  if test $debug = no; then rm -f cfg.04b; fi
else
  echo "FATAL unable to find TIOCGPGRP ioctl header (uinbg.c)"
  echo "  (you can patch config.0h by hand if you know header)"
  echo '#define USE_TIOCGPGRP_IOCTL <???>' >>config.0h
  fatality=1
fi

# find function to get current working directory
args="-DTEST_GETCWD $commonargs"
if $CC $args >cfg.05a 2>&1; then
  echo "using POSIX getcwd() function"
elif $CC -DUSE_GETWD $args >cfg.05b 2>&1; then
  echo "fallback to getwd(), POSIX getcwd() missing"
  echo '#define USE_GETWD' >>config.0h
else
  echo "FATAL getcwd(), getwd() both missing (dir.c)"
  fatality=1
fi

# find headers required to read directories
args="-DTEST_DIRENT $commonargs"
echo "$CC $args"
if $CC $args >cfg.06a 2>&1; then
  echo "using POSIX dirent.h header for directory ops"
elif $CC '-DDIRENT_HEADER=<sys/dir.h>' $args >cfg.06b 2>&1; then
  echo "using sys/dir.h header for directory ops"
  echo '#define DIRENT_HEADER <sys/dir.h>' >>config.0h
  if test $debug = no; then rm -f cfg.06a; fi
elif $CC '-DDIRENT_HEADER=<sys/ndir.h>' $args >cfg.06c 2>&1; then
  echo "using sys/ndir.h header for directory ops"
  echo '#define DIRENT_HEADER <sys/ndir.h>' >>config.0h
  if test $debug = no; then rm -f cfg.06a cfg.06b; fi
elif $CC '-DDIRENT_HEADER=<ndir.h>' $args >cfg.06d 2>&1; then
  echo "using ndir.h header for directory ops"
  echo '#define DIRENT_HEADER <ndir.h>' >>config.0h
  if test $debug = no; then rm -f cfg.06a cfg.06b cfg.06c; fi
else
  echo "FATAL dirent.h, sys/dir.h, sys/ndir.h, ndir.h all missing (dir.c)"
  fatality=1
fi

# find headers and functions required for poll/select functionality
args="-DTEST_POLL $commonargs"
maxdefs="-DUSE_SELECT -DNO_SYS_TIME_H -DNEED_SELECT_PROTO"
if $CC $args >cfg.07a 2>&1; then
  echo "using poll(), poll.h header"
elif $CC -DUSE_SYS_POLL_H $args >cfg.07b 2>&1; then
  echo "using poll(), sys/poll.h header"
  echo '#define USE_SYS_POLL_H' >>config.0h
  if test $debug = no; then rm -f cfg.07a; fi
elif $CC -DUSE_SELECT -DHAVE_SYS_SELECT_H $args >cfg.07c 2>&1; then
  echo "using select(), sys/select.h header"
  echo '#define USE_SELECT' >>config.0h
  echo '#define HAVE_SYS_SELECT_H' >>config.0h
  if test $debug = no; then rm -f cfg.07a cfg.07b; fi
elif $CC -DUSE_SELECT -DNEED_SELECT_PROTO $args >cfg.07d 2>&1; then
  echo "using select(), sys/time.h, sys/types.h headers"
  echo '#define USE_SELECT' >>config.0h
  echo '#define NEED_SELECT_PROTO' >>config.0h
  if test $debug = no; then rm -f cfg.07[a-c]; fi
elif $CC $maxdefs $args >cfg.07e 2>&1; then
  echo "using select(), time.h, sys/types.h headers"
  echo '#define USE_SELECT' >>config.0h
  echo '#define NO_SYS_TIME_H' >>config.0h
  echo '#define NEED_SELECT_PROTO' >>config.0h
  if test $debug = no; then rm -f cfg.07[a-d]; fi
elif $CC -DUSE_SELECT $args >cfg.07f 2>&1; then
  echo "using select(), sys/time.h header"
  echo '#define USE_SELECT' >>config.0h
  if test $debug = no; then rm -f cfg.07[a-e]; fi
elif $CC -DUSE_SELECT -DNO_SYS_TIME_H $args >cfg.07g 2>&1; then
  echo "using select(), time.h header"
  echo '#define USE_SELECT' >>config.0h
  echo '#define NO_SYS_TIME_H' >>config.0h
  if test $debug = no; then rm -f cfg.07[a-f]; fi
else
  echo "FATAL neither poll() nor select() usable? (uevent.c, upoll.c)"
  fatality=1
fi

#----------------------------------------------------------------------
# try to figure out how to get SIGFPE delivered
#----------------------------------------------------------------------
args="-DCONFIG_SCRIPT $CFLAGS -I. -I.. $LDFLAGS -o fputest fputest.c"
fpedef=
fpelib=
fpelibm=
if $CC -DFPU_DIGITAL $args >cfg.08 2>&1; then
  echo "using FPU_DIGITAL (SIGFPE delivery)"
  echo '#define FPU_DIGITAL' >>config.0h
  fpedef=-DFPU_DIGITAL
elif $CC -DFPU_AIX $args >cfg.08 2>&1; then
  echo "using FPU_AIX (SIGFPE delivery)"
  echo '#define FPU_AIX' >>config.0h
  fpedef=-DFPU_AIX
elif $CC -DFPU_HPUX $args $MATHLIB >cfg.08 2>&1; then
  echo "using FPU_HPUX (SIGFPE delivery)"
  echo '#define FPU_HPUX' >>config.0h
  fpedef=-DFPU_HPUX
  fpelibm=$MATHLIB
elif $CC -DFPU_SOLARIS $args >cfg.08 2>&1; then
  echo "using FPU_SOLARIS (SIGFPE delivery)"
  echo '#define FPU_SOLARIS' >>config.0h
  fpedef=-DFPU_SOLARIS
  # note this works under IRIX 6.3, while FPU_IRIX does not??
elif $CC -DFPU_SUN4 $args $MATHLIB >cfg.08 2>&1; then
  echo "using FPU_SUN4 (-lm) (SIGFPE delivery)"
  echo '#define FPU_SUN4' >>config.0h
  fpedef=-DFPU_SUN4
  fpelibm=$MATHLIB
elif $CC -DFPU_SUN4 $args -lsunmath >cfg.08 2>&1; then
  echo "using FPU_SUN4 (-lsunmath) (SIGFPE delivery)"
  echo '#define FPU_SUN4' >>config.0h
  fpedef=-DFPU_SUN4
  fpelib=-lsunmath
elif $CC -DFPU_IRIX $args -lfpe >cfg.08 2>&1; then
  # FPU_SOLARIS seems to work better??
  echo "using FPU_IRIX (SIGFPE delivery)"
  echo '#define FPU_IRIX' >>config.0h
  fpedef=-DFPU_IRIX
  fpelib=-lfpe
elif $CC -DFPU_IRIX $args >cfg.08 2>&1; then
  echo "using FPU_IRIX (SIGFPE delivery), but no libfpe??"
  echo '#define FPU_IRIX' >>config.0h
  fpedef=-DFPU_IRIX
elif $CC -DFPU_MACOSX $args >cfg.08 2>&1; then
  echo "using FPU_MACOSX (SIGFPE delivery)"
  echo '#define FPU_MACOSX' >>config.0h
  fpedef=-DFPU_MACOSX
elif $CC -DTEST_GCC $commonargs >cfg.08 2>&1; then
  if $CC -DFPU_ALPHA_LINUX $args >cfg.08 2>&1; then
    echo "using FPU_ALPHA_LINUX (SIGFPE delivery)"
    echo '#define FPU_ALPHA_LINUX' >>config.0h
    fpedef=-DFPU_ALPHA_LINUX
    echo "...libm may be broken -- read play/unix/README.fpu for more"
    echo "...fputest failure may not mean that yorick itself is broken"
    # CC="$CC -mfp-trap-mode=su -mtrap-precision=i"
  elif $CC -DFPU_GCC_I86 $args >cfg.08 2>&1; then
    echo "using FPU_GCC_I86 (SIGFPE delivery)"
    echo '#define FPU_GCC_I86' >>config.0h
    fpedef=-DFPU_GCC_I86
  elif $CC -DFPU_GCC_SPARC $args >cfg.08 2>&1; then
    echo "using FPU_GCC_SPARC (SIGFPE delivery)"
    echo '#define FPU_GCC_SPARC' >>config.0h
    fpedef=-DFPU_GCC_SPARC
  elif $CC -DFPU_GCC_M68K $args >cfg.08 2>&1; then
    echo "using FPU_GCC_M68K (SIGFPE delivery)"
    echo '#define FPU_GCC_M68K' >>config.0h
    fpedef=-DFPU_GCC_M68K
  elif $CC -DFPU_GCC_POWERPC $args >cfg.08 2>&1; then
    echo "using FPU_GCC_POWERPC (SIGFPE delivery)"
    echo '#define FPU_GCC_POWERPC' >>config.0h
    fpedef=-DFPU_GCC_POWERPC
  elif $CC -DFPU_GCC_ARM $args >cfg.08 2>&1; then
    echo "using FPU_GCC_ARM (SIGFPE delivery)"
    echo '#define FPU_GCC_ARM' >>config.0h
    fpedef=-DFPU_GCC_ARM
  fi
elif $CC -DFPU_GNU_FENV $args $MATHLIB >cfg.08 2>&1; then
  echo "using FPU_GNU_FENV (SIGFPE delivery)"
  echo '#define FPU_GNU_FENV' >>config.0h
  fpedef=-DFPU_GNU_FENV
  fpelibm=$MATHLIB
elif $CC -DFPU_UNICOS $args $MATHLIB >cfg.08 2>&1; then
  echo "using FPU_UNICOS (SIGFPE delivery)"
  echo '#define FPU_UNICOS' >>config.0h
  fpedef=-DFPU_UNICOS
  fpelibm=$MATHLIB
fi
if test -z "$fpedef"; then
  if $CC -DFPU_IGNORE $args $MATHLIB >cfg.08 2>&1; then
    echo "using FPU_IGNORE (SIGFPE delivery)"
    echo '#define FPU_IGNORE' >>config.0h
    fpedef=-DFPU_IGNORE
  else
    echo "FATAL unable to build SIGFPE fputest? (fputest.c, fpuset.c)"
    fatality=1
  fi
fi
if test -z "$fpelib"; then
  echo "FPELIB=" >>../../Make.cfg
else
  echo "FPELIB=$fpelib" >>../../Make.cfg
fi
if test -z "$fpelibm"; then
  echo "FPELIBM=" >>../../Make.cfg
else
  echo "FPELIBM=$fpelibm" >>../../Make.cfg
fi
if test -n "$fpedef"; then
  # on IRIX be sure that TRAP_FPE environment variable is turned off
  unset TRAP_FPE
fi
rm -f fputest
#----------------------------------------------------------------------

# clean up, issue warning if compiler gave fishy output
rm -f config.h config.o cfg cfg.c cfg.o
for f in cfg.[0-9]*; do
  if grep ... $f >/dev/null 2>&1; then   # or use test -s $f ?
    echo "WARNING - check compiler message in $f"
  else # remove empty files
    rm -f $f
  fi
done

if test $fatality = 1; then
  echo "*** at least one play/unix component could not be configured"
  echo "*** see configuration notes in play/unix/README.cfg"
else
  mv config.0h config.h
  echo "wrote config.h, ../../Make.cfg"
fi

echo ""
echo "  ============== end play/unix configuration =============="
exit $fatality

SHELL=/bin/sh
MAKE=make
include ../../Make.cfg

CFLAGS=$(COPTIONS) -I. -I..

OBJS=dir.o files.o fpuset.o handler.o pathnm.o stdinit.o timeu.o \
  timew.o uevent.o ugetc.o uinbg.o usernm.o slinks.o umain.o

all: make.done

make.done: $(OBJS)
	$(AR) rc ../libplay.a $(OBJS)
	@touch $@

config:
	CC="$(CC)" CFLAGS="$(COPTIONS)" LDFLAGS="$(LDOPTIONS)" MATHLIB="$(MATHLIB)" ./config.sh

clean::
	rm -f *~ '#'* *.o *.a *.done cfg* config.0* fputest core a.out

distclean:: clean
	rm -f config.h



dir.o: config.h ../play.h playu.h ../pstdlib.h ../pstdio.h
files.o: config.h ../pstdio.h ../pstdlib.h playu.h
fpuset.o: config.h playu.h
handler.o: config.h ../play.h playu.h ../pstdlib.h
pathnm.o: config.h ../play.h playu.h ../pstdlib.h
slinks.o: config.h playu.h ../pstdlib.h ../pstdio.h
stdinit.o: config.h ../play.h ../pstdlib.h ../pstdio.h playu.h
timeu.o: config.h ../play.h
timew.o: config.h ../play.h
uevent.o: config.h upoll.h upoll.c ../play.h playu.h ../pstdlib.h
ugetc.o: config.h playu.h ugetc.h ../pmin.h
uinbg.o: config.h playu.h
usernm.o: config.h ../play.h
umain.o: config.h ../play.h playu.h ../pstdlib.h

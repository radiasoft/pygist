SHELL=/bin/sh
MAKE=make
include ../../Make.cfg

CFLAGS=$(COPTIONS) -I. -I.. -I../unix -I../x11
X11LIB=$(XLIB) -lX11
CCLOAD=$(CC) $(LDOPTIONS) $(LDFLAGS)

OBJS=hash.o hash0.o hashctx.o hashid.o mm.o mminit.o \
  alarms.o pmemcpy.o pstrcpy.o pstrncat.o p595.o \
  bitrev.o bitlrot.o bitmrot.o

all: make.done numfmt

make.done: $(OBJS)
	$(AR) rc ../libplay.a $(OBJS)
	@touch $@

EXE_FILES=numfmt test2d hashtest
clean::
	rm -f *~ '#'* *.o *.a *.done pmain.c core a.out $(EXE_FILES)

distclean:: clean
	rm -f .gdbinit



numfmt: numfmt.c
	$(CC) -g -o $@ numfmt.c

test2d: pmain.o test2d.o ../libplay.a
	$(CCLOAD) -o $@ pmain.o test2d.o -L.. -lplay $(X11LIB)

test: test2d

hashtest: hashtest.o ../libplay.a
	$(CCLOAD) -o $@ hashtest.o -L.. -lplay

hash.o: ../unix/config.h ../phash.h ../pstdlib.h
hash0.o: ../unix/config.h ../phash.h
hashctx.o: ../unix/config.h ../phash.h
hashid.o: ../unix/config.h ../phash.h ../pstdlib.h

mm.o: ../unix/config.h ../pstdlib.h
mminit.o: ../unix/config.h ../pstdlib.h
pfrall.o: ../unix/config.h ../pstdlib.h

alarms.o: ../unix/config.h ../play.h ../pstdlib.h

pmemcpy.o: ../unix/config.h ../pstdlib.h
pstrcpy.o: ../unix/config.h ../pstdlib.h
pstrncat.o: ../unix/config.h ../pstdlib.h

bitrev.o: ../unix/config.h ../play.h
bitlrot.o: ../unix/config.h ../play.h
bitmrot.o: ../unix/config.h ../play.h
p595.o: ../play.h

test2d.o: ../play.h ../pstdlib.h ../pstdio.h
hashtest.o: ../phash.h ../pstdlib.h ../pstdio.h

pmain.o: pmain.c ../play.h
pmain.c: ../unix/pmain.c
	cp ../unix/pmain.c .

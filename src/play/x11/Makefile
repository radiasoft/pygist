SHELL=/bin/sh
MAKE=make
include ../../Make.cfg

CFLAGS=$(COPTIONS) -I. -I.. -I../unix $(XINC)

OBJS=colors.o connect.o cursors.o errors.o events.o feep.o fills.o \
  fonts.o images.o lines.o pals.o pwin.o resource.o rgbread.o textout.o \
  rect.o ellipse.o clips.o points.o

STD_DEPS=../unix/config.h ../play.h ../phash.h playx.h

all: make.done

make.done: $(OBJS)
	$(AR) rc ../libplay.a $(OBJS)
	@touch $@

config:
	CC="$(CC)" CFLAGS="$(COPTIONS)" LDFLAGS="$(LDOPTIONS)" ./xconfig.sh

clean::
	rm -f *~ '#'* *.o *.a *.done core a.out ../libx11.tgt

distclean:: clean



connect.o: $(STD_DEPS) ../pstdlib.h
resource.o: $(STD_DEPS)
pwin.o: $(STD_DEPS) ../pstdlib.h
errors.o: $(STD_DEPS) ../unix/playu.h ../pstdlib.h
events.o: $(STD_DEPS) ../unix/playu.h
clips.o: $(STD_DEPS)
feep.o: $(STD_DEPS)

colors.o: $(STD_DEPS)
pals.o: $(STD_DEPS) ../pstdlib.h
cursors.o: $(STD_DEPS)
fonts.o: $(STD_DEPS) ../pstdlib.h

points.o: $(STD_DEPS)
fills.o: $(STD_DEPS)
rect.o: $(STD_DEPS)
ellipse.o: $(STD_DEPS)
images.o: $(STD_DEPS) ../pstdlib.h
lines.o: $(STD_DEPS)
textout.o: $(STD_DEPS) ../pstdlib.h
rgbread.o: $(STD_DEPS)

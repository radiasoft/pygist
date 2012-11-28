SHELL=/bin/sh
MAKE=make
include ../Make.cfg

SUBDIRS=unix x11 any

all: libyor

# the library is actually built from subdirectories (in order)
# if any subdirectory modifies libplay.a, make.done will be older
libyor:
	@for d in $(SUBDIRS); do ( cd $$d; $(MAKE); ); done
	@$(MAKE) make.done

make.done: libplay.a
	$(RANLIB) libplay.a
	@touch $@

config:
	cd unix; $(MAKE) config
	cd x11; $(MAKE) config

numfmt: libyor
	cd any; $(MAKE) numfmt

test2d: libyor
	cd any; $(MAKE) test2d

clean:: cleantop
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done

distclean:: distcleantop
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) distclean; ); done

cleantop:
	rm -f *~ '#'* *.o *.a *.done core a.out

distcleantop: cleantop

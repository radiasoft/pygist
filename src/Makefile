SHELL=/bin/sh

ALLDIRS=play gist
CONFIGDIRS=play

all: libpyg gistexe

libpyg: Make.cfg
	@cd gist; $(MAKE) libpyg
	@$(MAKE) -f Makepyg

gistexe: Make.cfg
	@cd gist; $(MAKE) gist

Make.cfg:
	./configure $(PKGDIRS)

config: distclean
	@$(MAKE) Make.cfg

prefix=.
Y_PLATFORM=.
Y_SITE=.
Y_HOME=.
ysite:
	if S="s?^prefix=.*?prefix=$(prefix)?;\
	s?^Y_PLATFORM=.*?Y_PLATFORM=$(Y_PLATFORM)?;\
	s?^Y_SITE=.*?Y_SITE=$(Y_SITE)?;\
	s?^Y_HOME=.*?Y_HOME=$(Y_HOME)?";\
	sed -e "$$S" ysite.sh >ysite.sh1; then mv ysite.sh1 ysite.sh; fi

clean::
	@rm -f Make.del
	@if test ! -r Make.cfg; then touch Make.cfg Make.del; fi
	@for d in $(ALLDIRS); do ( cd $$d; $(MAKE) clean; ); done
	@if test -r Make.del; then rm -f Make.cfg Make.del; fi
	rm -f *~ '#'* *.o *.a cfg* ysite.sh? core a.out

distclean::
	@touch Make.cfg
	@for d in $(ALLDIRS); do ( cd $$d; $(MAKE) distclean; ); done
	rm -f *~ '#'* *.o cfg* Make.* ysite.sh? core a.out


check:
	@cd yorick; $(MAKE) check

INSTALL_ROOT=
Y_BINDIR=
install: gistexe 
	./install.sh +both "$(INSTALL_ROOT)" $(Y_BINDIR)

install1: gistexe
	./install.sh +home "$(INSTALL_ROOT)" $(Y_BINDIR)

uninstall:
	./install.sh -both "$(INSTALL_ROOT)" $(Y_BINDIR)

uninstall1:
	./install.sh -home "$(INSTALL_ROOT)" $(Y_BINDIR)

dist: distclean
	@$(MAKE) prefix=. Y_PLATFORM=. Y_SITE=. Y_HOME=. ysite
	W=`pwd`;N=`basename $$W`;R=`tail -1 VERSION`;cd ..;\
	tar cvf - $$N|gzip - >$$N.$$R.tgz;

# targets for ./configure
echocc:
	echo "$(CC)" >cfg.tmp
echorl:
	echo "$(RANLIB)" >cfg.tmp
echoml:
	echo "$(MATHLIB)" >cfg.tmp
pkgconfig:
	@for d in $(CONFIGDIRS); do ( cd $$d; $(MAKE) config; ); done

yorclean:
	@:

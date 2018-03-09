AUTOMAKE_OPTIONS = gnu
SUBDIRS = src scripts test vim gedit emacs doc
MAINTAINERCLEANFILES = config.h.in~
EXTRA_DIST = bootstrap getversion LICENCE README.1st README.windows  \
	     maketar makezip makedeb makesid README.2nd debian
maintainer-clean-local:
	rm -rf bin share autom4te.cache
if MAKEWIN
all-local:
	test -f src/findent && mv src/findent src/findent.exe
endif

builtparser builtlexer:
	cd src; $(MAKE) $@

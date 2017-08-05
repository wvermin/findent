AUTOMAKE_OPTIONS = gnu
SUBDIRS = src scripts test
MAINTAINERCLEANFILES = config.h.in~
maintainer-clean-local:
	rm -rf bin share autom4te.cache
if MAKEWIN
all-local:
	test -f src/findent && mv src/findent src/findent.exe
endif

# -*- mode: Makefile; -*-

## -----------------------------------------------------------------------------
## --SECTION--                                                    COMMON DEFINES
## -----------------------------------------------------------------------------

comma := ,
empty :=
space := $(empty) $(empty)
MAJOR_MINOR := $(subst $(space),.,$(wordlist 1,2,$(subst ., ,$(VERSION))))
VERSION_MAJOR := $(wordlist 1,1,$(subst ., ,$(VERSION)))
VERSION_MINOR := $(wordlist 2,2,$(subst ., ,$(VERSION)))
VERSION_PATCH := $(wordlist 3,3,$(subst ., ,$(VERSION)))

## -----------------------------------------------------------------------------
## --SECTION--                                                             FILES
## -----------------------------------------------------------------------------

################################################################################
### @brief source to build before compile
################################################################################

BUILT_SOURCES = build.h

################################################################################
### @brief man pages to install
################################################################################

dist_man_MANS =

################################################################################
### @brief files to cleanup
################################################################################

CLEANUP =

################################################################################
### @brief targets for cleanup of 3rd party libraries
################################################################################

CLEANUP_3RD =

################################################################################
### @brief flex files
################################################################################

FLEX_FILES = 

################################################################################
### @brief flex++ files
################################################################################

FLEXXX_FILES = 

################################################################################
### @brief bison files
################################################################################

BISON_FILES =

################################################################################
### @brief bison++ files
################################################################################

BISONXX_FILES = 

################################################################################
### @brief configuration files
################################################################################

BUILT_SOURCES += etc/arangodb/arangod-uid.conf

etc/arangodb/arangod-uid.conf: @srcdir@/etc/arangodb/arangod.conf
	sed -e 's:^# \([ug]id\):\1:' $< > $@

BUILT_SOURCES += etc/arangodb/arangod-arm.conf

etc/arangodb/arangod-arm.conf: @srcdir@/etc/arangodb/arangod.conf
	sed -e '/^\[server\]/,/^\[/s:^threads.*:threads = 1:' $< > $@

BUILT_SOURCES += etc/arangodb/arangod-uid-arm.conf

etc/arangodb/arangod-uid-arm.conf: @srcdir@/etc/arangodb/arangod.conf
	sed -e 's:^# \([ug]id\):\1:' $< \
      | sed -e '/^\[server\]/,/^\[/s:^threads.*:threads = 1:' > $@

## -----------------------------------------------------------------------------
## --SECTION--                                                             PATHS
## -----------------------------------------------------------------------------

if ENABLE_RELATIVE

TRI_PKGDATADIR = .
TRI_LOCALSTATEDIR = .
TRI_SBINDIR = ${abs_builddir}/bin
TRI_BINDIR = ${abs_builddir}/bin

else

TRI_LOCALSTATEDIR = ${localstatedir}
TRI_PKGDATADIR = ${pkgdatadir}
TRI_SBINDIR = ${sbindir}
TRI_BINDIR = ${bindir}

endif

## -----------------------------------------------------------------------------
## --SECTION--                                                             FLAGS
## -----------------------------------------------------------------------------

################################################################################
### @brief m4 directory
################################################################################

ACLOCAL_AMFLAGS = -I m4

################################################################################
### @brief preprocessor flags
################################################################################

AM_CPPFLAGS = \
	-I@top_srcdir@/lib \
	-I@top_builddir@/lib \
	@LIBEV_CPPFLAGS@ \
	@MATH_CPPFLAGS@ \
	@OPENSSL_CPPFLAGS@ \
	@READLINE_CPPFLAGS@ \
	@ICU_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@V8_CPPFLAGS@


if ENABLE_RELATIVE

AM_CPPFLAGS += \
	-D_SYSCONFDIR_='"etc/relative"'


else

AM_CPPFLAGS += \
	-D_SYSCONFDIR_='"${sysconfdir}/${PACKAGE_TARNAME}"'

endif

################################################################################
### @brief linker flags
################################################################################

AM_LDFLAGS = \
	@LIBEV_LDFLAGS@ \
	@MATH_LDFLAGS@ \
	@OPENSSL_LDFLAGS@ \
	@READLINE_LDFLAGS@ \
	@ICU_LDFLAGS@ \
	@ZLIB_LDFLAGS@ \
	@V8_LDFLAGS@

################################################################################
### @brief libraries
################################################################################

LIBS = \
	@LIBEV_LIBS@ \
	@MATH_LIBS@ \
	@OPENSSL_LIBS@ \
	@ICU_LIBS@ \
	@ZLIB_LIBS@ \
	@READLINE_LIBS@

################################################################################
### @brief ruby additions
################################################################################

if ENABLE_MRUBY

AM_CPPFLAGS += @MRUBY_CPPFLAGS@
AM_CPPFLAGS += @MRUBY_LDFLAGS@
LIBS += @MRUBY_LIBS@

endif

## -----------------------------------------------------------------------------
## --SECTION--                                              LIBRARIES & PROGRAMS
## -----------------------------------------------------------------------------

################################################################################
### @brief auxiliary libraries
################################################################################

noinst_LIBRARIES = \
	lib/libarango.a \
	lib/libarango_v8.a \
	lib/libarango_fe.a \
	lib/libarango_client.a

################################################################################
### @brief /bin programs
################################################################################

bin_PROGRAMS = \
	bin/arangob \
	bin/arangodump \
	bin/arangorestore \
	bin/arangosh \
	bin/arangoimp 

################################################################################
### @brief /sbin programs
################################################################################

sbin_PROGRAMS = \
	bin/arangod

################################################################################
### @brief /sbin scripts
################################################################################

bin_SCRIPTS =

################################################################################
### @brief uninstalled programs
################################################################################

noinst_PROGRAMS =

################################################################################
### @brief ruby additions
################################################################################

if ENABLE_MRUBY

noinst_LIBRARIES += lib/libarango_mruby.a
bin_PROGRAMS += bin/arangoirb

endif

################################################################################
### @brief /etc data
################################################################################

arangosysconfdir=$(sysconfdir)/$(PACKAGE_TARNAME)

arangosysconf_DATA = $(shell find @builddir@/etc/arangodb -name "*.conf" -print)

################################################################################
### @brief /share data
################################################################################

nobase_pkgdata_DATA = \
	$(shell find @srcdir@/js/actions -name "*.js" -print) \
	$(shell find @srcdir@/js/common -name "*.js" -print) \
	$(shell find @srcdir@/js/server -name "*.js" -print) \
	$(shell find @srcdir@/js/client -name "*.js" -print) \
	$(shell find @srcdir@/js/node -type f -print) \
	$(shell find @srcdir@/js/npm -type f "(" -name .travis.yml -o -name .npmignore -o -print ")") \
	$(shell find @srcdir@/js/apps/system -type f -print)

if ENABLE_MRUBY

nobase_pkgdata_DATA += \
	$(shell find @srcdir@/mr/actions -name "*.rb" -print) \
	$(shell find @srcdir@/mr/client -name "*.rb" -print) \
	$(shell find @srcdir@/mr/common -name "*.rb" -print) \
	$(shell find @srcdir@/mr/server -name "*.rb" -print)

endif

################################################################################
### @brief /var data
################################################################################

#if ENABLE_RELATIVE
#else

install-data-local:
	test -d $(DESTDIR)$(TRI_LOCALSTATEDIR)/lib/arangodb || mkdir -p $(DESTDIR)$(TRI_LOCALSTATEDIR)/lib/arangodb
	test -d $(DESTDIR)$(TRI_LOCALSTATEDIR)/lib/arangodb-apps || mkdir -p $(DESTDIR)$(TRI_LOCALSTATEDIR)/lib/arangodb-apps
	test -d $(DESTDIR)$(TRI_LOCALSTATEDIR)/log/arangodb || mkdir -p $(DESTDIR)$(TRI_LOCALSTATEDIR)/log/arangodb

#endif

## -----------------------------------------------------------------------------
## --SECTION--                                                           TARGETS
## -----------------------------------------------------------------------------

################################################################################
### @brief version number
################################################################################

build.h: configure.ac
	@echo '#define TRI_VERSION "@PACKAGE_VERSION@"' > build.h

################################################################################
### @brief source files
################################################################################

include lib/Makefile.files
include arangod/Makefile.files
include arangosh/Makefile.files
include etc/Makefile.files
include utils/Makefile.files

if ENABLE_MRUBY
include arangoirb/Makefile.files
endif

################################################################################
### @brief unit tests
################################################################################

include UnitTests/Makefile.unittests

################################################################################
### @brief error code file
################################################################################

if ENABLE_MAINTAINER_MODE

BUILT_SOURCES += \
	@top_srcdir@/lib/BasicsC/voc-errors.h \
	@top_srcdir@/lib/BasicsC/voc-errors.c \
	@top_srcdir@/js/common/bootstrap/errors.js

@top_srcdir@/lib/BasicsC/voc-errors.h: lib/BasicsC/errors.dat
	@top_srcdir@/config/build_errorfile.sh @top_srcdir@/config/generateErrorfile.py lib/BasicsC/errors.dat @top_srcdir@/lib/BasicsC/voc-errors.h

@top_srcdir@/lib/BasicsC/voc-errors.c: lib/BasicsC/errors.dat
	@top_srcdir@/config/build_errorfile.sh @top_srcdir@/config/generateErrorfile.py lib/BasicsC/errors.dat @top_srcdir@/lib/BasicsC/voc-errors.c

@top_srcdir@/js/common/bootstrap/errors.js: lib/BasicsC/errors.dat
	@top_srcdir@/config/build_errorfile.sh @top_srcdir@/config/generateErrorfile.py lib/BasicsC/errors.dat js/common/bootstrap/errors.js

endif

################################################################################
### @brief mimetypes file
################################################################################

if ENABLE_MAINTAINER_MODE

BUILT_SOURCES += \
	@top_srcdir@/lib/BasicsC/voc-mimetypes.h \
	@top_srcdir@/lib/BasicsC/voc-mimetypes.c \
	@top_srcdir@/js/common/modules/org/arangodb/mimetypes.js

@top_srcdir@/lib/BasicsC/voc-mimetypes.h: lib/BasicsC/mimetypes.dat
	@top_srcdir@/config/build_mimetypes.sh @top_srcdir@/config/generateMimetypes.py lib/BasicsC/mimetypes.dat @top_srcdir@/lib/BasicsC/voc-mimetypes.h

@top_srcdir@/lib/BasicsC/voc-mimetypes.c: lib/BasicsC/mimetypes.dat
	@top_srcdir@/config/build_mimetypes.sh @top_srcdir@/config/generateMimetypes.py lib/BasicsC/mimetypes.dat @top_srcdir@/lib/BasicsC/voc-mimetypes.c

@top_srcdir@/js/common/modules/org/arangodb/mimetypes.js: lib/BasicsC/mimetypes.dat
	@top_srcdir@/config/build_mimetypes.sh @top_srcdir@/config/generateMimetypes.py lib/BasicsC/mimetypes.dat js/common/modules/org/arangodb/mimetypes.js

endif

################################################################################
## cleanup
################################################################################

distclean-local:
	@for i in $(CLEANUP_3RD); do  $(MAKE) $$i;  done
	rm -f $(BUILT_SOURCES)

clean-local:
	rm -rf $(CLEANUP)

################################################################################
### @brief generated files
################################################################################

.PHONY: built-sources

built-sources: \
	build.h \
	@top_srcdir@/js/common/bootstrap/errors.js \
	@top_srcdir@/js/common/modules/org/arangodb/mimetypes.js

################################################################################
### @brief tags file
################################################################################

GTAGS_FILES = $(sort $(abspath $(SOURCES) $(HEADERS)))

GTAGS:
	echo $(GTAGS_FILES) | tr " " "\n" | gtags -i -f -

################################################################################
### @brief cleanup source and header files
################################################################################

.PHONY: update-disclaimer

update-disclaimer:
	find lib arangod arangosh arangoirb "(" -name "*.c" -o -name "*.h" -o -name "*.cpp" ")" -exec utils/cleanupCFiles "{}" ";"
	find lib arangod arangosh arangoirb "(" -name "*.c" -o -name "*.h" -o -name "*.cpp" ")" -exec chmod "644" "{}" ";"
	find js/common/bootstrap js/client/bootstrap js/server/bootstrap -name "*.js" -a ! -name errors.js -exec utils/cleanupCFiles "{}" ";"
	find js/common/modules/org/arangodb js/client/modules/org/arangodb js/server/modules/org/arangodb -name "*.js"
	find js -name "*.js"  -exec chmod "644" "{}" ";"

## -----------------------------------------------------------------------------
## --section--                                                      DEPENDENCIES
## -----------------------------------------------------------------------------

################################################################################
### @brief documentation
################################################################################

include Documentation/Makefile.files

################################################################################
### @brief javascript
################################################################################

include js/Makefile.files

################################################################################
### @brief mruby
################################################################################

if ENABLE_MRUBY
include mr/Makefile.mruby
endif

## -----------------------------------------------------------------------------
## --SECTION--                                                   GENERATED FILES
## -----------------------------------------------------------------------------

################################################################################
### @brief generate readme
################################################################################

if ENABLE_MARKDOWN
if ENABLE_HTML2TEXT

BUILT_SOURCES += README

README: README.md
	fgrep -v "[Build Status]" $< \
		| fgrep -v "ArangoDB-Logo" \
		| @MARKDOWN_EXEC@ \
		| @HTML2TEXT_EXEC@ -style pretty -nobs \
		| sed -e 's:&gt;:>:g' \
                | awk 'BEGIN { s = 0; } { if (length($0) == 0) {if (s != 0) print $0;} else {s = 1; print $0; }}' \
		> $@.tmp
	mv $@.tmp $@

endif
endif

################################################################################
### @brief generate all flex files
################################################################################

if ENABLE_MAINTAINER_MODE
include lib/Makefile.flex
endif

################################################################################
### @brief generate all bison files
################################################################################

if ENABLE_MAINTAINER_MODE
include lib/Makefile.bison
endif

## -----------------------------------------------------------------------------
## --SECTION--                                                EXTERNAL LIBRARIES
## -----------------------------------------------------------------------------

################################################################################
### @brief zlib
################################################################################

include 3rdParty/Makefile.zlib

################################################################################
### @brief libev
################################################################################

if ENABLE_ALL_IN_ONE_LIBEV
include 3rdParty/Makefile.all-in-one-libev
endif

################################################################################
### @brief V8
################################################################################

if ENABLE_ALL_IN_ONE_V8
include 3rdParty/Makefile.all-in-one-v8
endif

################################################################################
### @brief mruby
################################################################################

if ENABLE_MRUBY
include 3rdParty/Makefile.all-in-one-mruby
endif

################################################################################
### @brief ICU
################################################################################

if ENABLE_ALL_IN_ONE_ICU
include 3rdParty/Makefile.all-in-one-icu
endif

## -----------------------------------------------------------------------------
## --SECTION--                                                       END-OF-FILE
## -----------------------------------------------------------------------------

## Local Variables:
## mode: outline-minor
## outline-regexp: "^\\(### @brief\\|## --SECTION--\\|# -\\*- \\)"
## End:

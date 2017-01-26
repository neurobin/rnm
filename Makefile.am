ACLOCAL_AMFLAGS = -I m4
SUBDIRS = pcre2 gmpxx
EXTRA_DIST = autogen.sh \
 README.md \
 .travis.yml \
 .gitattributes \
 .gitignore \
 m4/ax_cxx_compile_stdcxx.m4

dist-hook:
	rm -rf jpcre2 pcre2 gmpxx

dist_man_MANS = rnm.1

rnm_CXXFLAGS =

AM_CPPFLAGS = -I$(srcdir)/pcre2 -Ipcre2/src \
  -I$(srcdir)/jpcre2/src \
  -I$(srcdir)/gmpxx -Igmpxx

bin_PROGRAMS = rnm
rnm_SOURCES = src/main.cpp src/func.hpp src/class.hpp src/regutils.hpp src/strutils.hpp src/inout.hpp src/futil.hpp src/strnatcmp.hpp src/proto.hpp src/global.hpp

rnm_MANS = rnm.1

rnm_LDADD = \
 -Lpcre2/.libs -lpcre2-8 \
 -Lgmpxx/.libs -lgmpxx -lgmp \
 -ldl \
 -lm

extra_opts = -DU_STATIC_IMPLEMENTATION

rnm_CXXFLAGS += \
  $(extra_opts) 

testdir = testdir

check: test

test: rnm
	mkdir -p $(testdir)
	for f in `seq 1 4000`; do touch $(testdir)/$$f; done
	./rnm -y -rs '//fn///fn/@ B16: /idr-b16/ L: /idr-l/ S: /idr-s/ mtime:/info-mtime//' -fo -dp -1 -s $(testdir)
	rm -rf $(testdir)

install: install-strip
install-strip: nonrecursive

nonrecursive:
	$(INSTALL_PROGRAM) -s -m 755 $(bin_PROGRAMS)  $(bindir)
	$(INSTALL_PROGRAM) -m 644 $(rnm_MANS) $(mandir)/man1


distclean-local:
	rm -rf pcre2 gmpxx jpcre2
	rm -rf .deps










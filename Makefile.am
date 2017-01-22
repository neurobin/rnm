ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src pcre2 gmpxx icu
EXTRA_DIST = autogen.sh \
 README.md \
 .travis.yml
dist_man_MANS = rnm.1

AM_CPPFLAGS = -Ipcre2 -Igmpxx -Iicu

bin_PROGRAMS = rnm
rnm_SOURCES = src/main.cpp src/func.hpp src/class.hpp src/regutils.hpp src/strutils.hpp src/inout.hpp src/futil.hpp src/strnatcmp.hpp src/proto.hpp src/global.hpp


#if ENABLE_ARCH_32
#rnm_CXXFLAGS = -m32 $(AM_CXXFLAGS)
#endif

#rnm_LDFLAGS = `icu/icu-config --cxx --cxxflags --cppflags --ldflags`
extra_opts = -DU_STATIC_IMPLEMENTATION

LDADD = \
 pcre2/.libs/libpcre2-8.a \
 gmpxx/.libs/libgmpxx.a \
 gmpxx/.libs/libgmp.a \
 icu/lib/libicuuc.a \
 icu/lib/libicuio.a \
 icu/lib/libicui18n.a \
 icu/lib/libicudata.a \
 -ldl \
 -lm

rnm_CXXFLAGS = \
  $(extra_opts) 

.PHONY: checkrnm
testdir = testdir
checkrnm: rnm
	mkdir -p $(testdir)
	for f in `seq 1 1000`; do touch $(testdir)/$$f; done
	rnm -y -ns '/fn/@ B16: /idr-b16/ L: /idr-l/ S: /idr-s/' -fo -dp -1 -s $(testdir)
	rm -rf $(testdir)


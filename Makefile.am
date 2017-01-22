ACLOCAL_AMFLAGS = -I m4
SUBDIRS = pcre2 gmpxx icu
EXTRA_DIST = autogen.sh \
 README.md \
 .travis.yml \
 .gitattributes \
 .gitignore \
 m4/ax_cxx_compile_stdcxx.m4

dist_man_MANS = rnm.1

AM_CPPFLAGS = -I$(srcdir)/pcre2 -Ipcre2/src \
  -I$(srcdir)/jpcre2/src \
  -I$(srcdir)/gmpxx -Igmpxx \
  -I$(srcdir)/icu/common \
  -I$(srcdir)/icu/i18n \
  -I$(srcdir)/icu/io

rnm_CXXFLAGS = \
  -Lpcre2/.libs \
  -Lgmpxx/.libs \
  -Licu/lib
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/
#  -I$(srcdir)/icu/

bin_PROGRAMS = rnm
rnm_SOURCES = src/main.cpp src/func.hpp src/class.hpp src/regutils.hpp src/strutils.hpp src/inout.hpp src/futil.hpp src/strnatcmp.hpp src/proto.hpp src/global.hpp


#if ENABLE_ARCH_32
#rnm_CXXFLAGS = -m32 $(AM_CXXFLAGS)
#endif

#rnm_LDFLAGS = `icu/icu-config --cxx --cxxflags --cppflags --ldflags`
extra_opts = -DU_STATIC_IMPLEMENTATION

LDADD = \
 -lpcre2-8 \
 -lgmpxx \
 -lgmp \
 -licuuc \
 -licuio \
 -licui18n \
 -licudata \
 -ldl \
 -lm

rnm_CXXFLAGS += \
  $(extra_opts) 

.PHONY: checkrnm
testdir = testdir
checkrnm: rnm
	mkdir -p $(testdir)
	for f in `seq 1 1000`; do touch $(testdir)/$$f; done
	./rnm -y -ns '/fn/@ B16: /idr-b16/ L: /idr-l/ S: /idr-s/' -fo -dp -1 -s $(testdir)
	rm -rf $(testdir)


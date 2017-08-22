ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh \
 README.md \
 .travis.yml \
 .gitattributes \
 .gitignore \
 m4/ax_cxx_compile_stdcxx.m4 \
 rnm.md \
 rnm.html \
 prepare.sh

dist_man_MANS = rnm.1

rnm_CXXFLAGS =

AM_CPPFLAGS = \
  -I$(srcdir)/jpcre2/src

bin_PROGRAMS = rnm
rnm_SOURCES = src/main.cpp src/func.hpp src/class.hpp src/regutils.hpp src/strutils.hpp src/inout.hpp src/futil.hpp src/strnatcmp.hpp src/proto.hpp src/global.hpp

rnm_MANS = rnm.1

rnm_LDADD = \
 -lpcre2-8 \
 -lgmpxx -lgmp

#extra_opts = -DU_STATIC_IMPLEMENTATION

#rnm_CXXFLAGS += \
#  $(extra_opts) 

testdir = testdir

check: test

test: rnm
	mkdir -p $(testdir)
	for f in `seq 1 4000`; do touch $(testdir)/$$f; done
	./rnm -y -rs '//fn///fn/@ B16: /idr-b16/ L: /idr-l/ S: /idr-s/ mtime:/info-mtime-%d-%m-%Y %H:%I %p//' -fo -dp -1 -s $(testdir)
	rm -rf $(testdir)

#install: install-strip
#install-strip: nonrecursive
#nonrecursive:
#	$(INSTALL_PROGRAM) -d $(DESTDIR)$(bindir) $(DESTDIR)$(mandir)/man1
#	$(INSTALL_PROGRAM) -s -m 755 $(bin_PROGRAMS) $(DESTDIR)$(bindir)
#	$(INSTALL_PROGRAM) -m 644 $(rnm_MANS) $(DESTDIR)$(mandir)/man1

#uninstall: nonrecursive-uninstall
#nonrecursive-uninstall: $(bin_PROGRAMS) $(rnm_MANS)
#	for prog in $(bin_PROGRAMS); do \
#	rm $(DESTDIR)$(bindir)/$$prog || true; \
#	done
#	for man in $(rnm_MANS); do \
#	rm $(DESTDIR)$(mandir)/man1/$$man || true; \
#	done

distclean-local:
	rm -rf .deps
#	rm -rf pcre2 gmpxx jpcre2









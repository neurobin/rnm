include([ax_cxx_compile_stdcxx.m4])
AC_INIT([rnm], [4.0.4], [http://github.com/neurobin/rnm/issues])
#AC_CONFIG_AUX_DIR([.])
#prefix="/usr"
AC_CONFIG_SRCDIR([src/main.cpp])


#AM_INIT_AUTOMAKE

AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CXX

#CXXFLAGS=" -O2 "

#check for c++11 support and add flags
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])

# Checks for libraries.

#AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([pcre2 jpcre2 gmpxx])
AC_CONFIG_LINKS([rnm.1:rnm.1])
#AC_CONFIG_SUBDIRS([icu])



# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h],[],
 [])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([mkdir realpath strerror])

##detect binary type
AC_ARG_ENABLE(arch-32,
              AS_HELP_STRING([--enable-arch-32],
                             [Build for 32 bit]),
              , enable_arch_32=unset)
AC_SUBST(enable_arch_32)

##check if we have to strip the binary
AC_ARG_ENABLE(strip,
              AS_HELP_STRING([--enable-strip],
                             [Make stripped binary]),
              , enable_strip=unset)
AC_SUBST(enable_strip)



## If we will build for 32 bit
if test "x$enable_arch_32" != "xunset";then
$enable_arch_32=yes
CXXFLAGS="-m32 $CXXFLAGS"
fi

##If we will stip the binary
if test "x$enable_strip" != "xunset";then
$enable_strip=yes
CXXFLAGS="-s $CXXFLAGS"
fi

#sAM_CONDITIONAL(ENABLE_ARCH_32, test "x$enable_arch_32" = "xyes")

AC_OUTPUT(Makefile)


cat <<EOF

rnm configuration summary:

    Install prefix .................... : ${prefix}
    C preprocessor .................... : ${CC}
    C++ compiler ...................... : ${CXX}
    C++ preprocessor flags ............ : ${CXXFLAGS}
    C++ compiler flags ................ : ${CXXFLAGS}

EOF

dnl end configure.ac

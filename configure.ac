AC_INIT([rnm], [3.1.2], [http://github.com/neurobin/rnm/issues])
AC_CONFIG_AUX_DIR(config)
prefix="/usr"
AC_CONFIG_SRCDIR([src/main.cpp])



AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
CXXFLAGS="$CXXFLAGS -std=c++0x"

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([mkdir realpath strerror])


AC_OUTPUT(Makefile src/Makefile)

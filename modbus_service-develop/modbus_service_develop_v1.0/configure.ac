# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([modbus_service], [0.0.1], [linhnm25@gmail.com])
AM_INIT_AUTOMAKE
#AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
#AC_DEFINE([DEBUG], [], [Description])

: ${CFLAGS=""}

# Checks for programs.
# AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# flags
CFLAGS+=" \
-Wall \
-Wextra \
-fno-common \
-Wstrict-aliasing=2 \
-Wshadow \
-Wmissing-prototypes \
-Wmissing-declarations \
-Wcast-align \
-Wuninitialized \
-Wno-strict-aliasing \
-Werror=pointer-arith \
-Werror=missing-declarations \
-Werror=redundant-decls \
-Werror=empty-body \
-Werror=format \
-Werror=format-security \
-Werror=format-nonliteral \
-Werror=init-self \
-Werror=vla \
-Wno-unused-parameter"

CFLAGS += " -DBOARD_TYPE=BOARD_DC_MAIN_CONTROLLER"

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

# Checks for libraries.
LT_INIT
AC_DISABLE_STATIC
AC_ENABLE_SHARED
AC_PROG_LIBTOOL(libtool)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

# Output Makefile files.
AC_CONFIG_FILES([Makefile
                src/Makefile
                test/Makefile
                ])


AC_OUTPUT

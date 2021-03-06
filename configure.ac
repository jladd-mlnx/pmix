# -*- shell-script -*-
#
# PMIx copyrights:
# Copyright (c) 2013      Intel, Inc. All rights reserved
#
#########################
# This code has been adapted from opal_get_version.m4sh in the Open MPI
# code base - per the Open MPI license, all copyrights are retained below.
#
# Copyright (c) 2004-2009 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2010 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2007 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2013 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2006-2008 Sun Microsystems, Inc.  All rights reserved.
# Copyright (c) 2006-2011 Los Alamos National Security, LLC.  All rights
#                         reserved.
# Copyright (c) 2009      Oak Ridge National Labs.  All rights reserved.
# Copyright (c) 2011-2013 NVIDIA Corporation.  All rights reserved.
# Copyright (c) 2012      Oracle and/or its affiliates.  All rights reserved.
# Copyright (c) 2013      Mellanox Technologies, Inc.
#                         All rights reserved.
#########################
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#


############################################################################
# Initialization, version number, and other random setup/init stuff
############################################################################
# Load the version code.  Because this is also used directly as a
# shell script, no ac_defun
m4_include([config/pmix_get_version.m4])
AC_LANG([C])

AC_INIT([pmix], 
        [m4_normalize(esyscmd([config/pmix_get_version.sh VERSION --version]))],
        [http://www.open-mpi.org/projects/pmix/], [pmix])
AC_PREREQ(2.69)
AC_CONFIG_AUX_DIR(config)
# Note that this directory must *exactly* match what was specified via
# -I in ACLOCAL_AMFLAGS in the top-level Makefile.am.
AC_CONFIG_MACRO_DIR(config)

# setup configure options
PMIX_CONFIGURE_SETUP
pmix_show_title "Configuring PMIx"

# Init automake
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE([foreign dist-bzip2 subdir-objects no-define 1.12.2 tar-ustar -Wall -Werror])
# SILENT_RULES is new in AM 1.11, but we require 1.11 or higher via
# autogen.  Limited testing shows that calling SILENT_RULES directly
# works in more cases than adding "silent-rules" to INIT_AUTOMAKE
# (even though they're supposed to be identical).  Shrug.
AM_SILENT_RULES([yes])

# Make configure depend on the VERSION file, since it's used in AC_INIT
AC_SUBST([CONFIGURE_DEPENDENCIES], ['$(top_srcdir)/VERSION'])

# Get the version of pmix that we are installing
PMIX_SAVE_VERSION([PMIX], [PMI-Exascale Client Library], 
                          [$srcdir/VERSION],
                          [src/include/version.h])
AM_PROG_AR

# Override/fixup the version numbers set by AC_INIT, since on
# developer builds, there's no good way to know what the version is
# before running configure :(.  We only use the base version number
# for the version set in AC_INIT.  This will always match reality
# because we add the VERSION file (the only way to change the
# major.minor.release{greek}) into the configure dependencies.

PACKAGE_VERSION="$PMIX_VERSION"
PACKAGE_STRING="${PACKAGE_NAME} ${PACKAGE_VERSION}"
VERSION="${PACKAGE_VERSION}"

# For standalone configurations, we also include a .so version number.

. $srcdir/VERSION
AC_SUBST([libpmix_so_version])

#
# Get the versions of the autotools that were used to bootstrap us
# (helpful for debugging reports)
#
AC_MSG_CHECKING([for bootstrap Autoconf version])
acversion=`grep "Generated by GNU Autoconf" $0 | head -n 1 | awk '{ print $6 }'`
AC_MSG_RESULT([$acversion])
AC_MSG_CHECKING([for bootstrap Automake version])
AC_MSG_RESULT([$am__api_version])
AC_MSG_CHECKING([for boostrap Libtool version])
ltversion=`grep VERSION= $srcdir/config/ltmain.sh | head -n 1 | cut -d= -f2`
AC_MSG_RESULT([$ltversion])

# List header files to generate
AC_CONFIG_HEADERS([src/include/pmix_config.h])

# Setup the top of the pmix_config.h file
AH_TOP([/* -*- c -*-
 *
 * PMIx copyrights:
 * Copyright (c) 2013-2014 Intel, Inc. All rights reserved
 *
 *#########################
 * This code has been adapted from opal_config.h in the Open MPI
 * code base - per the Open MPI license, all copyrights are retained below.
 *
 * Copyright (c) 2004-2005 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 *#########################
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 * This file is automatically generated by configure.  Edits will be lost
 * the next time you run configure!
 */

#ifndef PMIX_CONFIG_H
#define PMIX_CONFIG_H

#include "pmix_config_top.h"

])
AH_BOTTOM([
#include "pmix_config_bottom.h"
#endif /* PMIX_CONFIG_H */
])

# Other basic setup stuff (shared with components)

PMIX_BASIC_SETUP

PMIX_TOP_SRCDIR="`pwd`"
PMIX_TOP_BUILDDIR="`pwd`"
AC_SUBST(PMIX_TOP_SRCDIR)
AC_SUBST(PMIX_TOP_BUILDDIR)


############################################################################
# Configuration options
############################################################################

PMIX_CONFIGURE_OPTIONS
PMIX_CHECK_OS_FLAVORS


############################################################################
# Libtool: part one
# (before C compiler setup)
############################################################################

#
# Part one of libtool magic.  Enable static so that we have the --with
# tests done up here and can check for OS.  Save the values of
# $enable_static and $enable_shared before setting the defaults,
# because if the user specified --[en|dis]able-[static|shared] on the
# command line, they'll already be set.  In this way, we can tell if
# the user requested something or if the default was set here.
#

pmix_enable_shared="$enable_shared"
pmix_enable_static="$enable_static"
AM_ENABLE_SHARED
AM_DISABLE_STATIC

############################################################################
# Check for compilers and preprocessors
############################################################################
pmix_show_title "Compiler and preprocessor tests"

##################################
# C compiler characteristics
##################################

PMIX_SETUP_CC

# Does the compiler support "ident"-like constructs?

PMIX_CHECK_IDENT([CC], [CFLAGS], [c], [C])

#
# Check for some types
#

AC_CHECK_TYPES(int8_t)
AC_CHECK_TYPES(uint8_t)
AC_CHECK_TYPES(int16_t)
AC_CHECK_TYPES(uint16_t)
AC_CHECK_TYPES(int32_t)
AC_CHECK_TYPES(uint32_t)
AC_CHECK_TYPES(int64_t)
AC_CHECK_TYPES(uint64_t)
AC_CHECK_TYPES(int128_t)
AC_CHECK_TYPES(uint128_t)
AC_CHECK_TYPES(long long)

AC_CHECK_TYPES(__float128)
AC_CHECK_TYPES(long double)
# We only need these types if we're building the OMPI project, but
# PMIX currently doesn't protect for their lack of presence well.
AC_CHECK_HEADERS(complex.h)
AC_CHECK_TYPES(float _Complex)
AC_CHECK_TYPES(double _Complex)
AC_CHECK_TYPES(long double _Complex)

AC_CHECK_TYPES(intptr_t)
AC_CHECK_TYPES(uintptr_t)
AC_CHECK_TYPES(mode_t)
AC_CHECK_TYPES(ssize_t)
AC_CHECK_TYPES(ptrdiff_t)

#
# Check for type sizes
#

AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
if test "$ac_cv_type_long_long" = yes; then
    AC_CHECK_SIZEOF(long long)
fi
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
if test "$ac_cv_type_long_double" = yes; then
    AC_CHECK_SIZEOF(long double)
fi
if test "$ac_cv_type___float128" = yes; then
    AC_CHECK_SIZEOF(__float128)
fi
# We only need these types if we're building the OMPI project, but
# PMIX currently doesn't protect for their lack of presence well.
if test "$ac_cv_type_float__Complex" = yes; then
    AC_CHECK_SIZEOF(float _Complex)
fi
if test "$ac_cv_type_double__Complex" = yes; then
    AC_CHECK_SIZEOF(double _Complex)
fi
if test "$ac_cv_type_long_double__Complex" = yes; then
    AC_CHECK_SIZEOF(long double _Complex)
fi

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(size_t)
if test "$ac_cv_type_ssize_t" = yes ; then
    AC_CHECK_SIZEOF(ssize_t)
fi
if test "$ac_cv_type_ptrdiff_t" = yes; then
    AC_CHECK_SIZEOF(ptrdiff_t)
fi
AC_CHECK_SIZEOF(wchar_t)

AC_CHECK_SIZEOF(pid_t)


#
# Check for type alignments
#

PMIX_C_GET_ALIGNMENT(_Bool, PMIX_ALIGNMENT_BOOL)
PMIX_C_GET_ALIGNMENT(int8_t, PMIX_ALIGNMENT_INT8)
PMIX_C_GET_ALIGNMENT(int16_t, PMIX_ALIGNMENT_INT16)
PMIX_C_GET_ALIGNMENT(int32_t, PMIX_ALIGNMENT_INT32)
PMIX_C_GET_ALIGNMENT(int64_t, PMIX_ALIGNMENT_INT64)
if test "$ac_cv_type_int128_t" = yes ; then
  PMIX_C_GET_ALIGNMENT(int128_t, PMIX_ALIGNMENT_INT128)
fi
PMIX_C_GET_ALIGNMENT(char, PMIX_ALIGNMENT_CHAR)
PMIX_C_GET_ALIGNMENT(short, PMIX_ALIGNMENT_SHORT)
PMIX_C_GET_ALIGNMENT(wchar_t, PMIX_ALIGNMENT_WCHAR)
PMIX_C_GET_ALIGNMENT(int, PMIX_ALIGNMENT_INT)
PMIX_C_GET_ALIGNMENT(long, PMIX_ALIGNMENT_LONG)
if test "$ac_cv_type_long_long" = yes; then
    PMIX_C_GET_ALIGNMENT(long long, PMIX_ALIGNMENT_LONG_LONG)
fi
PMIX_C_GET_ALIGNMENT(float, PMIX_ALIGNMENT_FLOAT)
PMIX_C_GET_ALIGNMENT(double, PMIX_ALIGNMENT_DOUBLE)
if test "$ac_cv_type_long_double" = yes; then
    PMIX_C_GET_ALIGNMENT(long double, PMIX_ALIGNMENT_LONG_DOUBLE)
fi
if test "$ac_cv_type___float128" = yes; then
    PMIX_C_GET_ALIGNMENT(__float128, PMIX_ALIGNMENT___FLOAT128)
fi


# We only need these types if we're building the OMPI project, but
# PMIX currently doesn't protect for their lack of presence well.
if test "$ac_cv_type_float__Complex" = yes; then
    PMIX_C_GET_ALIGNMENT(float _Complex, PMIX_ALIGNMENT_FLOAT_COMPLEX)
fi
if test "$ac_cv_type_double__Complex" = yes; then
    PMIX_C_GET_ALIGNMENT(double _Complex, PMIX_ALIGNMENT_DOUBLE_COMPLEX)
fi
if test "$ac_cv_type_long_double__Complex" = yes; then
    PMIX_C_GET_ALIGNMENT(long double _Complex, PMIX_ALIGNMENT_LONG_DOUBLE_COMPLEX)
fi

PMIX_C_GET_ALIGNMENT(void *, PMIX_ALIGNMENT_VOID_P)
PMIX_C_GET_ALIGNMENT(size_t, PMIX_ALIGNMENT_SIZE_T)

#
# Does the C compiler native support "bool"? (i.e., without
# <stdbool.h> or any other help)
#

PMIX_VAR_SCOPE_PUSH([MSG])
AC_MSG_CHECKING(for C bool type)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
AC_INCLUDES_DEFAULT],
[[bool bar, foo = true; bar = foo;]])],
[PMIX_NEED_C_BOOL=0 MSG=yes],[PMIX_NEED_C_BOOL=1 MSG=no])
AC_DEFINE_UNQUOTED(PMIX_NEED_C_BOOL, $PMIX_NEED_C_BOOL,
    [Whether the C compiler supports "bool" without any other help (such as <stdbool.h>)])
AC_MSG_RESULT([$MSG])
AC_CHECK_SIZEOF(_Bool)
PMIX_VAR_SCOPE_POP

#
# Check for other compiler characteristics
#

PMIX_VAR_SCOPE_PUSH([CFLAGS_save])
if test "$GCC" = "yes"; then

    # gcc 2.96 will emit oodles of warnings if you use "inline" with
    # -pedantic (which we do in developer builds).  However,
    # "__inline__" is ok.  So we have to force gcc to select the
    # right one.  If you use -pedantic, the AC_C_INLINE test will fail
    # (because it names a function foo() -- without the (void)).  So
    # we turn off all the picky flags, turn on -ansi mode (which is
    # implied by -pedantic), and set warnings to be errors.  Hence,
    # this does the following (for 2.96):
    #
    # - causes the check for "inline" to emit a warning, which then
    # fails
    # - checks for __inline__, which then emits no error, and works
    #
    # This also works nicely for gcc 3.x because "inline" will work on
    # the first check, and all is fine.  :-)

    CFLAGS_save=$CFLAGS
    CFLAGS="$PMIX_CFLAGS_BEFORE_PICKY -Werror -ansi"
fi
AC_C_INLINE
# Microsoft compilers support 2 versions of restrict. One for functions, and
# one for variables. The problem is that they don't have an equivalent
# syntax, and the autoconf restrict detection is unable to detect them
# correctly. It detect the restrict keyword as __restrict which break the
# rules for function syntax which is declspec(restrict).
if test "x$pmix_cv_c_compiler_vendor" != "xmicrosoft"; then
    AC_C_RESTRICT
fi
PMIX_C_WEAK_SYMBOLS
PMIX_C_MACRO_WEAK_SYMBOLS
if test "$GCC" = "yes"; then
    CFLAGS=$CFLAGS_save
fi
PMIX_VAR_SCOPE_POP

if test "x$CC" = "xicc"; then
    PMIX_CHECK_ICC_VARARGS
fi

# If we want the profiling layer:
# - If the C compiler has weak symbols, use those.
# - If not, then set to compile the code again with #define's in a
#   separate directory.

if test "$WANT_WEAK_SYMBOLS" = "0"; then
    PMIX_C_HAVE_WEAK_SYMBOLS=0
fi
if test "$WANT_MPI_PROFILING" = "1"; then
    if test "$PMIX_C_HAVE_WEAK_SYMBOLS" = "1"; then
        OMPI_PROFILING_COMPILE_SEPARATELY=0
    else
        OMPI_PROFILING_COMPILE_SEPARATELY=1
    fi
else
    OMPI_PROFILING_COMPILE_SEPARATELY=0
fi

# Check if we support the offsetof compiler directive

PMIX_CHECK_OFFSETOF


##################################
# Only after setting up
# C do we check compiler attributes.
##################################

pmix_show_subtitle "Compiler characteristics"

PMIX_CHECK_ATTRIBUTES
PMIX_CHECK_COMPILER_VERSION_ID

##################################
# Lex Configuration
##################################

pmix_show_subtitle "Lex"

#
# We also need lex
#
AM_PROG_LEX


##################################
# Header files
##################################

pmix_show_title "Header file tests"

AC_CHECK_HEADERS([alloca.h aio.h arpa/inet.h dirent.h \
    dlfcn.h execinfo.h err.h fcntl.h grp.h inttypes.h libgen.h \
    libutil.h memory.h netdb.h netinet/in.h netinet/tcp.h \
    poll.h pwd.h sched.h stdint.h stddef.h \
    stdlib.h string.h strings.h stropts.h sys/fcntl.h sys/ipc.h sys/shm.h \
    sys/ioctl.h sys/mman.h sys/param.h sys/queue.h \
    sys/select.h sys/socket.h sys/sockio.h \
    stdarg.h sys/stat.h sys/statfs.h sys/statvfs.h sys/time.h sys/tree.h \
    sys/types.h sys/uio.h net/uio.h sys/utsname.h sys/vfs.h sys/wait.h syslog.h \
    time.h termios.h ulimit.h unistd.h util.h utmp.h malloc.h \
    ifaddrs.h crt_externs.h regex.h signal.h \
    ioLib.h sockLib.h hostLib.h shlwapi.h sys/synch.h limits.h])

AC_CHECK_HEADERS([sys/mount.h], [], [],
[AC_INCLUDES_DEFAULT
#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
])

AC_CHECK_HEADERS([sys/sysctl.h], [], [],
[AC_INCLUDES_DEFAULT
#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
])

# Needed to work around Darwin requiring sys/socket.h for
# net/if.h
AC_CHECK_HEADERS([net/if.h], [], [],
    [#include <stdio.h>
#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
])

# Note that sometimes we have <stdbool.h>, but it doesn't work (e.g.,
# have both Portland and GNU installed; using pgcc will find GNU's
# <stdbool.h>, which all it does -- by standard -- is define "bool" to
# "_Bool" [see
# http://www.opengroup.org/onlinepubs/009695399/basedefs/stdbool.h.html],
# and Portland has no idea what to do with _Bool).

# So first figure out if we have <stdbool.h> (i.e., check the value of
# the macro HAVE_STDBOOL_H from the result of AC_CHECK_HEADERS,
# above).  If we do have it, then check to see if it actually works.
# Define PMIX_USE_STDBOOL_H as approrpaite.
AC_CHECK_HEADERS([stdbool.h], [have_stdbool_h=1], [have_stdbool_h=0])
AC_MSG_CHECKING([if <stdbool.h> works])
if test "$have_stdbool_h" = "1"; then
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
AC_INCLUDES_DEFAULT[
#if HAVE_STDBOOL_H
#include <stdbool.h>
#endif]],
[[bool bar, foo = true; bar = foo;]])],
[PMIX_USE_STDBOOL_H=1 MSG=yes],[PMIX_USE_STDBOOL_H=0 MSG=no])
else
    PMIX_USE_STDBOOL_H=0
    MSG="no (don't have <stdbool.h>)"
fi
AC_DEFINE_UNQUOTED(PMIX_USE_STDBOOL_H, $PMIX_USE_STDBOOL_H,
    [Whether to use <stdbool.h> or not])
AC_MSG_RESULT([$MSG])

# checkpoint results
AC_CACHE_SAVE

##################################
# Types
##################################

pmix_show_title "Type tests"

AC_CHECK_TYPES([socklen_t, struct sockaddr_in, struct sockaddr_in6,
                struct sockaddr_storage],
               [], [], [AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif])

AC_CHECK_DECLS([AF_UNSPEC, PF_UNSPEC, AF_INET6, PF_INET6],
               [], [], [AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif])

# SA_RESTART in signal.h
PMIX_VAR_SCOPE_PUSH([MSG2])
AC_MSG_CHECKING([if SA_RESTART defined in signal.h])
AC_EGREP_CPP(yes, [
#include <signal.h>
#ifdef SA_RESTART
    yes
#endif ], [MSG2=yes VALUE=1], [MSG2=no VALUE=0])
AC_DEFINE_UNQUOTED(PMIX_HAVE_SA_RESTART, $VALUE,
    [Whether we have SA_RESTART in <signal.h> or not])
AC_MSG_RESULT([$MSG2])
PMIX_VAR_SCOPE_POP

AC_CHECK_MEMBERS([struct sockaddr.sa_len], [], [], [
#include <sys/types.h>
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif])

AC_CHECK_MEMBERS([struct dirent.d_type], [], [], [
#include <sys/types.h>
#include <dirent.h>])

AC_CHECK_MEMBERS([siginfo_t.si_fd],,,[#include <signal.h>])
AC_CHECK_MEMBERS([siginfo_t.si_band],,,[#include <signal.h>])

#
# Checks for struct member names in struct statfs
#
AC_CHECK_MEMBERS([struct statfs.f_type], [], [], [
AC_INCLUDES_DEFAULT
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif
#ifdef HAVE_SYS_STATFS_H
#include <sys/statfs.h>
#endif
])

AC_CHECK_MEMBERS([struct statfs.f_fstypename], [], [], [
AC_INCLUDES_DEFAULT
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif
#ifdef HAVE_SYS_STATFS_H
#include <sys/statfs.h>
#endif
])

#
# Checks for struct member names in struct statvfs
#
AC_CHECK_MEMBERS([struct statvfs.f_basetype], [], [], [
AC_INCLUDES_DEFAULT
#ifdef HAVE_SYS_STATVFS_H
#include <sys/statvfs.h>
#endif
])

AC_CHECK_MEMBERS([struct statvfs.f_fstypename], [], [], [
AC_INCLUDES_DEFAULT
#ifdef HAVE_SYS_STATVFS_H
#include <sys/statvfs.h>
#endif
])

#
# Check for ptrdiff type.  Yes, there are platforms where
# sizeof(void*) != sizeof(long) (64 bit Windows, apparently).
#
AC_MSG_CHECKING([for pointer diff type])
if test $ac_cv_type_ptrdiff_t = yes ; then
    pmix_ptrdiff_t="ptrdiff_t"
    pmix_ptrdiff_size=$ac_cv_sizeof_ptrdiff_t
elif test $ac_cv_sizeof_void_p -eq $ac_cv_sizeof_long ; then
    pmix_ptrdiff_t="long"
    pmix_ptrdiff_size=$ac_cv_sizeof_long
elif test $ac_cv_type_long_long = yes -a $ac_cv_sizeof_void_p -eq $ac_cv_sizeof_long_long ; then
    pmix_ptrdiff_t="long long"
    pmix_ptrdiff_size=$ac_cv_sizeof_long_long
#else
#    AC_MSG_ERROR([Could not find datatype to emulate ptrdiff_t.  Cannot continue])
fi
AC_DEFINE_UNQUOTED([PMIX_PTRDIFF_TYPE], [$pmix_ptrdiff_t],
    [type to use for ptrdiff_t])
AC_MSG_RESULT([$pmix_ptrdiff_t (size: $pmix_ptrdiff_size)])

##################################
# Libraries
##################################

pmix_show_title "Library and Function tests"

PMIX_SEARCH_LIBS_CORE([socket], [socket])

# IRIX and CentOS have dirname in -lgen, usually in libc
PMIX_SEARCH_LIBS_CORE([dirname], [gen])

# Darwin doesn't need -lm, as it's a symlink to libSystem.dylib
PMIX_SEARCH_LIBS_CORE([ceil], [m])

AC_CHECK_FUNCS([asprintf snprintf vasprintf vsnprintf strsignal socketpair strncpy_s usleep])

# On some hosts, htonl is a define, so the AC_CHECK_FUNC will get
# confused.  On others, it's in the standard library, but stubbed with
# the magic glibc foo as not implemented.  and on other systems, it's
# just not there.  This covers all cases.
AC_CACHE_CHECK([for htonl define],
  [ompi_cv_htonl_define],
  [AC_PREPROC_IFELSE([AC_LANG_PROGRAM([
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif],[
#ifndef ntohl
#error "ntohl not defined"
#endif
])], [ompi_cv_htonl_define=yes], [ompi_cv_htonl_define=no])])
AC_CHECK_FUNC([htonl], [ompi_have_htonl=yes], [ompi_have_htonl=no])
AS_IF([test "$ompi_cv_htonl_define" = "yes" -o "$ompi_have_htonl" = "yes"],
    [AC_DEFINE_UNQUOTED([HAVE_UNIX_BYTESWAP], [1],
        [whether unix byteswap routines -- htonl, htons, nothl, ntohs -- are available])])

#
# Make sure we can copy va_lists (need check declared, not linkable)
#

AC_CHECK_DECL(va_copy, PMIX_HAVE_VA_COPY=1, PMIX_HAVE_VA_COPY=0,
    [#include <stdarg.h>])
AC_DEFINE_UNQUOTED(PMIX_HAVE_VA_COPY, $PMIX_HAVE_VA_COPY,
    [Whether we have va_copy or not])

AC_CHECK_DECL(__va_copy, PMIX_HAVE_UNDERSCORE_VA_COPY=1,
    PMIX_HAVE_UNDERSCORE_VA_COPY=0, [#include <stdarg.h>])
AC_DEFINE_UNQUOTED(PMIX_HAVE_UNDERSCORE_VA_COPY, $PMIX_HAVE_UNDERSCORE_VA_COPY,
    [Whether we have __va_copy or not])

AC_CHECK_DECLS(__func__)

# checkpoint results
AC_CACHE_SAVE

##################################
# System-specific tests
##################################

pmix_show_title "System-specific tests"

AC_C_BIGENDIAN

PMIX_CHECK_BROKEN_QSORT

AC_CACHE_CHECK([if word-sized integers must be word-size aligned],
    [ompi_cv_c_word_size_align],
    [AC_LANG_PUSH(C)
     AC_RUN_IFELSE([AC_LANG_PROGRAM([dnl
#include <stdlib.h>], [[    long data[2] = {0, 0};
    long *lp;
    int *ip;
    ip = (int*) data;
    ip++;
    lp = (long*) ip;
    return lp[0]; ]])],
        [ompi_cv_c_word_size_align=no],
        [ompi_cv_c_word_size_align=yes],
        [ompi_cv_c_word_size_align=yes])])
AS_IF([test $ompi_cv_c_word_size_align = yes], [results=1], [results=0])
AC_DEFINE_UNQUOTED([PMIX_ALIGN_WORD_SIZE_INTEGERS], [$results],
    [set to 1 if word-size integers must be aligned to word-size padding to prevent bus errors])

# all: SYSV semaphores
# all: SYSV shared memory
# all: size of FD_SET
# all: sizeof struct stat members
# all: type of getsockopt optlen
# all: type of recvfrom optlen

#
# What is the local equivalent of "ln -s"
#

AC_PROG_LN_S

AC_PROG_GREP
AC_PROG_EGREP

#
# We need as and lex
#
AM_PROG_AS
AM_PROG_LEX

# If we don't have GNU Flex and we don't have a generated .c file
# (distribution tarballs will have the .c file included, but SVN
# checkouts will not), then error.  Must have GNU Flex -- other
# versions of Lex are not workable (all things being equal, since this
# is *only* required for developers, we decided that it really was not
# worth it to be portable between different versions of lex ;-).

if test -z "$LEX" -o -n "`echo $LEX | $GREP missing`" -o \
    "`basename $LEX`" != "flex"; then
    if test ! -f "$srcdir/pmix/util/show_help_lex.c"; then
#        AC_MSG_WARN([*** Could not find GNU Flex on your system.])
#        AC_MSG_WARN([*** GNU Flex required for developer builds of Open MPI.])
#        AC_MSG_WARN([*** Other versions of Lex are not supported.])
#        AC_MSG_WARN([*** YOU DO NOT NEED FLEX FOR DISTRIBUTION TARBALLS!])
#        AC_MSG_WARN([*** If you absolutely cannot install GNU Flex on this system])
#        AC_MSG_WARN([*** consider using a distribution tarball, or generate the])
#        AC_MSG_WARN([*** following files on another system (using Flex) and])
#        AC_MSG_WARN([*** copy them here:])
        for lfile in `find . -name \*.l -print`; do
            cfile="`echo $lfile | cut -d. -f-2`"
#            AC_MSG_WARN([***     $cfile.c])
        done
#        AC_MSG_ERROR([Cannot continue])
    fi
fi

#
# File system case sensitivity
#

PMIX_CASE_SENSITIVE_FS_SETUP

##################################
# Visibility
##################################

# Check the visibility declspec at the end to avoid problem with
# the previous tests that are not necessarily prepared for
# the visibility feature.
pmix_show_title "Symbol visibility feature"

PMIX_CHECK_VISIBILITY

##################################
# Libevent
##################################
pmix_show_title "Libevent"

PMIX_LIBEVENT_CONFIG

############################################################################
# Libtool: part two
# (after C compiler setup = no compiler/linker tests after this)
############################################################################

pmix_show_subtitle "Libtool configuration"

LTDL_CONVENIENCE
LT_INIT([dlopen win32-dll])

# What's the suffix of shared libraries?  Inspired by generated
# Libtool code (even though we don't support several of these
# platforms, there didn't seem to be any harm in leaving in some of
# them, alhtough I did remove some that we have never/will never
# support, like OS/2).
PMIX_DYN_LIB_PREFIX=lib
case $host_os in
cygwin*)
    PMIX_DYN_LIB_PREFIX=cyg
    PMIX_DYN_LIB_SUFFIX=dll
    ;;
mingw* | pw32* | cegcc*)
    PMIX_DYN_LIB_SUFFIX=dll
    ;;
darwin* | rhapsody*)
    PMIX_DYN_LIB_SUFFIX=dylib
    ;;
hpux9* | hpux10* | hpux11*)
    case $host_cpu in
        ia64*)
        PMIX_DYN_LIB_SUFFIX=so
        ;;
    *)
        PMIX_DYN_LIB_SUFFIX=sl
        ;;
    esac
    ;;
*)
   PMIX_DYN_LIB_SUFFIX=so
   ;;
esac
AC_SUBST(PMIX_DYN_LIB_PREFIX)
AC_SUBST(PMIX_DYN_LIB_SUFFIX)

#PMIX_SETUP_LIBLTDL

# Need the libtool binary before the rpathify stuff
LT_OUTPUT

############################################################################
# final compiler config
############################################################################

m4_ifdef([project_ompi], [pmix_show_subtitle "Compiler flags"],
         [m4_ifdef([project_orte], [pmix_show_subtitle "Compiler flags"])])

#
# This is needed for VPATH builds, so that it will -I the appropriate
# include directory.  We delayed doing it until now just so that
# '-I$(top_srcdir)' doesn't show up in any of the configure output --
# purely aesthetic.
#
# Because pmix_config.h is created by AC_CONFIG_HEADERS, we
# don't need to -I the builddir for pmix/include. However, we do
# need to add it for orte as it doesn't have an AC_CONFIG_HEADERS that
# will install it for us. If we VPATH building, we do need to include the
# source directories, however.
#
if test "$PMIX_TOP_BUILDDIR" != "$PMIX_TOP_SRCDIR"; then
    # Note the embedded m4 directives here -- we must embed them
    # rather than have successive assignments to these shell
    # variables, lest the $(foo) names try to get evaluated here.
    # Yuck!
    CPPFLAGS='-I$(top_srcdir) -I$(top_builddir) -I$(top_srcdir)/pmix/include'" $CPPFLAGS"
else
    CPPFLAGS='-I$(top_srcdir)'" $CPPFLAGS"
fi

#
# Delayed the substitution of CFLAGS and CXXFLAGS until now because
# they may have been modified throughout the course of this script.
#

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)

# pmixdatadir, pmixlibdir, and pmixinclude are essentially the same as
# pkg*dir, but will always be */pmix.
pmixdatadir='${datadir}/pmix'
pmixlibdir='${libdir}/pmix'
pmixincludedir='${includedir}/pmix'
AC_SUBST(pmixdatadir)
AC_SUBST(pmixlibdir)
AC_SUBST(pmixincludedir)

############################################################################
# final output
############################################################################

pmix_show_subtitle "Final output"

AC_CONFIG_FILES([
    Makefile
])
AC_OUTPUT

# PMIx copyrights:
# Copyright (c) 2013-2014 Intel, Inc. All rights reserved
#
#########################
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2009 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2010 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2012-2013 Los Alamos National Security, Inc.  All rights reserved.
#########################
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

AM_CFLAGS = $(PMIX_CFLAGS)
AM_CPPFLAGS = $(PMIX_CPPFLAGS)
AM_LDFLAGS = $(PMIX_LDFLAGS)

headers =
sources =
nodist_headers =

EXTRA_DIST = README INSTALL VERSION LICENSE autogen.sh \
             config/pmix_get_version.sh util/help-pmix-util.txt

# setup the distribution tarball
DIST_SUBDIRS = config src

include config/Makefile.am
include src/class/Makefile.am
include src/include/Makefile.am
include src/buffer_ops/Makefile.am
include src/util/Makefile.am

lib_LTLIBRARIES = libpmix.la
libpmix_la_SOURCES = $(headers) $(sources)
libpmix_la_LDFLAGS = $(ldflags) -version-info $(libpmix_so_version) $(PMIX_LIBS)

dist-hook:
	csh "$(top_srcdir)/config/distscript.csh" "$(top_srcdir)" "$(distdir)" "$(OMPI_VERSION)" "$(OMPI_SVN_R)"

ACLOCAL_AMFLAGS = -I config

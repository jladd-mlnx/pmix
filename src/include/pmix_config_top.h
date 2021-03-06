/*
 * PMIx copyrights:
 * Copyright (c) 2013      Intel, Inc. All rights reserved
 *
 *#########################
 * This code has been adapted from pmix_config_top.h in the Open MPI
 * code base - per the Open MPI license, all copyrights are retained below.
 *
 * Copyright (c) 2011 Cisco Systems, Inc.  All rights reserved.
 *#########################
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 * This file is included at the top of pmix_config.h, and is
 * therefore a) before all the #define's that were output from
 * configure, and b) included in most/all files in Open MPI.
 *
 * Since this file is *only* ever included by pmix_config.h, and
 * pmix_config.h already has #ifndef/#endif protection, there is no
 * need to #ifndef/#endif protection here.
 */

#ifndef PMIX_CONFIG_H 
#error "pmix_config_top.h should only be included from pmix_config.h"
#endif

/* The only purpose of this file is to undef the PACKAGE_<foo> macros
   that are put in by autoconf/automake projects.  Specifically, if
   you include a .h file from another project that defines these
   macros (e.g., gmp.h) and then include PMIX's config.h,
   you'll get a preprocessor conflict.  So put these undef's here to
   protect us from other package's PACKAGE_<foo> macros.  

   Note that we can't put them directly in pmix_config.h (e.g., via
   AH_TOP) because they will be turned into #define's by autoconf. */

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef PACKAGE_URL
#undef HAVE_CONFIG_H

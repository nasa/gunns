#!/bin/tcsh
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.

setenv GUNNS_HOME `pwd`

# For maintainability, we prefer that this script not source any other scripts.
# Simply determine what needs to be set, prepended, or appended, and place it here.

# TRICK_HOME setup
# Requires user to have defined GUNNS_TRICK_HOME.  GUNNS_TRICK_VER is also recommended.
set TRICK_DEFAULT_PATH=${GUNNS_TRICK_HOME}
# From Trick 15 onwards, they override TRICK_VER to the actual trick version at compile time, so we
# no longer need to define TRICK_VER here.
#set TRICK_DEFAULT_VER=${GUNNS_TRICK_VER}
if ( ! $?TRICK_HOME ) then
  if ( -e ${TRICK_DEFAULT_PATH} ) then
    setenv TRICK_HOME ${TRICK_DEFAULT_PATH}
#    setenv TRICK_VER ${TRICK_DEFAULT_VER}
  else
    echo "TRICK_HOME has not been set and $TRICK_DEFAULT_PATH does not exist\!\!"
    exit
  endif
endif
setenv PATH "${PATH}:${TRICK_HOME}/bin"

# TRICK_CFLAGS
setenv TRICK_CFLAGS ""
if ( -e "$TRICK_HOME/bin/Trick.cshrc" ) then
    source "$TRICK_HOME/bin/Trick.cshrc"
endif
setenv TRICK_CFLAGS "$TRICK_CFLAGS -I$GUNNS_HOME ${GUNNS_EXT_PATH}"
setenv CPPFLAGS "-I$GUNNS_HOME ${GUNNS_EXT_PATH}"

# add this directory to simulation SFLAGS
if ( $?TRICK_SFLAGS ) then
  setenv TRICK_SFLAGS "$TRICK_SFLAGS -I$GUNNS_HOME"
else
  setenv TRICK_SFLAGS "-I$GUNNS_HOME"
endif

# Add ms-utils & gunns-ts-models (optional).  These are optional since users
# may include their contents elsewhere (i.e. in GUNNS_EXT_PATH).  We check for
# the submodule's bin/ folder to determine whether it should be included.
# If the submodule has not been git init'ed, its bin/ will be missing.
if ( -d "$GUNNS_HOME/ms-utils/bin" ) then
  setenv MS_UTILS_HOME "$GUNNS_HOME/ms-utils"
  setenv TRICK_CFLAGS "$TRICK_CFLAGS -I$MS_UTILS_HOME"
  setenv CPPFLAGS "$CPPFLAGS -I$MS_UTILS_HOME"
endif

if ( -d "$GUNNS_HOME/gunns-ts-models/bin" ) then
  setenv TS_MODELS_HOME "$GUNNS_HOME/gunns-ts-models"
  setenv TRICK_CFLAGS "$TRICK_CFLAGS -I$TS_MODELS_HOME"
  setenv CPPFLAGS "$CPPFLAGS -I$TS_MODELS_HOME"
endif

# sqlite and glib (for TS H&S)
#setenv TRICK_CFLAGS "${TRICK_CFLAGS} `pkg-config --cflags glib-2.0` -lsqlite3"
#setenv TRICK_USER_LINK_LIBS "${TRICK_USER_LINK_LIBS} `pkg-config --libs-only-l glib-2.0` -lsqlite3"

# generic build settings
setenv MAKEFLAGS -j`$GUNNS_HOME/bin/utils/calculate_make_jobs`

# default TS H&S config file location
setenv HS_CONFIG_PATH "${GUNNS_HOME}/sims/Modified_data"

# TRICK_CXXFLAGS
setenv TRICK_CXXFLAGS "$TRICK_CFLAGS"

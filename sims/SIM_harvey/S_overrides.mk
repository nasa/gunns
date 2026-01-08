# Copyright 2025 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
# uncomment one of these out to use the trickified lib:
-include ${GUNNS_HOME}/lib/trick_if/S_gunns.mk

# this is probably a good idea to add to the GUNNS environment script:
export TRICK_GTE_EXT = GUNNS_HOME

# You should modify this to point to the SIM_ folder's parent folder as necessary.
# This is the folder containing the SIM_ folder that this file resides in.
# The thread_safe_bus/simbus_simobj.sm requires that the parent folder
# be in the Trick include paths.
TRICK_CFLAGS   += -I${GUNNS_HOME}/sims
TRICK_CXXFLAGS += -I${GUNNS_HOME}/sims

# Build-time generation of GunnsDraw network code
include S_gunnsdraw_buildtime.mk

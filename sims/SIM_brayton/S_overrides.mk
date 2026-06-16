# Copyright 2025 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
ifndef GUNNS_HOME
    $(error GUNNS_HOME is not set.)
endif
ifndef GUNNS_HOME
    $(error SIMBUS_HOME is not set.)
endif

# uncomment to use the trickified lib:
-include ${GUNNS_HOME}/lib/trick_if/S_gunns.mk

#=============================================================================
# Build the Sim Bus
#=============================================================================
# Point to the folder containing your sim bus ICD .txt files
# and icd_generate file:
ICD_HOME = ${GUNNS_HOME}/sims/icd
include ${GUNNS_HOME}/sims/SIM_simbus_example/S_simbus.mk

# You should modify this to point to the SIM_ folder's parent folder as necessary.
# This is the folder containing the SIM_ folder that this file resides in.
# The thread_safe_bus/simbus_simobj.sm requires that the parent folder
# be in the Trick include paths.
TRICK_CFLAGS   += -I${GUNNS_HOME}/sims
TRICK_CXXFLAGS += -I${GUNNS_HOME}/sims

# Build-time generation of GunnsDraw network code
include S_gunnsdraw_buildtime.mk

# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
# uncomment this out to use the trickified lib:
-include ${GUNNS_HOME}/lib/trick_if/S_gunns.mk

TRICK_CFLAGS   += -I${PWD}
TRICK_CXXFLAGS += -I${PWD}

#export GUNNS_HOME = $(realpath ../..)
# this is probably a good idea to add to the GUNNS environment script:
export TRICK_GTE_EXT = GUNNS_HOME

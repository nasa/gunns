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

#============================================================================
# HLA Build Evironment
#============================================================================
# Info and error message text colors.
RED_TXT   =[31m
GREEN_TXT =[32m
RESET_TXT =[00m

# Include TrickHLA and the HLA RTI.
ifneq ("$(wildcard $(RTI_HOME))","")
	ifeq ("$(wildcard ${TRICKHLA_HOME})","")
	   $(error ${RED_TXT}This sim requires you to define TRICKHLA_HOME or have it installed at the default location!${RESET_TXT})
	endif
   $(info ${GREEN_TXT}Building the sim with HLA.${RESET_TXT})
   include ${TRICKHLA_HOME}/makefiles/S_hla.mk
   TRICK_GTE_EXT += RTI_HOME
   TRICK_SFLAGS  += -DHLA -I${TRICKHLA_HOME}/S_modules

# Links to the FOM and Modified_data folders in the sim folder.
all: build_links
clean: clean_links
spotless: clean_links

build_links:
	@ echo "${GREEN_TXT}Creating links to FOM and Modified_data folders.${RESET_TXT}"
	mkdir -p FOMs Modified_data
	ln -s -fn ${TRICKHLA_HOME}/FOMs/DistIf FOMs/DistIf
	ln -s -fn ${TRICKHLA_HOME}/Modified_data/DistIf Modified_data/DistIf     #TrickHLA v3.2 changes "Modified_data" to "TrickHLA_data"
	ln -s -fn ${TRICKHLA_HOME}/Modified_data/TrickHLA Modified_data/TrickHLA #TrickHLA v3.2 changes "Modified_data" to "TrickHLA_data"

clean_links:
	@if [ -e FOMs/DistIf ] ; then \
		echo "${GREEN_TXT}Cleaning link to FOMs/DistIf folder.${RESET_TXT}" ;\
		rm -r -f FOMs/DistIf ;\
	fi
	@if [ -e Modified_data/DistIf ] ; then \
		echo "${GREEN_TXT}Cleaning link to Modified_data/DistIf folder.${RESET_TXT}" ;\
		rm -r -f Modified_data/DistIf ;\
	fi
	@if [ -e Modified_data/TrickHLA ] ; then \
		echo "${GREEN_TXT}Cleaning link to Modified_data/TrickHLA folder.${RESET_TXT}" ;\
		rm -r -f Modified_data/TrickHLA ;\
	fi

else
   $(info ${GREEN_TXT}Building the sim without HLA.${RESET_TXT})
endif

# @copyright Copyright 2022 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
# List of all GunnsDraw network drawings that are to be exported at build time:
GUNNSDRAWINGS := \
 $(GUNNS_HOME)/sims/networks/fluid/test/TestFluidNetwork.xml \
 $(GUNNS_HOME)/sims/networks/basic/test/TestBasicNetwork.xml \
 $(GUNNS_HOME)/sims/networks/thermal/gunnshowThermalTest/GunnShowThermalTest.xml
 
####################################################################################################
# Don't touch the rest of this...
# Make a list of network class headers from the drawing files.  Note this requires that the exported
# code has the exact same name as the drawing!
GUNNSDRAW_HEADERS := $(subst .xml,.hh,$(GUNNSDRAWINGS))

# Trick processing of the S_define depends on these headers
S_define: $(GUNNSDRAW_HEADERS)

# Prefer to use system python3, else fall back to python
PYTHON=python3
ifeq (, $(shell which python3))
  PYTHON=python
endif

# Headers depend on the .xml, and are produced by invoking the export script
# TODO add the -g option once I have that working.  This will skip the drawing maintenance and error
# checks, should be a little faster...
$(GUNNSDRAW_HEADERS) : %.hh : %.xml
	@ echo $(shell $(PYTHON) $(GUNNS_HOME)/draw/netexport.py $<)

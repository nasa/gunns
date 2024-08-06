# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
# List of all GunnsDraw network drawings that are to be exported at build time:
GUNNSDRAWINGS := \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow2.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow3.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow4.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow5.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow6.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow7.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow8.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow9.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow10.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow11.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow12.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow13.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow14.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow15.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow16.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow17.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow18.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow19.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow20.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow21.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow22.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow23.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow24.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow25.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow26.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow27.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow28.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow29.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow30.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow31.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow32.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow33.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow34.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow35.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow36.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow37.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow38.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow39.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow40.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow41.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow42.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow43.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow44.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow45.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow46.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow47.xml \
 $(GUNNS_HOME)/sims/networks/fluid/test/overflow/TestOverflow48.xml

####################################################################################################
# Don't touch the rest of this...
# Make a list of network class headers from the drawing files.  Note this requires that the exported
# code has the exact same name as the drawing!
GUNNSDRAW_HEADERS := $(subst .xml,.hh,$(GUNNSDRAWINGS))

# Trick processing of the S_define depends on these headers
S_define: $(GUNNSDRAW_HEADERS)

# Headers depend on the .xml, and are produced by invoking the export script
# TODO add the -g option once I have that working.  This will skip the drawing maintenance and error
# checks, should be a little faster...
$(GUNNSDRAW_HEADERS) : %.hh : %.xml
	@ echo $(shell GUNNS_EXT_PATH=${GUNNS_EXT_PATH} python $(GUNNS_HOME)/draw/netexport.py $<)

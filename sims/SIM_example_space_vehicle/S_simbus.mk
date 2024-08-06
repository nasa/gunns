# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
#=============================================================================
# Sim Bus ICD setup section
#=============================================================================

# This assumes that the Mac developer has installed termcolor-py27 using fink
ifeq (${TRICK_HOST_TYPE},Darwin)
PYTHONPATH  += ":/sw/lib/python2.7/site_packages"
endif

PYTHONPATH  += ":${SIMBUS_HOME}:${SIMBUS_HOME}/lib:${SIMBUS_HOME}/src"
simname = $(shell basename ${PWD} )
ICD_TXT_FILES = $(wildcard ${ICD_HOME}/*.txt)

thread_safe_bus/alloc_simbus.cpp:
	$(shell PYTHONPATH="${PYTHONPATH}" ICD_HOME="${ICD_HOME}" ${ICD_HOME}/icd_generate $(ICD_TXT_FILES) >& icd_files.out)
	$(shell PYTHONPATH="${PYTHONPATH}" ICD_HOME="${ICD_HOME}" ${ICD_HOME}/icd_generate $(simname) >& alloc_simbus.out )

ifneq "$(MAKECMDGOALS)" "spotless"
ifneq "$(MAKECMDGOALS)" "apocalypse"
S_define: thread_safe_bus/alloc_simbus.cpp
endif
endif

clean_icd:
	rm -rf thread_safe_bus ;\
	rm -f icd_files.out alloc_simbus.out

clean: clean_icd
spotless: clean_icd
apocalypse: clean_icd

# This forceicd works best if you make clean_icd first
forceicd: thread_safe_bus/alloc_simbus.cpp

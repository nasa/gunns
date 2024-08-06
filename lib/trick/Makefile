################################################################################
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# This builds the GUNNS compiled library for Trick simulations.  If the
# compilation is successful, this also outputs the list of source file headers
# S_sources.hh to the "trickified" library trick_if.
#
# The default make command compiles GUNNS with the default -O2 -g optimization:
# This builds the libgunns.a library:
# $ make
#
# The roses target builds the libgunnsroses.a library, which is the same code
# base as libgunns.a, recompiled with dependencies on CUDA, plus those
# dependency CUDA classes.  This also outputs the list of source file headers
# S_sources.hh to the "trickified" library trick_if:
# $ make roses
#
# To compile without debugging capability, use:
# $ make OPTS='-O2'
#
# To compile a 32-bit library, use:
# $ make OPTS='-O2 -m32'
#
# GUNNS runs faster in run-time when compiled with -O2 (the default).  -g runs
# a little slower but is sometimes needed for debugging.
#
# To avoid a mix of objects with different compile options, we recommend first
# cleaning out prior compilations with:
# $ make clean
################################################################################
GUNNS_HOME ?= $(abspath ${PWD}/../..)

OPTS = -O2 -g

GUNNSLIB := libgunns.a
GUNNSROSESLIB := libgunnsroses.a
CXXFLAGS = -Wall $(OPTS)

INCDIRS = -I$(GUNNS_HOME) -I$(GUNNS_HOME)/core -I$(GUNNS_HOME)/aspects \
 -I$(GUNNS_HOME)/ms-utils -I$(GUNNS_HOME)/gunns-ts-models \
 -I$(GUNNS_TRICK_HOME)/include -I$(GUNNS_TRICK_HOME)/include/trick/compat \
 -I$(GUNNS_TRICK_HOME)/trick_source

include ${GUNNS_HOME}/lib/sources.mk
include ${GUNNS_HOME}/lib/sources_omit.mk
OBJECTS_CC := $(addprefix build,$(subst .cpp,.o,$(SOURCES)))
OBJECTS_CC := $(filter-out %.c,$(OBJECTS_CC))
OBJDIRS_CC := $(sort $(dir $(OBJECTS_CC)))

OBJECTS_C := $(addprefix build,$(subst .c,.o,$(SOURCES)))
OBJECTS_C := $(filter-out %.opp,$(OBJECTS_C))
OBJDIRS_C := $(sort $(dir $(OBJECTS_C)))

include ${GUNNS_HOME}/lib/sources_cuda.mk
CUDAOBJECTS_CC := $(addprefix buildroses,$(subst .cpp,.o,$(CUDASOURCES)))
CUDAOBJECTS_CC := $(filter-out %.c,$(CUDAOBJECTS_CC))
CUDAOBJDIRS_CC := $(sort $(dir $(CUDAOBJECTS_CC)))

CUDAOBJECTS_C := $(addprefix buildroses,$(subst .c,.o,$(CUDASOURCES)))
CUDAOBJECTS_C := $(filter-out %.opp,$(CUDAOBJECTS_C))
CUDAOBJDIRS_C := $(sort $(dir $(CUDAOBJECTS_C)))

# Convert the space-separated list of absolute path .cpp's and .c's in SOURCES to a
# newline-separated list of relative path include .hh and .h statements in HEADERS:
#
# This defines the suffix for substitution including a newline:
define sufhh
.hh"

endef

define sufh
.h"

endef

# This defines the prefix for substitution:
define pre
#include "
endef

HEADERS2 := $(subst .cpp,${sufhh}, $(SOURCES))
HEADERS := $(subst .c,${sufh}, $(HEADERS2))
HEADERS := $(subst $(GUNNS_HOME)/,${pre}, $(HEADERS))
export HEADERS
#$(info $$HEADERS is [${HEADERS}])

all: ${GUNNSLIB}

roses: CXXFLAGS += -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DGUNNS_CUDA_ENABLE=1
roses: ${GUNNSROSESLIB}

$(GUNNSLIB) : $(OBJECTS_CC) $(OBJECTS_C)
	ar crs $@ $^
	@ echo "-------------------------------------------------"
	@ echo " GUNNS LIBRARY FOR TRICK SIMS BUILD COMPLETE"
	@ echo " Library file is: ${GUNNSLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "-------------------------------------------------"
	@ echo "$$HEADERS" > ../trick_if/S_source.hh

$(GUNNSROSESLIB) : $(CUDAOBJECTS_CC) $(CUDAOBJECTS_C)
	ar crs $@ $^
	@ echo "----------------------------------------------------------"
	@ echo " GUNNS & ROSES LIBRARY FOR TRICK SIMS BUILD COMPLETE"
	@ echo " Includes CUDA classes and compiled with CUDA dependencies"
	@ echo " Library file is: ${GUNNSROSESLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "----------------------------------------------------------"
	@ echo "$$HEADERS" > ../trick_if/S_source.hh

$(OBJECTS_CC) : build%.o : %.cpp | $(OBJDIRS_CC)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(OBJECTS_C) : build%.o : %.c | $(OBJDIRS_C)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(OBJDIRS_CC) $(OBJDIRS_C) :
	mkdir -p $@

$(CUDAOBJECTS_CC) : buildroses%.o : %.cpp | $(CUDAOBJDIRS_CC)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(CUDAOBJECTS_C) : buildroses%.o : %.c | $(CUDAOBJDIRS_C)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(CUDAOBJDIRS_CC) $(CUDAOBJDIRS_C) :
	mkdir -p $@

clean:
	rm -rf build buildroses $(GUNNSLIB) $(GUNNSROSESLIB)
	rm ../trick_if/S_source.hh

################################################################################
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# This builds the Trick-less GUNNS compiled library for simulations.
#
# The default make command compiles GUNNS with the default -O2 -g optimization.
# This builds the libgunns.a library:
# $ make
#
# The roses target builds the libgunnsroses.a library, which is the same code
# base as libgunns.a, recompiled with dependencies on CUDA, plus those
# dependency CUDA classes:
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
CXXFLAGS = -Dno_TRICK_ENV=1 -Wall $(OPTS)

INCDIRS = -I$(GUNNS_HOME) -I$(GUNNS_HOME)/core -I$(GUNNS_HOME)/aspects \
 -I$(GUNNS_HOME)/ms-utils -I$(GUNNS_HOME)/gunns-ts-models

include ${GUNNS_HOME}/lib/sources.mk
include ${GUNNS_HOME}/lib/sources_omit.mk
include ${GUNNS_HOME}/lib/sources_omit_trickless.mk
OBJECTS := $(addprefix build,$(subst .cpp,.o,$(SOURCES)))
OBJDIRS := $(sort $(dir $(OBJECTS)))

include ${GUNNS_HOME}/lib/sources_cuda.mk
CUDAOBJECTS := $(addprefix buildroses,$(subst .cpp,.o,$(CUDASOURCES)))
CUDAOBJDIRS := $(sort $(dir $(CUDAOBJECTS)))

all: ${GUNNSLIB}

roses: CXXFLAGS += -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DGUNNS_CUDA_ENABLE=1
roses: ${GUNNSROSESLIB}

$(GUNNSLIB) : $(OBJECTS)
	ar crs $@ $^
	@ echo "-------------------------------------------------"
	@ echo " GUNNS TRICK-LESS LIBRARY FOR SIMS BUILD COMPLETE"
	@ echo " Library file is: ${GUNNSLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "-------------------------------------------------"

$(GUNNSROSESLIB) : $(CUDAOBJECTS)
	ar crs $@ $^
	@ echo "----------------------------------------------------------"
	@ echo " GUNNS & ROSES TRICK-LESS LIBRARY FOR SIMS BUILD COMPLETE"
	@ echo " Includes CUDA classes and compiled with CUDA dependencies"
	@ echo " Library file is: ${GUNNSROSESLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "----------------------------------------------------------"

$(OBJECTS) : build%.o : %.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(CUDAOBJECTS) : buildroses%.o : %.cpp | $(CUDAOBJDIRS)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(OBJDIRS) :
	mkdir -p $@

$(CUDAOBJDIRS) :
	mkdir -p $@

clean:
	rm -rf build $(GUNNSLIB)
	rm -rf buildroses $(GUNNSROSESLIB)


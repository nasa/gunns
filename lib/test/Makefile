################################################################################
# Copyright 2024 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# This builds the Trick-less GUNNS compiled libraries for unit tests.
#
# Normally, making any of the unit tests will also make this lib.  But you can
# also invoke this makefile directly to look at/debug this lib without running
# unit tests.
#
# Currently, two independent target libraries are supported here:
# $ make
#   - builds the libgunns.a library, which is the regular GUNNS code base lib.
# $ make roses
#   - builds the libgunnsroses.a library, which is the same GUNNS code base as
#     libgunns.a, recompiled with dependencies on CUDA, plus those dependency
#     CUDA classes.
#     Note that right now this roses target implementation is messy, using a
#     lot of duplicated rules to the default (all) target.  We'd like to clean
#     this up & make it more elegant & maintainable.
#
# For this lib, we should always compile with:
# - NO Trick dependencies
# - optimization level -O2, since that's the recommended level for GUNNS in
#   sims, we also test at this level.
# - debug option -g for debugger support, if needed
# - test coverage and profile arcs option for test coverage metrics in the unit
#   tests.
################################################################################
GUNNS_HOME ?= $(abspath ${PWD}/../..)

# Default options, enabling LCOV and Valgrind:
# For asan, we suggest calling this with overriding OPTS on the command line:
#     $ make OPTS='-O2 -g -fstack-protector-all -fsanitize=address'
# Note that asan interferes with Valgrind, and any compilation with asan directives
# or inclusion of asan libraries breaks Valgrind, so its best to use them
# completely separately.
OPTS = -O2 -g -ftest-coverage -fprofile-arcs

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
	@ echo "-------------------------------------------------------"
	@ echo " GUNNS TRICK-LESS LIBRARY FOR UNIT TESTS BUILD COMPLETE"
	@ echo " Library file is: ${GUNNSLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "-------------------------------------------------------"

$(GUNNSROSESLIB) : $(CUDAOBJECTS)
	ar crs $@ $^
	@ echo "---------------------------------------------------------------"
	@ echo " GUNNS & ROSES TRICK-LESS LIBRARY FOR UNIT TESTS BUILD COMPLETE"
	@ echo " Includes CUDA classes and compiled with CUDA dependencies"
	@ echo " Library file is: ${GUNNSROSESLIB}"
	@ echo " built with options: ${OPTS}"
	@ echo "---------------------------------------------------------------"

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


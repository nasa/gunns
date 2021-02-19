# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# All of the source files from sources.mk, plus the CUDA files needed for GUNNS & ROSES.
CUDASOURCES := $(SOURCES) $(wildcard $(GUNNS_HOME)/ms-utils/math/linear_algebra/cuda/*.cpp)

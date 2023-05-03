# Copyright 2023 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
# uncomment this out to use the trickified lib:
#-include ${GUNNS_HOME}/lib/trick_if/S_gunns_roses.mk

#export GUNNS_HOME = $(realpath ../..)
# this is probably a good idea to add to the GUNNS environment script:
export TRICK_GTE_EXT = GUNNS_HOME

# uncomment these for different optimization & debugging support:
TRICK_CFLAGS += -g -O2
TRICK_CXXFLAGS += -g -O2
#TRICK_CFLAGS += -g
#TRICK_CXXFLAGS += -g

# uncomment this to build & enable the CUDA stuff, or comment out if using the trickified lib:
#TRICK_CFLAGS += -fopenmp -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DGUNNS_CUDA_ENABLE=1
#TRICK_CXXFLAGS += -fopenmp -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DGUNNS_CUDA_ENABLE=1
#TRICK_EXCLUDE = /usr/local/cuda-8.0
#TRICK_USER_LINK_LIBS += -lcublas_static -lculibos -lcudart_static -lcusolver -lcusparse_static -lculibos

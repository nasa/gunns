# @copyright Copyright 2021 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
# uncomment this out to use the trickified lib:
-include ${GUNNS_HOME}/lib/trick_if/S_gunns.mk

#export GUNNS_HOME = $(realpath ../..)
# this is probably a good idea to add to the GUNNS environment script:
export TRICK_GTE_EXT = GUNNS_HOME

# uncomment this to build & enable the CUDA stuff:
#GUNNS_CUDA_ENABLE = 1
#TRICK_CFLAGS += -fopenmp -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -g
#TRICK_CXXFLAGS += -fopenmp -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -g
#TRICK_EXCLUDE = /usr/local/cuda-8.0
#TRICK_USER_LINK_LIBS += -lcublas_static -lculibos -lcudart_static -lcusolver -lcusparse_static -lculibos

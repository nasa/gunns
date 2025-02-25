# @copyright Copyright 2025 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
# uncomment one of these out to use the trickified lib:
-include ${GUNNS_HOME}/lib/trick_if/S_gunns.mk
#-include ${GUNNS_HOME}/lib/trick_if/S_gunns_roses.mk

# Build-time generation of GunnsDraw network code
include S_gunnsdraw_buildtime.mk

#export GUNNS_HOME = $(realpath ../..)
# this is probably a good idea to add to the GUNNS environment script:
export TRICK_GTE_EXT = GUNNS_HOME

# If not using the ROSES trickified lib above, uncomment this to build & enable the CUDA stuff:
#TRICK_CFLAGS += -fopenmp -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -g -DGUNNS_CUDA_ENABLE=1
#TRICK_CXXFLAGS += -fopenmp -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -g -DGUNNS_CUDA_ENABLE=1
#TRICK_EXCLUDE = /usr/local/cuda
#TRICK_USER_LINK_LIBS += -L/usr/local/cuda/lib64 -lcublas_static -lculibos -lcudart_static -lcusolver -lcusparse_static -lculibos

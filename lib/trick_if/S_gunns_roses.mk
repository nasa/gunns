# Copyright 2025 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# Make sure the user has defined $GUNNS_HOME.
ifeq ($(wildcard $(GUNNS_HOME)),)
    $(error $GUNNS_HOME must be set to your project's gunns/ folder.)
endif

# Make GUNNS_HOME available to the sim at run time.
TRICK_GTE_EXT += $(GUNNS_HOME)

# Specify root paths for header file includes.
TRICK_CFLAGS   += -I$(GUNNS_HOME) -I$(GUNNS_HOME)/ms-utils -I$(GUNNS_HOME)/gunns-ts-models
TRICK_CXXFLAGS += -I$(GUNNS_HOME) -I$(GUNNS_HOME)/ms-utils -I$(GUNNS_HOME)/gunns-ts-models

# Add 3rd party libraries.
#ifeq ($(TRICK_HOST_TYPE), Linux)
#    TRICK_USER_LINK_LIBS += -lsqlite3
#else ifeq ($(TRICK_HOST_TYPE), Darwin)
#    TRICK_USER_LINK_LIBS += -lsqlite3
#endif

# Compiled libraries for Trick sim integration.
GUNNS_TRICK    := $(GUNNS_HOME)/lib/trick/libgunnsroses.a
GUNNS_TRICK_IF := $(GUNNS_HOME)/lib/trick_if/libgunns.o

# Header file includes and compiler options for CUDA stuff.
CUDA_FLAGS := -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -DGUNNS_CUDA_ENABLE=1

# Enable library support if Trick >= 17.1 and the libraries are built.  Otherwise, let Trick
# generate all of the io_* and py_* code as usual.
ifeq ($(wildcard $(TRICK_HOME)/share/trick/makefiles/trickify.mk),)
    # GUNNS trickified lib is not being included because this version of Trick doesn't support it.
    TRICK_CFLAGS += $(CUDA_FLAGS)
    TRICK_CXXFLAGS += $(CUDA_FLAGS)

else ifeq ($(wildcard $(GUNNS_TRICK)),)
    # GUNNS trickified lib is not being included because GUNNS_TRICK is missing.
    TRICK_CFLAGS += $(CUDA_FLAGS)
    TRICK_CXXFLAGS += $(CUDA_FLAGS)

else ifeq ($(wildcard $(GUNNS_TRICK_IF)),)
    # GUNNS trickified lib is not being included because GUNNS_TRICK_IF is missing.
    TRICK_CFLAGS += $(CUDA_FLAGS)
    TRICK_CXXFLAGS += $(CUDA_FLAGS)

else
    $(info GUNNS & ROSES trickified lib $(GUNNS_TRICK_IF) is being included.)

    # Tell Trick to expect io_* and py_* code for these headers, but not to generate it itself.
    TRICK_EXT_LIB_DIRS += :$(GUNNS_HOME)/core:$(GUNNS_HOME)/aspects:$(GUNNS_HOME)/ms-utils:$(GUNNS_HOME)/gunns-ts-models

    # Tell Trick where to find the Python modules generated by SWIG.
    TRICK_PYTHON_PATH += :$(GUNNS_HOME)/lib/trick_if/python

    # Tell SWIG where to find *.i files.
    TRICK_SWIG_FLAGS += -I$(GUNNS_HOME)/lib/trick_if
    SWIG_FLAGS += -I$(GUNNS_HOME)/lib/trick_if

    # Link in the Trickified library.  We must use -whole-archive to ensure that io_* and py_* code
    # not needed at link time is nevertheless available at run time.
    ifeq ($(TRICK_HOST_TYPE), Linux)
        TRICK_USER_LINK_LIBS += -Wl,-whole-archive $(GUNNS_TRICK_IF) -Wl,-no-whole-archive
    else ifeq ($(TRICK_HOST_TYPE), Darwin)
        TRICK_USER_LINK_LIBS += -Wl,-force_load $(GUNNS_TRICK_IF)
    endif

    # Link in the model library.
    TRICK_USER_LINK_LIBS += $(GUNNS_TRICK)
endif

# Link in the CUDA libraries.
TRICK_EXCLUDE = /usr/local/cuda
TRICK_USER_LINK_LIBS += -L/usr/local/cuda/lib64 -lcublas_static -lcudart_static -lcusolver -lcusparse_static -lculibos

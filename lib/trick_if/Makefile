# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
ifndef TRICK_HOME
    $(error TRICK_HOME must be set)
endif

TRICKIFY := $(TRICK_HOME)/share/trick/makefiles/trickify.mk

ifeq ($(wildcard $(TRICKIFY)),)
    $(error This makefile requires at least Trick 17.1)
endif

GUNNS_HOME     ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../..)
MS_UTILS_HOME  ?= $(GUNNS_HOME)/ms-utils
TS_MODELS_HOME ?= $(GUNNS_HOME)/gunns-ts-models

export TRICKIFY_OBJECT_NAME = libgunns.o
export TRICKIFY_CXX_FLAGS = -I$(GUNNS_HOME) -I$(MS_UTILS_HOME) -I$(TS_MODELS_HOME)

all :
	@ $(MAKE) -s -f $(TRICKIFY)

clean :
	rm -rf build python trick $(TRICKIFY_OBJECT_NAME)

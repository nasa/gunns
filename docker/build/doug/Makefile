DOUG_HOME ?= ~/EDGE_v2.4.1

ifeq ($(wildcard $(DOUG_HOME)),)
$(error DOUG_HOME must be set to the top-level DOUG directory, but is set to "$(DOUG_HOME)", which does not exist)
endif

THIS_DIRECTORY := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
REGISTRY := esgl-gitlab.jsc.nasa.gov:5005/givs/givs

doug:
	docker build -t $(REGISTRY)/$@ -f $(THIS_DIRECTORY)/Dockerfile $(DOUG_HOME)

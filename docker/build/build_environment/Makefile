THIS_DIRECTORY := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
REPOSITORY := esgl-gitlab.jsc.nasa.gov:5005/gunns/gunns

build_environment:
	docker build -t $(REPOSITORY)/$@ - < $(THIS_DIRECTORY)/Dockerfile

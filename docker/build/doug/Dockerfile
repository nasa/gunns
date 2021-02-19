# NOTE: The top-level EDGE directory on the host machine must be passed to the
# docker build command as the build context for this Dockerfile.

# Start with NVIDIA's Centos 7 image. It's not required for this image, but any
# image derived from this one that wants to run DOUG will need it. Since you
# can't derive from more than one base image, we have to include it here.
FROM nvidia/opengl:1.0-glvnd-devel-centos7

# Might as well install the epel repository and update yum for the same reason.
RUN yum install -y epel-release && yum update -y

# cd to /home
WORKDIR /home

# Copy EDGE in from the host machine. This assumes that the top-level EDGE
# directory was specified as the build context, as noted at the top of this
# file.
COPY . EDGE_v2.4.1

# Set DOUG_HOME
ENV DOUG_HOME /home/EDGE_v2.4.1

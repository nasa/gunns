# Start with NVIDIA's Centos 7 image. It's not required for this image, but any
# image derived from this one that wants to run DOUG will need it. Since you
# can't derive from more than one base image, we have to include it here.
FROM nvidia/opengl:1.0-glvnd-devel-centos7

# Might as well install the epel repository and update yum for the same reason.
RUN yum install -y epel-release && yum update -y

# cd to /home
WORKDIR /home


# Use all the cores!
ENV MAKEFLAGS -j4

# Install all the packages!
RUN yum install -y \
 bison \
 clang-devel \
 cmake \
 curl-devel \
 flex \
 gcc \
 gcc-c++ \
 git \
 glibc-devel \
 gtest-devel \
 java-1.8.0-openjdk-devel \
 libX11-devel \
 libxml2-devel \
 libXi-devel \
 libXmu-devel \
 libXt-devel \
 llvm-devel \
 llvm-static \
 make \
 tcsh \
 csh \
 mesa-libGL-devel \
 mesa-libGLU-devel \
 maven \
 cppunit \
 ncurses-devel \
 openmotif-devel \
 perl-Digest-MD5 \
 protobuf-devel \
 python-devel \
 sqlite-devel \
 swig \
 systemd-devel \
 udunits2-devel \
 which \
 zlib-devel \
 tkinter

# Clone Trick
RUN git clone https://github.com/nasa/trick.git

# Build Trick
WORKDIR trick
RUN ./configure && make
WORKDIR ..

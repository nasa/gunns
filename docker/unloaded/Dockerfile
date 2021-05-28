# NOTE we recommend you tag this image as 'gunns_unloaded', as the 'loaded' image
# uses that as its base.
#
# Start with Centos 7 image.
FROM centos:7

# Install the epel repository and update yum.
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
 firefox \
 flex \
 gcc \
 gcc-c++ \
 git \
 glibc-devel \
 gtest-devel \
 https://github.com/jgraph/drawio-desktop/releases/download/v14.5.1/drawio-x86_64-14.5.1.rpm \
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
 cppunit-devel \
 lcov \
 valgrind \
 ncurses-devel \
 openmotif-devel \
 perl-Digest-MD5 \
 protobuf-devel \
 python-devel \
 sqlite-devel \
 swig \
 systemd-devel \
 tkinter \
 udunits2-devel \
 which \
 zip \
 zlib-devel

# Clone Trick
RUN git clone https://github.com/nasa/trick.git

# Make Trick
WORKDIR trick
RUN ./configure && make
WORKDIR ..

# Set environment variables, assuming GUNNS will be cloned or mapped to /home/gunns.
ENV TRICK_HOME /home/trick
ENV GUNNS_TRICK_HOME /home/trick
ENV GUNNS_HOME /home/gunns
ENV PATH "$PATH:/home/trick/bin"

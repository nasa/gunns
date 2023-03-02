# NOTE we recommend you tag this image as 'gunns_unloaded_or8', as the 'gunns_loaded_or8' image
# uses that as its base.
#
# Start with Oracle 8 image.  We use Oracle 8 as a substitute for RHEL8 since it is still 'RHEL-like', and free.
FROM oraclelinux:8

# Install optional latest repo
RUN yum install -y yum-utils
RUN dnf config-manager --set-enabled ol8_codeready_builder

# Reset repo list
RUN yum repolist

# Install the epel repository and Update yum
RUN yum install -y epel-release && yum update -y

# cd to /home
WORKDIR /home

# Use some cores
ENV MAKEFLAGS -j4

# Install all the packages!
# Note we're using tk-devel as a substitute for tkinter since tkinter is not currently available
# (or we don't know where to find it)
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
 https://github.com/jgraph/drawio-desktop/releases/download/v20.8.16/drawio-x86_64-20.8.16.rpm \
 java-11-openjdk-devel \
 lcov \
 libX11-devel \
 libxml2-devel \
 libXi-devel \
 libXmu-devel \
 libXt-devel \
 llvm-devel \
 llvm-static \
 make \
 maven \
 tcsh \
 csh \
 mesa-libGL-devel \
 mesa-libGLU-devel \
 ncurses-devel \
 openmotif \
 openmotif-devel \
 cppunit \
 cppunit-devel \
 valgrind \
 perl \
 perl-Digest-MD5 \
 protobuf-devel \
 python3-devel \
 sqlite-devel \
 swig \
 systemd-devel \
 tk-devel \
 udunits2 \
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

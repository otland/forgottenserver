FROM ubuntu:xenial
RUN export TERM=xterm

#INSTALL SOME TOOLS
RUN apt-get -y update && apt-get -y install curl \
    gdb \
    wget \
    nano \
    htop \
    build-essential \
    cmake \
    git-core \
    libc6 \
    libcurl3 \
    zlib1g \
    libgmp-dev \
    libmysqld-dev \
    mysql-client \
    libmysqlclient-dev \
    libpugixml-dev\
    lua5.1-dev \
    libphysfs-dev \
    libboost1.58-dev \
    libboost-system1.58-dev \
    libboost-filesystem1.58-dev \
    libboost-thread1.58-dev \
    libboost-regex1.58-dev \
    libboost-iostreams1.58-dev \
    libssl-dev \
    gdbserver

COPY build_engine /bin/
COPY run /bin/run_container

RUN chmod +x /bin/build_engine

#Needed directory.
RUN mkdir -p /shrd/data/world/patches

ENTRYPOINT /bin/run_container

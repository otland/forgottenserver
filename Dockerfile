FROM ubuntu:xenial

RUN apt-get update
RUN apt-get install -y build-essential clang cmake g++ git
RUN apt-get install -y libboost-iostreams-dev libboost-system-dev libgmp3-dev liblua5.2-dev libluajit-5.1-dev libmysqlclient-dev libpugixml-dev
RUN apt-get clean

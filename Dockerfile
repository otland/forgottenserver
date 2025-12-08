FROM debian:forky-slim AS build
RUN apt-get update -q && apt-get install -yq \
  build-essential \
  libboost-iostreams1.88-dev \
  libboost-json1.88-dev \
  libboost-system1.88-dev \
  cmake \
  liblua5.4-dev \
  libmariadb-dev \
  libpugixml-dev \
  libsimdutf-dev \
  libssl-dev \
  ninja-build

COPY cmake /usr/src/forgottenserver/cmake/
COPY src /usr/src/forgottenserver/src/
COPY CMakeLists.txt CMakePresets.json /usr/src/forgottenserver/
WORKDIR /usr/src/forgottenserver
RUN cmake --preset default && cmake --build --config RelWithDebInfo --preset default

FROM debian:forky-slim
RUN apt-get update -q && apt-get install -yq \
  libboost-iostreams1.88.0 \
  libboost-json1.88.0 \
  libboost-system1.88.0 \
  liblua5.4-0 \
  libmariadb3 \
  libpugixml1v5 \
  libsimdutf27 \
  libssl3t64

COPY --from=build /usr/src/forgottenserver/build/RelWithDebInfo/tfs /bin/tfs
COPY LICENSE key.pem /srv/

EXPOSE 7171 7172
WORKDIR /srv
VOLUME /srv
ENTRYPOINT ["/bin/tfs"]

# Stage 1: Build
FROM alpine:3.19 AS build

# Install build dependencies including Clang and libc++
RUN apk add --no-cache \
  clang \
  libc++ \
  build-base \
  boost-dev \
  cmake \
  fmt-dev \
  luajit-dev \
  mariadb-connector-c-dev \
  openssl-dev \
  pugixml-dev \
  samurai

# Set Clang as the compiler
ENV CC=clang
ENV CXX=clang++

# Copy source and cmake files
COPY cmake /usr/src/forgottenserver/cmake/
COPY src /usr/src/forgottenserver/src/
COPY CMakeLists.txt CMakePresets.json /usr/src/forgottenserver/
WORKDIR /usr/src/forgottenserver

# Build the project
RUN cmake --preset default -DUSE_LUAJIT=ON -DCMAKE_CXX_COMPILER=clang++ && \
  cmake --build --config RelWithDebInfo --preset default

# Stage 2: Runtime
FROM alpine:3.19

# Install runtime dependencies
RUN apk add --no-cache \
  boost-iostreams \
  boost-locale \
  boost-system \
  boost-json \
  fmt \
  luajit \
  mariadb-connector-c \
  openssl \
  pugixml

# Copy built binary and data
COPY --from=build /usr/src/forgottenserver/build/RelWithDebInfo/tfs /bin/tfs
COPY data /srv/data/
COPY LICENSE README.md *.dist *.sql key.pem /srv/

EXPOSE 7171 7172
WORKDIR /srv
VOLUME /srv
ENTRYPOINT ["/bin/tfs"]

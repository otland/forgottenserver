FROM alpine:3.15.0 AS build
# crypto++-dev is in edge/testing
RUN apk add --no-cache --repository http://dl-cdn.alpinelinux.org/alpine/edge/testing/ \
  binutils \
  boost-dev \
  build-base \
  clang \
  crypto++-dev \
  gcc \
  gmp-dev \
  luajit-dev \
  mariadb-connector-c-dev \
  meson \
  ninja

COPY src /usr/src/forgottenserver/src/
COPY meson.build /usr/src/forgottenserver/
WORKDIR /usr/src/forgottenserver
RUN meson build && ninja -C build

FROM alpine:3.15.0
# crypto++ is in edge/testing
RUN apk add --no-cache --repository http://dl-cdn.alpinelinux.org/alpine/edge/testing/ \
  boost-iostreams \
  boost-system \
  boost-filesystem \
  crypto++ \
  gmp \
  luajit \
  mariadb-connector-c

COPY --from=build /usr/src/forgottenserver/build/tfs /bin/tfs
COPY data /srv/data/
COPY LICENSE README.md *.dist *.sql key.pem /srv/

EXPOSE 7171 7172
WORKDIR /srv
VOLUME /srv
ENTRYPOINT ["/bin/tfs"]

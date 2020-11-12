FROM alpine:edge AS build
# crypto++-dev is in edge/testing
RUN apk add --no-cache --repository http://dl-3.alpinelinux.org/alpine/edge/testing/ \
  binutils \
  boost-dev \
  build-base \
  clang \
  cmake \
  crypto++-dev \
  gcc \
  gmp-dev \
  make \
  mariadb-connector-c-dev \
  pugixml-dev
# luajit-dev is in edge/main
RUN apk add --no-cache --repository http://dl-3.alpinelinux.org/alpine/edge/main/ \
	  luajit-dev 

COPY cmake /usr/src/forgottenserver/cmake/
COPY src /usr/src/forgottenserver/src/
COPY CMakeLists.txt /usr/src/forgottenserver/
WORKDIR /usr/src/forgottenserver/build
RUN cmake .. && make

FROM alpine:edge
# crypto++-dev is in edge/testing
RUN apk add --no-cache --repository http://dl-3.alpinelinux.org/alpine/edge/testing/ \
  boost-iostreams \
  boost-system \
  boost-filesystem \
  crypto++ \
  gmp \
  mariadb-connector-c \
  pugixml
# luajit is in edge/main
RUN apk add --no-cache --repository http://dl-3.alpinelinux.org/alpine/edge/main/ \
	  luajit 

RUN ln -s /usr/lib/libcryptopp.so /usr/lib/libcryptopp.so.5.6
COPY --from=build /usr/src/forgottenserver/build/tfs /bin/tfs
COPY data /srv/data/
COPY LICENSE README.md *.dist *.sql key.pem /srv/

EXPOSE 7171 7172
WORKDIR /srv
VOLUME /srv
ENTRYPOINT ["/bin/tfs"]

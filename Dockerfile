FROM ubuntu:18.04

# add our user and group first to make sure their IDs get assigned consistently, regardless of other deps added later
RUN groupadd developer \
  && useradd -g developer developer

RUN apt update && apt install -y \
    gnupg \
    ca-certificates

RUN echo "deb https://dl.mumble.info/mirror/pkg.mxe.cc/repos/apt bionic main" > /etc/apt/sources.list.d/mxe.list
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 86B72ED9
RUN apt update && apt install -y \
	mxe-x86-64-w64-mingw32.static-qtbase \
	mxe-x86-64-w64-mingw32.static-qtsvg \
	mxe-x86-64-w64-mingw32.static-qttools \
	mxe-x86-64-w64-mingw32.static-qttranslations \
	mxe-x86-64-w64-mingw32.static-boost \
	mxe-x86-64-w64-mingw32.static-protobuf \
	mxe-x86-64-w64-mingw32.static-sqlite \
	mxe-x86-64-w64-mingw32.static-flac \
 	mxe-x86-64-w64-mingw32.static-ogg \
	mxe-x86-64-w64-mingw32.static-vorbis \
	mxe-x86-64-w64-mingw32.static-libsndfile

ENV MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-pc-linux-gnu/bin/protoc
ENV PATH="${PATH}:/usr/lib/mxe/usr/bin"
ENV ARCH=x86-64

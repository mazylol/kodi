FROM ubuntu:latest
LABEL authors="Landon"

RUN apt-get update
RUN apt-get install -y build-essential clang gcc ninja-build cmake git zlib1g-dev openssl libcurl4 libssl-dev ca-certificates

WORKDIR /usr/src/app
COPY . .
RUN mkdir build
WORKDIR /usr/src/app/build

RUN cmake -DCMAKE_GENERATOR=Ninja -DCMAKE_BUILD_TYPE=Release ..
RUN ninja

ENV PROD=1

CMD ["./kodi"]
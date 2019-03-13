FROM ubuntu:18.04

MAINTAINER Dmitriy Salman

RUN apt-get -y update

RUN apt-get -y install g++
# RUN apt-get install -y software-properties-common
# RUN add-apt-repository -y ppa:acooks/libwebsockets6
RUN apt-get -y update
RUN apt-get -y install libuv1.dev
RUN apt-get -y install make

WORKDIR /home/server

ADD . .
RUN make

EXPOSE 80
CMD ./server
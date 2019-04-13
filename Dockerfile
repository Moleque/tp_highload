FROM ubuntu:18.04

MAINTAINER Dmitriy Salman

RUN apt-get -y update

RUN apt-get -y install g++
RUN apt-get -y update
RUN apt-get -y install libuv1.dev
RUN apt-get -y install make
RUN apt-get -y install lsof

WORKDIR /home/server

ADD . .
RUN make

EXPOSE 80
CMD ./server
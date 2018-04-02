FROM ubuntu:xenial

ADD sources.list /etc/apt/sources.list
ADD waitForever.sh /
RUN chmod u+x waitForever.sh

RUN apt-get update
RUN apt-get install -y g++ gdb

CMD ./waitForever.sh

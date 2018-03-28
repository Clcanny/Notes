FROM ubuntu:latest

ADD sources.list /etc/apt/sources.list
ADD waitForever.sh /
RUN chmod u+x waitForever.sh
RUN apt-get update
RUN apt-get install -y iputils-ping iproute2
RUN apt-get install -y tshark

ENTRYPOINT ./waitForever.sh

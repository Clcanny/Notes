#!/bin/bash
docker run -itd --name compile gdb:ubuntu
docker cp icmp4.c compile:/ping.cpp
docker exec compile bash -c "g++ -std=c++11 -fpermissive ping.cpp -o ping.out"
docker cp compile:/ping.out ./

docker run -itd --privileged --name ping network:ubuntu /bin/bash -c "tshark -w ping.tshark"
docker cp ping.out ping:/
docker exec ping bash -c "./ping.out 127.0.0.1"
sleep 1
docker cp ping:/ping.tshark ./

docker rm -f compile ping
rm ping.out

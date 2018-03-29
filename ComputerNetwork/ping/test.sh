#!/bin/bash
docker run -itd --name compile gdb:ubuntu
docker cp ping.cpp compile:/
docker exec compile bash -c "g++ -std=c++11 ping.cpp -o ping.out"
docker cp compile:/ping.out ./

docker run -itd --name ping network:ubuntu bash -c "tshark -w ping.tshark"
docker cp ping.out ping:/
docker exec ping bash -c "./ping.out"
docker exec ping bash -c "ping baidu.com -c 4"
docker cp ping:/ping.tshark ./

docker rm -f compile ping

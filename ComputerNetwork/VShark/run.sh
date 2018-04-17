#!/bin/bash

docker run -d --name compile gdb:ubuntu
docker run -d --privileged --name network network:ubuntu

docker cp VShark.cpp compile:/
docker cp mac.h compile:/
docker cp mac.cpp compile:/
docker cp arp.h compile:/
docker cp arp.cpp compile:/
docker cp ip.h compile:/
docker cp ip.cpp compile:/
docker cp icmp.h compile:/
docker cp icmp.cpp compile:/
docker cp udp.h compile:/
docker cp udp.cpp compile:/
docker cp tcp.h compile:/
docker cp tcp.cpp compile:/
docker cp helper.h compile:/
docker cp helper.cpp compile:/

docker exec compile bash -c "g++ -Wall -g -std=c++11 VShark.cpp mac.cpp arp.cpp ip.cpp icmp.cpp udp.cpp tcp.cpp helper.cpp -o VShark"
docker cp compile:/VShark VShark
docker cp VShark network:/VShark
rm VShark
docker exec -it network bash -c "./VShark"

# 打开另一个Terminal
# docker exec network bash -c "arp-scan 172.17.0.1"
# docker exec network bash -c "nslookup baidu.com"
# docker exec network bash -c "ping 111.13.101.208 -c 1"
# docker exec network bash -c "curl 111.13.101.208"

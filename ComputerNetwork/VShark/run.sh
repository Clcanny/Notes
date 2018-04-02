#!/bin/bash

mkdir runnable

docker run -d --name compile gdb:ubuntu
docker cp VShark.cpp compile:/
docker exec compile bash -c "g++ -Wall -g -std=c++11 VShark.cpp -o VShark"
docker cp compile:/VShark runnable/VShark

docker network create VMNet1 --subnet 172.0.1.0/28 --gateway 172.0.1.14
docker network create VMNet2 --subnet 172.0.2.0/28 --gateway 172.0.2.14

docker run -d --name router network:ubuntu
docker network connect VMNet1 router --ip 172.0.1.1
docker network connect VMNet2 router --ip 172.0.2.1
docker network disconnect bridge router

docker run -d --privileged -v runnable:runnbale --name container1 network:ubuntu bash -c "./runnable/VShark"
docker network connect VMNet1 container1 --ip 172.0.1.2
docker network disconnect bridge container1
docker exec container1 bash -c "ip route change default via 172.0.1.1 dev eth1"

docker run -d --privileged --name container2 network:ubuntu
docker network connect VMNet2 container2 --ip 172.0.2.2
docker network disconnect bridge container2
docker exec container2 bash -c "ip route change default via 172.0.2.1 dev eth1"
docker exec container2 bash -c "ping 172.0.1.2"

docker logs container1

docker rm -f compile router container1 container2
docker network rm VMNet1 VMNet2
rm -r runnable

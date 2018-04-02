#!/bin/bash

docker run -d --name compile gdb:ubuntu
docker cp ping.h compile:/
docker cp ping.cpp compile:/
docker cp main.cpp compile:/
docker exec compile bash -c "g++ -std=c++11 ping.cpp main.cpp -o ping"
docker cp compile:/ping .

docker network create VMNet --subnet 172.0.0.0/28 --gateway 172.0.0.14

docker run -d --privileged --name container1 network:ubuntu bash -c "tshark -w container1.tshark"
docker network connect VMNet container1 --ip 172.0.0.1
docker network disconnect bridge container1
docker cp ping container1:/ping

docker run -d --name container2 network:ubuntu
docker network connect VMNet container2 --ip 172.0.0.2
docker network disconnect bridge container2
docker cp ping container2:/ping

docker exec container1 bash -c "./ping 172.0.0.2"
docker exec container2 bash -c "./ping 172.0.0.1"

# docker cp container1:/container1.tshark ping.tshark

docker rm -f compile container1 container2
docker network rm VMNet
rm ping

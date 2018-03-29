#!/bin/bash

docker run -d --name compile gdb:ubuntu
docker cp sendRawPacket.cpp compile:/send.cpp
docker cp receiveRawpacket.cpp compile:/receive.cpp
docker exec compile bash -c "g++ -g -std=c++11 send.cpp -o send"
docker exec compile bash -c "g++ -g -std=c++11 receive.cpp -o receive"
docker cp compile:/send send
docker cp compile:/receive receive

docker network create VMNet --subnet 172.0.0.0/28 --gateway 172.0.0.14

docker run -d --name container1 network:ubuntu
docker network connect VMNet container1 --ip 172.0.0.1
docker network disconnect bridge container1
docker cp send container1:/send

docker run -d --name container2 network:ubuntu
docker network connect VMNet container2 --ip 172.0.0.2
docker network disconnect bridge container2
docker cp receive container2:/receive

docker exec container2 bash -c "./receive" &
docker exec container1 bash -c "./send"

echo "container1's logs:\n"
docker logs container1
echo "container2's logs:\n"
docker logs container1
echo "\n"

docker rm -f compile container1 container2
docker network rm VMNet
rm send receive

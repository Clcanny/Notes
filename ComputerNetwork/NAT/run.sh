#/bin/bash
docker network create --subnet 172.0.0.0/24 --gateway 172.0.0.254 insideSubnet
docker network create --subnet 172.1.0.0/24 --gateway 172.1.0.254 fakeInternet

docker run -d --privileged --name natRouter network:ubuntu
docker network disconnect bridge natRouter
docker network connect insideSubnet natRouter --ip 172.0.0.253
docker network connect fakeInternet natRouter --ip 172.1.0.252
docker exec natRouter bash -c "ip route change default via 172.1.0.253 dev eth2"
docker exec natRouter bash -c "apt-get install -y iptables"
docker exec natRouter bash -c "iptables -t nat -A POSTROUTING -s 172.0.0.1/32 -o eth1 -j SNAT --to 172.1.0.1"
docker exec natRouter bash -c "iptables -t nat -A POSTROUTING -s 172.0.0.2/32 -o eth1 -j SNAT --to 172.1.0.2"
docker exec natRouter bash -c "iptables -L POSTROUTING -t nat"

docker run -d --privileged --name firstHopRouter network:ubuntu
docker network disconnect bridge firstHopRouter
docker network connect fakeInternet firstHopRouter --ip 172.1.0.253
docker exec firstHopRouter bash -c "ip route add 172.1.0.0/30 via 172.1.0.252 dev eth1"

docker run -d --privileged --name pc1 network:ubuntu
docker network disconnect bridge pc1
docker network connect insideSubnet pc1 --ip 172.0.0.1
docker exec pc1 bash -c "ip route change default via 172.0.0.253 dev eth1"

docker run -d --privileged --name pc2 network:ubuntu
docker network disconnect bridge pc2
docker network connect insideSubnet pc2 --ip 172.0.0.2
docker exec pc2 bash -c "ip route change default via 172.0.0.253 dev eth1"

docker exec pc1 bash -c "ping 172.0.0.253 -c 4"
docker exec pc1 bash -c "ping 172.1.0.253 -c 4"

# docker rm -f natRouter
# docker rm -f pc1 pc2
# docker network rm insideSubnet

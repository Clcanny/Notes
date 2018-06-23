#/bin/bash
docker network create VMNet1 --subnet 172.0.1.0/24 --gateway 172.0.1.254
docker network create VMNet2 --subnet 172.0.2.0/24 --gateway 172.0.2.254

docker run -d --privileged --name router network:ubuntu
docker network disconnect bridge router
docker network connect VMNet1 router --ip 172.0.1.1
docker network connect VMNet2 router --ip 172.0.2.1
docker exec router bash -c "ip route delete default"
docker exec router bash -c "ip route change 172.0.1.0/24 via 172.0.1.1 dev eth1"
docker exec router bash -c "ip route change 172.0.2.0/24 via 172.0.2.1 dev eth2"
# docker exec router bash -c "iptables -t nat -A PREROUTING -d 172.0.2.3 -i eth2 -j DNAT --to-destination 172.0.1.2"
docker exec router bash -c "iptables -t nat -A POSTROUTING -s 172.0.1.2 -o eth2 -j SNAT --to-source 172.0.2.3"
docker exec -d router bash -c "tshark -w test.tshark"

docker run -d --privileged --name container1.2 network:ubuntu
docker network disconnect bridge container1.2
docker network connect VMNet1 container1.2 --ip 172.0.1.2
docker exec container1.2 bash -c "ip route change default via 172.0.1.1 dev eth1"
docker exec container1.2 bash -c "ip route delete 172.0.1.0/24"
docker exec container1.2 bash -c "ip route"
docker exec -d container1.2 bash -c "tshark -w test.tshark"

docker run -d --privileged --name container2.2 network:ubuntu
docker network disconnect bridge container2.2
docker network connect VMNet2 container2.2 --ip 172.0.2.2
docker exec container2.2 bash -c "ip route change default via 172.0.2.1 dev eth1"
docker exec container2.2 bash -c "ip route delete 172.0.2.0/24"
docker exec container2.2 bash -c "ip route"

docker exec container1.2 bash -c "ping 172.0.2.2 -c 4"
docker exec container2.2 bash -c "ping 172.0.2.3 -c 4"

docker cp router:/test.tshark router.tshark
docker cp container1.2:/test.tshark container1.2.tshark

docker rm -f router container1.2 container2.2
docker network rm VMNet1 VMNet2

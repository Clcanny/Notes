#/bin/bash
docker network create VMNet1 --subnet 192.168.2.0/25 --gateway 192.168.2.126
docker network create VMNet2 --subnet 192.168.2.128/27 --gateway 192.168.2.158
docker network create NetworkBetweenRouters --subnet 210.28.120.0/24 --gateway 210.28.120.254
docker network create Internet --subnet 210.28.130.0/24 --gateway 210.28.130.254

docker run -d --privileged --name router1 network:ubuntu
docker network disconnect bridge router1
docker network connect VMNet1 router1 --ip 192.168.2.1
docker network connect VMNet2 router1 --ip 192.168.2.129
docker network connect NetworkBetweenRouters router1 --ip 210.28.120.1
# 顺序十分重要
docker exec router1 bash -c "ip route add 210.28.0.0/16 via 210.28.120.2 dev eth3"
docker exec router1 bash -c "ip route delete 210.28.120.0/24"
docker exec router1 bash -c "ip route delete default"
docker exec router1 bash -c "ip route delete 192.168.2.0/25"
docker exec router1 bash -c "ip route add 192.168.2.0/25 via 192.168.2.1 dev eth1"
docker exec router1 bash -c "ip route delete 192.168.2.128/27"
docker exec router1 bash -c "ip route add 192.168.2.128/27 via 192.168.2.129 dev eth2"
# 显示新的路由规则
docker exec router1 bash -c "ip route"
# docker exec router1 bash -c "iptables -t nat -A POSTROUTING -s 192.168.2.2 -o eth3 -j SNAT --to-source 210.28.130.3"

docker run -d --privileged --name router2 network:ubuntu
docker network disconnect bridge router2
docker network connect NetworkBetweenRouters router2 --ip 210.28.120.2
docker network connect Internet router2 --ip 210.28.130.1
docker exec router2 bash -c "ip route delete default"
docker exec router2 bash -c "ip route add default via 210.28.120.1 dev eth1"
docker exec router2 bash -c "ip route delete 210.28.120.0/24"
docker exec router2 bash -c "ip route delete 210.28.130.0/24"
docker exec router2 bash -c "ip route add 210.28.130.0/24 via 210.28.130.1 dev eth2"
# 显示新的路由规则
docker exec router2 bash -c "ip route"

docker run -d --privileged --name container1 network:ubuntu
docker network disconnect bridge container1
docker network connect VMNet1 container1 --ip 192.168.2.2
docker exec container1 bash -c "ip route change default via 192.168.2.1 dev eth1"

docker run -d --privileged --name container2 network:ubuntu
docker network disconnect bridge container2
docker network connect VMNet1 container2 --ip 192.168.2.3
docker exec container2 bash -c "ip route change default via 192.168.2.1 dev eth1"

docker run -d --privileged --name container3 network:ubuntu
docker network disconnect bridge container3
docker network connect VMNet2 container3 --ip 192.168.2.130
docker exec container3 bash -c "ip route change default via 192.168.2.129 dev eth1"

docker run -d --privileged --name container4 network:ubuntu
docker network disconnect bridge container4
docker network connect Internet container4 --ip 210.28.130.2
docker exec container4 bash -c "ip route change default via 210.28.130.1 dev eth1"
docker exec -d container4 bash -c "tshark -w test.tshark"

docker exec container1 bash -c "ping 192.168.2.3 -c 4" | tail -1
docker exec container1 bash -c "ping 192.168.2.130 -c 4" | tail -1
docker exec container1 bash -c "ping 210.28.130.2 -c 4" | tail -1

docker exec container2 bash -c "ping 192.168.2.2 -c 4" | tail -1
docker exec container2 bash -c "ping 192.168.2.130 -c 4" | tail -1
docker exec container2 bash -c "ping 210.28.130.2 -c 4" | tail -1

docker exec container4 bash -c "ping 192.168.2.2 -c 4" | tail -1
docker exec container4 bash -c "ping 192.168.2.3 -c 4" | tail -1
docker exec container4 bash -c "ping 192.168.2.130 -c 4" | tail -1

docker exec container3 bash -c "ping 192.168.2.2 -c 4" | tail -1
docker exec container3 bash -c "ping 192.168.2.3 -c 4" | tail -1
docker exec container3 bash -c "ping 210.28.130.2 -c 4" | tail -1

sleep 5
docker cp container4:/test.tshark .

docker rm -f router1 router2 container1 container2 container4 container3
docker network rm VMNet1 VMNet2 NetworkBetweenRouters Internet

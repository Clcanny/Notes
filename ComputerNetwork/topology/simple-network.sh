docker network create --subnet=172.0.1.0/28 --gateway 172.0.1.14 VMNet1
docker network create --subnet=172.0.2.0/28 --gateway 172.0.2.14 VMNet2

docker run -itd --privileged --name router network:ubuntu
docker network connect VMNet1 router --ip 172.0.1.1
docker network connect VMNet2 router --ip 172.0.2.1
docker network disconnect bridge router

docker run -itd --privileged --name container1 network:ubuntu
docker network connect VMNet1 container1 --ip 172.0.1.2
docker network disconnect bridge container1

docker run -itd --privileged --name container2 network:ubuntu
docker network connect VMNet2 container2 --ip 172.0.2.2
docker network disconnect bridge container2

# 设置gateway
docker exec container1 bash -c "ip route change default via 172.0.1.1 dev eth1"
docker exec container2 bash -c "ip route change default via 172.0.2.1 dev eth1"

docker exec container1 bash -c "ping 172.0.2.2 -c 4"
docker exec container2 bash -c "ping 172.0.1.2 -c 4"

# 删除默认的路由表
docker exec router bash -c "ip route delete default"
docker exec router bash -c "ip route delete 172.0.1.0/28"
docker exec router bash -c "ip route delete 172.0.2.0/28"
# 添加两条路由规则
docker exec router bash -c "ip route add 172.0.1.0/28 via 172.0.1.1 dev eth1"
docker exec router bash -c "ip route add 172.0.2.0/28 via 172.0.2.1 dev eth2"

docker exec container1 bash -c "ping 172.0.2.2 -c 4"
docker exec container2 bash -c "ping 172.0.1.2 -c 4"

# 清理工作
docker rm -f container1 container2 router
docker network rm VMNet1 VMNet2

docker network create --subnet=172.0.1.0/28 --gateway 172.0.1.14 VMNet1
docker network create --subnet=172.0.2.0/28 --gateway 172.0.2.14 VMNet2
docker network create --subnet=172.0.3.0/28 --gateway 172.0.3.14 NetworkBetweenRouters

docker run -itd --privileged --name router1 network:ubuntu
docker network connect VMNet1 router1 --ip 172.0.1.1
docker network connect NetworkBetweenRouters router1 --ip 172.0.3.1
docker network disconnect bridge router1
# 删除默认的路由规则
docker exec router1 bash -c "ip route del default"
docker exec router1 bash -c "ip route del 172.0.1.0/28"
# docker exec router1 bash -c "ip route del 172.0.3.0/28"
# 添加两条路由规则
docker exec router1 bash -c "ip route add 172.0.1.0/28 via 172.0.1.1 dev eth1"
# docker exec router1 bash -c "ip route add 172.0.3.0/28 via 172.0.3.1 dev eth2"
docker exec router1 bash -c "ip route add 172.0.2.0/28 via 172.0.3.2 dev eth2"
# 显示新的路由规则
docker exec router1 bash -c "ip route"
# 查看是否允许转发
docker exec router1 bash -c "cat /proc/sys/net/ipv4/ip_forward"

docker run -itd --privileged --name router2 network:ubuntu
docker network connect VMNet2 router2 --ip 172.0.2.1
docker network connect NetworkBetweenRouters router2 --ip 172.0.3.2
docker network disconnect bridge router2
# 删除默认的路由规则
docker exec router2 bash -c "ip route del default"
docker exec router2 bash -c "ip route del 172.0.2.0/28"
# docker exec router2 bash -c "ip route del 172.0.3.0/28"
# 添加两条路由规则
docker exec router2 bash -c "ip route add 172.0.1.0/28 via 172.0.3.1 dev eth2"
# docker exec router2 bash -c "ip route add 172.0.3.0/28 via 172.0.3.2 dev eth2"
docker exec router2 bash -c "ip route add 172.0.2.0/28 via 172.0.2.1 dev eth1"
# 显示新的路由规则
docker exec router2 bash -c "ip route"
# 查看是否允许转发
docker exec router2 bash -c "cat /proc/sys/net/ipv4/ip_forward"

docker run -itd --privileged --name container1.2 network:ubuntu
docker network connect VMNet1 container1.2 --ip 172.0.1.2
docker network disconnect bridge container1.2
# 设置gateway
docker exec container1.2 bash -c "ip route change default via 172.0.1.1 dev eth1"

docker run -itd --privileged --name container1.3 network:ubuntu
docker network connect VMNet1 container1.3 --ip 172.0.1.3
docker network disconnect bridge container1.3
# 设置gateway
docker exec container1.3 bash -c "ip route change default via 172.0.1.1 dev eth1"

docker run -itd --privileged --name container2.2 network:ubuntu
docker network connect VMNet2 container2.2 --ip 172.0.2.2
docker network disconnect bridge container2.2
# 设置gateway
docker exec container2.2 bash -c "ip route change default via 172.0.2.1 dev eth1"

docker run -itd --privileged --name container2.3 network:ubuntu
docker network connect VMNet2 container2.3 --ip 172.0.2.3
docker network disconnect bridge container2.3
# 设置gateway
docker exec container2.3 bash -c "ip route change default via 172.0.2.1 dev eth1"

docker exec container1.2 bash -c "ping 172.0.1.3 -c 4" | tail -1
docker exec container1.2 bash -c "ping 172.0.2.2 -c 4" | tail -1
docker exec container1.2 bash -c "ping 172.0.2.3 -c 4" | tail -1

docker exec container1.3 bash -c "ping 172.0.1.2 -c 4" | tail -1
docker exec container1.3 bash -c "ping 172.0.2.2 -c 4" | tail -1
docker exec container1.3 bash -c "ping 172.0.2.3 -c 4" | tail -1

docker exec container2.2 bash -c "ping 172.0.1.2 -c 4" | tail -1
docker exec container2.2 bash -c "ping 172.0.1.3 -c 4" | tail -1
docker exec container2.2 bash -c "ping 172.0.2.3 -c 4" | tail -1

docker exec container2.3 bash -c "ping 172.0.1.2 -c 4" | tail -1
docker exec container2.3 bash -c "ping 172.0.1.3 -c 4" | tail -1
docker exec container2.3 bash -c "ping 172.0.2.2 -c 4" | tail -1

# 清理工作
docker rm -f router1 router2 container1.2 container1.3 container2.2 container2.3
docker network rm VMNet1 VMNet2 NetworkBetweenRouters

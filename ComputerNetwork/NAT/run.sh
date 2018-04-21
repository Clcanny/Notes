#/bin/bash
docker network create --subnet 172.0.0.0/20 --ip-range 172.0.0.0/25 --gateway 172.0.0.126 subnet1
docker network create --subnet 172.0.0.0/20 --ip-range 172.0.1.0/30 --gateway 172.0.1.3 subnet2

# docker run -d --privileged --name router network:ubuntu
# docker network disconnect bridge router
# docker network connect VMNet router --ip 172.0.0.0 --ip 172.0.0.10
# docker inspect router
# docker exec router bash -c "hostname -i"
# docker exec router bash -c "ip link"
# 
# docker run -d --privileged --name pc1 network:ubuntu
# docker network disconnect bridge pc1
# docker network connect VMNet pc1 --ip 172.0.0.1
# 
# docker run -d --privileged --name pc2 network:ubuntu
# docker network disconnect bridge pc2
# docker network connect VMNet pc2 --ip 172.0.0.2
# 
# docker run -d --privileged --name pc3 network:ubuntu
# docker network disconnect bridge pc3
# docker network connect VMNet pc3 --ip 172.0.0.3
# 
# docker rm -f router
# docker rm -f pc1 pc2 pc3
docker network rm subnet1 subnet2

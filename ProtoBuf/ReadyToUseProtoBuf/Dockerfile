FROM vimcpp:latest
MAINTAINER demons(VivianSJ's boyfriend) 837940593@qq.com

RUN apt-get update -y
RUN apt-get install -y autoconf automake libtool curl make g++ unzip

RUN apt-get install -y git
RUN cd /root && git clone https://github.com/google/protobuf.git

WORKDIR /root/protobuf
RUN chmod u+x autogen.sh && ./autogen.sh
RUN chmod u+x configure && ./configure --prefix=/usr
RUN make && make check && make install
# refresh shared library cache.
RUN ldconfig

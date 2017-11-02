# Ready To Use ProtoBuf #

## Installation ##

[C++ Installation - Unix](https://github.com/google/protobuf/blob/master/src/README.md)

[Dockerfile](Dockerfile)

## Compile ##

```shell
protoc -I=./ --cpp_out ./ ./addressbook.proto
g++ -std=c++11 -g -Wall write.cpp addressbook.pb.cc -lprotobuf -o write.out
g++ -std=c++11 -g -Wall write.cpp addressbook.pb.cc -lprotobuf -lpthread -static -static-libgcc -static-libstdc++ -o write.out
```

`ProtoBuf`可以分为两部分：

+ 静态：前段编译器，把按照`ProtoBuf`语法规范写的文件编译成C++／Java／……
+ 动态链接库

具体见addressbook的例子
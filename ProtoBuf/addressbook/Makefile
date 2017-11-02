pre-compile:
	protoc -I=./ --cpp_out ./ ./addressbook.proto

compile-dynamic: pre-compile
	g++ -std=c++11 -g -Wall write.cpp addressbook.pb.cc -lprotobuf -o write.out
	g++ -std=c++11 -g -Wall read.cpp addressbook.pb.cc -lprotobuf -o read.out

compile-static: pre-compile
	g++ -std=c++11 -g -Wall write.cpp addressbook.pb.cc -lprotobuf -lpthread -static -static-libgcc -static-libstdc++ -o write.out
	g++ -std=c++11 -g -Wall read.cpp addressbook.pb.cc -lprotobuf -lpthread -static -static-libgcc -static-libstdc++ -o read.out

clean:
	- rm *.pb.*
	- rm test
	- rm *.out

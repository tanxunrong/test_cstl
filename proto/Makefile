

install : test_proto.o message.pb-c.o
	cc test_proto.o message.pb-c.o  \
		-L/usr/local/Cellar/protobuf-c/0.15/lib -lprotobuf-c -o test_proto

message.pb-c.o : message.pb-c.c message.pb-c.h
	cc -c -g message.pb-c.c -Wall \
		-I/usr/local/Cellar/protobuf-c/0.15/include
test_proto.o : test_proto.c message.pb-c.c message.pb-c.h 
	cc -c -g test_proto.c -Wall

clean :
	rm *.o
	rm test_proto

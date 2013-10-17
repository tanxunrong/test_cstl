

install : test_proto.o message.pb-c.o
	if [ "$(shell uname)" = "Linux" ] ; then \
		cc test_proto.o message.pb-c.o -lprotobuf-c -o test_proto ; \
		fi
	if [ "$(shell uname)" = "Darwin" ]; then \
	cc test_proto.o message.pb-c.o  \
		-L/usr/local/Cellar/protobuf-c/0.15/lib -lprotobuf-c -o test_proto; \
		fi

message.pb-c.o : message.pb-c.c message.pb-c.h
	cc -c -g -std=c99 message.pb-c.c -Wall \
		-I/usr/local/Cellar/protobuf-c/0.15/include
test_proto.o : test_proto.c message.pb-c.c message.pb-c.h 
	cc -c -g -std=c99 test_proto.c -Wall

clean :
	rm *.o
	rm test_proto

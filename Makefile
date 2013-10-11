Common_cflags= -Wall -g -std=c99
Glib_cflags := $(shell pkg-config --cflags glib-2.0)
Event_cflags := $(shell pkg-config --cflags libevent)
CFLAGS=$(Common_cflags) $(Glib_cflags) $(Event_cflags) 

Objs = list.o rbtree.o hashmap.o queue.o vector.o

Test_objs = $(patsubst %,test_%,$(Objs)) 

Targets = $(patsubst %.o,test_%,$(Objs))

install : $(Objs) $(Test_objs)
	cc test_list.o list.o -o test_list
	#for t in $(Test_objs);do cc $$t -o $(patsubst %.o,%,$$t);done

$(Objs) : %.o : %.c %.h
	cc -c $< $(CFLAGS) -o test/$@

$(Test_objs) : %.o : test/%.c
	cc -c $< $(CFLAGS) -o test/$@

clean:
	rm $(Objs)
	rm $(Targets)

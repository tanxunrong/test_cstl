Common_cflags= -Wall -g -std=c99
Glib_cflags := $(shell pkg-config --cflags glib-2.0)
Event_cflags := $(shell pkg-config --cflags libevent)
CFLAGS=$(Common_cflags) $(Glib_cflags) $(Event_cflags) 

Objs = list.o rbtree.o hashmap.o queue.o vector.o

Test_objs = $(patsubst %,test_%,$(Objs)) 

Targets = $(patsubst %.o,test_%,$(Objs))

install : $(Objs) $(Test_objs)
	#for t in $(Test_objs) ; do obj=`echo $$t | sed -e 's/^test_//g'`; tar=`echo $$t | sed -e 's/\.o//g'`; echo $$obj ; echo $$tar ; cc $$t $$obj -o $$tar ; done
	cd test/ ; \
	cc test_list.o list.o -o test_list ; \
	cc test_hashmap.o hashmap.o vector.o -o test_hashmap ; \
	cc test_vector.o vector.o -lcunit -o test_vector ; \
	cc test_rbtree.o rbtree.o -o test_rbtree ; \
	cc test_queue.o queue.o -o test_queue ; 

$(Objs) : %.o : %.c %.h
	cc -c $< $(CFLAGS) -o test/$@

$(Test_objs) : %.o : test/%.c
	cc -c $< $(CFLAGS) -o test/$@

hashmap.o : vector.h vector.c

clean:
	for i in $(Objs);do rm test/$$i ;done
	for i in $(Test_objs);do rm test/$$i ;done
	for i in $(Targets);do rm test/$$i ;done

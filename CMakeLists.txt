project(test-cstl)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

add_definitions( -Wall -std=gnu99 -g )

include_directories( container/ lock/ /home/tanxr/local/include /home/tanxr/local/include/glib-2.0)

add_executable(testvec test/test_vector.c container/vector.c container/vector.h)
add_executable(testhash test/test_hashmap.c container/hashmap.c container/hashmap.h container/vector.c container/vector.h)
add_executable(testlist test/test_list.c container/list.c container/list.h)
add_executable(testbklock test/test_bakery_lock.c lock/bakery_lock.c lock/bakery_lock.h)
add_executable(testflblock test/test_filter_lock.c lock/filter_lock.c lock/filter_lock.h)

target_link_libraries(testbklock pthread)
target_link_libraries(testflblock pthread)


add_executable(mark_sweep gc/gc_mark_sweep.c)
add_executable(tri_color gc/gc_tri_color.c)

link_directories(/home/tanxr/local/lib)

#add_executable(testuv event/test_uv.c event/test_uv.h)
#target_link_libraries(testuv uv.a pthread glib-2.0)

#add_executable(testuvclient event/client.c)
#target_link_libraries(testuvclient uv.a glib-2.0 pthread)

add_executable(testleveldb leveldb/test.c)
target_link_libraries(testleveldb leveldb)

#add_executable(testrocksdb rocksdb/test.c)
#target_link_libraries(testrocksdb rocksdb)

add_library(mrb STATIC mrb/lib.c)
add_library(mruby-local STATIC IMPORTED)
set_target_properties(mruby-local PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)
target_include_directories(mrb SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/mruby/include)
target_link_libraries(mrb mruby-local m)

add_executable(mrb_test mrb/main.c)
target_link_libraries(mrb_test mrb)

aux_source_directory(${PROJECT_SOURCE_DIR}/duktape/src-separate DUKTAPE_SRC)
add_executable(duk ${DUKTAPE_SRC} duk/main.c)
target_include_directories(duk SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/duktape/src)
target_link_libraries(duk m)

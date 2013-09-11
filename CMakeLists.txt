project(test-cstl)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

add_definitions( -std=c99 -g )
#message(STATUS "${DEFINITIONS}")

add_executable(clist ./test/clist.c)
add_executable(testvec test/test_vec.c vector.c vector.h)
add_executable(testhash test/test_hash.c hashmap.c hashmap.h vector.c vector.h)
add_executable(testlist test/test_list.c list.c list.h)
if(UNIX)
    if(APPLE)
        include_directories(${INCLUDE_DIRECTORIES}
                                /usr/include
                                /usr/local/include)
        link_directories(${LINK_DIRECTORIES} /lib/x86_64-linux-gnu /usr/local/lib)
    else()
        include_directories(${INCLUDE_DIRECTORIES}
                                /usr/include
                                /usr/local/include)
        link_directories(${LINK_DIRECTORIES} /lib/x86_64-linux-gnu /usr/local/lib)
        target_link_libraries(clist cstl)
        target_link_libraries(testvec cunit)
    endif()
endif()

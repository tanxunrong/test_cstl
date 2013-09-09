project(test-cstl)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

add_executable(clist ./test/clist.c)
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
    endif()
endif()

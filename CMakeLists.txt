cmake_minimum_required(VERSION 3.20)
project(HashTableApp C)

set(SOURCES
    main.c
    src/hash_table.c
)

include_directories(src)

add_executable(HashTableApp ${SOURCES})

target_link_libraries(HashTableApp m)
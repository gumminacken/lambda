project(lambda)
cmake_minimum_required(VERSION 3.17)

cmake_policy(SET CMP0015 NEW)

string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

set(EMBREE_VERSION 3.9.0)
set(EMBREE_FLAVOUR ${EMBREE_VERSION}.x86_64.${PLATFORM})

include_directories(
    ./thirdparty/stb
    ./thirdparty/embree-${EMBREE_FLAVOUR}/include/embree3
)

link_directories(
    ./thirdparty/embree-${EMBREE_FLAVOUR}/lib
)

link_libraries(
    embree3
)

add_executable(lambda src/main.cpp)


project(lambda)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)
cmake_policy(SET CMP0015 NEW)

string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

set(EMBREE_VERSION 3.9.0)
set(EMBREE_FLAVOUR ${EMBREE_VERSION}.x86_64.${PLATFORM})

include_directories(
    ./thirdparty/stb
    ./thirdparty/embree-${EMBREE_FLAVOUR}/include/embree3
    ./thirdparty/sdl2-2.12/include
)

link_directories(
    ./thirdparty/embree-${EMBREE_FLAVOUR}/lib
    ./thirdparty/sdl2-2.12/lib/x64
)

link_libraries(
    embree3
    SDL2
    SDL2main
)

add_executable(lambda
  src/main.cpp
  src/math.cpp
  src/sampler.cpp
  src/scene.cpp
  )

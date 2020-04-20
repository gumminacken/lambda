
string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

include_directories(
    thirdparty/stb
    thirdparty/embree_${PLATFORM}/include/embree3
)

link_directories(
    ../thirdparty/embree_${PLATFORM}/lib
    ../thirdparty/embree_${PLATFORM}/bin
)

link_libraries(
    embree3
)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat")
    # execute_process(COMMAND ${vcvarsall} x64)
    message("Test")
endif()

add_executable(lambda src/main.cpp)



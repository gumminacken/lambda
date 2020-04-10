#!/bin/env bash

clang main.cpp \
    -std=c++14 \
    -lm -lstdc++ \
    -I../thirdparty/arnold_linux/include \
    -I../thirdparty/stb \
    -L../thirdparty/arnold_linux/bin -lai \
    -o ../build/lambda

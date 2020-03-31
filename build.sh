#!/bin/env bash

clang main.cpp \
    -std=c++14 \
    -lm -lstdc++ \
    -I../thirdparty/Arnold/include \
    -L../thirdparty/Arnold/bin -lai \
    -o ../build/lambda

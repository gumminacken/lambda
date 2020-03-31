#!/bin/env bash

clang main.cpp \
    -std=c++14 \
    -lm -lstdc++ \
    -I../../thirdparty/Arnold-6.0.2.0-linux/include \
    -L../../thirdparty/Arnold-6.0.2.0-linux/bin -lai \
    -o ../build/lambda

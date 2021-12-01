#!/bin/bash

cmake -Scalculator -Bbuild/calculator && cmake --build build/calculator -j8 && ./build/calculator/main

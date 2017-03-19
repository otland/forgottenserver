#!/bin/bash

cpp_files=$(find src -type f -iname "*.cpp")
h_files=$(find src -type f -iname "*.h")
diff -u <(clang-format-3.8 -style=file $cpp_files) <(cat $cpp_files) && \
diff -u <(clang-format-3.8 -style=file $h_files) <(cat $h_files)

if [ $? -ne 0 ]; then
    echo -e "\e[31mFormatting errors detected !\e[0m"
    exit -1
else
    exit 0
fi

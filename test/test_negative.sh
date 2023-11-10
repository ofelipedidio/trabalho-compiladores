#!/bin/bash

for file in negative/*; do 
    echo -n "Testing '$file' "
    ./program < $file > /dev/null 2> /dev/null;
    if [[ $? -ne 0 ]]; then
        echo -e "\r(\033[32mSUCC\033[0m) Testing '$file' "
    else
        echo -e "\r(\033[31mFAIL\033[0m) Testing '$file' "
    fi
done
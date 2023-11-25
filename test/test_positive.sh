#!/bin/bash

for file in positive/*; do 
    echo -n "Testing '$file' "
    ./program < $file > /dev/null;
    if [[ $? -eq 0 ]]; then
        echo -e "\r(\033[32mSUCC\033[0m) Testing '$file' "
    else
        echo -e "\r(\033[31mFAIL\033[0m) Testing '$file' "
    fi
done

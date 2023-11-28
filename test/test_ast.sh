#!/bin/bash

cp ../etapa3 ./program

for file in positive/*; do 
    echo "[$file]"
    cat $file
    echo ""
    echo "--------------------"
    echo ""
    ./program < $file
    read -r anyvar
done

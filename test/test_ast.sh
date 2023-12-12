#!/bin/bash

pushd ..
make
popd

if [[ $# -eq 1 ]]; then
    SKIP=$1
else
    SKIP=0
fi

for file in e3_tests/*; do
    if [[ $(echo "$file" | grep -c "\.") -eq 0 ]]; then
        if [[ $SKIP -gt 0 ]]; then
            SKIP=$(( $SKIP - 1 ))
        else
            echo "--- running ---"
            ../etapa4 < $file > out.txt;
            echo "Return code: " $?
            bat out.txt
            bat $file;
            if [[ $? -eq 0 ]]; then
                echo "converting"
                ../output2dot.sh < out.txt > out.dot;
                echo "displaying"
                xdot out.dot & xdot $file.ref.dot & wait;
            else
                echo "syntax error"
                read -r asd
            fi
            rm out.txt out.dot
        fi
    fi
done

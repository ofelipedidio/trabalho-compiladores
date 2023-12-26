#!/bin/bash

for file in test1/*; do echo -n "$file "; ./etapa5 < $file > /dev/null 2> /dev/null; echo "$?"; done

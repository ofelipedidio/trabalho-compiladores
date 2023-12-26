#!/bin/bash

for file in test1/*; do echo -e "[\033[32m$file\033[0m]"; ./etapa5 < $file; done

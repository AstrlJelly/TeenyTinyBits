#!/bin/bash

if [[ ! ( "./build.sh" ) ]]; then
  printf "\n"
  ./build/TeenyTinyBits
fi

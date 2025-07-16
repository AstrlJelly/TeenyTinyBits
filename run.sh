#!/bin/bash

if ./build.sh ; then
  printf "\n"
  cd ./build/
  ./TeenyTinyBits
fi

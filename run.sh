#!/bin/bash

usage="Usage: $PROGNAME [-v] [-d <dir>] [-f <file>]

 -nb --nobuild: Does not build new program for the session."

if [ "$1" != "--nobuild" -a "$1" != "-nb" ]; then
	./build.sh
	echo
	echo "Finished build script."
	echo
fi

./build/TeenyTinyBits

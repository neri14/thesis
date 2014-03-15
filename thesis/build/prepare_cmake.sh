#!/bin/bash

echo $1

cd ./bin

if [ $1 -eq 1 ]; then
	cmake ../build -DCMAKE_BUILD_TYPE=Debug
else
	cmake ../build
fi

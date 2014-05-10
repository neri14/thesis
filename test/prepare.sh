#!/bin/bash

rm -rf bin/*
rm -f ./simulator/*.xml
rm -f ./controller/*.xml

cp ../thesis/simulator/etc/*.xml ./simulator/
cp ../thesis/controller/etc/*.xml ./controller/

cd ../thesis
make dispatcher_server simulator controller

cp ./bin/dispatcher_server ../test/bin/
cp ./bin/simulator ../test/bin/
cp ./bin/controller ../test/bin/
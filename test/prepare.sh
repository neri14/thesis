#!/bin/bash

./clean.sh

cp ../thesis/simulator/etc/*.xml ./simulator/
cp ../thesis/controller/etc/*.xml ./controller/

cd ../thesis
make dispatcher_server simulator controller

cp ./bin/dispatcher_server ../test/bin/
cp ./bin/simulator ../test/bin/
cp ./bin/controller ../test/bin/
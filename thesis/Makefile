all: help

######

help:
	cat ./build/build_help.rst

target: prepare_cmake dispatcher_server

test: prepare_cmake

clean: clean_3rdparty
	rm -rf ./bin/*

######

prepare_cmake: prepare_3rdparty
	sh ./build/prepare_cmake.sh

prepare_3rdparty:
	make -C ../3rdparty

dispatcher_server:
	make -C ./bin dispatcher_server

clean_3rdparty:
	make -C ../3rdparty clean

.PHONY: all help target test clean prepare_cmake prepare_3rdparty dispatcher_server clean_3rdparty


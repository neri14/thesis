all: help

######

help:
	cat ./build/build_help.rst

target: prepare_cmake
	make -C ./bin controller
	make -C ./bin dispatcher_generic_client
	make -C ./bin dispatcher_server
	make -C ./bin manager
	make -C ./bin simulator
	make -C ./bin visualiser

test: prepare_cmake

clean: clean_bin clean_3rdparty

######

prepare_cmake: prepare_3rdparty
	sh ./build/prepare_cmake.sh

prepare_3rdparty:
	make -C ../3rdparty

clean_bin:
	rm -rf ./bin/*

clean_3rdparty:
	make -C ../3rdparty clean

.PHONY: all help target test clean prepare_cmake prepare_3rdparty clean_bin clean_3rdparty


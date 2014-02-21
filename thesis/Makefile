### MAIN TARGETS ###
all: help

help:
	less ./build/build_help.rst

clean: clean_bin clean_3rdparty

.PHONY: all help clean

### SYSTEM ###
SYSTEM_ = controller dispatcher_generic_client dispatcher_server manager simulator visualiser
system: $(SYSTEM_)

controller:
	make -C ./bin controller

dispatcher_generic_client:
	make -C ./bin dispatcher_generic_client

dispatcher_server:
	make -C ./bin dispatcher_server

manager:
	make -C ./bin manager

simulator:
	make -C ./bin simulator

visualiser:
	make -C ./bin visualiser

.PHONY: system $(SYSTEM_)

### TEST ###
TEST_ = common_test controller_test dispatcher_generic_client_test dispatcher_server_test manager_test simulator_test visualiser_test

test: $(TEST_)

gmock_main: prepare_cmake
	make -C ./bin gmock_main

common_test: gmock_main
	make -C ./bin common_test

controller_test: gmock_main
	make -C ./bin controller_test

dispatcher_generic_client_test: gmock_main
	make -C ./bin dispatcher_generic_client_test

dispatcher_server_test: gmock_main
	make -C ./bin dispatcher_server_test

manager_test: gmock_main
	make -C ./bin manager_test

simulator_test: gmock_main
	make -C ./bin simulator_test

visualiser_test: gmock_main
	make -C ./bin visualiser_test

.PHONY: test gmock_main $(TEST_)

### TEST RUN ###

TEST_RUN_ = common_test_run controller_test_run dispatcher_generic_client_test_run dispatcher_server_test_run manager_test_run simulator_test_run visualiser_test_run

test_run: $(TEST_RUN_)

common_test_run:
	./bin/common_test

controller_test_run:
	./bin/controller_test

dispatcher_generic_client_test_run:
	./bin/dispatcher_generic_client_test

dispatcher_server_test_run:
	./bin/dispatcher_server_test

manager_test_run:
	./bin/manager_test

simulator_test_run:
	./bin/simulator_test

visualiser_test_run:
	./bin/visualiser_test

.PHONY: test_run $(TEST_RUN_)

### HELPERS ###
prepare_cmake: prepare_3rdparty
	sh ./build/prepare_cmake.sh

prepare_3rdparty:
	make -C ../3rdparty
	mkdir -p ./bin/gmock

clean_bin:
	rm -rf ./bin/*

clean_3rdparty:
	make -C ../3rdparty clean

.PHONY: prepare_cmake prepare_3rdparty clean_bin clean_3rdparty

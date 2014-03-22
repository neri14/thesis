### MAIN TARGETS ###
all: help

help:
	less ./build/build_help.rst

clean: clean_bin clean_3rdparty

proto:
	protoc --proto_path=./common/proto --cpp_out=./common/src/proto ./common/proto/*

proto_clean:
	rm -rf ./common/src/proto/*

.PHONY: all help clean proto proto_clean

### SYSTEM ###
SYSTEM_ = controller dispatcher_generic_client dispatcher_server manager simulator visualiser
system: $(SYSTEM_)

controller: prepare_cmake
	make -C ./bin controller

dispatcher_generic_client: prepare_cmake
	make -C ./bin dispatcher_generic_client

dispatcher_server: prepare_cmake
	make -C ./bin dispatcher_server

manager: prepare_cmake
	make -C ./bin manager

simulator: prepare_cmake
	make -C ./bin simulator

visualiser: prepare_cmake
	make -C ./bin visualiser

.PHONY: system $(SYSTEM_)

### SYSTEM RUN ###
SYSTEM_RUN_ = controller_run dispatcher_generic_client_run dispatcher_server_run manager_run simulator_run visualiser_run

controller_run:
	./bin/controller --config ./controller/etc/config.ini

dispatcher_generic_client_run:
	./bin/dispatcher_generic_client --config ./dispatcher_generic_client/etc/config.ini

dispatcher_server_run:
	./bin/dispatcher_server --config ./dispatcher_server/etc/config.ini

manager_run:
	./bin/manager --config ./manager/etc/config.ini

simulator_run:
	./bin/simulator --config ./simulator/etc/config.ini

visualiser_run:
	./bin/visualiser --config ./visualiser/etc/config.ini

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
GTEST_FLAGS_ = --gtest_output=xml:bin/

test_run: $(TEST_RUN_)

common_test_run:
	./bin/common_test $(GTEST_FLAGS_)

controller_test_run:
	./bin/controller_test $(GTEST_FLAGS_)

dispatcher_generic_client_test_run:
	./bin/dispatcher_generic_client_test $(GTEST_FLAGS_)

dispatcher_server_test_run:
	./bin/dispatcher_server_test $(GTEST_FLAGS_)

manager_test_run:
	./bin/manager_test $(GTEST_FLAGS_)

simulator_test_run:
	./bin/simulator_test $(GTEST_FLAGS_)

visualiser_test_run:
	./bin/visualiser_test $(GTEST_FLAGS_)

.PHONY: test_run $(TEST_RUN_)

### HELPERS ###
prepare_cmake: prepare_3rdparty
	sh ./build/prepare_cmake.sh ${debug}

prepare_3rdparty:
	make -C ../3rdparty
	mkdir -p ./bin/gmock

clean_bin:
	rm -rf ./bin/*

clean_3rdparty:
	make -C ../3rdparty clean

.PHONY: prepare_cmake prepare_3rdparty clean_bin clean_3rdparty

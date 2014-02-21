cmake_minimum_required(VERSION 2.8)

#BOOST
set(BOOST_ROOT ../../3rdparty/boost_1_55_0)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

### FILES ###

#COMMON CODE
file(GLOB_RECURSE SRC_COMMON ../common/src/*)
file(GLOB_RECURSE SRC_UT_COMMON ../common/testing/*)

#CONTROLLER
file(GLOB_RECURSE SRC_CONTROLLER ../controller/src/*)
file(GLOB_RECURSE SRC_MAIN_CONTROLLER ../controller/src_main/*)
file(GLOB_RECURSE SRC_UT_CONTROLLER ../controller/testing/*)

#DISPATCHER_GENERIC_CLIENT
file(GLOB_RECURSE SRC_DISPATCHER_GENERIC_CLIENT ../dispatcher_generic_client/src/*)
file(GLOB_RECURSE SRC_MAIN_DISPATCHER_GENERIC_CLIENT ../dispatcher_generic_client/src_main/*)
file(GLOB_RECURSE SRC_UT_DISPATCHER_GENERIC_CLIENT ../dispatcher_generic_client/testing/*)

#DISPATCHER_SERVER
file(GLOB_RECURSE SRC_DISPATCHER_SERVER ../dispatcher_server/src/*)
file(GLOB_RECURSE SRC_MAIN_DISPATCHER_SERVER ../dispatcher_server/src_main/*)
file(GLOB_RECURSE SRC_UT_DISPATCHER_SERVER ../dispatcher_server/testing/*)

#MANAGER
file(GLOB_RECURSE SRC_MANAGER ../manager/src/*)
file(GLOB_RECURSE SRC_MAIN_MANAGER ../manager/src_main/*)
file(GLOB_RECURSE SRC_UT_MANAGER ../manager/testing/*)

#SIMULATOR
file(GLOB_RECURSE SRC_SIMULATOR ../simulator/src/*)
file(GLOB_RECURSE SRC_MAIN_SIMULATOR ../simulator/src_main/*)
file(GLOB_RECURSE SRC_UT_SIMULATOR ../simulator/testing/*)

#VISUALISER
file(GLOB_RECURSE SRC_VISUALISER ../visualiser/src/*)
file(GLOB_RECURSE SRC_MAIN_VISUALISER ../visualiser/src_main/*)
file(GLOB_RECURSE SRC_UT_VISUALISER ../visualiser/testing/*)

### TARGET ###
include_directories(../common/src)

#CONTROLLER
add_executable(controller ${SRC_MAIN_CONTROLLER} ${SRC_CONTROLLER} ${SRC_COMMON})
target_link_libraries(controller ${Boost_LIBRARIES})

#DISPATCHER_GENERIC_CLIENT
add_executable(dispatcher_generic_client ${SRC_MAIN_DISPATCHER_GENERIC_CLIENT} ${SRC_DISPATCHER_GENERIC_CLIENT} ${SRC_COMMON})
target_link_libraries(dispatcher_generic_client ${Boost_LIBRARIES})

#DISPATCHER_SERVER
add_executable(dispatcher_server ${SRC_MAIN_DISPATCHER_SERVER} ${SRC_DISPATCHER_SERVER} ${SRC_COMMON})
target_link_libraries(dispatcher_server ${Boost_LIBRARIES})

#MANAGER
add_executable(manager ${SRC_MAIN_MANAGER} ${SRC_MANAGER} ${SRC_COMMON})
target_link_libraries(manager ${Boost_LIBRARIES})

#SIMULATOR
add_executable(simulator ${SRC_MAIN_SIMULATOR} ${SRC_SIMULATOR} ${SRC_COMMON})
target_link_libraries(simulator ${Boost_LIBRARIES})

#VISUALISER
add_executable(visualiser ${SRC_MAIN_VISUALISER} ${SRC_VISUALISER} ${SRC_COMMON})
target_link_libraries(visualiser ${Boost_LIBRARIES})

### TESTS ###
#GMOCK
set(GMOCK_ROOT_DIR ../../3rdparty/gmock-1.7.0)
set(GMOCK_BIN_DIR ../bin/gmock)
add_subdirectory(${GMOCK_ROOT_DIR} ${GMOCK_BIN_DIR})

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

#COMMON
add_executable(common_test ${SRC_COMMON} ${SRC_UT_COMMON})
target_link_libraries(common_test gtest gtest_main gmock gmock_main)

#CONTROLLER
add_executable(controller_test ${SRC_CONTROLLER} ${SRC_UT_CONTROLLER})
target_link_libraries(controller_test gtest gtest_main gmock gmock_main)

#DISPATCHER_GENERIC_CLIENT
add_executable(dispatcher_generic_client_test ${SRC_DISPATCHER_GENERIC_CLIENT} ${SRC_UT_DISPATCHER_GENERIC_CLIENT})
target_link_libraries(dispatcher_generic_client_test gtest gtest_main gmock gmock_main)

#DISPATCHER_SERVER
add_executable(dispatcher_server_test ${SRC_DISPATCHER_SERVER} ${SRC_UT_DISPATCHER_SERVER})
target_link_libraries(dispatcher_server_test gtest gtest_main gmock gmock_main)

#MANAGER
add_executable(manager_test ${SRC_MANAGER} ${SRC_UT_MANAGER})
target_link_libraries(manager_test gtest gtest_main gmock gmock_main)

#SIMULATOR
add_executable(simulator_test ${SRC_SIMULATOR} ${SRC_UT_SIMULATOR})
target_link_libraries(simulator_test gtest gtest_main gmock gmock_main)

#VISUALISER
add_executable(visualiser_test ${SRC_VISUALISER} ${SRC_UT_VISUALISER})
target_link_libraries(visualiser_test gtest gtest_main gmock gmock_main)

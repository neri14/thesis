cmake_minimum_required(VERSION 2.8)

#BOOST
set(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/boost_1_55_0)
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/proto)
include_directories(${BOOST_ROOT})
include_directories(${PROTO_DIR}/src)
link_directories(${BOOST_ROOT}/stage/lib)
link_directories(${PROTO_DIR})

set(BOOST_LIBS boost_system boost_thread boost_program_options)

set(MAIN_LIBS ${BOOST_LIBS} protobuf pthread)

### FILES ###

#COMMON CODE
file(GLOB_RECURSE SRC_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../common/src/*)
file(GLOB_RECURSE SRC_UT_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../common/testing/*)

#CONTROLLER
file(GLOB_RECURSE SRC_CONTROLLER ${CMAKE_CURRENT_SOURCE_DIR}/../controller/src/*)
file(GLOB_RECURSE SRC_MAIN_CONTROLLER ${CMAKE_CURRENT_SOURCE_DIR}/../controller/src_main/*)
file(GLOB_RECURSE SRC_UT_CONTROLLER ${CMAKE_CURRENT_SOURCE_DIR}/../controller/testing/*)

#DISPATCHER_GENERIC_CLIENT
file(GLOB_RECURSE SRC_DISPATCHER_GENERIC_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_generic_client/src/*)
file(GLOB_RECURSE SRC_MAIN_DISPATCHER_GENERIC_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_generic_client/src_main/*)
file(GLOB_RECURSE SRC_UT_DISPATCHER_GENERIC_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_generic_client/testing/*)

#DISPATCHER_SERVER
file(GLOB_RECURSE SRC_DISPATCHER_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_server/src/*)
file(GLOB_RECURSE SRC_MAIN_DISPATCHER_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_server/src_main/*)
file(GLOB_RECURSE SRC_UT_DISPATCHER_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher_server/testing/*)

#MANAGER
file(GLOB_RECURSE SRC_MANAGER ${CMAKE_CURRENT_SOURCE_DIR}/../manager/src/*)
file(GLOB_RECURSE SRC_MAIN_MANAGER ${CMAKE_CURRENT_SOURCE_DIR}/../manager/src_main/*)
file(GLOB_RECURSE SRC_UT_MANAGER ${CMAKE_CURRENT_SOURCE_DIR}/../manager/testing/*)

#SIMULATOR
file(GLOB_RECURSE SRC_SIMULATOR ${CMAKE_CURRENT_SOURCE_DIR}/../simulator/src/*)
file(GLOB_RECURSE SRC_MAIN_SIMULATOR ${CMAKE_CURRENT_SOURCE_DIR}/../simulator/src_main/*)
file(GLOB_RECURSE SRC_UT_SIMULATOR ${CMAKE_CURRENT_SOURCE_DIR}/../simulator/testing/*)

#VISUALISER
file(GLOB_RECURSE SRC_VISUALISER ${CMAKE_CURRENT_SOURCE_DIR}/../visualiser/src/*)
file(GLOB_RECURSE SRC_MAIN_VISUALISER ${CMAKE_CURRENT_SOURCE_DIR}/../visualiser/src_main/*)
file(GLOB_RECURSE SRC_UT_VISUALISER ${CMAKE_CURRENT_SOURCE_DIR}/../visualiser/testing/*)

### TARGET ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/src)

#CONTROLLER
add_executable(controller ${SRC_MAIN_CONTROLLER} ${SRC_CONTROLLER} ${SRC_COMMON})
target_link_libraries(controller ${MAIN_LIBS})

#DISPATCHER_GENERIC_CLIENT
add_executable(dispatcher_generic_client ${SRC_MAIN_DISPATCHER_GENERIC_CLIENT} ${SRC_DISPATCHER_GENERIC_CLIENT} ${SRC_COMMON})
target_link_libraries(dispatcher_generic_client ${MAIN_LIBS})

#DISPATCHER_SERVER
add_executable(dispatcher_server ${SRC_MAIN_DISPATCHER_SERVER} ${SRC_DISPATCHER_SERVER} ${SRC_COMMON})
target_link_libraries(dispatcher_server ${MAIN_LIBS})

#MANAGER
add_executable(manager ${SRC_MAIN_MANAGER} ${SRC_MANAGER} ${SRC_COMMON})
target_link_libraries(manager ${MAIN_LIBS})

#SIMULATOR
add_executable(simulator ${SRC_MAIN_SIMULATOR} ${SRC_SIMULATOR} ${SRC_COMMON})
target_link_libraries(simulator ${MAIN_LIBS})

#VISUALISER
add_executable(visualiser ${SRC_MAIN_VISUALISER} ${SRC_VISUALISER} ${SRC_COMMON})
target_link_libraries(visualiser ${MAIN_LIBS})

### TESTS ###
#GMOCK
set(GMOCK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/gmock-1.7.0)
set(GMOCK_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin/gmock)
add_subdirectory(${GMOCK_ROOT_DIR} ${GMOCK_BIN_DIR})

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

#COMMON
add_executable(common_test ${SRC_COMMON} ${SRC_UT_COMMON})
target_link_libraries(common_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#CONTROLLER
add_executable(controller_test ${SRC_CONTROLLER} ${SRC_COMMON} ${SRC_UT_CONTROLLER})
target_link_libraries(controller_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#DISPATCHER_GENERIC_CLIENT
add_executable(dispatcher_generic_client_test ${SRC_DISPATCHER_GENERIC_CLIENT} ${SRC_COMMON} ${SRC_UT_DISPATCHER_GENERIC_CLIENT})
target_link_libraries(dispatcher_generic_client_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#DISPATCHER_SERVER
add_executable(dispatcher_server_test ${SRC_DISPATCHER_SERVER} ${SRC_COMMON} ${SRC_UT_DISPATCHER_SERVER})
target_link_libraries(dispatcher_server_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#MANAGER
add_executable(manager_test ${SRC_MANAGER} ${SRC_COMMON} ${SRC_UT_MANAGER})
target_link_libraries(manager_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#SIMULATOR
add_executable(simulator_test ${SRC_SIMULATOR} ${SRC_COMMON} ${SRC_UT_SIMULATOR})
target_link_libraries(simulator_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

#VISUALISER
add_executable(visualiser_test ${SRC_VISUALISER} ${SRC_COMMON} ${SRC_UT_VISUALISER})
target_link_libraries(visualiser_test ${MAIN_LIBS} gtest gtest_main gmock gmock_main protobuf)

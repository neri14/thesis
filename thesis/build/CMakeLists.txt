cmake_minimum_required(VERSION 2.8)
set(BOOST_ROOT ../../3rdparty/boost_1_55_0)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

file(GLOB_RECURSE SRC_COMMON ../common/src/*)

file(GLOB_RECURSE SRC_CONTROLLER ../controller/src/*)
add_executable(controller ${SRC_CONTROLLER} ${SRC_COMMON})
target_link_libraries(controller ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC_DISPATCHER_GENERIC_CLIENT ../dispatcher_generic_client/src/*)
add_executable(dispatcher_generic_client ${SRC_DISPATCHER_GENERIC_CLIENT} ${SRC_COMMON})
target_link_libraries(dispatcher_generic_client ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC_DISPATCHER_SERVER ../dispatcher_server/src/*)
add_executable(dispatcher_server ${SRC_DISPATCHER_SERVER} ${SRC_COMMON})
target_link_libraries(dispatcher_server ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC_MANAGER ../manager/src/*)
add_executable(manager ${SRC_MANAGER} ${SRC_COMMON})
target_link_libraries(manager ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC_SIMULATOR ../simulator/src/*)
add_executable(simulator ${SRC_SIMULATOR} ${SRC_COMMON})
target_link_libraries(simulator ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC_VISUALISER ../visualiser/src/*)
add_executable(visualiser ${SRC_VISUALISER} ${SRC_COMMON})
target_link_libraries(visualiser ${Boost_LIBRARIES})

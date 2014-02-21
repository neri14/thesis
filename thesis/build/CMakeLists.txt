cmake_minimum_required(VERSION 2.8)
set(BOOST_ROOT ../../3rdparty/boost_1_55_0)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(dispatcher_server ../dispatcher_server/main.cpp)
target_link_libraries( dispatcher_server ${Boost_LIBRARIES} )

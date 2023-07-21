cmake_minimum_required(VERSION 3.12)
project(async_server)

set(SOURCE_FILES Server.cpp CLient.cpp)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
	add_executable(client Client.cpp)
	include_directories(${Boost_INCLUDE_DIR})
	target_link_libraries(client PRIVATE Boost::system)

	add_executable(server Server.cpp)
	include_directories(${Boost_INCLUDE_DIR})
	target_link_libraries(server PRIVATE Boost::system)
endif()

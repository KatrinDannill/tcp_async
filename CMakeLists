cmake_minimum_required(VERSION 3.8)
project(Server)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.82 COMPONENTS program_options)
 
set(SOURCE_FILES
        Server.cpp Server.exe)
 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Server${SOURCE_FILES})
    target_link_libraries(Server${Boost_LIBRARIES})
endif()

cmake_minimum_required(VERSION 3.14)
project(port_scheduling)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(entities)

add_executable(port_scheduling
        entities/cargo.cpp
        entities/cargo.hpp
        entities/port.cpp
        entities/port.hpp
        entities/ship.cpp
        entities/ship.hpp
        main.cpp
        )

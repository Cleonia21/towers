cmake_minimum_required(VERSION 3.23)
project(towers)

set(CMAKE_CXX_STANDARD 14)

add_executable(towers main.cpp Game.hpp Game.cpp Map.cpp Map.hpp map/Line.cpp map/Line.hpp map/Point.cpp map/Point.hpp)


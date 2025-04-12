cmake_minimum_required(VERSION 3.27)
project(simulation_game_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(simulation_game_cpp main.cpp
        Person.cpp
        NameGenerator.cpp)

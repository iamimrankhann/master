cmake_minimum_required(VERSION 3.10)
project(LAMP_LIFE_PREDICTION)

set(CMAKE_CXX_STANDARD 11)

add_executable(lamp_life_calculator LAMP.cpp)

enable_testing()
add_test(NAME LampLifeTest COMMAND lamp_life_calculator)

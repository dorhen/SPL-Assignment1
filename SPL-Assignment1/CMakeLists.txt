cmake_minimum_required(VERSION 3.12)
project(SPL_Assignment1)

set(CMAKE_CXX_STANDARD 11)

add_executable(SPL_Assignment1 main.cpp src/Table.cpp include/Table.h src/Restaurant.cpp include/Restaurant.h src/Dish.cpp include/Dish.h src/Customer.cpp include/Customer.h src/Action.cpp include/Action.h)
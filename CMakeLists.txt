cmake_minimum_required(VERSION 2.8.12)

project(roulette)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#find_package(Qt5Core)

#add_executable(${PROJECT_NAME} "main.cpp")
#target_link_libraries(${PROJECT_NAME})

add_executable(${PROJECT_NAME}2 "main2.cpp")
target_link_libraries(${PROJECT_NAME}2)

add_executable(test2 "main_test.cpp")
target_link_libraries(test2)

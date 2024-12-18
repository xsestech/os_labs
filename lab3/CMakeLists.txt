cmake_minimum_required(VERSION 3.28)
project(os_labs C)


set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")
add_subdirectory(libconfig)
add_subdirectory(libio)
add_subdirectory(parent)
add_subdirectory(child)
target_link_libraries(parent libconfig libio)
target_link_libraries(child libconfig libio)
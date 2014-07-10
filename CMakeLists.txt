project(Adwaita)
cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_AUTOMOC ON)
add_definitions(-std=c++11)

find_package(KDE4 REQUIRED)

add_subdirectory(src)
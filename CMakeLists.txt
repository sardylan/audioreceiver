cmake_minimum_required(VERSION 3.16)

project(audiorecevier)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)

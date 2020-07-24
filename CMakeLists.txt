cmake_minimum_required(VERSION 3.16)

project(audioreceiver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (APPLICATION_NAME "Audio Receiver")
set (APPLICATION_VERSION "0.9.3")

set (ORGANIZATION_NAME "The HellNet.org")
set (ORGANIZATION_DOMAIN "thehellnet.org")

enable_testing()

add_subdirectory(src)

cmake_minimum_required(VERSION 3.16)

project(audioreceiver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(ORGANIZATION_NAME "The HellNet.org")
set(ORGANIZATION_DOMAIN "thehellnet.org")

set(APPLICATION_NAME "Audio Receiver")
set(APPLICATION_VERSION "0.9.6")

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(APPLICATION_BUILD_ARCH "64bit")
else ()
    set(APPLICATION_BUILD_ARCH "32bit")
endif ()

set(APPLICATION_BUILD "${CMAKE_GENERATOR} ${APPLICATION_BUILD_ARCH}")

enable_testing()

add_subdirectory(src)

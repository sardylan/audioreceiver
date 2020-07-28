cmake_minimum_required(VERSION 3.16)
project(audioreceiver)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(ORGANIZATION_NAME "The HellNet.org")
set(ORGANIZATION_DOMAIN "thehellnet.org")

set(APPLICATION_NAME "Audio Receiver")
set(APPLICATION_DESCRIPTION "Simple VLF receiver using PC soundcard with direct sampling")

set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "9")
set(APPLICATION_VERSION_PATCH "7")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(APPLICATION_BUILD_ARCH "64bit")
else ()
    set(APPLICATION_BUILD_ARCH "32bit")
endif ()

set(APPLICATION_BUILD "${CMAKE_GENERATOR} ${APPLICATION_BUILD_ARCH}")

if (WIN32 OR APPLE)
    if (WIN32)
        find_program(QT5_DEPLOYQT_BIN windeployqt)
    elseif (APPLE)
        find_program(QT5_DEPLOYQT_BIN macdeployqt)
    endif ()

    if (NOT QT5_DEPLOYQT_BIN)
        message(FATAL_ERROR "No Qt Deploy command found")
    endif ()
endif ()

if (UNIX)
    include(GNUInstallDirs)
endif ()

enable_testing()

add_subdirectory(src)

include(cmake/packaging.cmake)

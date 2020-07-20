find_package(Qt5 REQUIRED
        COMPONENTS Core Gui Widgets)

find_package(OpenGL REQUIRED
        COMPONENTS OpenGL)

add_library(widgets
        vumeter.cpp vumeter.hpp)

target_link_libraries(widgets
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        OpenGL::OpenGL)

target_compile_options(widgets
        PRIVATE
        -Wall)

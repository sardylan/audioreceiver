find_package(Qt5 REQUIRED
        COMPONENTS Core Gui Widgets)

add_library(windows
        main.cpp main.hpp main.ui
        config.cpp config.hpp config.ui)

target_link_libraries(windows
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        widgets)

target_compile_options(windows
        PRIVATE
        -Wall)

target_include_directories(windows
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

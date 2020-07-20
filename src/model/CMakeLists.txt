find_package(Qt5 REQUIRED
        COMPONENTS Core)

add_library(model
        frame.cpp frame.hpp)

target_link_libraries(model
        Qt5::Core)

target_compile_options(model
        PRIVATE
        -Wall)

target_include_directories(model
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

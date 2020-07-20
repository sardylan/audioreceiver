find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia)

add_library(audio
        source.cpp source.hpp
        destination.cpp destination.hpp)

target_link_libraries(audio
        Qt5::Core
        Qt5::Multimedia
        utilities
        dsp)

target_compile_options(audio
        PRIVATE
        -Wall)

target_include_directories(audio
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
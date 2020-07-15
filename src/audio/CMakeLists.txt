find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia)

add_library(audio
        source.cpp source.hpp
        destination.cpp destination.hpp)

target_link_libraries(audio
        Qt5::Core
        Qt5::Multimedia
        utilities)

target_compile_options(audio
        PRIVATE
        -Wall)
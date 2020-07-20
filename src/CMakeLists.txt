add_subdirectory(utilities)
add_subdirectory(model)
add_subdirectory(dsp)
add_subdirectory(audio)
add_subdirectory(widgets)
add_subdirectory(windows)

find_package(Qt5 REQUIRED
        COMPONENTS Core Concurrent)

add_executable(audiorecevier
        audioreceiver.cpp audioreceiver.hpp
        config.cpp config.hpp
        default.hpp)

target_link_libraries(audiorecevier
        Qt5::Core
        Qt5::Concurrent
        utilities
        model
        dsp
        audio
        windows)

target_compile_options(audiorecevier
        PRIVATE
        -Wall)
add_subdirectory(utilities)
add_subdirectory(model)
add_subdirectory(dsp)
add_subdirectory(audio)

find_package(Qt5 REQUIRED
        COMPONENTS Core)

add_executable(audiorecevier
        audioreceiver.cpp audioreceiver.hpp)

target_link_libraries(audiorecevier
        Qt5::Core
        utilities
        model
        dsp
        audio)

target_compile_options(audiorecevier
        PRIVATE
        -Wall)
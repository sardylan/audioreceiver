add_subdirectory(utilities)
add_subdirectory(model)
add_subdirectory(dsp)
add_subdirectory(audio)
add_subdirectory(widgets)
add_subdirectory(windows)

find_package(Qt5 REQUIRED
        COMPONENTS Core Concurrent)

configure_file(version.hpp.in version.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(audioreceiver
        audioreceiver.cpp audioreceiver.hpp
        worker.cpp worker.hpp
        config.cpp config.hpp
        status.cpp status.hpp
        default.hpp)

target_link_libraries(audioreceiver
        Qt5::Core
        Qt5::Concurrent
        utilities
        model
        dsp
        audio
        windows)

target_compile_options(audioreceiver
        PRIVATE
        -Wall)
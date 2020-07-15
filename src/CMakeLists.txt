add_subdirectory(utilities)
add_subdirectory(dsp)
add_subdirectory(audio)

find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia)

find_package(FFTW REQUIRED
        COMPONENTS DOUBLE_LIB DOUBLE_THREADS_LIB)

add_executable(audiorecevier
        model/frame.cpp model/frame.hpp
        audioreceiver.cpp audioreceiver.hpp)

target_link_libraries(audiorecevier
        Qt5::Core
        Qt5::Multimedia
        FFTW::Double
        FFTW::DoubleThreads
        utilities
        dsp
        audio)

target_compile_options(audiorecevier
        PRIVATE
        -Wall)
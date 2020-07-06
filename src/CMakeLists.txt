find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia)

find_package(FFTW REQUIRED
        COMPONENTS DOUBLE_LIB DOUBLE_THREADS_LIB)

add_executable(audiorecevier
        service.cpp service.hpp
        audio/source.cpp audio/source.hpp
        audio/destination.cpp audio/destination.hpp
        dsp/utility.cpp dsp/utility.hpp
        dsp/bfo.cpp dsp/bfo.hpp
        dsp/fft.cpp dsp/fft.hpp
        audioreceiver.cpp audioreceiver.hpp)

target_link_libraries(audiorecevier
        Qt5::Core
        Qt5::Multimedia
        FFTW::Double
        FFTW::DoubleThreads)

message(${FFTW_LIBRARIES})
target_compile_options(audiorecevier
        PRIVATE
        -Wall)
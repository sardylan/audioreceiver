find_package(Qt5 REQUIRED
        COMPONENTS Core)

find_package(FFTW REQUIRED
        COMPONENTS DOUBLE_LIB DOUBLE_THREADS_LIB)

add_library(dsp
        bfo.cpp bfo.hpp
        fft.cpp fft.hpp
        fir.cpp fir.hpp
        utility.cpp utility.hpp)

target_link_libraries(dsp
        Qt5::Core
        FFTW::Double
        FFTW::DoubleThreads)

target_compile_options(dsp
        PRIVATE
        -Wall)
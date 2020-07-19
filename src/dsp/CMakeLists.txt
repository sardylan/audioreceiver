find_package(Qt5 REQUIRED
        COMPONENTS Core)

find_package(FFTW3 REQUIRED
        COMPONENTS fftw3)

add_library(dsp
        bfo.cpp bfo.hpp
        fft.cpp fft.hpp
        fir.cpp fir.hpp
        utility.cpp utility.hpp)

target_link_libraries(dsp
        Qt5::Core
        FFTW3::fftw3)

target_compile_options(dsp
        PRIVATE
        -Wall)
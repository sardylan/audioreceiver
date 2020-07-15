find_package(Qt5 REQUIRED
        COMPONENTS Core)

add_library(dsp
        bfo.cpp bfo.hpp
        fft.cpp fft.hpp
        rms.cpp rms.hpp
        utility.cpp utility.hpp)

target_link_libraries(dsp
        Qt5::Core
        utilities)

target_compile_options(dsp
        PRIVATE
        -Wall)
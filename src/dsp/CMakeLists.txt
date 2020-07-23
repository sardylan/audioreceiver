find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia Test)

find_package(FFTW3 REQUIRED
        COMPONENTS fftw3)

add_library(dsp
        bfo.cpp bfo.hpp
        fft.cpp fft.hpp
        fir.cpp fir.hpp
        utility.cpp utility.hpp)

target_link_libraries(dsp
        Qt5::Core
        Qt5::Multimedia
        FFTW3::fftw3)

target_compile_options(dsp
        PRIVATE
        -Wall)

target_include_directories(dsp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_dsp_utility
        test/utilitytest.cpp
        test/utilitytest.hpp)

target_link_libraries(test_dsp_utility
        Qt5::Core
        Qt5::Test
        dsp)

add_test(TestDspUtility
        test_dsp_utility -o testresult-dsp-utility.xml,xunitxml)

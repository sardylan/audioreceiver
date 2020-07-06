find_package(Qt5 REQUIRED
        COMPONENTS Core Multimedia)

add_executable(audiorecevier
        service.cpp service.hpp
        audio_source.cpp audio_source.hpp
        audio_destination.cpp audio_destination.hpp
        dsp.cpp dsp.hpp
        bfo.cpp bfo.hpp
        audioreceiver.cpp audioreceiver.hpp)

target_link_libraries(audiorecevier
        Qt5::Core
        Qt5::Multimedia)


target_compile_options(audiorecevier
        PRIVATE
        -Wall)
find_package(Qt5 REQUIRED
        COMPONENTS Core)

add_library(utilities
        service.cpp service.hpp)

target_link_libraries(utilities
        Qt5::Core)

target_compile_options(utilities
        PRIVATE
        -Wall)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

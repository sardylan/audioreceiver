find_package(OpenGL
        REQUIRED
        COMPONENTS
        OpenGL GLX)

find_package(Qt5
        REQUIRED
        COMPONENTS Core Gui Svg Widgets OpenGL Test)

set(CUSTOMWIDGETS_HEADERS_PUBLIC
        src/vumeter.hpp
        src/waterfall.hpp)

set(CUSTOMWIDGETS_HEADERS_PRIVATE
        src/utility.hpp
        src/waterfall_color_table.hpp)

set(CUSTOMWIDGETS_SOURCES
        src/vumeter.cpp
        src/waterfall.cpp
        src/utility.cpp)

add_library(customwidgets
        ${CUSTOMWIDGETS_SOURCES}
        ${CUSTOMWIDGETS_HEADERS_PUBLIC}
        ${CUSTOMWIDGETS_HEADERS_PRIVATE})

target_link_libraries(customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL::OpenGL
        OpenGL::GLX)

set_target_properties(customwidgets
        PROPERTIES
        PUBLIC_HEADER "${CUSTOMWIDGETS_HEADERS_PUBLIC}")

add_executable(testutility
        test/utilitytest.cpp
        test/utilitytest.hpp)

target_link_libraries(testutility
        Qt5::Core
        Qt5::Test
        customwidgets)

add_test(TestUtility
        testutility -o testresult-testutility.xml,xunitxml)

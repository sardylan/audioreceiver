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

set_source_files_properties(../icons/application/macos/icon.icns
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)

set(MACOSX_BUNDLE_EXECUTABLE_NAME audioreceiver)
set(MACOSX_BUNDLE_BUNDLE_NAME AudioReceiver)
set(MACOSX_BUNDLE_ICON_FILE icon.icns)

add_executable(audioreceiver
        WIN32 MACOSX_BUNDLE
        audioreceiver.cpp audioreceiver.hpp
        worker.cpp worker.hpp
        config.cpp config.hpp
        default.hpp
        audioreceiver.rc
        ../icons/application/macos/icon.icns)

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

if (WIN32 OR APPLE)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QT5_DEPLOYQT_TYPE "--debug")
    else ()
        set(QT5_DEPLOYQT_TYPE "--release")
    endif ()
endif ()

if (WIN32)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)
    add_custom_command(TARGET audioreceiver
            POST_BUILD COMMAND
            ${QT5_DEPLOYQT_BIN} $<TARGET_FILE:audioreceiver> ${QT5_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)
elseif (APPLE)
    set_target_properties(audioreceiver
            PROPERTIES
            MACOSX_BUNDLE_ICON_FILE ${AUDIORECEIVER_MAC_ICON_NAME})
    add_custom_command(TARGET audioreceiver
            POST_BUILD COMMAND
            ${QT5_DEPLOYQT_BIN} $<TARGET_FILE:audioreceiver> ${QT5_DEPLOYQT_TYPE})
endif ()

install(TARGETS audioreceiver
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        COMPONENT audioreceiver)

if (WIN32 OR APPLE)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT qtlibs)
endif ()

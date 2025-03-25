cmake_minimum_required(VERSION 3.19)
project(simdez LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick Quick3D Gui)

qt_standard_project_setup()

qt_add_executable(simdez
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    satellite.h satellite.cpp
    main.qml Sat.qml
)

target_link_libraries(simdez
    PRIVATE
        Qt6::Core
        Qt::Widgets
        Qt6::Quick
        Qt6::Quick3D
        Qt6::Gui
)

include(GNUInstallDirs)

install(TARGETS simdez
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET simdez
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

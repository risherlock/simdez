cmake_minimum_required(VERSION 3.19)
project(simdez LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick Quick3D Gui Concurrent PrintSupport)

qt_standard_project_setup()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(simdez
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    satellite.h satellite.cpp
    main.qml Sat.qml

    time.cpp time.h
    igrf.cpp igrf.h
    dcm.cpp dcm.h
    frame.cpp frame.h
    quat.cpp quat.h
    euler.h
    maps.cpp maps.h
    iau06.cpp iau06.h
    iau06_data.cpp iau06_data.h
    attitude.h
    SGP4.cpp SGP4.h
    TLE.cpp TLE.h
    qcustomplot.h qcustomplot.cpp
)

target_link_libraries(simdez
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Quick
        Qt6::Quick3D
        Qt6::Gui
        Qt6::PrintSupport
        Qt6::Concurrent
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
# include_directories(${CMAKE_SOURCE_DIR})

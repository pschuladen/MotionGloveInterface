cmake_minimum_required(VERSION 3.16)

project(MotionGloveInterface VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Qml REQUIRED)

qt_add_executable(appMotionGloveInterface
    main.cpp
    devicestatuscontroller.h
    devicestatuscontroller.cpp
    devicedatainput.h
    devicedatainput.cpp
    devicedata.h
    devicedata.cpp
    vectorviewbackend.h vectorviewbackend.cpp
    valuelistmodel.h valuelistmodel.cpp
    quatviewbackend.h quatviewbackend.cpp
    Singleton.h
    mainbackend.h mainbackend.cpp
)

qt_add_qml_module(appMotionGloveInterface
    URI MotionGloveInterface
    VERSION 1.0
    QML_FILES #${application_qml_files}
    main.qml
                DeviceStatusView.qml
                VectorView.qml
                ValueWithSliderView.qml
                QuatView.qml
                SensorInputContainer.qml
                FlexScrollViewLayout.qml

#    SOURCES
#        vectorviewbackend.cpp vectorviewbackend.h
)

set_target_properties(appMotionGloveInterface PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appMotionGloveInterface
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appMotionGloveInterface
    PRIVATE Qt6::Quick)

#target_include_directories(oscpp)
#include_directories(oscpp)
#target_sources(appMotionGloveInterface
#    PRIVATE oscpp/*
#    )

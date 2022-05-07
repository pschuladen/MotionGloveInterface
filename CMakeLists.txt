cmake_minimum_required(VERSION 3.16)

project(MotionGloveInterface VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) #added from me

find_package(Qt6 6.2 COMPONENTS Quick Qml REQUIRED)

qt_add_executable(appMotionGloveInterface
    main.cpp
#    devicestatuscontroller.h devicestatuscontroller.cpp
    devicestatusmanager.h devicestatusmanager.cpp
    devicedatainput.h devicedatainput.cpp
    valuelistmodel.h valuelistmodel.cpp
    Singleton.h
    mainbackend.h mainbackend.cpp
    valuenotifierclass.h valuenotifierclass.cpp
#    valueviewbackend.h valueviewbackend.cpp
    inputvalueviewcontroller.h inputvalueviewcontroller.cpp
    dataprocessingnode.h dataprocessingnode.cpp
    processnodecontroller.h processnodecontroller.cpp
    oscoutputdevice.h oscoutputdevice.cpp
    typehelper.h typehelper.cpp
    pn_scale.h pn_scale.cpp
    processnode.h processnode.cpp
)

qt_add_qml_module(appMotionGloveInterface
    URI MotionGloveInterface
    VERSION 1.0
    QML_FILES #${application_qml_files}
    main.qml
                ValueWithSliderView.qml
                SensorInputContainer.qml
                FlexScrollViewLayout.qml
                CreateProcessNodeWindow.qml
                SensorValuesView.qml
                Pn_ScaleView.qml
                TextNumberField.qml
                OscOutputView.qml
                OscOutputDeviceView.qml
                AttachedConnectorView.qml
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

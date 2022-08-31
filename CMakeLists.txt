cmake_minimum_required(VERSION 3.16)

project(MotionGloveInterface VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) #added from me

find_package(Qt6 6.2 COMPONENTS Quick Qml Xml REQUIRED)

set(CMAKE_AUTORCC ON)

qt_add_executable(appMotionGloveInterface
    main.cpp
    images.qrc
#    devicestatuscontroller.h devicestatuscontroller.cpp
    devicestatusmanager.h devicestatusmanager.cpp
#    devicedatainput.h devicedatainput.cpp
    oscinputdevice.h oscinputdevice.cpp
    valuelistmodel.h valuelistmodel.cpp
    Singleton.h
    mainbackend.h mainbackend.cpp
    valuenotifierclass.h valuenotifierclass.cpp
#    valueviewbackend.h valueviewbackend.cpp
    inputvalueviewcontroller.h inputvalueviewcontroller.cpp
#    dataprocessingnode.h dataprocessingnode.cpp
#    processnodecontroller.h processnodecontroller.cpp
    oscoutputdevice.h oscoutputdevice.cpp
    typehelper.h typehelper.cpp
    pn_scale.h pn_scale.cpp
    processnode.h processnode.cpp
    saveloadinterfaceclass.h saveloadinterfaceclass.cpp
#    oscviewcontroller.h oscviewcontroller.cpp
    oscoutputviewcontroller.h oscoutputviewcontroller.cpp
    oscpacketbuilder.h oscpacketbuilder.cpp
    oscinputparser.h oscinputparser.cpp
    oscinputviewcontroller.h oscinputviewcontroller.cpp
    outputnodecontroller.h outputnodecontroller.cpp
    inputnodecontroller.h inputnodecontroller.cpp
    connectionviewcontroller.h connectionviewcontroller.cpp
    nodeviewcontroller.h nodeviewcontroller.cpp
    projectfilemanager.h projectfilemanager.cpp
    pn_sumcomponents.h pn_sumcomponents.cpp
    pn_absvalue.h pn_absvalue.cpp
    pn_diffvalue.h pn_diffvalue.cpp
    pn_splitcomponents.h pn_splitcomponents.cpp
    pn_touchrecognizer.h pn_touchrecognizer.cpp
    pn_noteselector.h pn_noteselector.cpp

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
                OutputConnector.qml
                InputConnector.qml
                InputNodeView.qml
                OutputNodeView.qml
                ConnectionView.qml
                Pn_SumComponentsView.qml
                Pn_AbsValueView.qml
                Pn_DiffValueView.qml
                Pn_SplitComponentsView.qml
                Pn_TouchRecognizerView.qml
                Pn_NoteSelectorView.qml
)



set_target_properties(appMotionGloveInterface PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER xyz.psch.moglin
    MACOSX_BUNDLE_BUNDLE_NAME "MoGlIn"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
#    OUTPUT_NAME "your name"
)

target_compile_definitions(appMotionGloveInterface
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appMotionGloveInterface
    PRIVATE Qt6::Quick Qt6::Xml)

#target_include_directories(oscpp)
#include_directories(oscpp)
#target_sources(appMotionGloveInterface
#    PRIVATE oscpp/*
#    )

cmake_minimum_required(VERSION 3.16)
project(qmlmqttdemo LANGUAGES CXX)

if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "26.0" CACHE STRING "Minimum macOS version" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Mqtt Network)
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_standard_project_setup()

qt_add_executable(qmlmqttdemo
    src/main.cpp
    src/appcontroller.h
    src/appcontroller.cpp
    src/messagemodel.h
    src/messagemodel.cpp
    src/telemetryprocessor.h
    src/telemetryprocessor.cpp
)

qt_add_qml_module(qmlmqttdemo
    URI DemoApp
    VERSION 1.0
    QML_FILES
        src/qml/App.qml
    SOURCES
        src/appcontroller.h
        src/appcontroller.cpp
        src/messagemodel.h
        src/messagemodel.cpp
        src/telemetryprocessor.h
        src/telemetryprocessor.cpp
)

target_include_directories(qmlmqttdemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_source_files_properties(src/appcontroller.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)


target_link_libraries(qmlmqttdemo PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Mqtt
    Qt6::Network
)


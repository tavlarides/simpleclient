cmake_minimum_required(VERSION 3.16)
project(qmlmqttdemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Mqtt Network)

qt_add_executable(qmlmqttdemo
    src/main.cpp
    src/appcontroller.h
    src/appcontroller.cpp
    src/messagemodel.h
    src/messagemodel.cpp
    src/telemetryprocessor.h
    src/telemetryprocessor.cpp
    resources.qrc
)

target_link_libraries(qmlmqttdemo PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Mqtt
    Qt6::Network
)

set_target_properties(qmlmqttdemo PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE FALSE
)

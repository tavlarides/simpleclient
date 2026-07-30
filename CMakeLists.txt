cmake_minimum_required(VERSION 3.16)
project(mqtt_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "26.0" CACHE STRING "Minimum macOS version" FORCE)
endif()

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Mqtt
    Network
    Qml
    Quick
    QuickControls2
    Sql
    Test
)

qt_standard_project_setup(REQUIRES 6.8)
enable_testing()

add_subdirectory(qmlmqttdemo)
add_subdirectory(surveillance)

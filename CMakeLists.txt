cmake_minimum_required(VERSION 3.16)
project(mqtt_demo LANGUAGES CXX)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "26.0" CACHE STRING "Minimum macOS version" FORCE)
endif()

add_subdirectory(qmlmqttdemo)
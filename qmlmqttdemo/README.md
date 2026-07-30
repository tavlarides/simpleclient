# QML MQTT Telemetry Demo

This project is a Qt Quick/QML application that demonstrates a modern C++ backend connected to a QML frontend.

## What it demonstrates
- Modern C++ application structure
- Qt Quick/QML UI development
- C++ to QML integration through properties and models
- MQTT publish/subscribe interaction
- Event-driven state handling for interview discussion

## Build and run
From this directory:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/Users/tassos/Qt/6.10.2/macos
cmake --build build
./build/qmlmqttdemo
```

## Interview story
The app ingests MQTT messages in a C++ backend, exposes the state to QML, and renders a live list of telemetry payloads in the UI. This makes it a strong example of C++/QML integration, event-driven design, and asynchronous messaging.

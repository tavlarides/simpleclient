# QML MQTT Telemetry Demo

This project is a Qt Quick/QML application designed to showcase a modern C++ backend connected to a dynamic user interface. It was created as an interview-ready demonstration of C++/Qt/QML development, MQTT messaging, and event-driven architecture.

## What this project demonstrates

- Modern C++ application structure
- Qt Quick and QML UI development
- C++ to QML integration through `Q_PROPERTY`, models, and QObject-based services
- MQTT publish/subscribe interaction
- Event-driven state handling and responsive UI updates
- A practical example of bridging asynchronous networking with a real-time interface

## Project structure

- `src/appcontroller.cpp` / `src/appcontroller.h`
  - Main application controller that manages connection state, user actions, and backend updates
- `src/messagemodel.cpp` / `src/messagemodel.h`
  - Custom list model exposed to QML for rendering incoming messages
- `src/telemetryprocessor.cpp` / `src/telemetryprocessor.h`
  - Lightweight processing layer for incoming MQTT payloads
- `qml/main.qml`
  - QML UI for connection controls, status, publish/subscribe actions, and message history

## Build and run

From the project directory:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/Users/tassos/Qt/6.10.2/macos
cmake --build build
./build/qmlmqttdemo
```

## Interview talking points

This project is a good conversation starter for roles involving:

- C++ application development
- Qt and QML integration
- Messaging systems such as MQTT
- Event-driven architecture and state management
- Building a practical desktop application with a clear separation between backend logic and UI

## Example interview story

“I built a Qt Quick application that ingests MQTT messages in a C++ backend, exposes the resulting state to QML, and renders live telemetry updates in the UI. The project demonstrates modern C++ design, asynchronous message handling, and a clean C++/QML integration pattern that is relevant for real-world embedded and software engineering scenarios.”

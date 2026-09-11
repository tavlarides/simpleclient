qt_add_executable(operator-client
    src/main.cpp
    src/operatorcontroller.h
    src/operatorcontroller.cpp
    src/incidentsmodel.h
    src/incidentsmodel.cpp
    src/natssubscriber.h
    src/natssubscriber.cpp
)

qt_add_qml_module(operator-client
    URI Surveillance.Operator
    QML_FILES
        qml/Main.qml
        qml/Dashboard.qml
        qml/IncidentList.qml
    SOURCES
        src/operatorcontroller.h
        src/operatorcontroller.cpp
        src/incidentsmodel.h
        src/incidentsmodel.cpp
)

target_include_directories(operator-client
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(operator-client
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        surveillance_common
)

set_target_properties(operator-client PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

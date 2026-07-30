qt_add_executable(security-server
    src/main.cpp
    src/securityserver.h
    src/securityserver.cpp
    src/ingestion/mqttingestion.h
    src/ingestion/mqttingestion.cpp
    src/processing/eventprocessor.h
    src/processing/eventprocessor.cpp
    src/storage/datastore.h
    src/storage/datastore.cpp
    src/presentation/natspublisher.h
    src/presentation/natspublisher.cpp
)

target_include_directories(security-server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(security-server
    PRIVATE
        Qt6::Core
        Qt6::Mqtt
        Qt6::Network
        Qt6::Sql
        surveillance_common
)

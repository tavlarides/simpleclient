qt_add_executable(surveillance_processing_tests
    eventprocessortest.cpp
    ../server/src/processing/eventprocessor.h
    ../server/src/processing/eventprocessor.cpp
)

target_include_directories(surveillance_processing_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../server/src
)

target_link_libraries(surveillance_processing_tests
    PRIVATE
        Qt6::Core
        Qt6::Test
        surveillance_common
)

add_test(NAME surveillance_processing_tests COMMAND surveillance_processing_tests)

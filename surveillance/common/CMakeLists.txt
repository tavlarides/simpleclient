qt_add_library(surveillance_common STATIC
    include/surveillance/domain.h
    src/domain.cpp
)

target_include_directories(surveillance_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(surveillance_common
    PUBLIC
        Qt6::Core
)

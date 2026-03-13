add_library(DashboardModule SHARED
    DashboardModule.cpp
    resources.qrc
)

target_include_directories(DashboardModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DashboardModule PRIVATE QtPrismFramework Qt5::Core)

set_target_properties(DashboardModule PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    PREFIX ""
)

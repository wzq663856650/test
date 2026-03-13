add_library(SettingsModule SHARED
    SettingsModule.cpp
    resources.qrc
)

target_include_directories(SettingsModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SettingsModule PRIVATE QtPrismFramework Qt5::Core)

set_target_properties(SettingsModule PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    PREFIX ""
)

add_executable(AdminApp
    main.cpp
    App.cpp
    Shell/ShellController.cpp
    resources.qrc
)

target_include_directories(AdminApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AdminApp PRIVATE
    QtPrismFramework
    Qt5::Quick
    Qt5::QuickControls2
)

# Module plugins (each builds as a separate shared library)
add_subdirectory(Modules/DashboardModule)
add_subdirectory(Modules/OrderModule)
add_subdirectory(Modules/SettingsModule)

add_executable(AdminApp
    main.cpp
    App.cpp
    resources.qrc
    Modules/DashboardModule/DashboardModule.cpp
    Modules/OrderModule/OrderModule.cpp
    Modules/OrderModule/Services/OrderServiceImpl.cpp
    Modules/OrderModule/ViewModels/OrderListViewModel.cpp
    Modules/OrderModule/ViewModels/OrderDetailViewModel.cpp
    Modules/SettingsModule/SettingsModule.cpp
)

target_include_directories(AdminApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Modules/OrderModule
)

target_link_libraries(AdminApp PRIVATE
    QtPrismFramework
    Qt6::Quick
    Qt6::QuickControls2
)

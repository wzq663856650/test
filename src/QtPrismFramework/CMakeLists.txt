add_library(QtPrismFramework STATIC
    Modularity/ModuleCatalog.cpp
    Modularity/ModuleManager.cpp
    Regions/Region.cpp
    Regions/RegionManager.cpp
    Regions/RegionAdapterMappings.cpp
    Navigation/NavigationJournal.cpp
    Views/ViewRegistry.cpp
    QtPrismApplication.cpp
)

target_include_directories(QtPrismFramework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtPrismFramework PUBLIC Qt5::Core Qt5::Quick Qt5::Qml QtPrismCore)

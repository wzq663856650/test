add_library(QtPrismCore STATIC
    Navigation/NavigationContext.cpp
    Mvvm/ViewModelBase.cpp
    Regions/IRegion.cpp
    Regions/IRegionManager.cpp
    Modularity/IModuleManager.cpp
)

target_include_directories(QtPrismCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtPrismCore PUBLIC Qt5::Core QtPrismContainer QtPrismEvents)

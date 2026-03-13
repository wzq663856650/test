add_library(QtPrismContainer STATIC
    ContainerLocator.cpp
    QtPrismContainerImpl.cpp
)

target_include_directories(QtPrismContainer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtPrismContainer PUBLIC Qt5::Core)

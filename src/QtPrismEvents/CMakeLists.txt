add_library(QtPrismEvents STATIC
    EventAggregator.cpp
)

target_include_directories(QtPrismEvents PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtPrismEvents PUBLIC Qt5::Core Qt5::Concurrent QtPrismContainer)

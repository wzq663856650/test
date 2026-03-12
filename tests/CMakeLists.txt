find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(QtPrismTests
    main.cpp
    ContainerTests.cpp
    EventAggregatorTests.cpp
    ModuleCatalogTests.cpp
    RegionTests.cpp
    NavigationJournalTests.cpp
)

target_include_directories(QtPrismTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QtPrismTests PRIVATE
    QtPrismFramework
    Qt6::Test
    Qt6::Quick
)

add_test(NAME QtPrismTests COMMAND QtPrismTests)

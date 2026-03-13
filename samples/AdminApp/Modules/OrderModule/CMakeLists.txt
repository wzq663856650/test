add_library(OrderModule SHARED
    OrderModule.cpp
    Services/OrderServiceImpl.cpp
    ViewModels/OrderListViewModel.cpp
    ViewModels/OrderDetailViewModel.cpp
)

target_include_directories(OrderModule PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(OrderModule PRIVATE QtPrismFramework Qt5::Core Qt5::Quick)

set_target_properties(OrderModule PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    PREFIX ""
)

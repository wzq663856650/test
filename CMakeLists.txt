cmake_minimum_required(VERSION 3.21)
project(QtPrism VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Qml Concurrent Test)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/QtPrismContainer)
add_subdirectory(src/QtPrismEvents)
add_subdirectory(src/QtPrismCore)
add_subdirectory(src/QtPrismFramework)
add_subdirectory(samples/AdminApp)
add_subdirectory(tests)

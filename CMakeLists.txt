cmake_minimum_required(VERSION 3.16)

project(FibonacciVisualizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets Gui QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Gui)
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/fibonaccitree.cpp
    src/fibonaccitree.h
    src/treevisualizer.cpp
    src/treevisualizer.h
)

if(Qt6_FOUND)
    qt_add_executable(FibonacciVisualizer ${PROJECT_SOURCES})
    target_link_libraries(FibonacciVisualizer PRIVATE 
        Qt6::Core 
        Qt6::Widgets 
        Qt6::Gui
    )
else()
    add_executable(FibonacciVisualizer ${PROJECT_SOURCES})
    target_link_libraries(FibonacciVisualizer PRIVATE 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
    )
endif()

target_include_directories(FibonacciVisualizer PRIVATE src)

# Set output directories
set_target_properties(FibonacciVisualizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

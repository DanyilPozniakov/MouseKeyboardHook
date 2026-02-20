cmake_minimum_required(VERSION 4.1)
project(MouseKeyboardHook)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_PREFIX_PATH "/root/Qt/6.5.2/gcc_64/")

find_package(Qt6 COMPONENTS Core Widgets Network Quick Qml  QuickControls2  REQUIRED)

add_executable(MouseKeyboardHook main.cpp
        src/AppEngine.cpp
        src/AppEngine.h
        src/EventHandler.cpp
        src/EventHandler.h)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

qt_add_qml_module(
        MouseKeyboardHook
        URI MouseKeyboardHook
        VERSION 1.0
        QML_FILES
        qml/main.qml

)


target_link_libraries(MouseKeyboardHook PRIVATE
        Qt::Core
        Qt6::Widgets
        Qt::Qml
        Qt::Quick
        Qt6::QuickControls2
)
set_target_properties(MouseKeyboardHook PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
cmake_minimum_required(VERSION 3.18)

project(Mall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23) # si nécessaire
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

add_subdirectory(src)

target_link_libraries(Mall PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
cmake_minimum_required(VERSION 3.16)
project(Book_Club)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Sql)

add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)
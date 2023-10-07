cmake_minimum_required(VERSION 3.5)

project(08_PreDataBase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Sql)

set(PROJECT_SOURCES
        ${CMAKE_SOURCE_DIR}/source/main.cpp
        ${CMAKE_SOURCE_DIR}/source/mainwindow.cpp
        ${CMAKE_SOURCE_DIR}/headers/mainwindow.h
        ${CMAKE_SOURCE_DIR}/source/mainwindow.ui
        ${CMAKE_SOURCE_DIR}/headers/database.h
        ${CMAKE_SOURCE_DIR}/source/database.cpp
        ${CMAKE_SOURCE_DIR}/headers/dbdata.h
        ${CMAKE_SOURCE_DIR}/source/dbdata.cpp
        ${CMAKE_SOURCE_DIR}/source/dbdata.ui
        ${CMAKE_SOURCE_DIR}/source/about.cpp
        ${CMAKE_SOURCE_DIR}/source/about.ui
        ${CMAKE_SOURCE_DIR}/source/dialog.cpp
        ${CMAKE_SOURCE_DIR}/source/dialog.ui
        ${CMAKE_SOURCE_DIR}/headers/about.h
        ${CMAKE_SOURCE_DIR}/headers/dialog.h
        ${CMAKE_SOURCE_DIR}/resource/resource.qrc
)

add_executable(08_PreDataBase ${PROJECT_SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/PG_Libs)

include_directories("${PROJECT_SOURCE_DIR}/PG_Libs")

target_link_libraries(08_PreDataBase PRIVATE Qt6::Widgets
                                             Qt6::Concurrent
                                             Qt6::Sql
                                             libcrypto-3-x64
                                             libiconv-2
                                             libintl-9
                                             libpq
                                             libssl-3-x64
                                         )

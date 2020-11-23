cmake_minimum_required(VERSION 3.5)

project(File_Manager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(LibArchive REQUIRED)
find_package(QuaZip5)
include_directories(${LibArchive_INCLUDE_DIRS})
include_directories(${QUAZIP_INCLUDE_DIRS})

set(images_resources images.qrc)
qt5_add_resources(images_resources_rcc ${images_resources})

add_executable(File_Manager
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    searchdialog.cpp
    searchdialog.h
    searchdialog.ui
    file_info_functions.h
    ${images_resources_rcc}
)

target_link_libraries(File_Manager PRIVATE Qt5::Widgets ${LibArchive_LIBRARIES} ${QUAZIP_LIBRARIES})

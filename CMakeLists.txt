project(Adwaita)
cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_AUTOMOC ON)
add_definitions(-std=c++11)

find_package(Qt4 REQUIRED)

if (NOT CMAKE_PREFIX_PATH)
    set (CMAKE_PREFIX_PATH "/usr/local")
endif()

if (NOT QT_ROOT)
    set(QT_ROOT "${CMAKE_PREFIX_PATH}/lib${LIB_SUFFIX}/qt4")
endif()
if (NOT QT_PLUGIN_PATH)
    set(QT_PLUGIN_PATH "${QT_ROOT}/plugins")
endif()
if (NOT QT_STYLE_PATH)
    set(QT_STYLE_PATH "${QT_PLUGIN_PATH}/styles")
endif()

if (NOT DATA_INSTALL_DIR)
    set (DATA_INSTALL_DIR "${CMAKE_PREFIX_PATH}/share")
endif()
if (NOT THEME_INSTALL_DIR)
    set (THEME_INSTALL_DIR "${DATA_INSTALL_DIR}/themes")
endif()

set(ADWAITA_THEME_DIR "${THEME_INSTALL_DIR}/Adwaita/qt4")

add_subdirectory(assets)
add_subdirectory(src)
add_subdirectory(stylesheet)
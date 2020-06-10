project(Adwaita)

cmake_minimum_required(VERSION 3.0)

set(QT_MIN_VERSION "5.12.0")
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

add_definitions(-std=c++11)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Widgets
)

find_package(XCB 1.10 COMPONENTS XCB)

set(ADWAITA_HAVE_X11 ${XCB_FOUND})

get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE}
                    IMPORTED_LOCATION)
if (NOT QT_PLUGINS_DIR)
    execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
                    OUTPUT_VARIABLE QT_PLUGINS_DIR
                    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

option(BUILD_EXAMPLE "Build an example widget factory app" OFF)
if (BUILD_EXAMPLE)
    add_subdirectory(demo)
endif()

add_subdirectory(style)

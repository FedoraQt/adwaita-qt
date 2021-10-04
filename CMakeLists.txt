project(Adwaita)

cmake_minimum_required(VERSION 3.17)

set(ADWAITAQT_VERSION_MAJOR 1)
set(ADWAITAQT_VERSION "1.4.50")

set(QT_MIN_VERSION "5.12.0")
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

include(GNUInstallDirs)

add_definitions(-std=c++11)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Widgets
)

if (NOT APPLE AND NOT WIN32)
    find_package(XCB 1.10 COMPONENTS XCB)

    set(ADWAITA_HAVE_X11 ${XCB_FOUND})

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
        X11Extras
    )
endif()

get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE}
                    IMPORTED_LOCATION)
if (NOT QT_PLUGINS_DIR)
    execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
                    OUTPUT_VARIABLE QT_PLUGINS_DIR
                    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

add_subdirectory(src)

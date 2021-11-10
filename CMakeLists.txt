project(Adwaita)

cmake_minimum_required(VERSION 3.17)

set(ADWAITAQT_VERSION_MAJOR 1)
set(ADWAITAQT_VERSION "1.4.1")

option(USE_QT6 "Use Qt6 instead of Qt5" OFF)

if (USE_QT6)
    set(QT_MIN_VERSION "6.2.0")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(ADWAITAQT_SUFFIX "6")
else()
    set(QT_MIN_VERSION "5.15.2")
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(ADWAITAQT_SUFFIX "")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

include(GNUInstallDirs)

if (USE_QT6)
    find_package(QT NAMES Qt6 COMPONENTS Core DBus Gui Widgets REQUIRED)
else()
    find_package(QT NAMES Qt5 COMPONENTS Core DBus Gui Widgets REQUIRED)
endif()


find_package(Qt${QT_VERSION_MAJOR} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Widgets
)

if (NOT APPLE AND NOT WIN32 AND NOT USE_QT6)
    find_package(XCB 1.10 COMPONENTS XCB)

    set(ADWAITA_HAVE_X11 ${XCB_FOUND})

    find_package(Qt${QT_VERSION_MAJOR} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
        X11Extras
    )
endif()

if (NOT USE_QT6)
    get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)
    if (NOT QT_PLUGINS_DIR)
        execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
                        OUTPUT_VARIABLE QT_PLUGINS_DIR
                        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
endif()

add_subdirectory(src)

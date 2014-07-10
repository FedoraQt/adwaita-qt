project(Adwaita)
cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_AUTOMOC ON)

add_definitions(-std=c++11)

find_package(KDE4 REQUIRED)

include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Adwaita_SRCS
    widgetshowcase.cpp
    adwaita.cpp
    widgetfactory.cpp
    main.cpp
)
set(Adwaita_UI
    widgetshowcase.ui
)
qt4_wrap_ui(Adwaita_SRCS ${Adwaita_UI})
add_executable(Adwaita ${Adwaita_SRCS})
target_link_libraries(Adwaita ${QT_QTCORE_LIBRARY} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY})


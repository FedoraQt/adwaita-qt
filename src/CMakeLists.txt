include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Adwaita_SRCS
    adwaita.cpp
    styleplugin.cpp
)
add_definitions(-DQT_PLUGIN)

add_library(adwaita-qt4 MODULE ${Adwaita_SRCS})
target_link_libraries(adwaita-qt4 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
set_target_properties(adwaita-qt4 PROPERTIES
    LINK_FLAGS "-Wl,--no-undefined"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    OUTPUT_NAME "adwaita-qt4"
    PREFIX "")

set(Showcase_SRCS
    main.cpp
    widgetfactory.cpp
    widgetshowcase.cpp
)
set(Showcase_UI
    widgetshowcase.ui
)
qt4_wrap_ui(Showcase_SRCS ${Showcase_UI})
#add_executable(Showcase ${Showcase_SRCS})
#target_link_libraries(Showcase adwaita-qt4 ${QT_QTCORE_LIBRARY} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY})


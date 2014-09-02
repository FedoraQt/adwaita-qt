include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Showcase_SRCS
    main.cpp
    widgetfactory.cpp
    widgetshowcase.cpp
)
set(Showcase_UI
    widgetshowcase.ui
)
qt4_wrap_ui(Showcase_SRCS ${Showcase_UI})

add_executable(Showcase ${Showcase_SRCS})
target_link_libraries(Showcase ${QT_QTCORE_LIBRARY} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY})


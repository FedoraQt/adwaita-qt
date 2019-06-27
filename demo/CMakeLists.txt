
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(showcase_SRCS
    main.cpp
    widgetfactory.cpp
)

set(showcase_UI
    widgetfactory.ui
)

qt5_wrap_ui(showcase_SRCS ${showcase_UI})

add_executable(showcase ${showcase_SRCS})
target_link_libraries(showcase Qt5::Core Qt5::Widgets)

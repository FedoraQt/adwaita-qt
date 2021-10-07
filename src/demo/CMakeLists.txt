
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(showcase_SRCS
    main.cpp
    widgetfactory.cpp
)

set(showcase_UI
    widgetfactory.ui
)

if (USE_QT6)
    qt6_wrap_ui(showcase_SRCS ${showcase_UI})
else()
    qt5_wrap_ui(showcase_SRCS ${showcase_UI})
endif()

add_executable(showcase ${showcase_SRCS})
target_link_libraries(showcase Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

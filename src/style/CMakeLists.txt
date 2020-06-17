
configure_file(config-adwaita.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-adwaita.h )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} # for adwaita-config.h
    ${CMAKE_SOURCE_DIR}/src/lib
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/
)

set(Adwaita_SRCS
    debug/adwaitawidgetexplorer.cpp

    adwaitaaddeventfilter.cpp
    adwaitahelper.cpp
    adwaitamnemonics.cpp
    adwaitasplitterproxy.cpp
    adwaitastyle.cpp
    adwaitastyleplugin.cpp
    adwaitawindowmanager.cpp
)

add_definitions(-DQT_PLUGIN)

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES) # Needed for M_PI on MSVC
endif()

set(LIBRARY_NAME "adwaita-qt")

add_library(${LIBRARY_NAME} MODULE ${Adwaita_SRCS})
target_link_libraries(${LIBRARY_NAME}
    adwaitaqt
    ${XCB_LIBRARIES}
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
    LINK_FLAGS "-Wl,--no-undefined"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    OUTPUT_NAME "adwaita"
    PREFIX "")

install(TARGETS ${LIBRARY_NAME} DESTINATION "${QT_PLUGINS_DIR}/styles")

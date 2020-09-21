
include_directories(
    ${CMAKE_BINARY_DIR}/src/lib # for adwaita-config.h
    ${CMAKE_SOURCE_DIR}/src/lib
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(Adwaita_SRCS
    adwaitahelper.cpp
    adwaitastyle.cpp
    adwaitastyleplugin.cpp
)

add_definitions(-DQT_PLUGIN)

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES) # Needed for M_PI on MSVC
endif()

set(LIBRARY_NAME "adwaita-qt")

add_library(${LIBRARY_NAME} MODULE ${Adwaita_SRCS})
target_link_libraries(${LIBRARY_NAME}
    adwaitaqt
    adwaitaqtpriv
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
)

if (ADWAITA_HAVE_X11)
    target_link_libraries(${LIBRARY_NAME} ${XCB_LIBRARIES} Qt5::X11Extras)
endif()

if (NOT APPLE AND NOT WIN32)
    set_target_properties(${LIBRARY_NAME} PROPERTIES
        LINK_FLAGS "-Wl,--no-undefined"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        OUTPUT_NAME "adwaita"
        PREFIX "")
endif()

install(TARGETS ${LIBRARY_NAME} DESTINATION "${QT_PLUGINS_DIR}/styles")

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
    OUTPUT_NAME "adwaita"
    PREFIX "")

install(TARGETS adwaita-qt4 DESTINATION "${QT_STYLE_PATH}/")
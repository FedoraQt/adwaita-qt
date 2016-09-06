set(Adwaita_SRCS
    adwaita.cpp
    styleplugin.cpp
    qstyleanimation.cpp
    qstyleanimation_p.h
)
add_definitions(-DQT_PLUGIN)

set(LIBRARY_NAME "adwaita-qt${QT_VERSION_NUMBER}")

add_library(${LIBRARY_NAME} MODULE ${Adwaita_SRCS})
target_link_libraries(${LIBRARY_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
set_target_properties(${LIBRARY_NAME} PROPERTIES
    LINK_FLAGS "-Wl,--no-undefined"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    OUTPUT_NAME "adwaita"
    PREFIX "")

install(TARGETS ${LIBRARY_NAME} DESTINATION "${QT_STYLE_PATH}/")

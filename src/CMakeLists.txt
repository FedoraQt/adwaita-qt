include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (KDE4_FOUND)
    add_subdirectory(example)
endif()
add_subdirectory(style)
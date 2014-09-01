configure_file(adwaita.css.in "${CMAKE_BINARY_DIR}/adwaita.css" @ONLY)

install(FILES "${CMAKE_BINARY_DIR}/adwaita.css" DESTINATION "${ADWAITA_THEME_DIR}/")
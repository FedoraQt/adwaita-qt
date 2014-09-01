set(ADWAITA_CSS_FILE "${ADWAITA_THEME_DIR}/adwaita.css")
mark_as_advanced(ADWAITA_CSS_FILE)

configure_file(adwaita.css.in adwaita.css @ONLY)

install(FILES "adwaita.css" DESTINATION "${ADWAITA_THEME_DIR}/")
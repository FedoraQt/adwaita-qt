### configuration plugin
add_definitions(-DTRANSLATION_DOMAIN=\"adwaita_style_config\")
set(kstyle_adwaita_config_PART_SRCS adwaitastyleconfig.cpp)

if(ADWAITA_USE_KDE4)

  kde4_add_kcfg_files(kstyle_adwaita_config_PART_SRCS ../adwaitastyleconfigdata.kcfgc)
  kde4_add_ui_files(kstyle_adwaita_config_PART_SRCS ui/adwaitastyleconfig.ui)
  kde4_add_plugin(kstyle_adwaita_config ${kstyle_adwaita_config_PART_SRCS})

  target_link_libraries(kstyle_adwaita_config ${KDE4_KDEUI_LIBS} )
  install(TARGETS kstyle_adwaita_config DESTINATION ${PLUGIN_INSTALL_DIR})

else()

  set(kstyle_adwaita_config_PART_SRCS ${kstyle_adwaita_config_PART_SRCS} adwaitastyleconfigmodule.cpp)

  kconfig_add_kcfg_files(kstyle_adwaita_config_PART_SRCS ../adwaitastyleconfigdata.kcfgc)
  set(kstyle_adwaita_config_PART_FORMS ui/adwaitastyleconfig.ui)
  ki18n_wrap_ui(kstyle_adwaita_config_PART_FORMS_HEADERS ${kstyle_adwaita_config_PART_FORMS})

  add_library(kstyle_adwaita_config MODULE
    ${kstyle_adwaita_config_PART_SRCS}
    ${kstyle_adwaita_config_PART_FORMS_HEADERS}
 )

  target_link_libraries(kstyle_adwaita_config Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
  target_link_libraries(kstyle_adwaita_config KF5::ConfigCore KF5::ConfigWidgets KF5::I18n)
  install(TARGETS kstyle_adwaita_config DESTINATION ${QT_PLUGIN_INSTALL_DIR})
  install(FILES adwaitastyleconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR})

endif()

########### next target ###############
if(NOT ADWAITA_USE_KDE4)

  find_package(KF5KCMUtils CONFIG)
  set_package_properties(KF5KCMUtils PROPERTIES
      DESCRIPTION "KF5 KCM utils Framework"
      URL "https://projects.kde.org/projects/frameworks/kcmutils"
      TYPE OPTIONAL
      PURPOSE "Required for adwaita-settings5 application to build")

  if( KF5KCMUtils_FOUND )

      set(adwaita_settings_SOURCES main.cpp)
      add_executable(adwaita-settings5 ${adwaita_settings_SOURCES} )
      target_link_libraries(adwaita-settings5 Qt5::Core Qt5::Gui Qt5::Widgets )
      target_link_libraries(adwaita-settings5 KF5::I18n KF5::KCMUtils)

      install(TARGETS adwaita-settings5 ${INSTALL_TARGETS_DEFAULT_ARGS})

      # icon
      ecm_install_icons(ICONS sc-apps-adwaita-settings.svgz
        DESTINATION ${ICON_INSTALL_DIR}
        THEME hicolor )
    endif()
endif()

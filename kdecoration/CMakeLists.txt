add_definitions(-DTRANSLATION_DOMAIN="adwaita_kwin_deco")

find_package(KF5 REQUIRED COMPONENTS CoreAddons GuiAddons ConfigWidgets WindowSystem I18n)
find_package(Qt5 CONFIG REQUIRED COMPONENTS DBus)

### XCB
find_package(XCB COMPONENTS XCB)
set_package_properties(XCB PROPERTIES
  DESCRIPTION "X protocol C-language Binding"
  URL "http://xcb.freedesktop.org"
  TYPE OPTIONAL
  PURPOSE "Required to pass style properties to native Windows on X11 Platform"
)

if(UNIX AND NOT APPLE)

  set(ADWAITA_HAVE_X11 ${XCB_XCB_FOUND})
  if (XCB_XCB_FOUND)
    find_package(Qt5 REQUIRED CONFIG COMPONENTS X11Extras)
  endif()

else()

  set(ADWAITA_HAVE_X11 FALSE)

endif()

################# configuration #################
configure_file(config-adwaita.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-adwaita.h )

################# newt target #################
### plugin classes
set(adwaitadecoration_SRCS
    adwaitabutton.cpp
    adwaitadecoration.cpp
    adwaitaexceptionlist.cpp
    adwaitasettingsprovider.cpp
    adwaitasizegrip.cpp)

kconfig_add_kcfg_files(adwaitadecoration_SRCS adwaitasettings.kcfgc)

### config classes
### they are kept separately because they might move in a separate library in the future
set(adwaitadecoration_config_SRCS
    config/adwaitaconfigwidget.cpp
    config/adwaitadetectwidget.cpp
    config/adwaitaexceptiondialog.cpp
    config/adwaitaexceptionlistwidget.cpp
    config/adwaitaexceptionmodel.cpp
    config/adwaitaitemmodel.cpp
)

set(adwaitadecoration_config_PART_FORMS
   config/ui/adwaitaconfigurationui.ui
   config/ui/adwaitadetectwidget.ui
   config/ui/adwaitaexceptiondialog.ui
   config/ui/adwaitaexceptionlistwidget.ui
)

ki18n_wrap_ui(adwaitadecoration_config_PART_FORMS_HEADERS ${adwaitadecoration_config_PART_FORMS})

### build library
add_library(adwaitadecoration MODULE
    ${adwaitadecoration_SRCS}
    ${adwaitadecoration_config_SRCS}
    ${adwaitadecoration_config_PART_FORMS_HEADERS})

target_link_libraries(adwaitadecoration
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::DBus
    PRIVATE
        KDecoration2::KDecoration
        KF5::ConfigCore
        KF5::CoreAddons
        KF5::ConfigWidgets
        KF5::GuiAddons
        KF5::I18n
        KF5::WindowSystem)

if(ADWAITA_HAVE_X11)
  target_link_libraries(adwaitadecoration
    PUBLIC
      Qt5::X11Extras
      XCB::XCB)
endif()


install(TARGETS adwaitadecoration DESTINATION ${PLUGIN_INSTALL_DIR}/org.kde.kdecoration2)
install(FILES config/adwaitadecorationconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR})

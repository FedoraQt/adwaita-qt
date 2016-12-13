set(ADWAITA_USE_KDE4 ${USE_KDE4})
if(ADWAITA_USE_KDE4)

  ############ Language and toolchain features
  ############ copied from ECM
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" AND NOT WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()

endif()

################# Qt/KDE #################
if(ADWAITA_USE_KDE4)

  find_package(KDE4 REQUIRED)
  include(KDE4Defaults)
  include(MacroLibrary)

  add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
  include_directories(${KDE4_INCLUDES})

  ### XCB
  if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XCB xcb x11-xcb)
    add_feature_info("x11-xcb" XCB_FOUND "Required to pass style properties to native Windows on X11 Platform")
    set_feature_info("x11-xcb" "Required to pass style properties to native Windows on X11 Platform" "http://xcb.freedesktop.org")
    set(ADWAITA_HAVE_X11 ${XCB_FOUND})
  else()
    set(ADWAITA_HAVE_X11 FALSE)
  endif()

  set(ADWAITA_HAVE_KWAYLAND FALSE)

  ### KStyle
  set(ADWAITA_HAVE_KSTYLE FALSE)

else()

  find_package(Qt5 REQUIRED CONFIG COMPONENTS Widgets DBus)
  find_package(KF5 REQUIRED COMPONENTS
      I18n
      Config
      GuiAddons
      ConfigWidgets
      WindowSystem)

  find_package( KF5FrameworkIntegration CONFIG )
  set_package_properties(KF5FrameworkIntegration PROPERTIES
      DESCRIPTION "KF5 Framework Integration"
      URL "https://projects.kde.org/projects/frameworks/frameworkintegration"
      TYPE OPTIONAL
      PURPOSE "Required to use KStyle convenience functionalities in style")

  set(ADWAITA_HAVE_KSTYLE ${KF5FrameworkIntegration_FOUND})

  find_package(XCB COMPONENTS XCB)
  set_package_properties(XCB PROPERTIES
      DESCRIPTION "X protocol C-language Binding"
      URL "http://xcb.freedesktop.org"
      TYPE OPTIONAL
      PURPOSE "Required to pass style properties to native Windows on X11 Platform"
  )

  find_package(KF5Wayland CONFIG)
  set(ADWAITA_HAVE_KWAYLAND ${KF5Wayland_FOUND})

  if(UNIX AND NOT APPLE)

    set(ADWAITA_HAVE_X11 ${XCB_XCB_FOUND})
    if (XCB_XCB_FOUND)
      find_package(Qt5 REQUIRED CONFIG COMPONENTS X11Extras)
    endif()

  else()

    set(ADWAITA_HAVE_X11 FALSE)

  endif()


endif()

################# includes #################
include_directories(
  animations
  debug
)

################# configuration #################
configure_file(config-adwaita.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-adwaita.h )

########### next target ###############
set(adwaita_PART_SRCS
    animations/adwaitaanimation.cpp
    animations/adwaitaanimations.cpp
    animations/adwaitaanimationdata.cpp
    animations/adwaitabaseengine.cpp
    animations/adwaitabusyindicatordata.cpp
    animations/adwaitabusyindicatorengine.cpp
    animations/adwaitadialdata.cpp
    animations/adwaitadialengine.cpp
    animations/adwaitaenabledata.cpp
    animations/adwaitagenericdata.cpp
    animations/adwaitaheaderviewdata.cpp
    animations/adwaitaheaderviewengine.cpp
    animations/adwaitascrollbardata.cpp
    animations/adwaitascrollbarengine.cpp
    animations/adwaitaspinboxengine.cpp
    animations/adwaitaspinboxdata.cpp
    animations/adwaitastackedwidgetdata.cpp
    animations/adwaitastackedwidgetengine.cpp
    animations/adwaitatabbarengine.cpp
    animations/adwaitatabbardata.cpp
    animations/adwaitatoolboxengine.cpp
    animations/adwaitatransitiondata.cpp
    animations/adwaitatransitionwidget.cpp
    animations/adwaitawidgetstateengine.cpp
    animations/adwaitawidgetstatedata.cpp
    debug/adwaitawidgetexplorer.cpp
    adwaitaaddeventfilter.cpp
    adwaitaframeshadow.cpp
    adwaitahelper.cpp
    adwaitamdiwindowshadow.cpp
    adwaitamnemonics.cpp
    adwaitapropertynames.cpp
    adwaitashadowhelper.cpp
    adwaitasplitterproxy.cpp
    adwaitastyle.cpp
    adwaitastyleplugin.cpp
    adwaitatileset.cpp
    adwaitawindowmanager.cpp
)

if(ADWAITA_USE_KDE4)

  kde4_add_kcfg_files(adwaita_PART_SRCS adwaitastyleconfigdata.kcfgc)
  kde4_add_plugin(adwaita ${adwaita_PART_SRCS} kstylekde4compat.cpp)
  target_link_libraries(adwaita ${KDE4_KDEUI_LIBS})

  if(ADWAITA_HAVE_X11)
    target_link_libraries(adwaita ${X11_XCB_LIBRARIES})
    target_link_libraries(adwaita ${XCB_LIBRARIES})
  endif()

  install(TARGETS adwaita  DESTINATION ${LIB_INSTALL_DIR}/qt4/plugins/styles/)

else()

    kconfig_add_kcfg_files(adwaita_PART_SRCS adwaitastyleconfigdata.kcfgc)
    add_library(adwaita MODULE ${adwaita_PART_SRCS})
    target_link_libraries(adwaita Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
    target_link_libraries(adwaita KF5::ConfigCore KF5::ConfigWidgets KF5::GuiAddons KF5::WindowSystem)

  if( KF5FrameworkIntegration_FOUND )
  target_link_libraries(adwaita KF5::Style)
  endif()

  if (WIN32)
    # As stated in http://msdn.microsoft.com/en-us/library/4hwaceh6.aspx M_PI only gets defined
    # when if _USE_MATH_DEFINES is defined
    target_compile_definitions(adwaita PRIVATE _USE_MATH_DEFINES _BSD_SOURCE)
  endif()

  if(ADWAITA_HAVE_X11)
    target_link_libraries(adwaita ${XCB_LIBRARIES})
    target_link_libraries(adwaita Qt5::X11Extras)
  endif()

  if(ADWAITA_HAVE_KWAYLAND)
    target_link_libraries(adwaita KF5::WaylandClient)
  endif()

  install(TARGETS adwaita DESTINATION ${LIB_INSTALL_DIR}/qt5/plugins/styles/)

endif()

########### install files ###############
#install(FILES adwaita.themerc  DESTINATION  ${DATA_INSTALL_DIR}/kstyle/themes)

########### subdirectories ###############
#add_subdirectory(config)

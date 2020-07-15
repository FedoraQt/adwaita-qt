
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(config-adwaita.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-adwaita.h )
configure_file(adwaita-qt.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt.pc @ONLY)

set(libadwaitaqt_priv_SRCS
    # Animations
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

    # Others
    adwaitaaddeventfilter.cpp
    adwaitamnemonics.cpp
    adwaitasplitterproxy.cpp
    adwaitawidgetexplorer.cpp
    adwaitawindowmanager.cpp
)

set(libadwaitaqt_SRCS
    adwaita.cpp
    adwaitacolors.cpp
)

set(libadwaitaqt_HEADERS
    adwaita.h
    adwaitacolors.h
    adwaitaqt_export.h
)

add_library(adwaitaqtpriv SHARED ${libadwaitaqt_priv_SRCS})
target_link_libraries(adwaitaqtpriv
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
)

add_library(adwaitaqt SHARED ${libadwaitaqt_SRCS})
add_library(AdwaitaQt ALIAS adwaitaqt)

target_link_libraries(adwaitaqt
    PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    PRIVATE
    adwaitaqtpriv
)

set_target_properties(adwaitaqt PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
target_include_directories(adwaitaqt PUBLIC ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

install(TARGETS adwaitaqtpriv DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} EXPORT AdwaitaQtTargets)
install(TARGETS adwaitaqt DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} EXPORT AdwaitaQtTargets)

install(FILES ${libadwaitaqt_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/AdwaitaQt)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/AdwaitaQt")

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/AdwaitaQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfigVersion.cmake
  VERSION ${ADWAITAQT_VERSION}
  COMPATIBILITY SameMinorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT AdwaitaQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE AdwaitaQtTargets.cmake)

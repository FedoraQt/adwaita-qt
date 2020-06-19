
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

configure_file(adwaita-qt.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt.pc @ONLY)

set(libadwaitaqt_SRCS
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
)

set(libadwaitaqt_HEADERS
    # Animations
    animations/adwaitaanimation.h
    animations/adwaitaanimations.h
    animations/adwaitaanimationdata.h
    animations/adwaitabaseengine.h
    animations/adwaitabusyindicatordata.h
    animations/adwaitabusyindicatorengine.h
    animations/adwaitadatamap.h
    animations/adwaitadialdata.h
    animations/adwaitadialengine.h
    animations/adwaitaenabledata.h
    animations/adwaitagenericdata.h
    animations/adwaitaheaderviewdata.h
    animations/adwaitaheaderviewengine.h
    animations/adwaitascrollbardata.h
    animations/adwaitascrollbarengine.h
    animations/adwaitaspinboxengine.h
    animations/adwaitaspinboxdata.h
    animations/adwaitastackedwidgetdata.h
    animations/adwaitastackedwidgetengine.h
    animations/adwaitatabbarengine.h
    animations/adwaitatabbardata.h
    animations/adwaitatoolboxengine.h
    animations/adwaitatransitiondata.h
    animations/adwaitatransitionwidget.h
    animations/adwaitawidgetstateengine.h
    animations/adwaitawidgetstatedata.h

    # Others
    adwaita.h
    adwaitaqt_export.h
    adwaitaaddeventfilter.h
    adwaitamnemonics.h
    adwaitasplitterproxy.h
    adwaitawidgetexplorer.h
)

add_library(adwaitaqt SHARED ${libadwaitaqt_SRCS})
add_library(AdwaitaQt ALIAS adwaitaqt)

target_link_libraries(adwaitaqt
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
)

set_target_properties(adwaitaqt PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
target_include_directories(adwaitaqt PUBLIC ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5X11Extras_INCLUDE_DIRS})

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

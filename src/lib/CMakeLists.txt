
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
    adwaitarenderer.cpp
)

set(libadwaitaqt_HEADERS
    adwaita.h
    adwaitacolors.h
    adwaitarenderer.h
    adwaitaqt_export.h
)

add_library(adwaitaqtpriv SHARED ${libadwaitaqt_priv_SRCS})
target_link_libraries(adwaitaqtpriv
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
)

# We need this done before we start building adwaitaqt public library
find_program(SASSC_EXECUTABLE sassc REQUIRED)
set(Variants "light" "dark" "hc" "hc-dark")
foreach(VARIANT IN LISTS Variants )
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/Adwaita-${VARIANT}.css"
                       COMMAND "${SASSC_EXECUTABLE}"
                               -a -M -t compact
                               "${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/Adwaita-${VARIANT}.scss"
                               "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/Adwaita-${VARIANT}.css"
                       COMMENT "Stylesheet variant: ${VARIANT}")
endforeach()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/stylesheet.qrc.in" "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/stylesheet.qrc")

qt5_add_resources(libadwaitaqt_SRCS "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/stylesheet.qrc")

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

set_target_properties(adwaitaqtpriv PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
if(MINGW AND BUILD_SHARED_LIBS)
    get_target_property(ADWAITA_QT_SOVERSION adwaitaqtpriv SOVERSION)
    set_target_properties(adwaitaqtpriv PROPERTIES SUFFIX "-${ADWAITA_QT_SOVERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

set_target_properties(adwaitaqt PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
if(MINGW AND BUILD_SHARED_LIBS)
    get_target_property(ADWAITA_QT_SOVERSION adwaitaqt SOVERSION)
    set_target_properties(adwaitaqt PROPERTIES SUFFIX "-${ADWAITA_QT_SOVERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

target_include_directories(adwaitaqt PUBLIC ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

install(TARGETS adwaitaqtpriv EXPORT AdwaitaQtTargets RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS adwaitaqt EXPORT AdwaitaQtTargets RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

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
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT AdwaitaQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE AdwaitaQtTargets.cmake)

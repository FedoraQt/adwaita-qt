
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(config-adwaita.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-adwaita.h )
configure_file(adwaita-qt.pc.cmake "${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt${ADWAITAQT_SUFFIX}.pc" @ONLY)

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

add_library("adwaitaqt${ADWAITAQT_SUFFIX}priv" SHARED ${libadwaitaqt_priv_SRCS})
target_link_libraries("adwaitaqt${ADWAITAQT_SUFFIX}priv"
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

# We need this done before we start building adwaitaqt public library
find_program(SASSC_EXECUTABLE sassc)
if (SASSC_EXECUTABLE)
    set(Variants "light" "dark" "hc" "hc-dark")
    foreach(VARIANT IN LISTS Variants)
        add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/Adwaita-${VARIANT}.css"
                           COMMAND "${SASSC_EXECUTABLE}"
                                  -a -M -t compact
                                  "${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/Adwaita-${VARIANT}.scss"
                                  "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/Adwaita-${VARIANT}.css"
                           COMMENT "Stylesheet variant: ${VARIANT}")
    endforeach()
    set(STYLESHEET_PATH "${CMAKE_CURRENT_BINARY_DIR}/stylesheet")
else()
    set(STYLESHEET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/processed")
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/stylesheet.qrc.in" "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/stylesheet.qrc")

if (USE_QT6)
    qt6_add_resources(libadwaitaqt_SRCS "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/stylesheet.qrc")
else()
    qt5_add_resources(libadwaitaqt_SRCS "${CMAKE_CURRENT_BINARY_DIR}/stylesheet/stylesheet.qrc")
endif()

add_library("adwaitaqt${ADWAITAQT_SUFFIX}" SHARED ${libadwaitaqt_SRCS})
add_library("AdwaitaQt${ADWAITAQT_SUFFIX}" ALIAS "adwaitaqt${ADWAITAQT_SUFFIX}")

target_link_libraries("adwaitaqt${ADWAITAQT_SUFFIX}"
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE
    "adwaitaqt${ADWAITAQT_SUFFIX}priv"
)

set_target_properties("adwaitaqt${ADWAITAQT_SUFFIX}priv" PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
if(MINGW AND BUILD_SHARED_LIBS)
    get_target_property(ADWAITA_QT_SOVERSION "adwaitaqt${ADWAITAQT_SUFFIX}priv" SOVERSION)
    set_target_properties("adwaitaqt${ADWAITAQT_SUFFIX}priv" PROPERTIES SUFFIX "-${ADWAITA_QT_SOVERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

set_target_properties("adwaitaqt${ADWAITAQT_SUFFIX}" PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)
if(MINGW AND BUILD_SHARED_LIBS)
    get_target_property(ADWAITA_QT_SOVERSION "adwaitaqt${ADWAITAQT_SUFFIX}" SOVERSION)
    set_target_properties("adwaitaqt${ADWAITAQT_SUFFIX}" PROPERTIES SUFFIX "-${ADWAITA_QT_SOVERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

if (USE_QT6)
    target_include_directories("adwaitaqt${ADWAITAQT_SUFFIX}" PUBLIC ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS})
else()
    target_include_directories("adwaitaqt${ADWAITAQT_SUFFIX}" PUBLIC ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
endif()

install(TARGETS "adwaitaqt${ADWAITAQT_SUFFIX}priv" EXPORT "AdwaitaQt${ADWAITAQT_SUFFIX}Targets" RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS "adwaitaqt${ADWAITAQT_SUFFIX}" EXPORT "AdwaitaQt${ADWAITAQT_SUFFIX}Targets" RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${libadwaitaqt_HEADERS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/AdwaitaQt${ADWAITAQT_SUFFIX}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt${ADWAITAQT_SUFFIX}.pc" DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/AdwaitaQt${ADWAITAQT_SUFFIX}")

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/AdwaitaQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQt${ADWAITAQT_SUFFIX}Config.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQt${ADWAITAQT_SUFFIX}ConfigVersion.cmake"
  VERSION ${ADWAITAQT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQt${ADWAITAQT_SUFFIX}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQt${ADWAITAQT_SUFFIX}ConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT "AdwaitaQt${ADWAITAQT_SUFFIX}Targets" DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE "AdwaitaQt${ADWAITAQT_SUFFIX}Targets.cmake")

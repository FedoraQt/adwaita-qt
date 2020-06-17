
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

configure_file(adwaita-qt.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt.pc @ONLY)
configure_file(cmake/AdwaitaQtConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfig.cmake @ONLY)
configure_file(cmake/AdwaitaQtConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfigVersion.cmake @ONLY)

set(libadwaitaqt_SRCS
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
)

set(libadwaitaqt_HEADERS
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
    adwaita.h
    adwaitaqt_export.h
)

add_library(adwaitaqt SHARED ${libadwaitaqt_SRCS})

target_link_libraries(adwaitaqt
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
)

set_target_properties(adwaitaqt PROPERTIES VERSION ${ADWAITAQT_VERSION} SOVERSION 1)

install(TARGETS adwaitaqt DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
install(FILES ${libadwaitaqt_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/AdwaitaQt)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/AdwaitaQtConfigVersion.cmake
        DESTINATION
        ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/AdwaitaQt)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/adwaita-qt.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)

prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_FULL_LIBDIR@
includedir=@CMAKE_INSTALL_FULL_INCLUDEDIR@

Name: adwaita-qt
Description: Qt Adwaita Style
Version: @ADWAITAQT_VERSION@
Requires: @PC_REQUIRES@
@PC_REQUIRES_PRIVATE@

Libs: -L${libdir} -ladwaitaqt
Cflags: -I${includedir}
Requires: Qt5Core Qt5Widgets

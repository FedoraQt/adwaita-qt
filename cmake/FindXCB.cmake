# Try to find XCB on a Unix system
#
# This will define:
#
#   XCB_FOUND       - True if xcb is available
#   XCB_LIBRARIES   - Link these to use xcb
#   XCB_INCLUDE_DIR - Include directory for xcb
#   XCB_DEFINITIONS - Compiler flags for using xcb
#
# In addition the following more fine grained variables will be defined:
#
#   XCB_XCB_FOUND        XCB_XCB_INCLUDE_DIR        XCB_XCB_LIBRARIES
#
# Copyright (c) 2012 Fredrik Höglund <fredrik@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (XCB_INCLUDE_DIR AND XCB_LIBRARIES)
    # In the cache already
    set(XCB_FIND_QUIETLY TRUE)
endif (XCB_INCLUDE_DIR AND XCB_LIBRARIES)

# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PKG_XCB QUIET xcb)

set(XCB_DEFINITIONS ${PKG_XCB_CFLAGS})

find_path(XCB_XCB_INCLUDE_DIR
            NAMES xcb/xcb.h
            HINTS ${PKG_XCB_INCLUDE_DIRS})

find_library(XCB_XCB_LIBRARIES
            NAMES xcb
            HINTS ${PKG_XCB_LIBRARY_DIRS})

set(XCB_INCLUDE_DIR ${XCB_XCB_INCLUDE_DIR})

set(XCB_LIBRARIES ${XCB_XCB_LIBRARIES})

list(REMOVE_DUPLICATES XCB_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(XCB
    FOUND_VAR
        XCB_FOUND
    REQUIRED_VARS
        XCB_LIBRARIES
        XCB_INCLUDE_DIR
)

mark_as_advanced(XCB_INCLUDE_DIR XCB_LIBRARIES)

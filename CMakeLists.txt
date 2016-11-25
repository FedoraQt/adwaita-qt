project(adwaita)
set(PROJECT_VERSION "1.0")
set(PROJECT_VERSION_MAJOR 1)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

option(USE_KDE4 "Build a widget style for KDE4 (and nothing else)")

include(WriteBasicConfigVersionFile)
include(FeatureSummary)

if(USE_KDE4)
  add_subdirectory(kstyle)
else()
  find_package(ECM 0.0.9 REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_SOURCE_DIR}/cmake)

  include(ECMInstallIcons)
  include(KDEInstallDirs)
  include(KDECMakeSettings)
  include(KDECompilerSettings NO_POLICY_SCOPE)
  include(GenerateExportHeader)
  include(GtkUpdateIconCache)

  option(WITH_DECORATIONS "Build Adwaita window decorations for KWin" OFF)
  if(WITH_DECORATIONS)
    find_package(KDecoration2 REQUIRED)
    add_subdirectory(kdecoration)
  endif()

  add_subdirectory(kstyle)
  #add_subdirectory(qtquickcontrols)

  #INSTALL(FILES colors/Adwaita.colors DESTINATION ${DATA_INSTALL_DIR}/color-schemes/)
  #INSTALL(FILES colors/AdwaitaDark.colors DESTINATION ${DATA_INSTALL_DIR}/color-schemes/)
  #INSTALL(FILES colors/AdwaitaHighContrast.colors DESTINATION ${DATA_INSTALL_DIR}/color-schemes/)

  find_package(KF5Package CONFIG REQUIRED)
  #if(EXISTS ${CMAKE_SOURCE_DIR}/po AND IS_DIRECTORY ${CMAKE_SOURCE_DIR}/po)
  #  find_package(KF5I18n CONFIG REQUIRED)
  #  ki18n_install(po)
  #endif()

  include(ECMSetupVersion)
  ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX ADWAITA
                    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaConfigVersion.cmake"
                    )

  # create a Config.cmake and a ConfigVersion.cmake file and install them
  #set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/Adwaita")

  #ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/AdwaitaConfig.cmake.in"
  #                                  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaConfig.cmake"
  #                                  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
  #                                  )

  #install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaConfig.cmake"
  #               "${CMAKE_CURRENT_BINARY_DIR}/AdwaitaConfigVersion.cmake"
  #        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  #        COMPONENT Devel
  #       )
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

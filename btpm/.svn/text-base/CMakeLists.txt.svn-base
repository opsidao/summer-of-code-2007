PROJECT(btpm)

set(btpm_SOURCES main.cpp iconfinder.cpp mainwindow.cpp models.cpp)
set(btpm_UI_FILES mainwindow.ui)

KDE4_ADD_UI_FILES(btpm_SOURCES ${btpm_UI_FILES})

KDE4_ADD_EXECUTABLE(btpm ${btpm_SOURCES})

KDE4_ADD_LIBRARY(btpmactionplugin btpmactionplugin.cpp)

TARGET_LINK_LIBRARIES(btpm ${KDE4_KPARTS_LIBS} ${KDE4_KIO_LIBS} ${KDE4_SOLID_LIBS} solidcontrol plasma btpmactionplugin)

INSTALL(TARGETS btpm RUNTIME DESTINATION ${BIN_INSTALL_DIR})
INSTALL(TARGETS btpmactionplugin DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES btpmactionplugin.h DESTINATION ${INCLUDE_INSTALL_DIR})
INSTALL(FILES btpmactionplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

ADD_SUBDIRECTORY(actions)


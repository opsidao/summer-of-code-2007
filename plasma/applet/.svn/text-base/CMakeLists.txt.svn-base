PROJECT(plasma_applet)

set(plasma_applet_SRCS bluetoothapplet.cpp visuals.cpp)

kde4_add_plugin(plasma_applet_bluetooth ${plasma_applet_SRCS})
target_link_libraries(plasma_applet_bluetooth ${KDE4_KDECORE_LIBS} plasma solidcontrol ${KDE4_KIO_LIBS})

install(TARGETS plasma_applet_bluetooth DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES bluetooth-monitor-main.svg DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)
install(FILES plasma-bluetooth-default.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


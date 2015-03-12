project(btpm_actions)

kde4_add_plugin(btpm_action_exec btpm_action_exec.cpp)

target_link_libraries(btpm_action_exec btpmactionplugin ${KDE4_KDECORE_LIBS} ${QT_QTGUI_LIBRARY} ${KDE4_KDEUI_LIBRARY} )

INSTALL(TARGETS btpm_action_exec DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES btpm_action_exec.desktop DESTINATION ${SERVICES_INSTALL_DIR})


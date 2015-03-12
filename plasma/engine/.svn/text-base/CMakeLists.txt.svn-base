PROJECT(plasma_engine)

set(plasma_engine_SRCS bluetoothdataengine.cpp bluetoothdataengine_p.cpp)

kde4_add_plugin(plasma_engine_bluetooth ${plasma_engine_SRCS})
target_link_libraries(plasma_engine_bluetooth ${KDE4_KDECORE_LIBS} plasma solidcontrol)

install(TARGETS plasma_engine_bluetooth DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-bluetooth.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


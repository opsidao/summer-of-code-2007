PROJECT(plasma_dbus)

set(plasma_dbus_SRCS main.cpp dbusdataenginemanager.cpp dbusdataengine.cpp)
set(plasma_dbus_proxy_SRCS dataenginemanager.cpp dataengine.cpp)
set(plasma_dbus_proxy_HEADERS dataenginemanager.h dataengine.h)

kde4_add_executable(plasma_dbus ${plasma_dbus_SRCS})
target_link_libraries(plasma_dbus ${KDE4_KDECORE_LIBS} plasma ${KDE4_KIO_LIBS})

kde4_add_library(plasma_dbus_proxy ${plasma_dbus_proxy_SRCS})
target_link_libraries(plasma_dbus_proxy ${KDE4_KDECORE_LIBS} )

install(TARGETS plasma_dbus DESTINATION ${BIN_INSTALL_DIR} )

install(TARGETS plasma_dbus_proxy DESTINATION ${PLUGIN_INSTALL_DIR} )
install(FILES ${plasma_dbus_proxy_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/plasma/dbus/)

configure_file(org.kde.plasma.service.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/org.kde.plasma.service)

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.plasma.service DESTINATION ${DBUS_SERVICES_DIR} )

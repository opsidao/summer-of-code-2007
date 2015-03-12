PROJECT(kbluemon)

set(kbluemon_SOURCES rssidelegate.cpp main.cpp monitor.cpp localDevicesDialog.cpp)
set(kbluemon_UI_FILES mainwindow.ui localDevicesDialog.ui)

KDE4_ADD_UI_FILES(kbluemon_SOURCES ${kbluemon_UI_FILES})

KDE4_ADD_EXECUTABLE(kbluemon ${kbluemon_SOURCES})

TARGET_LINK_LIBRARIES(kbluemon ${KDE4_KIO_LIBS} ${KDE4_SOLID_LIBS} solidcontrol plasma)

INSTALL(TARGETS kbluemon RUNTIME DESTINATION ${BIN_INSTALL_DIR})


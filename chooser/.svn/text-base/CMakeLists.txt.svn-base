PROJECT(bluetoothchooser)

set(bluetoothchooser_SRCS bluetoothchooser.cpp bluetoothchoosermodel.cpp)
set(bluetoothchooser_UI chooser.ui)

KDE4_ADD_UI_FILES(bluetoothchooser_SRCS ${bluetoothchooser_UI})

KDE4_AUTOMOC(${bluetoothchooser_SRCS})

KDE4_ADD_EXECUTABLE(bluetoothchooser ${bluetoothchooser_SRCS})

TARGET_LINK_LIBRARIES(bluetoothchooser ${KDE4_KIO_LIBS} ${KDE4_SOLID_LIBS} solidcontrol)
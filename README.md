KDEBluetooth 4 and BtPM
=======================

Author: Juan González Aguilera 
Mentor: Daniel Gollub

This package provides part of the kdebluetooth for kde4 package which is still a work in progress. 
In this package you will find the following components:
- solid_sdp.diff: This is the patch that was applied to KDE  svn trunk to make use of the bluez 
  Service Discovery Protocol D-Bus API to obtain information about services available in 
  remote bluetooth devices.
- solid_security.diff: This is the patch that was applied to KDE SVN trunk to allow creating 
  Passkey/Authorization agents for pairing/service authorization from a KDE program.
- A Plasma data engine that provides access to all information available on the bluetooth system.
  It provides local adapter, remote device and remote services information.
- A Plasmoid that makes use of that engine to show information about remote device in a fashionable way (Incomplete)
- KBluemon for kde4, re-write of a kde3 application used to monitor remote devices
- a kio_slave to browse bluetooth remote devices and services. The slave is not functional in this package because the KDE kioslave 
  system have been broken until after SoC end.
- A custom dialog to browse remote devices/services.
- Bluetooth Presence Manager, the tool to asign actions to remote device arrival/leave. The version in this package is half done.

You can refer to svn://anonsvn.kde.org/home/kde/branches/work/soc_kdebluetooth4 for a more up to date version.


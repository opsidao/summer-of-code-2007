/*
 *
 *  BTPM icon finder
 *
 *  Copyright (C) 2007  Juan González Aguilera <kde-devel@ka-tet>
 *
 *
 *  This file is part of btpm (Bluetooth Presence Manager).
 *
 *  btpm is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  btpm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with kbluemon; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef _BLUETOOTH_ICON_FINDER_
#define _BLUETOOTH_ICON_FINDER_


#include <QIcon>
#include <QMap>
#include <QObject>

#include <K3Icon>//FIXME when kiconloader is updated

class BluetoothIconFinder : public QObject
{
	Q_OBJECT
	public:
		BluetoothIconFinder();
	public:
		QPixmap findIcon(const QString &major, const QString &minor, K3Icon::Group group =K3Icon::Desktop, K3Icon::StdSizes size = K3Icon::SizeMedium,K3Icon::States state = K3Icon::DefaultState );
};
#endif

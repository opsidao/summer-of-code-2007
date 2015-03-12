/*
 *
 *  BTPM assignable action plugin base
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
#include "btpmactionplugin.h"
BtpmActionPlugin::BtpmActionPlugin(QObject * parent)
	:KLibFactory(parent)
{
}

void BtpmActionPlugin::setLinkedUbi(const QString & ubi)
{
	m_linkedUbi = ubi;
}

QString BtpmActionPlugin::linkedUbi()
{
	return m_linkedUbi;
}

#include "btpmactionplugin.moc"

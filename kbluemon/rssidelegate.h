/*
 *
 *  KDE Blutooth Monitor 
 *
 *  Copyright (C) 2007  Juan González Aguilera <kde-devel@ka-tet>
 *
 *
 *  This file is part of kbluemon.
 *
 *  kbluemon is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  kbluemon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with kbluemon; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __RSSI_DELEGATE
#define __RSSI_DELEGATE

#include <QItemDelegate>
#include <QObject>



class RssiDelegate : public QItemDelegate
{
	Q_OBJECT
	public:
		RssiDelegate(QObject *parent = 0);
		void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;	
};

#endif

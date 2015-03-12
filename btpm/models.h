/*
 *
 *  BTPM widgets
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
#ifndef _MODELS_
#define _MODELS_

#include <kconfig.h>
#include <QAbstractListModel>
#include <QMimeData>

#include <solid/control/bluetoothinterface.h>
#include <solid/control/bluetoothremotedevice.h>
#include <solid/control/bluetoothmanager.h>

#include "iconfinder.h"
#include <QListWidget>

class KnownRemotesListWidget: public QListWidget
{
	Q_OBJECT
	public:
		KnownRemotesListWidget(QWidget * parent = 0);
		~KnownRemotesListWidget();
		Solid::Control::BluetoothRemoteDevice remoteDevice(const QString &ubi);		
	private Q_SLOTS:
		void remoteDeviceFound(const QString& ubi);
		void remoteDeviceDisappeared(const QString&);
		void showOnlyActive(bool onlyActive);
	private:
		QMap<QString,QListWidgetItem*> remotes;
		QMap<QString,bool> remotesStatus;
		BluetoothIconFinder iconfinder;
		Solid::Control::BluetoothInterface *iface;
		bool onlyActive;
};

class BtpmActionPlugin;
class BtpmActionMimeData:public QMimeData
{
	Q_OBJECT
	public:
		BtpmActionMimeData(BtpmActionPlugin * action = 0);
		bool hasFormat ( const QString & mimeType ) const;
		QStringList formats () const;

		BtpmActionPlugin *action;
};


class AvailableActionsListWidget: public QListWidget
{
	Q_OBJECT
	public:
		AvailableActionsListWidget(QWidget * parent = 0);
		QMimeData * mimeData(const QList< QListWidgetItem * > items) const;
	public Q_SLOTS:
		void selectedRemoteChanged( QListWidgetItem * current, QListWidgetItem * previous );
	private:
		QString activeUbi;
		QList<BtpmActionPlugin*> actions;
		QMap<QListWidgetItem *,BtpmActionPlugin*> actionMap;
};

class AssignedActionsListWidget: public QListWidget
{
	Q_OBJECT
	public:
		AssignedActionsListWidget(QWidget * parent = 0);
		QStringList mimeTypes() const;
		bool dropMimeData(int index,const QMimeData *data,Qt::DropAction action);
	public Q_SLOTS:
		void selectedRemoteChanged( QListWidgetItem * current, QListWidgetItem * previous );
	private:
		QMap<QString,QList<BtpmActionPlugin*> > actions;
		QMap<QString,QList<QListWidgetItem*> > items;
};
#endif

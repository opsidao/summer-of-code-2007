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
#ifndef __LOCAL_DEVICES_DIALOG__
#define __LOCAL_DEVICES_DIALOG__

#include "ui_localDevicesDialog.h"
#include <QDialog>
#include <solid/control/bluetoothmanager.h>

class LocalDevicesDialog : public QDialog, public Ui::LocalDevicesDialog
{
	Q_OBJECT
	public:
		LocalDevicesDialog(QWidget *parent);
		virtual ~LocalDevicesDialog();
	private Q_SLOTS:
		//Stay updated with adapter info
		void modeChanged(Solid::Control::BluetoothInterface::Mode);
		void discoverableTimeoutChanged(int timeout);
		void minorClassChanged(const QString &minor);
		void nameChanged(const QString &name);

		
		//GUI related
		void adaptorSelected(int index);
		void enableApply();
		void applyChanges();
	private:
		const Solid::Control::BluetoothManager & mgr;
		Solid::Control::BluetoothInterface *iface;
};

#endif

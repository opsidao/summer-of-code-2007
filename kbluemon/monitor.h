/*
 *
 *  KDE Blutooth Monitor 
 *
 *  Copyright (C) 2007  Tom Patzig <tpatzig@suse.de>
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
#ifndef MONITOR_H
#define MONITOR_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QTimer>


#include <solid/control/bluetoothinterface.h>

#include "ui_mainwindow.h"

#include <plasma/dataengine.h>

class RemoteDevice;
class QStringListModel;
class Monitor : public QMainWindow,public Ui::MainWindow 
{
	Q_OBJECT
	private:		
		Solid::Control::BluetoothInterface *interface;
		QTimer feedbackTimer;
// 		int selectedIndex;
		QStringList knownRemotes;
		void updateServiceTree(const QString &ubi);
	public:
		Monitor();
		~Monitor();

 	private slots:
		void showLocalDevicesDialog();
		void updated(const QString& source,const Plasma::DataEngine::Data &data);
		void newSourceAvailable(const QString &source);
		void sourceRemoved(const QString &source);
		void updateFeedbackBar();
		void remoteSelected(int);
		void updateAlias();
	private:
		Plasma::DataEngine *engine;
};
	
#endif


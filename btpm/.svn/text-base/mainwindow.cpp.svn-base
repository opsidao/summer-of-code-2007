/*
 *
 *  BTPM main window
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
#include "iconfinder.h"
#include "mainwindow.h"
#include "models.h"

#include <kdebug.h>

#include <solid/control/bluetoothremotedevice.h>

MainWindow::MainWindow( QWidget * parent , Qt::WindowFlags flags )
	:QMainWindow(parent,flags)
{
	setupUi(this);
	
	selectedDeviceLabel.setHidden(true);
	statusBar()->insertPermanentWidget(0,&selectedDeviceLabel);
	
	remotesDockWidget->setWidget(new KnownRemotesListWidget(remotesDockWidget));
	availableActionsDockWidget->setWidget(new AvailableActionsListWidget(availableActionsDockWidget));
	availableActionsDockWidget->setEnabled(false);
	assignedActionsDockWidget->setWidget(new AssignedActionsListWidget(assignedActionsDockWidget));
	assignedActionsDockWidget->setEnabled(false);
	
	
	QObject::connect(actionOnly_show_active_devices,SIGNAL(toggled(bool)),
			 remotesDockWidget->widget(),SLOT(showOnlyActive(bool)),Qt::DirectConnection);
	QObject::connect(remotesDockWidget->widget(),SIGNAL( currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			 this,SLOT(selectedRemoteChanged(QListWidgetItem*, QListWidgetItem*)),Qt::DirectConnection);
	QObject::connect(remotesDockWidget->widget(),SIGNAL( currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			 availableActionsDockWidget->widget(),SLOT(selectedRemoteChanged(QListWidgetItem*, QListWidgetItem*)),Qt::DirectConnection);
	QObject::connect(remotesDockWidget->widget(),SIGNAL( currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			 assignedActionsDockWidget->widget(),SLOT(selectedRemoteChanged(QListWidgetItem*, QListWidgetItem*)),Qt::DirectConnection);
	KConfig config;
	bool only_show_active =config.readBoolEntry("only_show_active",true);
	actionOnly_show_active_devices->setChecked(only_show_active);
		
}

void MainWindow::selectedRemoteChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
	Q_UNUSED(previous)
	if(current)
	{
		QString ubi = current->data(Qt::UserRole).toString();
		KnownRemotesListWidget* widget = qobject_cast<KnownRemotesListWidget*>(remotesDockWidget->widget());
		Q_ASSERT(widget);
		Solid::Control::BluetoothRemoteDevice remote = widget->remoteDevice(ubi);
		QString msg=" Remote : <b>" +remote.name() + " </b> ("+remote.address() +") Class:<b>" +remote.majorClass()+"</b>";
		if(remote.majorClass() != remote.minorClass())
			msg+="<b> "+remote.minorClass()+"</b>";
		selectedDeviceLabel.setText(msg);
		selectedDeviceLabel.setHidden(false);
		
		availableActionsDockWidget->setEnabled(true);
		assignedActionsDockWidget->setEnabled(true);
	} else
	{
		selectedDeviceLabel.setHidden(false);
		
		availableActionsDockWidget->setEnabled(false);
		assignedActionsDockWidget->setEnabled(false);
	}
}

#include "mainwindow.moc"

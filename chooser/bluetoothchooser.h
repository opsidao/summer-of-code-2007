//
// C++ Interface: chooser
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BLUETOOTH_CHOOSER
#define __BLUETOOTH_CHOOSER

#include <QDialog>
#include <QStringListModel>
#include <QPair>

#include "ui_chooser.h"
#include "bluetoothchoosermodel.h"

/**
 * Provides a dialog to select an available remote device and a service (not mandatory).
 * When the user hits the ok button, remote device and service selected (if any)
 * will be available using the getSelected() method. 
 * You can check if the user pushed ok or cancel by comparing the result 
 * returned by exec() with KBluetoothChooser::Accepted and KBluetoothChooser::Rejected
 */
class KBluetoothChooser : public QDialog, public Ui::BluetoothChooser
{
	Q_OBJECT
	public:
		KBluetoothChooser(QWidget *parent = 0,Qt::WindowFlags f = 0);
		virtual ~KBluetoothChooser();
		const BluetoothRemoteDevice &selectedRemoteDevice();
		const BluetoothServiceRecord &selectedBluetoothServiceRecord();
	private slots:
		void restartDiscovery();
		
		void updateInterface(const QString &ubi = QString::null);
		
		void discoveryStart();
		void addRemote(const QString &ubi,int,int);
		void discoveryEnd();
		
 		void deviceSelected(const QModelIndex &);
		void serviceSelected(const QModelIndex &);
		
	private:
		BluetoothRemoteModel *devicesModel;
		BluetoothServiceModel *servicesModel;
		
		QList<BluetoothRemoteDevice> remotes;
		
		BluetoothRemoteDevice selectedDevice;
		
		BluetoothServiceRecord selectedService;
		
		bool firstDeviceReceived;
		
};
#endif

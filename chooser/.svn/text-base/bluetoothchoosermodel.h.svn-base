//
// C++ Interface: remotedevicemodel
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __REMOTE_DEVICE_MODEL
#define __REMOTE_DEVICE_MODEL

#include <QAbstractListModel>

#include <solid/control/bluetoothremotedevice.h>

using namespace Solid::Control;

typedef QList<BluetoothServiceRecord> BluetoothServiceRecordList;
class BluetoothServiceModel: public QAbstractListModel
{
	Q_OBJECT
	public:
		BluetoothServiceModel(QObject *parent = 0);
		
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role) const;
		
		void setCurrentRemote(BluetoothRemoteDevice remote);
		BluetoothServiceRecord &getService(const QModelIndex &index);
		int countServices();
		void clearAllServices();
	public slots:
		void serviceDiscoveryStarted(const QString &ubi);
		void remoteServiceFound(const QString &ubi, const Solid::Control::BluetoothServiceRecord &service);
		void serviceDiscoveryFinished(const QString &ubi);	
	private:
		BluetoothRemoteDevice currentRemote;
		BluetoothServiceRecordList currentServices;
		
		QList<BluetoothRemoteDevice> remotes;
		QMap<QString,BluetoothServiceRecordList> services;
		QList<BluetoothRemoteDevice *> deviceReferences;
};

class BluetoothRemoteModel : public QAbstractListModel
{
	Q_OBJECT
	public:
		BluetoothRemoteModel(QObject *parent = 0,BluetoothServiceModel *serviceModel = 0);
		
		void addRemote(const BluetoothRemoteDevice &remote);
		const BluetoothRemoteDevice &getRemote(const QModelIndex &index);
		void clearRemotes();
		int deviceCount();
		
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role) const;	
	private:
		BluetoothRemoteDeviceList devices;
		BluetoothServiceModel *serviceModel;	
// 		QList<BluetoothRemoteDevice *> deviceReferences;
};


#endif

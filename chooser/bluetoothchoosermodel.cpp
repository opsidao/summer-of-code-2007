//
// C++ Implementation: remotedevicemodel
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "bluetoothchoosermodel.h"
#include <kdebug.h>

BluetoothRemoteModel::BluetoothRemoteModel(QObject * parent,BluetoothServiceModel *serviceModel) 
	: QAbstractListModel(parent),serviceModel(serviceModel)
{	
}

int BluetoothRemoteModel::rowCount(const QModelIndex &) const
{	
	return devices.size();
}

QVariant BluetoothRemoteModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= devices.size() || devices.size()==0)
		return QVariant();

	if (role == Qt::DisplayRole)
		return devices.at(index.row()).name()+" ("+devices.at(index.row()).address()+")";
	else
		return QVariant();
	
}

void BluetoothRemoteModel::addRemote(const BluetoothRemoteDevice & remote)
{
	if(!devices.contains(remote))
	{
		beginInsertRows(QModelIndex(),devices.size(),devices.size()+1);
		devices << remote;
		endInsertRows();
	}
}

void BluetoothRemoteModel::clearRemotes()
{
	if(devices.size() > 0)
	{
		beginRemoveRows(QModelIndex(),0,devices.size()-1);
		devices.clear();
		endRemoveRows();
	}
}

const BluetoothRemoteDevice & BluetoothRemoteModel::getRemote(const QModelIndex & index)
{
	return devices.at(index.row());
}

int BluetoothRemoteModel::deviceCount()
{
	return devices.size();
}

BluetoothServiceModel::BluetoothServiceModel(QObject * parent)
	:QAbstractListModel(parent)
{
}

int BluetoothServiceModel::rowCount(const QModelIndex &) const
{
	return currentServices.size();
}

QVariant BluetoothServiceModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int size = currentServices.size();
	if (index.row() >= size || size==0)
		return QVariant();

	if (role == Qt::DisplayRole)
		return currentServices[index.row()].name;
	else
		return QVariant();
}

void BluetoothServiceModel::setCurrentRemote(BluetoothRemoteDevice remote)
{
	currentRemote = remote;
	if(!remotes.contains(currentRemote))
	{
		remotes << currentRemote;
	}
	deviceReferences << new BluetoothRemoteDevice(remote);
	QObject::connect(deviceReferences.last(),SIGNAL(remoteServiceFound(const QString&, const Solid::Control::BluetoothServiceRecord&)),
			 this,SLOT(remoteServiceFound(const QString&, const Solid::Control::BluetoothServiceRecord&)));
	QObject::connect(deviceReferences.last(),SIGNAL(serviceDiscoveryStarted(const QString &)),
			 this,SLOT(serviceDiscoveryStarted(const QString &)));
	QObject::connect(deviceReferences.last(),SIGNAL(serviceDiscoveryFinished(const QString &)),
			 this,SLOT(serviceDiscoveryFinished(const QString &)));
	deviceReferences.last()->findServices();
}

void BluetoothServiceModel::remoteServiceFound(const QString &ubi, const Solid::Control::BluetoothServiceRecord &service)
{
	BluetoothServiceRecordList s = services[ubi];
	s << service;
	services[ubi] = s;
	
	if(currentRemote.ubi() == ubi)
	{
		beginInsertRows(QModelIndex(),s.size(),s.size()+1);
		currentServices = s;
		endInsertRows();		
	}
}

int BluetoothServiceModel::countServices()
{
	return currentServices.size();
}

BluetoothServiceRecord & BluetoothServiceModel::getService(const QModelIndex & index)
{
	return currentServices[index.row()];
}

void BluetoothServiceModel::clearAllServices()
{
	services.clear();
	remotes.clear();
	currentRemote = BluetoothRemoteDevice();
	beginRemoveRows(QModelIndex(),0,currentServices.size());
	currentServices.clear();
	endRemoveRows();
}

void BluetoothServiceModel::serviceDiscoveryStarted(const QString & ubi)
{
// 	kDebug() << k_funcinfo << endl;
	services[ubi] = BluetoothServiceRecordList();
}

void BluetoothServiceModel::serviceDiscoveryFinished(const QString & ubi)
{
// 	kDebug() << k_funcinfo << " ubi: " << ubi << endl;
	for(int i = 0; i < deviceReferences.size(); i++)
	{
		BluetoothRemoteDevice *d = deviceReferences[i];
		if(d->ubi() == ubi)
		{
			QObject::disconnect(d,SIGNAL(remoteServiceFound(const QString&, const Solid::Control::BluetoothServiceRecord&)),
					    this,SLOT(remoteServiceFound(const QString&, const Solid::Control::BluetoothServiceRecord&)));
			QObject::disconnect(d,SIGNAL(serviceDiscoveryStarted(const QString &)),
					    this,SLOT(serviceDiscoveryStarted(const QString &)));
			QObject::disconnect(d,SIGNAL(serviceDiscoveryFinished(const QString &)),
					    this,SLOT(serviceDiscoveryFinished(const QString &)));
			deviceReferences.removeAt(i);
			d->deleteLater();
			break;
		}
	}
}
#include "bluetoothchoosermodel.moc"

/*  This file is part of the KDE project
    Copyright (C) 2007 Juan González Aguilera <kde_devel@opsiland.info>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
*/

#include "bluetoothdataengine_p.h"

#include <kdebug.h>

using namespace Solid::Control;

bool SdpXmlHandler::startElement(const QString & , const QString & localName, const QString & , const QXmlAttributes & atts)
{
//TODO: Should also parse:
//BluetoothServiceRecordState 0x0002 32-bit unsigned integer
//ServiceID 0x0003 UUID
//BrowseGroupList 0x0005 Data Element Sequence
//ServiceInfoTimeToLive 0x0007 32-bit unsigned integer
//ServiceAvailability 0x0008 8-bit unsigned intege
//DocumentationURL 0x000A URL
//ClientExecutableURL 0x000B URL
//IconURL 0x000C URL
	if(localName=="attribute" && atts.count()==1)
		lastAttribute = atts.value("id");
	if(lastAttribute=="0x0000" && localName == "uint32")//Mandatory
		record.handle = atts.value("value");
	else if(lastAttribute=="0x0001" && localName == "uuid")//Mandatory
		record.serviceClasses << atts.value("value");
	else if(lastAttribute=="0x0004")
	{
		if(localName == "uuid")
			record.protocolDescriptors << atts.value("value");
		else if(localName == "uint8")
			record.protocolChannels[record.protocolDescriptors.last()] = atts.value("value");
	} 
	else if(lastAttribute=="0x0006" && localName == "uint16")
		record.langBaseAttributes << atts.value("value");
	else if(lastAttribute=="0x0009")
	{
		if(localName == "uuid")
			record.profileDescriptors << atts.value("value");
		else if(localName == "uint16")
			record.profileVersions[record.profileDescriptors.last()] = atts.value("value");
	}
	else if(lastAttribute=="0x0100" && localName == "text")
		record.name = atts.value("value");
	return true;
}

bool SdpXmlHandler::endElement(const QString & , const QString & localName, const QString & )
{
	if(localName=="attribute")
		lastAttribute = "";
	return true;
}

bool SdpXmlHandler::endDocument()
{
	lastAttribute="";
	return true;
}

bool SdpXmlHandler::startDocument()
{
	record = BluetoothServiceRecord();
	return true;
}

BluetoothEnginePrivate::BluetoothEnginePrivate(QObject *parent)
	: QObject(parent),doServiceSearch(false),interface(0),serviceSearchRunning(false)
{
	const Solid::Control::BluetoothManager &mgr = BluetoothManager::self();
	QString adapterUbi = mgr.defaultInterface();
	if(!adapterUbi.isEmpty())
	{
		// Keep information about local devices up to date
		QObject::connect(&mgr,SIGNAL(defaultInterfaceChanged(const QString&)),
				  this,SLOT(defaultInterfaceChanged(const QString&)));
		QObject::connect(&mgr,SIGNAL(interfaceAdded(const QString&)),
				  this,SLOT(interfaceAdded(const QString&)));
		QObject::connect(&mgr,SIGNAL(interfaceRemoved(const QString&)),
				  this,SLOT(interfaceRemoved(const QString&)));

		interface = new BluetoothInterface(mgr.findBluetoothInterface(adapterUbi));
		interfaceAdded(adapterUbi);
		QObject::connect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
				 this, SLOT(remoteDeviceFound( const QString&, int, int )));
		QObject::connect(interface,SIGNAL(remoteDeviceDisappeared( const QString& )),
				 this, SLOT(remoteDeviceDisappeared( const QString& )));
		QObject::connect(&serviceSearchTimer,SIGNAL(timeout()),
				  this,SLOT(searchServices()));
		serviceSearchTimer.start(100);
		interface->startPeriodicDiscovery();
	} else
	{
		kDebug() << "WARNING: No adapters found" << endl;
	}
}
BluetoothEnginePrivate::~ BluetoothEnginePrivate()
{
	kDebug() << k_funcinfo << endl;
	if(interface)
		interface->stopPeriodicDiscovery();
	delete interface;
}
void BluetoothEnginePrivate::remoteDeviceFound(const QString & ubi, int deviceClass, int rssi)
{
	Q_UNUSED(deviceClass);
	Q_UNUSED(rssi);
// kDebug() << k_funcinfo << " ubi: " << ubi << endl;
	QMap<QString,QString> props;
	BluetoothRemoteDevice remote = interface->findBluetoothRemoteDevice(ubi);
	if(!knownUbis.contains(ubi)) //Add all data
	{
		knownUbis << ubi;
		props["address"] = remote.address();
		props["name"] = remote.name();
		props["rssi"] = QString::number(rssi);
		props["alias"] = remote.alias();
		props["version"] = remote.version();
		props["connected"] = remote.isConnected() ? "true":"false";
		props["revision"] = remote.revision();
		props["manufacturer"] = remote.manufacturer();
		props["company"] = remote.company();
		props["majorclass"] = remote.majorClass();
		props["minorclass"] = remote.minorClass();
		props["lastseen"] = remote.lastSeen();
		props["lastused"] = remote.lastUsed();
		props["bonded"] = remote.hasBonding();
		props["category"] = remote.majorClass() +"/"+remote.minorClass();
		if(doServiceSearch)
		{
			BluetoothRemoteDevice *remref = new BluetoothRemoteDevice(remote);
			remotesInSearch.append(remref);
			QObject::connect(remref,SIGNAL(serviceHandlesAvailable(const QString &, const QList<uint> &)),
					 this,SLOT(serviceHandlesAvailable(const QString&, const QList<uint>&)));
			QObject::connect(remref,SIGNAL(serviceRecordXmlAvailable(const QString &, const QString &)),
					 this,SLOT(serviceRecordXmlAvailable(const QString&, const QString&)));

			serviceHandleRequests << QPair<QString,QString>(ubi,"");
			QTimer::singleShot(0,this,SLOT(searchServices()));
		}
	} else //Only update things that can change
	{
		props["name"] = remote.name();
		props["rssi"] = QString::number(rssi);
		props["alias"] = remote.alias();
		props["connected"] = remote.isConnected() ? "true":"false";
		props["lastseen"] = remote.lastSeen();
		props["lastused"] = remote.lastUsed();
		props["bonded"] = remote.hasBonding() ? "true":"false";
	}
	emit remoteDeviceArrived(ubi,props);
}

void BluetoothEnginePrivate::remoteDeviceDisappeared(const QString & ubi)
{
// kDebug() << k_funcinfo << " ubi: " << ubi << endl;
	QMutexLocker locker(&runningMutex);
	if(knownUbis.contains(ubi))
	{
		foreach(BluetoothRemoteDevice *remote,remotesInSearch)
		{
			if(remote->ubi() == ubi)
			{
				remotesInSearch.removeAll(remote);
				QObject::disconnect(remote,SIGNAL(serviceHandlesAvailable(const QString &, const QList<uint> &)),
						this,SLOT(serviceHandlesAvailable(const QString&, const QList<uint>&)));
				QObject::disconnect(remote,SIGNAL(serviceRecordXmlAvailable(const QString &, const QString &)),
						this,SLOT(serviceRecordXmlAvailable(const QString&, const QString&)));
				delete remote;
				break;
			}
		}
		knownUbis.removeAll(ubi);
		emit remoteDeviceLeft(ubi);
	}
}

void BluetoothEnginePrivate::serviceRecordXmlAvailable(const QString &ubi, const QString &record)
{
	Q_UNUSED(record)
// kDebug() << k_funcinfo << " ubi: " << ubi << endl;
			QMutexLocker locker(&runningMutex);
	if(!record.isEmpty())
	{
		xmlReader.setContentHandler(&handler);
		xmlReader.setErrorHandler(&handler);

		input.setData(record);
		xmlReader.parse(input); //Result will be available on handler.record

		serviceSearchRequests.dequeue();
		if(!serviceSearchRequests.isEmpty() || !serviceHandleRequests.isEmpty())
			QTimer::singleShot(0,this,SLOT(searchServices()));

		BluetoothServiceRecord bsr = handler.record;
		QList<BluetoothServiceRecord> records;
		records << bsr;
		emit remoteDeviceArrived(ubi,QMap<QString,QString>(),records);
	}
	serviceSearchRunning=false;
}

void BluetoothEnginePrivate::searchServices()
{
	QMutexLocker locker(&runningMutex);
	if(!serviceSearchRunning)
	{
		if(!serviceHandleRequests.isEmpty())
		{
			QString ubi = serviceHandleRequests.head().first;
			QString handle = serviceHandleRequests.head().second;
			serviceHandleRequests.dequeue();
			BluetoothRemoteDevice *remote = 0;
			foreach(BluetoothRemoteDevice *r,remotesInSearch)
			{
				if(r->ubi() == ubi)
				{
					remote=r;
					break;
				}
			}
			Q_ASSERT(remote);
			serviceSearchRunning=true;
			remote->serviceHandles();
		} else if(!serviceSearchRequests.isEmpty())
		{
			QString ubi = serviceSearchRequests.head().first;
			uint handle = serviceSearchRequests.head().second;
			BluetoothRemoteDevice *remote = 0;
			foreach(BluetoothRemoteDevice *r,remotesInSearch)
			{
				if(r->ubi() == ubi)
				{
					remote=r;
					break;
				}
			}
			Q_ASSERT(remote);
			serviceSearchRunning=true;
			remote->serviceRecordAsXml(handle);
		}
	}
}

void BluetoothEnginePrivate::serviceHandlesAvailable(const QString & ubi, const QList< uint > & handles)
{
	QMutexLocker locker(&runningMutex);
	foreach(uint handle,handles)
	{
		serviceSearchRequests << QPair<QString,uint>(ubi,handle);
	}
	serviceSearchRunning=false;
}

QStringList BluetoothEnginePrivate::localDevices()
{
// kDebug() << k_funcinfo << endl;
	const Solid::Control::BluetoothManager &mgr = BluetoothManager::self();
	BluetoothInterfaceList ifaces = mgr.bluetoothInterfaces();
	QStringList ifacesUbis;
	foreach(BluetoothInterface iface, ifaces)
	{
		ifacesUbis << iface.ubi();
	}
	return ifacesUbis;
}

void BluetoothEnginePrivate::defaultInterfaceChanged(const QString & ubi)
{
	kDebug() << k_funcinfo << " ubi: " << ubi <<  endl;
	if(interface)
	{
		if(interface->isPeriodicDiscoveryActive())
			interface->stopPeriodicDiscovery();
		delete interface;
		interface = 0;
	}
	if(!ubi.isEmpty())
	{
		const Solid::Control::BluetoothManager &mgr = BluetoothManager::self();
		interface = new BluetoothInterface(mgr.findBluetoothInterface(ubi));
		QObject::connect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
				 this, SLOT(remoteDeviceFound( const QString&, int, int )));
		QObject::connect(interface,SIGNAL(remoteDeviceDisappeared( const QString& )),
				 this, SLOT(remoteDeviceDisappeared( const QString& )));
		QObject::connect(&serviceSearchTimer,SIGNAL(timeout()),
				  this,SLOT(searchServices()));
	}
}

void BluetoothEnginePrivate::interfaceAdded(const QString & ubi)
{
	kDebug() << k_funcinfo << " ubi: " << ubi <<  endl;
	QStringList devices;
	const Solid::Control::BluetoothManager &mgr = BluetoothManager::self();
	foreach(Solid::Control::BluetoothInterface iface,mgr.bluetoothInterfaces())
	{
		devices << iface.ubi();
	}
	emit localDevicesChanged(devices);
}

void BluetoothEnginePrivate::interfaceRemoved(const QString & ubi)
{
	kDebug() << k_funcinfo << " ubi: " << ubi <<  endl;
	QStringList devices;
	const Solid::Control::BluetoothManager &mgr = BluetoothManager::self();
	foreach(Solid::Control::BluetoothInterface iface,mgr.bluetoothInterfaces())
	{
		devices << iface.ubi();
	}
	emit localDevicesChanged(devices);
}

#include "bluetoothdataengine_p.moc"

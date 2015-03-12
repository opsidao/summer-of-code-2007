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
#ifndef __BLUETOOTH_ENGINE_PRIVATE
#define __BLUETOOTH_ENGINE_PRIVATE
#include <QThread>
#include <QQueue>

#include <solid/control/bluetoothinterface.h>
#include <solid/control/bluetoothremotedevice.h>
#include <solid/control/bluetoothmanager.h>

#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include <QStringList>


class BluetoothServiceRecord
{
	public:
		QString state;
		QString name;
		QString handle;
		QStringList serviceClasses;
		QStringList langBaseAttributes;
		
		QStringList protocolDescriptors;
		QMap<QString,QString> protocolChannels;
		
		QStringList profileDescriptors;
		QMap<QString,QString> profileVersions;
};
class SdpXmlHandler: public QXmlDefaultHandler
{
	private:
		QString lastAttribute;
	public:
		bool startDocument();
		bool endDocument ();
		bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
		bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
		
		BluetoothServiceRecord record;
};
class BluetoothEnginePrivate: public QObject
{
	Q_OBJECT
	public:
		explicit BluetoothEnginePrivate(QObject *parent=0);
		~BluetoothEnginePrivate();
		QStringList localDevices();
		bool doServiceSearch;
	private:
		Solid::Control::BluetoothInterface *interface;
		QStringList knownUbis;
		QList<Solid::Control::BluetoothRemoteDevice*> remotesInSearch;
		QQueue< QPair<QString,uint> > serviceSearchRequests;
		QQueue< QPair<QString,QString> > serviceHandleRequests;
		SdpXmlHandler handler;
		QXmlSimpleReader xmlReader;
		QXmlInputSource input;
		QTimer serviceSearchTimer;
		bool serviceSearchRunning;
		QMutex runningMutex;
	Q_SIGNALS:
		void localDevicesChanged(const QStringList &devices);
		void remoteDeviceArrived(const QString &ubi,const QMap<QString,QString> &properties,const QList<BluetoothServiceRecord> &services = QList<BluetoothServiceRecord>());
		void remoteDeviceLeft(const QString &ubi);
	public Q_SLOTS:
		void searchServices();
                
		void interfaceAdded(const QString  & ubi);
		void interfaceRemoved(const QString  & ubi);
		void defaultInterfaceChanged(const QString  & ubi);
                
		void remoteDeviceFound(const QString &ubi, int deviceClass, int rssi);
		void remoteDeviceDisappeared(const QString &ubi);
		void serviceHandlesAvailable(const QString &ubi, const QList<uint> &handles);
		void serviceRecordXmlAvailable(const QString &ubi, const QString &record);
};

#endif

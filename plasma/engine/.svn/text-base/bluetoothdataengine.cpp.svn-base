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
#include "bluetoothdataengine.h"
#include "bluetoothdataengine_p.h"
#include <kdebug.h>
#include <QThread>
#include <QTimer>
#include <plasma/datacontainer.h>
BluetoothEngine::BluetoothEngine(QObject * parent,const QStringList &):
		Plasma::DataEngine(parent),privateEngine(new BluetoothEnginePrivate(this))
{
	kDebug() << k_funcinfo << endl;
}

BluetoothEngine::~ BluetoothEngine()
{
	kDebug() << k_funcinfo << endl;
}
void BluetoothEngine::init()
{
// 	kDebug() << k_funcinfo << endl;
	QObject::connect(privateEngine,SIGNAL(remoteDeviceArrived(const QString&,const QMap<QString,QString> &,const QList<BluetoothServiceRecord> &)),
			 this,SLOT(remoteDeviceArrived(const QString&,const QMap<QString,QString> &,const QList<BluetoothServiceRecord> &)));
	QObject::connect(privateEngine,SIGNAL(remoteDeviceLeft(const QString&)),
			 this,SLOT(remoteDeviceLeft(const QString&)));
}

void BluetoothEngine::localDevicesChanged(const QStringList & devices)
{
    if(devices.isEmpty())
        clearData("adapters");
    else
    {
        QStringList inEngine = query("adapters").keys();
        if(inEngine != devices)
        {
            clearData("adapters");
            foreach(QString device,devices)
                setData("adapters",device);
        }
    }
}

void BluetoothEngine::remoteDeviceArrived(const QString & ubi,const QMap<QString,QString> &properties,const QList<BluetoothServiceRecord> &services)
{
// 	kDebug() << k_funcinfo << endl;
	if(!properties.isEmpty())
		setData("devices",ubi,properties["name"]);
	foreach(QString prop, properties.keys())
	{
		setData(ubi,prop,properties[prop]);
	}
	foreach(BluetoothServiceRecord service, services)
	{
		QString serviceId = ubi+"/"+service.handle;
		
		setData(ubi+"/services",serviceId,service.name);
		
		setData(serviceId,"name",service.name);
		setData(serviceId,"handle",service.handle);
		setData(serviceId,"state",service.state);
		setData(serviceId,"classes",service.serviceClasses);
		setData(serviceId,"langBaseAttributes",service.langBaseAttributes);
		if(service.protocolDescriptors.size() > 0)
		{
			serviceId = ubi+"/"+service.handle + "/protocols";
			foreach(QString p, service.protocolDescriptors)
			{
				QString channel =service.protocolChannels.contains(p)? service.protocolChannels[p]:"";
				setData(serviceId,p,channel);
			}
		}
		if(service.profileDescriptors.size() > 0)
		{
			serviceId = ubi+"/"+service.handle + "/profiles";
			foreach(QString p, service.profileDescriptors)
			{
				QString version =service.profileVersions.contains(p) ? service.profileVersions[p]:"";
				setData(serviceId,p,version);
			}
		}
	}
}

void BluetoothEngine::remoteDeviceLeft(const QString & ubi)
{
// 	kDebug() << k_funcinfo << endl;
	removeData("devices",ubi);
	QStringList srcs = sources();
	foreach(QString source, srcs)
	{
		if(source.startsWith(ubi))
			removeSource(source);
	}
	checkForUpdates();
}

bool BluetoothEngine::sourceRequested(const QString & source)
{
// 	kDebug() << k_funcinfo << " source : " << source << endl;
	if(source =="adapters")
	{
		foreach(QString localDevice, privateEngine->localDevices())
		{
			setData("adapters",localDevice,"");
		}
		return true;
	} else if(source == "devices")
	{
		Plasma::DataContainer *devices = new Plasma::DataContainer;
		addSource(devices);
		return true;
	}
	return false;
}

void BluetoothEngine::setRemoteToUpdate(const QString & remote)
{
	//FIXME I don't really like this
	kDebug() << k_funcinfo << endl;
	privateEngine->remoteDeviceFound(remote,0,0);
}

QString BluetoothEngine::remoteToUpdate() const
{
	//This is needed by the property system, but not used
	return QString();
}

bool BluetoothEngine::doServiceSearch()
{
	return privateEngine->doServiceSearch;
}

void BluetoothEngine::setDoServiceSearch(bool doServiceSearch)
{
	privateEngine->doServiceSearch = doServiceSearch;
}



#include "bluetoothdataengine.moc"


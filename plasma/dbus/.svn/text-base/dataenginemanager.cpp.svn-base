//
// C++ Implementation: dataenginemanager
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dataenginemanager.h"
#include <kdebug.h>
Plasma::DBus::DataEngineManager::DataEngineManager(QObject * parent)
	:QObject(parent)
{
	iface=new QDBusInterface("org.kde.Plasma","/DataEngineManager","org.kde.Plasma.DataEngineManager");
}
Plasma::DBus::DataEngineManager::~ DataEngineManager()
{
	delete iface;
}
QStringList Plasma::DBus::DataEngineManager::knownEngines()
{
	QDBusReply<QStringList> reply = iface->call("knownEngines");
	if(iface->lastError().isValid())
		kDebug() << "Error: " << iface->lastError().name() << ":" << iface->lastError().message() << endl;
	return reply.value();	
}

QString Plasma::DBus::DataEngineManager::dataEngine(const QString & name)
{
	QDBusReply<QString> reply = iface->call("dataEngine",name);
	if(iface->lastError().isValid())
		kDebug() << "Error: " << iface->lastError().name() << ":" << iface->lastError().message() << endl;
	return reply.value();
}

bool Plasma::DBus::DataEngineManager::loadDataEngine(const QString & name)
{
	QDBusReply<bool> reply = iface->call("loadDataEngine",name);
	if(iface->lastError().isValid())
		kDebug() << "Error: " << iface->lastError().name() << ":" << iface->lastError().message() << endl;
	return reply.value();
}

void Plasma::DBus::DataEngineManager::unloadDataEngine(const QString & name)
{
	iface->call("unloadDataEngine",name);
}

#include "dataenginemanager.moc"



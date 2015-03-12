//
// C++ Implementation: plasmadbusinterface
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dbusdataenginemanager.h"
#include "dbusdataengine.h"

Plasma::DBus::DBusDataEngineManager::DBusDataEngineManager(Plasma::DataEngineManager *mgr)
	:QDBusAbstractAdaptor(mgr),engineManager(mgr)
{
}

QStringList Plasma::DBus::DBusDataEngineManager::knownEngines()
{
	return engineManager->knownEngines();
}

QString Plasma::DBus::DBusDataEngineManager::dataEngine(const QString & name)
{
	Plasma::DataEngine *engine = engineManager->dataEngine(name);
	QString out;
	if(engine->isValid())
	{
		out = QString("/DataEngines/%1").arg(name);
	}
	return out;
}

bool Plasma::DBus::DBusDataEngineManager::loadDataEngine(const QString & name)
{
	Plasma::DataEngine *engine = engineManager->loadDataEngine(name);
	if(engine->isValid())
	{
		bool done = QDBusConnection::sessionBus().registerObject(
					    QString("/DataEngines/%1").arg(name),
	 				    new Plasma::DBus::DBusDataEngine(name,engine),
					    QDBusConnection::ExportAllContents);
		if(done)
			kDebug() << "DataEngine with name " << name << " has been registered in the bus" << endl;
		else
			kDebug() << "Couldn't register the " << name << " DataEngine" << endl;
		return done;
	} 
	return false;
}

void Plasma::DBus::DBusDataEngineManager::unloadDataEngine(const QString & name)
{
	QDBusConnection::sessionBus().unregisterObject(QString("/DataEngines/%1").arg(name));
	engineManager->unloadDataEngine(name);
}

#include "dbusdataenginemanager.moc"

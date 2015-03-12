//
// C++ Implementation: dataengine
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dbusdataengine.h"

Plasma::DBus::DBusDataEngine::DBusDataEngine(const QString &name,Plasma::DataEngine * engine)
	:QDBusAbstractAdaptor(engine),engine(engine),name(name),path("/DataEngines/"+name)
{
	QObject::connect(engine,SIGNAL(newSource(const QString&)),
			 SIGNAL(newSource(const QString&)));
	QObject::connect(engine,SIGNAL(sourceRemoved(const QString&)),
			 SIGNAL(sourceRemoved(const QString&)));
}

QStringList Plasma::DBus::DBusDataEngine::sources()
{
	return engine->sources();
}
void Plasma::DBus::DBusDataEngine::connectSource(const QString & source)
{
	QDBusConnection conn("");
	QMetaObject::invokeMethod(
			QObject::parent(),
			"connection",
			Qt::DirectConnection,
			Q_RETURN_ARG(QDBusConnection, conn));
	if(!sourcesConnected.contains(source))
	{
		QList<QDBusConnection> newList;
		newList << conn;
		sourcesConnected[source] = newList;
		engine->connectSource(source,this);
	} else
	{
		sourcesConnected[source] << conn;
	}	
}

void Plasma::DBus::DBusDataEngine::disconnectSource(const QString & source)
{
	if(sourcesConnected.contains(source))
	{
		engine->disconnectSource(source,this);
		sourcesConnected.remove(source);
	}
}

void Plasma::DBus::DBusDataEngine::connectAllSources()
{
	QDBusConnection conn("");
	QMetaObject::invokeMethod(
			QObject::parent(),
			"connection",
			Qt::DirectConnection,
			Q_RETURN_ARG(QDBusConnection, conn));
	kDebug() << "Connection : " << conn.baseService() << endl;
	foreach(QString source,engine->sources())
	{
		if(!sourcesConnected.contains(source))
		{
			QList<QDBusConnection> newList;
			newList << conn;
			sourcesConnected[source] = newList;
			engine->connectSource(source,this);
		} else
		{
			sourcesConnected[source] << conn;
		}
	}
}

QMap< QString, QVariant > Plasma::DBus::DBusDataEngine::query(const QString & source)
{
	QMap<QString,QVariant> out;
	Plasma::DataEngine::Data data = engine->query(source);
	kDebug() << "query reply size : " << data.keys().size() << endl;
	foreach(QString key,data.keys())
	{
		out[key] = data[key];
	}
	return out;
}

void Plasma::DBus::DBusDataEngine::updated(const QString & source, const Plasma::DataEngine::Data & data)
{
	kDebug() << k_funcinfo << " source: " << source << endl;
	QMap<QString,QVariant> out;	
	foreach(QString key,data.keys())
	{
		out[key] = data[key];
	}
	emit updated(source,out);
}

#include "dbusdataengine.moc"

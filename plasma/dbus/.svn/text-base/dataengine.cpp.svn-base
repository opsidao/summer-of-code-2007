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
#include "dataengine.h"

Plasma::DBus::DataEngine::DataEngine(const QString & name, QObject * parent)
	:QObject(parent),name(name)
{
	iface = new QDBusInterface("org.kde.Plasma","/DataEngines/"+name,"org.kde.Plasma.DataEngine");
	connect(iface,SIGNAL(updated(const QString& , const QVariantMap& )),
		this,SLOT(updated(const QString&, const QVariantMap&)));
}

QStringList Plasma::DBus::DataEngine::sources()
{
	QDBusReply<QStringList> reply = iface->call("sources");
	return reply.value();
}

void Plasma::DBus::DataEngine::connectSource(const QString & source, QObject * visualizer)
{
	const QMetaObject *meta = visualizer->metaObject();
	if(meta->indexOfSlot(SLOT(updated(const QString&, const Plasma::DataEngine::Data&)))!= -1)
	{	
		iface->call("connectSource",source);
		if(!visualizers.contains(source))
			visualizers.insert(source,QList<QObject*>());
		visualizers[source].append(visualizer);
	} else {
		kDebug() << "Class " << meta->className() << " doesn't have a valid updated(const QString&, const Plasma::DataEngine::Data&)) slot" << endl;
	}
}

void Plasma::DBus::DataEngine::disconnectSource(const QString & source, QObject * visualizer)
{
	if(visualizers.contains(source))
		visualizers[source].removeAll(visualizer);
}

void Plasma::DBus::DataEngine::connectAllSources(QObject * visualizer)
{
	foreach(QString s,sources())
	{
		connectSource(s,visualizer);
	}
}

Plasma::DataEngine::Data Plasma::DBus::DataEngine::query(const QString & source)
{
	QDBusReply< QMap<QString,QVariant> > reply = iface->call("query",source);
	QMap<QString,QVariant> map = reply.value();
	Plasma::DataEngine::Data data;
	foreach(QString k,map.keys())
	{
		data[k] = map[k];
	}
	return data;
}

void Plasma::DBus::DataEngine::updated(const QString & source, const QVariantMap & data)
{
	Plasma::DataEngine::Data newData;
	foreach(QString k,data.keys())
	{
		newData[k] = data[k];
	}
	if(visualizers.contains(source))
	{
		foreach(QObject *vis,visualizers[source])
		{
			if(!QMetaObject::invokeMethod(
					vis,
					SLOT(updated(const QString&, const Plasma::DataEngine::Data&)),
					Qt::DirectConnection,
					Q_ARG(QString, source),
					Q_ARG(Plasma::DataEngine::Data, newData)
						    ))
			{
				kDebug() << k_funcinfo << "Failed to call updated slot" << endl;
			}
		}
	}
// 	QMetaObject::
}

#include "dataengine.moc"

//
// C++ Interface: dataengine
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __DBUS_PLASMA_DATAENGINE
#define __DBUS_PLASMA_DATAENGINE

#include <QtDBus>
#include <plasma/dataengine.h>

namespace Plasma
{
	namespace DBus
	{
		class DataEngine :public QObject
		{
			Q_OBJECT
			public:
				DataEngine(const QString &name,QObject *parent = 0);
			public slots:
				QStringList sources();		
				void connectSource(const QString & source,QObject *visualizer);
				void disconnectSource(const QString & source,QObject *visualizer);
				void connectAllSources(QObject *visualizer);
				Plasma::DataEngine::Data query(const QString& source);
			private slots:
				void updated(const QString& source, const QVariantMap& data);
			signals:
				void updated(const QString& source, const Plasma::DataEngine::Data& data);
				void newSource(const QString& source);
				void sourceRemoved(const QString& source);
			private:
				QDBusInterface *iface;
				QString name;
				QMap<QString,QList<QObject *> > visualizers;
		};
	}; //namespace DBus
}; //namespace Plasma
#endif


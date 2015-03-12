//
// C++ Interface: dataenginemanager
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __DBUS_PLASMA_DATAENGINEMANAGER
#define __DBUS_PLASMA_DATAENGINEMANAGER

#include <QtDBus>
namespace Plasma
{
	namespace DBus
	{			
		class DataEngineManager: public QObject
		{
			Q_OBJECT					
			Q_PROPERTY(QStringList knownEngines READ knownEngines);
			public:
				explicit DataEngineManager(QObject *parent = 0);
				~DataEngineManager();
				QStringList knownEngines();
			public slots:
				QString dataEngine(const QString &name);
				bool loadDataEngine(const QString &name);
				void unloadDataEngine(const QString &name);
			private:
				QDBusInterface *iface;
		};
	}; //Namespace DBus
}; //Namespace Plasma
#endif


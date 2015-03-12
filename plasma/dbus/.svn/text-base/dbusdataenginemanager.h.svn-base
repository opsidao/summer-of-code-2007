//
// C++ Interface: plasmadbusinterface
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __DBUS_PLASMA_DBUSDATAENGINEMANAGER
#define __DBUS_PLASMA_DBUSDATAENGINEMANAGER

#include <QtDBus>
#include <plasma/dataenginemanager.h>
namespace Plasma
{
namespace DBus
{			
class DBusDataEngineManager : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface","org.kde.Plasma.DataEngineManager")	
	public:
		explicit DBusDataEngineManager(Plasma::DataEngineManager * mgr);
	public slots:
		QStringList knownEngines();
		QString dataEngine(const QString &name);
		bool loadDataEngine(const QString &name);
		Q_NOREPLY void unloadDataEngine(const QString &name);
	private:
		Plasma::DataEngineManager *engineManager;
};
}; //Namespace DBus
}; //Namespace Plasma
#endif

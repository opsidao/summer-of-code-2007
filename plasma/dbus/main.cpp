//
// C++ Implementation: main
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtDBus>
#include <QCoreApplication>
#include <kdebug.h>
#include <kcomponentdata.h>

#include "dbusdataenginemanager.h" 

int main(int argc,char **argv)
{
	KComponentData cdata("plasma_dbus");
	QCoreApplication app(argc, argv);
	bool done;
	QDBusConnection conn = QDBusConnection::sessionBus();
	done = conn.registerService("org.kde.Plasma");
	if(!done)
		kDebug() << "Failed to register service: " << conn.lastError().name() << " : " << conn.lastError().message() << endl;
	else
	{
		Plasma::DataEngineManager *mgr = Plasma::DataEngineManager::self();
		new Plasma::DBus::DBusDataEngineManager(mgr);
		done = conn.registerObject(
				"/DataEngineManager", 
				mgr,
				QDBusConnection::ExportAdaptors);
		if(!done)
			kDebug() << "Failed to register the object: " << conn.lastError().name() << " : " << conn.lastError().message() << endl;
		else
		{
			kDebug() << "DBus service registered!" <<endl;
 			return app.exec();
		}
	}
	return -1;
}

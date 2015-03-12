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

#ifndef __DBUS_PLASMA_DBUSDATAENGINE
#define __DBUS_PLASMA_DBUSDATAENGINE

#include <QtDBus>
#include <plasma/dataengine.h>

namespace Plasma
{
namespace DBus
{
	class DBusDataEngine :public QDBusAbstractAdaptor/*, public QDBusContext*/
	{
		Q_OBJECT
		Q_CLASSINFO("D-Bus Interface","org.kde.Plasma.DataEngine")
		public:
			DBusDataEngine(const QString &name,Plasma::DataEngine *engine);
		public slots:
			QStringList sources();		
			void connectSource(const QString & source);
			void disconnectSource(const QString & source);
			void connectAllSources();
			QMap<QString,QVariant> query(const QString& source);
		private slots:
			void updated(const QString& source, const Plasma::DataEngine::Data& data);
		signals:
			void updated(const QString& source, const QMap<QString,QVariant>& data);
			void newSource(const QString& source);
			void sourceRemoved(const QString& source);
		private:
			Plasma::DataEngine *engine;
			QMap<QString,QList<QDBusConnection> > sourcesConnected;
			QString name;
			QString path;
	};
}; //namespace DBus
}; //namespace Plasma
#endif


/*
 *
 *  BTPM assignable action plugin base
 *
 *  Copyright (C) 2007  Juan González Aguilera <kde-devel@ka-tet>
 *
 *
 *  This file is part of btpm (Bluetooth Presence Manager).
 *
 *  btpm is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  btpm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with kbluemon; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef _BTPM_ACTION_PLUGIN_BASE_
#define _BTPM_ACTION_PLUGIN_BASE_

#include <QIcon>
#include <QWidget>
#include <kparts/plugin.h>
#include <kgenericfactory.h>
class KDE_EXPORT BtpmActionPlugin:public KLibFactory
{
 	Q_OBJECT
	Q_PROPERTY(QString linkedUbi READ linkedUbi WRITE setLinkedUbi)
	public:
		BtpmActionPlugin(QObject *parent = 0);
		QString linkedUbi();
		void setLinkedUbi(const QString &ubi);
		virtual ~BtpmActionPlugin(){};
		virtual QString nameForAction() = 0;
		virtual QString descriptionForAction() = 0;
		virtual QIcon iconForAction() = 0;
		virtual void configureAction(QWidget *parent) = 0;
		virtual void arrivalEvent() = 0;
		virtual void leaveEvent() = 0;
		virtual QObject* createObject(  QObject *   parent = 0, 
				const char *   className = "BtpmActionPlugin", 
				const QStringList &   args = QStringList()) = 0;
	private:
		QString m_linkedUbi;

};

Q_DECLARE_INTERFACE(BtpmActionPlugin,"org.kde.bluetooth.BtpmActionPlugin/0.1")
		
#define K_EXPORT_BTPM_ACTION_PLUGIN(name,classname)\
		K_EXPORT_COMPONENT_FACTORY(btpm_action_##name,KGenericFactory< classname >("btpm_action_" #name));\
		Q_EXPORT_PLUGIN2(btpm_action_##name,classname)

#endif

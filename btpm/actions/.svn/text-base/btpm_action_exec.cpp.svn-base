/*
 *
 *  BTPM command execution plugin
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

#include "btpm_action_exec.h"
#include <kiconloader.h>

BtpmExecActionPlugin::BtpmExecActionPlugin(QObject * parent,const QStringList &args)
	:BtpmActionPlugin(parent)
{
	Q_UNUSED(args)
}

BtpmExecActionPlugin::~BtpmExecActionPlugin()
{
}

QString BtpmExecActionPlugin::nameForAction()
{
	return "Execute command";
}

QString BtpmExecActionPlugin::descriptionForAction()
{
	return "Executes an specified command when a device arrives, leaves or both";
}

QIcon BtpmExecActionPlugin::iconForAction()
{
	return QIcon(KIconLoader::global()->loadIcon("application-x-executable",K3Icon::Desktop));
}

void BtpmExecActionPlugin::configureAction(QWidget * parent)
{
}

void BtpmExecActionPlugin::arrivalEvent()
{
}

void BtpmExecActionPlugin::leaveEvent()
{
}

QObject * BtpmExecActionPlugin::createObject(QObject * parent, const char * className, const QStringList & args)
{	
	Q_ASSERT(QString(className) == "BtpmActionPlugin");
	return new BtpmExecActionPlugin(parent,args);
}

#include "btpm_action_exec.moc"

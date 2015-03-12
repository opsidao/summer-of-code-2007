/*
 *
 *  BTPM widgets
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

#include "models.h"
#include "btpmactionplugin.h"


#include <QDropEvent>
#include <QIcon>
#include <kdebug.h>
#include <kiconloader.h>

#include <kservicetypetrader.h>
#include <kservice.h>
#include <kparts/componentfactory.h>

using namespace Solid::Control;

/**************************** KnownRemotes ****************************/
KnownRemotesListWidget::KnownRemotesListWidget(QWidget * parent)
	:QListWidget(parent),onlyActive(false)
{
	setViewMode(IconMode);
	setIconSize(QSize(128,128));
	setDragEnabled(false);
	setDropIndicatorShown(false);
	
	KConfig config;	
	
        //TODO track local interfaces
	const BluetoothManager &mgr = BluetoothManager::self();
	iface = new BluetoothInterface(mgr.defaultInterface());
	QObject::connect(iface,SIGNAL(remoteDeviceFound(const QString&, int, int)),
			 this,SLOT(remoteDeviceFound(const QString&)));
	QObject::connect(iface,SIGNAL(remoteDeviceDisappeared(const QString&)),
			 this,SLOT(remoteDeviceDisappeared(const QString&)));
	QStringList list = config.readListEntry("known_remotes_ubis",remotes.keys());
	foreach(QString ubi, list)
		remoteDeviceDisappeared(ubi);
	foreach(QString ubi, iface->listRemoteDevices())
		remoteDeviceDisappeared(ubi);
	
        showOnlyActive(config.readBoolEntry("only_show_active",true));
	iface->startPeriodicDiscovery();
}
KnownRemotesListWidget::~KnownRemotesListWidget()
{
	KConfig config;
	//TODO Still not sure for how long does the bluez system cache the information
	//Maybe too old devices would be removed from the cache, if so we must store name/address/major/minor
	config.writeEntry("known_remotes_ubis",remotes.keys());
}
Solid::Control::BluetoothRemoteDevice KnownRemotesListWidget::remoteDevice(const QString & ubi)
{
	if(iface)
		return iface->findBluetoothRemoteDevice(ubi);
	return Solid::Control::BluetoothRemoteDevice(); 
}
void KnownRemotesListWidget::remoteDeviceFound(const QString & ubi)
{
	BluetoothRemoteDevice remote = iface->findBluetoothRemoteDevice(ubi);
	QIcon icon(iconfinder.findIcon(remote.majorClass(),remote.minorClass(),K3Icon::Desktop,K3Icon::SizeEnormous,K3Icon::DefaultState));
	if(remotes.contains(ubi))
		remotes[ubi]->setIcon(icon);
	else
	{
		remotes[ubi] = new QListWidgetItem(icon,remote.name(),this);
		remotes[ubi]->setData(Qt::UserRole,ubi);
	}
	remotesStatus[ubi] = true;
	remotes[ubi]->setHidden(false);
	sortItems(Qt::DescendingOrder);
}

void KnownRemotesListWidget::remoteDeviceDisappeared(const QString &ubi)
{
	BluetoothRemoteDevice remote = iface->findBluetoothRemoteDevice(ubi);
	QIcon icon(iconfinder.findIcon(remote.majorClass(),remote.minorClass(),K3Icon::Desktop,K3Icon::SizeEnormous,K3Icon::DisabledState));
	if(remotes.contains(ubi))		
		remotes[ubi]->setIcon(icon);
	else //Creating unknow remotes dissapearing is used to add the seen, but not alive, remotes at initialization
	{
		remotes[ubi] = new QListWidgetItem(icon,remote.name(),this);
		remotes[ubi]->setData(Qt::UserRole,ubi);
	}
	remotesStatus[ubi] = false;
	remotes[ubi]->setHidden(onlyActive);
	sortItems(Qt::DescendingOrder);
}
void KnownRemotesListWidget::showOnlyActive(bool onlyActive)
{
// 	kDebug() << k_funcinfo << " onlyActive :" << onlyActive;
        KConfig config;
        config.writeEntry("only_show_active",onlyActive);
	foreach(QString ubi,remotes.keys())
	{
		if(!remotesStatus[ubi])
			remotes[ubi]->setHidden(onlyActive);
	}
	this->onlyActive = onlyActive;
        sortItems(Qt::DescendingOrder);
}

/**************************** BtpmActionMimeData ****************************/
BtpmActionMimeData::BtpmActionMimeData(BtpmActionPlugin * action)
	:action(action)
{
	
}

bool BtpmActionMimeData::hasFormat(const QString & mimeType) const
{
	return mimeType == "x-btpmaction";
}

QStringList BtpmActionMimeData::formats() const
{
	return QStringList("x-btpmaction");
}

/**************************** AvailableActionsListWidget ****************************/
AvailableActionsListWidget::AvailableActionsListWidget(QWidget * parent)
	:QListWidget(parent)
{
	setViewMode(IconMode);
	setIconSize(QSize(64,64));
	setDragEnabled(true);
	setAcceptDrops(false);
	
// 	kDebug() << k_funcinfo;
	KService::List offers = KServiceTypeTrader::self()->query("BtpmActionPlugin");
	KService::List::ConstIterator iter;
	for(iter = offers.begin(); iter != offers.end(); ++iter) {
		KService::Ptr service = *iter;
		BtpmActionPlugin* plugin = KService::createInstance<BtpmActionPlugin>(service);
				/*
				KParts::ComponentFactory::createPartInstanceFromService<BtpmActionPlugin>
				( service, 0,this);*/
        	if (plugin) {			
			kdDebug() << "BtpmActionPlugin: Loaded plugin " << plugin << " " 
					<< plugin->nameForAction() << endl;
			actions << plugin;
		} else
			kdDebug() << "BtpmActionPlugin: Not loaded plugin ";
	}
	foreach(BtpmActionPlugin* action, actions)
	{
		QListWidgetItem *item = new QListWidgetItem(action->iconForAction(),action->nameForAction(),this);
		actionMap[item] = action;
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
	}
}

QMimeData * AvailableActionsListWidget::mimeData(const QList< QListWidgetItem * > items) const
{
// 	QListWidget::mimeData(items);
// 	kDebug() << k_funcinfo;
	Q_ASSERT(items.size()>0);
	BtpmActionPlugin *action = qobject_cast<BtpmActionPlugin *>(actionMap[items[0]]->createObject());
	Q_ASSERT(action);
	Q_ASSERT(!activeUbi.isEmpty());
	action->setLinkedUbi(activeUbi);
	QMimeData *mimedata= new BtpmActionMimeData(action);
	return mimedata;
}

void AvailableActionsListWidget::selectedRemoteChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
	kDebug() << k_funcinfo;
	if(current)
	{
		activeUbi = current->data(Qt::UserRole).toString();
		Q_ASSERT(!activeUbi.isEmpty());
	} else
		activeUbi ="";
}

/**************************** AssignedActionsListWidget ****************************/
AssignedActionsListWidget::AssignedActionsListWidget(QWidget * parent)
	:QListWidget(parent)
{
	setViewMode(IconMode);
	setIconSize(QSize(64,64));
	setAcceptDrops(true);
	setDragEnabled(false);
	setDropIndicatorShown(true);
}


QStringList AssignedActionsListWidget::mimeTypes() const
{
	return QStringList("x-btpmaction");
}

bool AssignedActionsListWidget::dropMimeData(int index, const QMimeData * data, Qt::DropAction action)
{
// 	kDebug() << k_funcinfo;
	const BtpmActionMimeData *mimedata = qobject_cast<const BtpmActionMimeData*>(data);
	if(mimedata)
	{
		BtpmActionPlugin * action = mimedata->action;
		/*kDebug() << "New action for " + mimedata->action->linkedUbi() +
				", " << mimedata->action->nameForAction();
		*/
		if(!actions.contains(action->linkedUbi()))
		{
			actions[action->linkedUbi()] = QList<BtpmActionPlugin*>();
			Q_ASSERT(!items.contains(action->linkedUbi()));
			items[action->linkedUbi()] = QList<QListWidgetItem*>();
		}
		actions[action->linkedUbi()] << action;
		items[action->linkedUbi()] << new QListWidgetItem(action->iconForAction(),action->nameForAction(),this);
		parentWidget()->setWindowState(Qt::WindowActive);
		return true;
	} else
		return false;
}

void AssignedActionsListWidget::selectedRemoteChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
// 	kDebug() << k_funcinfo;
	QString ubi;
	if(current)
		ubi = current->data(Qt::UserRole).toString();
	foreach(QString remote,items.keys())
	{
		foreach(QListWidgetItem *item, items[remote])
			item->setHidden(ubi!=remote);
	}
	sortItems(Qt::DescendingOrder);
}


#include "models.moc"

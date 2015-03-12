/*
 *
 *  KDE Blutooth Monitor 
 *
 *  Copyright (C) 2007  Tom Patzig <tpatzig@suse.de>
 *  Copyright (C) 2007  Juan González Aguilera <kde-devel@ka-tet> 
 *
 *
 *  This file is part of kbluemon.
 *
 *  kbluemon is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  kbluemon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with kbluemon; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include "monitor.h"
#include "rssidelegate.h"
#include "localDevicesDialog.h"

#include <kiconloader.h>

#include <QMessageBox>
#include <QTimer>
#include <QHeaderView>
#include <QStringListModel>

#include <solid/control/bluetoothmanager.h>
#include <solid/control/bluetoothremotedevice.h>

#include <plasma/dataenginemanager.h>

using namespace Solid::Control;

Monitor::Monitor() : MainWindow()
{
	setupUi(this);
	
	//First setup icons for buttons
	QIcon icon = KIconLoader::global()->loadIconSet("document-properties",K3Icon::Toolbar,32,false);
	toolBox->setItemIcon(0,icon);
	icon = KIconLoader::global()->loadIconSet("services",K3Icon::Toolbar,32,false);
	toolBox->setItemIcon(1,icon);
	icon = KIconLoader::global()->loadIconSet("dialog-ok",K3Icon::Toolbar,32,false);
	updateAliasButton->setIcon(icon);
	
	deviceProperties->close();
	
	RssiDelegate *delegate = new RssiDelegate(this);
	remoteListWidget->setItemDelegateForColumn(1,delegate);
	
	Plasma::DataEngineManager::self()->loadDataEngine("bluetooth");
	engine = Plasma::DataEngineManager::self()->dataEngine("bluetooth");
	if(!engine->isValid())
	{
		QMessageBox::critical(this,
				      i18n("No bluetooth data engine"),
					   i18n("You need the Bluetooth Plasma Data Engine to use KBluemon"));
		//TODO Once updated plasma
		//setFailedToLoad(true);
		Plasma::DataEngineManager::self()->unloadDataEngine("bluetooth");
		QApplication::exit(-1);
		return;
	}
	engine->connectSource("adapters",this);
	Plasma::DataEngine::Data adapters = engine->query("adapters");	
	if(adapters.isEmpty())
	{
		QMessageBox::critical(this,
				      i18n("No bluetooth adapters available"),
					   i18n("I can't find any compatible bluetooth adapter, make sure your adapter is detected correctly by the operating system"));
		Plasma::DataEngineManager::self()->unloadDataEngine("bluetooth");
		QApplication::exit(-1);
		return;
	}
	engine->setProperty("doServiceSearch",true);
	
	interface = new BluetoothInterface(adapters.keys()[0]);
	
	connect(engine,SIGNAL(newSource( const QString& )),
		this,SLOT(newSourceAvailable(const QString&)));
	connect(engine,SIGNAL(sourceRemoved( const QString& )),
		this,SLOT(sourceRemoved(const QString&)));

	connect(remoteListWidget,SIGNAL(cellClicked( int, int )),
		this,SLOT(remoteSelected(int)));		
	
	connect(updateAliasButton,SIGNAL(clicked()), 
		this, SLOT(updateAlias()));
	connect(dev_alias,SIGNAL(returnPressed()),
		this,SLOT(updateAlias()));
	
	connect(&feedbackTimer,SIGNAL(timeout()),
		 this,SLOT(updateFeedbackBar()));
	
	connect(action_Local_devices,SIGNAL(triggered()),this, SLOT(showLocalDevicesDialog()));

	feedbackTimer.start(200);
}

Monitor::~Monitor() {
	if(feedbackTimer.isActive())
		feedbackTimer.stop();
	Plasma::DataEngineManager::self()->unloadDataEngine("bluetooth");
}

void Monitor::updateFeedbackBar()
{
	bool direction = feedBackBar->invertedAppearance();
	int value = feedBackBar->value();
	if(!direction)
	{
		if(value >= 96)
		{	
			feedBackBar->setInvertedAppearance(!direction);
			feedBackBar->setValue(value-5);
		} else
			feedBackBar->setValue(value+5);
	} else
	{
		if(value <= 4)
		{	
			feedBackBar->setInvertedAppearance(!direction);
			feedBackBar->setValue(value+5);
		} else
			feedBackBar->setValue(value-5);
	}
}

void Monitor::remoteSelected(int row)
{
	QString ubi = knownRemotes[row];
	Plasma::DataEngine::Data data = engine->query(ubi);
	QString notAvailableMsg = i18n("Not available");
	
	QString tmp = data["address"].toString();
	dev_address->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["name"].toString();
	dev_name->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["alias"].toString();
	dev_alias->setText(tmp);
	
	tmp = data["rssi"].toString();
	dev_strength->setText(tmp+" db");
	
	tmp = data["version"].toString();
	dev_version->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["revision"].toString();
	dev_revision->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["manufacturer"].toString();
	dev_manufacturer->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["category"].toString();
	dev_category->setText(tmp.isEmpty() ? notAvailableMsg:tmp);
	
	tmp = data["lastseen"].toString();
	dev_lastseen->setText(tmp);
	
	tmp = data["lastused"].toString();
	dev_lastused->setText(tmp);
	
	deviceProperties->show();
	
	updateServiceTree(ubi);
}

void Monitor::updateAlias()
{
	QString newAlias = dev_alias->text();
	Q_ASSERT(remoteListWidget->selectedItems().size() > 0);
	int selectedIndex = remoteListWidget->row(remoteListWidget->selectedItems()[0]);
	QString selectedUbi = knownRemotes[selectedIndex];
	BluetoothRemoteDevice remote = interface->findBluetoothRemoteDevice(selectedUbi);
	remote.setAlias(newAlias);
	engine->setProperty("remoteToUpdate",selectedUbi);
}

void Monitor::newSourceAvailable(const QString & source)
{
	engine->connectSource(source,this);	
}

void Monitor::sourceRemoved(const QString & source)
{
	engine->disconnectSource(source,this);
}

void Monitor::updated(const QString & source, const Plasma::DataEngine::Data & data)
{
//  	kDebug() << k_funcinfo << " source: " << source << endl;	
	if(source == "devices")
	{
		QStringList devices = data.keys();
		for(int i = 0; i < devices.size(); i++)
		{
			QString ubi = devices[i];
			Plasma::DataEngine::Data devInfo = engine->query(ubi);
			if(!knownRemotes.contains(ubi)) //We don't know about that device yet
			{
				knownRemotes << ubi;
				int row = remoteListWidget->rowCount();
				remoteListWidget->insertRow(row);
				remoteListWidget->setItem(row,0,new QTableWidgetItem(devInfo["name"].toString() + ((devInfo["alias"].toString().isEmpty()) ? "":(" ("+devInfo["alias"].toString()+")"))));
				remoteListWidget->setItem(row,1,new QTableWidgetItem(devInfo["rssi"].toString()));
				remoteListWidget->setItem(row,2,new QTableWidgetItem(devInfo["majorclass"].toString()+"/"+devInfo["minorclass"].toString()));
				remoteListWidget->setItem(row,3,new QTableWidgetItem(devInfo["address"].toString()));
				remoteListWidget->setItem(row,4,new QTableWidgetItem(data["bonded"].toBool()?"true":"false"));
				remoteListWidget->resizeColumnsToContents();
				remoteListWidget->verticalHeader()->hide();
			} else
			{
				int index = knownRemotes.indexOf(ubi);
				remoteListWidget->setItem(index,1,new QTableWidgetItem(devInfo["rssi"].toString()));
			}	
		}
		for(int i = knownRemotes.size()-1; i >=0 ; i--)
		{
			if(!devices.contains(knownRemotes[i]))
			{
				knownRemotes.removeAt(i);				
				remoteListWidget->removeRow(i);
			}
		}
		int selectedIndex = -1;
		if(remoteListWidget->selectedItems().size() > 0)
			selectedIndex = remoteListWidget->row(remoteListWidget->selectedItems()[0]);
		if(knownRemotes.size() <= selectedIndex)
		{
			if(knownRemotes.size()>0)
			{
				remoteListWidget->selectRow(knownRemotes.size()-1);
				remoteSelected(knownRemotes.size()-1);
			}
			else
				deviceProperties->close();
		}
	} else 
	{
		int selectedIndex = -1;
		if(remoteListWidget->selectedItems().size() > 0)
			selectedIndex = remoteListWidget->row(remoteListWidget->selectedItems()[0]);
		if(selectedIndex>-1 && ((knownRemotes[selectedIndex]+"/services") == source))
		{
			updateServiceTree(knownRemotes[selectedIndex]);
		} else if(knownRemotes.contains(source))
		{
			int row = knownRemotes.indexOf(source);
			remoteListWidget->setItem(row,0,new QTableWidgetItem(data["name"].toString() + ((data["alias"].toString().isEmpty()) ? "":(" ("+data["alias"].toString()+")"))));
			remoteListWidget->setItem(row,1,new QTableWidgetItem(data["rssi"].toString()));
			remoteListWidget->setItem(row,2,new QTableWidgetItem(data["majorclass"].toString()+"/"+data["minorclass"].toString()));
			remoteListWidget->setItem(row,3,new QTableWidgetItem(data["address"].toString()));
			remoteListWidget->setItem(row,4,new QTableWidgetItem(data["bonded"].toBool()?"true":"false"));
			remoteListWidget->resizeColumnsToContents();
			remoteListWidget->verticalHeader()->hide();
			if(row==selectedIndex)
				remoteSelected(row);
		}
	}
}

void Monitor::updateServiceTree(const QString & ubi)
{
	Plasma::DataEngine::Data data = engine->query(ubi+"/services");
	QStringList services;
// 	QTreeWidget *service_tree;
	service_tree->clear();
	QIcon folderIcon = KIconLoader::global()->loadIconSet("folder",K3Icon::Toolbar,32,false);
	QIcon handleIcon = KIconLoader::global()->loadIconSet("spread",K3Icon::Toolbar,32,false);
	QIcon classIcon = KIconLoader::global()->loadIconSet("stamp",K3Icon::Toolbar,32,false);
	QIcon protocolIcon= KIconLoader::global()->loadIconSet("kalzium",K3Icon::Toolbar,32,false);
	foreach(QString s,data.keys())
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(data[s].toString()));
		item->setIcon(0,folderIcon);
		Plasma::DataEngine::Data info = engine->query(s);
		if(!info.isEmpty())
		{
			QTreeWidgetItem *i = new QTreeWidgetItem(item,QStringList("handle"));
			i->setIcon(0,handleIcon);
			new QTreeWidgetItem(i,QStringList(info["handle"].toString()));
		
			if(info.contains("classes"))
			{
				i = new QTreeWidgetItem(item,QStringList("classes"));
				i->setIcon(0,classIcon);
				new QTreeWidgetItem(i,info["classes"].toStringList());
			}
			info = engine->query(s+"/protocols");
			if(!info.isEmpty())
			{
				i = new QTreeWidgetItem(item,QStringList("protocols"));
				i->setIcon(0,protocolIcon);
				foreach(QString p,info.keys())
				{
					if(info[p].toString().isEmpty())
						new QTreeWidgetItem(i,QStringList(p));
					else
					{
						QTreeWidgetItem *ii=new QTreeWidgetItem(i,QStringList(p));
						new QTreeWidgetItem(ii,QStringList(info[p].toString()));
					}
				}
			}
			service_tree->addTopLevelItem(item);
		}
	}
}

void Monitor::showLocalDevicesDialog()
{
	LocalDevicesDialog dialog(this);
	dialog.exec();
}

#include "monitor.moc"

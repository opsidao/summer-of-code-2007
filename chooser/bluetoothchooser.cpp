//
// C++ Implementation: chooser
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "bluetoothchooser.h"

#include <kcomponentdata.h>
#include <QApplication>

#include <kdebug.h>
#include <kiconloader.h>
#include <QMessageBox>
#include <QTextTable>
#include <QTimer>

#include <solid/control/bluetoothinterface.h>
#include <solid/control/bluetoothmanager.h>
#include <solid/control/bluetoothremotedevice.h>

#include <QTreeWidget>

#include <QThread>

using namespace Solid::Control;

BluetoothInterface *interface;

KBluetoothChooser::KBluetoothChooser(QWidget *parent,Qt::WindowFlags f):QDialog(parent,f)
{	
	setupUi(this);
	
	QObject::connect(devices,SIGNAL(clicked( const QModelIndex& )),
			 this,SLOT(deviceSelected(const QModelIndex&)));
	
	QObject::connect(services,SIGNAL(clicked( const QModelIndex& )),
			 this,SLOT(serviceSelected(const QModelIndex&)));
	
	services->setVisible(false);
	
	servicesModel = new BluetoothServiceModel(services);
 	services->setModel(servicesModel);
	
	devicesModel = new BluetoothRemoteModel(devices,servicesModel);
 	devices->setModel(devicesModel);

	QIcon icon = KIconLoader::global()->loadIconSet("view-refresh",K3Icon::Toolbar,32,false);
	refreshButton->setIcon(icon);
	refreshButton->setVisible(false);	
	QObject::connect(refreshButton,SIGNAL(clicked()),
			 this,SLOT(restartDiscovery()));
	updateInterface();
	restartDiscovery();

//  	QColumnView *columnView;
// 	BluetoothChooserModel *model = new BluetoothChooserModel(this);
// 	QThread::currentThread()->wait(2);
// 	columnView->setPreviewWidget(new QTextEdit(this));
// 	columnView->setModel(model);
	
// 	QTreeWidget w(this);
// 	w.setColumnCount(1);
// 	w.setHeaderLabel(QString("Devices"));
// 	QTreeWidgetItem *root = new QTreeWidgetItem((QTreeWidgetItem*)0,QStringList(QString("Root")));
// 	w.addTopLevelItem(root);
// 	root->addChild(new QTreeWidgetItem((QTreeWidgetItem*)0,QStringList(QString("Child"))));
// 	QColumnView *columnView;
// 	columnView->set
// 	columnView->setModel(w.model());
	
// 	
	
}
KBluetoothChooser::~ KBluetoothChooser()
{
}


void KBluetoothChooser::updateInterface(const QString & ubi)
{
	BluetoothManager &mgr = BluetoothManager::self();
	QString m_ubi = ubi;
	if(m_ubi.isEmpty())//Check if we are looking for default iface
	{
		m_ubi = mgr.defaultInterface();
	}
	if(interface)
	{
		QObject::disconnect(interface,SIGNAL(discoveryStarted()),
				    this,SLOT(discoveryStart()));
		QObject::disconnect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
				    this,SLOT(addRemote(const QString&, int, int)));
		QObject::disconnect(interface,SIGNAL(discoveryCompleted()),
				    this,SLOT(discoveryEnd()));
		delete interface;
		interface = 0;
	}
	if(m_ubi.isEmpty())//Check if there is no available iface
	{
		QMessageBox::critical(this,
				      "No bluetooth adapter detected",
	  "Impossible to monitor remote bluetooth devices because there are no local bluetooth adapters available");
		//FIXME Exit cleanly
		exit(-1);
	}
	else
	{
		interface = new BluetoothInterface(m_ubi);
		QObject::connect(interface,SIGNAL(discoveryStarted()),
				    this,SLOT(discoveryStart()));
		QObject::connect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
				    this,SLOT(addRemote(const QString&, int, int)));
		QObject::connect(interface,SIGNAL(discoveryCompleted()),
				    this,SLOT(discoveryEnd()));
	}
}

void KBluetoothChooser::restartDiscovery()
{
	runningBar->setMaximum(0);
	runningBar->setValue(0);
	
	okButton->setEnabled(false);
	services->setVisible(false);
	properties->setVisible(false);
	
	refreshButton->setVisible(false);
	
	devicesModel->clearRemotes();
	servicesModel->clearAllServices();
		
	QObject::connect(interface,SIGNAL(discoveryStarted()),
			 this,SLOT(discoveryStart()));
	QObject::connect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
			 this,SLOT(addRemote(const QString&, int, int)));
	QObject::connect(interface,SIGNAL(discoveryCompleted()),
			 this,SLOT(discoveryEnd()));
	
	interface->discoverDevices();
}

void KBluetoothChooser::discoveryStart()
{
	firstDeviceReceived = false;
}

void KBluetoothChooser::addRemote(const QString & ubi, int , int )
{
	if(!firstDeviceReceived)
	{
		devicesModel->clearRemotes();
		firstDeviceReceived = true;
	}	
	BluetoothRemoteDevice remote = interface->findBluetoothRemoteDevice(ubi);
	devicesModel->addRemote(remote);
}
void KBluetoothChooser::discoveryEnd()
{
	kDebug() << k_funcinfo << endl;
	runningBar->setMaximum(1);
	runningBar->setValue(1);
	
	refreshButton->setVisible(true);
	
	QObject::disconnect(interface,SIGNAL(discoveryStarted()),
			    this,SLOT(discoveryStart()));
	QObject::disconnect(interface,SIGNAL(remoteDeviceFound( const QString&, int, int )),
			    this,SLOT(addRemote(const QString&, int, int)));
	QObject::disconnect(interface,SIGNAL(discoveryCompleted()),
			    this,SLOT(discoveryEnd()));
	if(devicesModel->deviceCount()==0)
		runningBar->setFormat("No remotes found");
	else
		runningBar->setFormat("Search finished");
	
}

void KBluetoothChooser::deviceSelected(const QModelIndex &index)
{
	if(devicesModel->deviceCount()>0)
	{
		selectedDevice = devicesModel->getRemote(index);
		
		selectedService = BluetoothServiceRecord();
		
		okButton->setEnabled(true);
		services->setVisible(true);
		servicesModel->setCurrentRemote(selectedDevice);
		
		properties->setVisible(true);
		QTextCharFormat textFormat;
		QTextCharFormat boldFormat;
		boldFormat.setFontWeight(QFont::Bold);
		
		properties->clear();
		
		QTextBlockFormat centerFormat;
		centerFormat.setAlignment(Qt::AlignCenter);
		
		QTextCursor cursor(properties->textCursor());
		cursor.beginEditBlock();
		cursor.setBlockFormat(centerFormat);
		
		cursor.insertText("Remote device properties",boldFormat);
		cursor.insertBlock();
		
		QTextTableFormat tableFormat;
		tableFormat.setAlignment(Qt::AlignCenter);
		tableFormat.setCellPadding(4);
		
		QTextTable *table = cursor.insertTable(3,2,tableFormat);
		cursor = table->cellAt(0,0).firstCursorPosition();
 		cursor.insertText("Class:",boldFormat);
		cursor = table->cellAt(0,1).firstCursorPosition();
 		cursor.insertText(selectedDevice.majorClass()+"/"+selectedDevice.minorClass(),textFormat);
		cursor = table->cellAt(1,0).firstCursorPosition();
 		cursor.insertText("Version:",boldFormat);
		cursor = table->cellAt(1,1).firstCursorPosition();
 		cursor.insertText(selectedDevice.version(),textFormat);
		cursor = table->cellAt(2,0).firstCursorPosition();
 		cursor.insertText("HCI Revision:",boldFormat);
		cursor = table->cellAt(2,1).firstCursorPosition();
 		cursor.insertText(selectedDevice.revision(),textFormat);
		
		cursor = properties->textCursor();
		cursor.setBlockFormat(centerFormat);
		
		cursor.insertBlock();
		cursor.insertBlock();
		KIconLoader *loader = KIconLoader::global();
		QString path;
		if(selectedDevice.hasBonding())
			loader->loadIcon("cellular-phone-mount",K3Icon::Panel,64,K3Icon::DefaultState,&path,false);
		else
			loader->loadIcon("cellular-phone-unmount",K3Icon::Panel,64,K3Icon::DefaultState,&path,false);
		cursor.insertImage(path);
		
		cursor.insertBlock();
		cursor.insertBlock();
		cursor.insertText("Device is ",textFormat);
		if(selectedDevice.hasBonding())
			cursor.insertText("paired",boldFormat);
		else
			cursor.insertText("not paired",boldFormat);
		
		cursor.endEditBlock();
	}
}

void KBluetoothChooser::serviceSelected(const QModelIndex &index)
{
	if(servicesModel->countServices()>0)
	{
		selectedService = servicesModel->getService(index);
		QTextCharFormat textFormat;
		QTextCharFormat boldFormat;
		boldFormat.setFontWeight(QFont::Bold);
		
		properties->clear();
		
		QTextBlockFormat centerFormat;
		centerFormat.setAlignment(Qt::AlignCenter);
		
		QTextCursor cursor(properties->textCursor());
		cursor.insertText("Remote Service properties",boldFormat);
		cursor.insertBlock();
		cursor.insertText("Handle: ");
		cursor.insertText(selectedService.handle,textFormat);
	}
}
int main(int argc, char **argv)
{
	KComponentData cdata("KBluetoothChooser");
	QApplication app(argc,argv);
	KBluetoothChooser chooser;
	int result = chooser.exec();
	if(result==KBluetoothChooser::Accepted)
	{
		kDebug() << "User accepted" << endl;
		kDebug() << "Selected device  : " << chooser.selectedRemoteDevice().name() << endl;
		kDebug() << "Selected service : " << chooser.selectedBluetoothServiceRecord().name << endl;
	}
};

const BluetoothRemoteDevice & KBluetoothChooser::selectedRemoteDevice()
{
	return selectedDevice;
}

const BluetoothServiceRecord & KBluetoothChooser::selectedBluetoothServiceRecord()
{
	return selectedService;
}

#include "bluetoothchooser.moc"

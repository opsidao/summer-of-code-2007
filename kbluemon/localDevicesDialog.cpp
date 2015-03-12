/*
 *
 *  KDE Blutooth Monitor 
 *
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

#include "localDevicesDialog.h"
#include <QPushButton>
#include <QStringList>

#include <solid/control/bluetoothinterface.h>
#include <kdebug.h>
LocalDevicesDialog::LocalDevicesDialog(QWidget *parent)
	:QDialog(parent),mgr(Solid::Control::BluetoothManager::self()),iface(0)
{
	setupUi(this);
	buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	QObject::connect(buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),
			 this,SLOT(applyChanges()));
	QObject::connect(buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()),
			 this,SLOT(applyChanges()));
	
	QObject::connect(adapterCombo,SIGNAL(currentIndexChanged(int)),
		this,SLOT(adaptorSelected(int)),Qt::DirectConnection);
	
	//To enable the apply button when something changes
	QObject::connect(nameLine,SIGNAL(textChanged(QString)),this,SLOT(enableApply()),Qt::DirectConnection);
	QObject::connect(minorclassCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(enableApply()),Qt::DirectConnection);
	QObject::connect(modeCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(enableApply()),Qt::DirectConnection);
	QObject::connect(discoverableSlider,SIGNAL(valueChanged(int)),this,SLOT(enableApply()),Qt::DirectConnection);
	
	Solid::Control::BluetoothInterfaceList adapters = mgr.bluetoothInterfaces();
	Q_ASSERT(!adapters.isEmpty());
	foreach(Solid::Control::BluetoothInterface adapter,adapters)
		adapterCombo->addItem(adapter.ubi());
}

LocalDevicesDialog::~ LocalDevicesDialog()
{
}

void LocalDevicesDialog::modeChanged(Solid::Control::BluetoothInterface::Mode mode)
{
	switch(mode)
	{
		case Solid::Control::BluetoothInterface::Off:
			modeCombo->setCurrentIndex(0);
			break;
		case Solid::Control::BluetoothInterface::Connectable:
			modeCombo->setCurrentIndex(1);
			break;
		case Solid::Control::BluetoothInterface::Discoverable:
			modeCombo->setCurrentIndex(2);
			break;
	}
}

void LocalDevicesDialog::minorClassChanged(const QString & minor)
{
	for(int i= 0; i< minorclassCombo->count();i++)
		if(minorclassCombo->itemText(i)==minor)
		{
			minorclassCombo->setCurrentIndex(i);
			break;
		}
}

void LocalDevicesDialog::discoverableTimeoutChanged(int timeout)
{
	discoverableSlider->setValue(timeout);
}

void LocalDevicesDialog::nameChanged(const QString & name)
{
	nameLine->setText(name);
}


void LocalDevicesDialog::adaptorSelected(int index)
{
// 	kDebug() << k_funcinfo << " index : " << index << endl;
	
	QString ubi = adapterCombo->itemText(index);
	if(iface)
	{
		delete iface;//This should disconnect signals...
	}
	iface = new Solid::Control::BluetoothInterface(mgr.findBluetoothInterface(ubi));
	QObject::connect(iface,SIGNAL(modeChanged(Solid::Control::BluetoothInterface::Mode)),
			 this,SLOT(modeChanged(Solid::Control::BluetoothInterface::Mode)));
	QObject::connect(iface,SIGNAL(discoverableTimeoutChanged(int)),
			 this,SLOT(discoverableTimeoutChanged(int)));
	QObject::connect(iface,SIGNAL(minorClassChanged(const QString&)),
			 this,SLOT(minorClassChanged(const QString&)));
	QObject::connect(iface,SIGNAL(nameChanged(const QString&)),
			 this,SLOT(nameChanged(const QString&)));
	//General
	nameLine->setText(iface->name());
	addressLine->setText(iface->address());
	majorclassLine->setText(iface->majorClass());
	minorclassCombo->clear();
	QStringList minors = iface->listAvailableMinorClasses();
	minorclassCombo->addItems(minors);
	minorclassCombo->setCurrentIndex(minors.indexOf(iface->minorClass()));
	
	//Chip info
	versionLine->setText(iface->version());
	revisionLine->setText(iface->revision());
	manufacturerLine->setText(iface->manufacturer());
	companyLine->setText(iface->company());
	
	//Mode
	switch(iface->mode())
	{
		case Solid::Control::BluetoothInterface::Off:
			modeCombo->setCurrentIndex(0);
			break;
		case Solid::Control::BluetoothInterface::Connectable:
			modeCombo->setCurrentIndex(1);
			break;
		case Solid::Control::BluetoothInterface::Discoverable:
			modeCombo->setCurrentIndex(2);
			break;
	}
// 	kDebug() << "DT 1 " <<iface->discoverableTimeout() << endl;
	discoverableSlider->setValue(iface->discoverableTimeout());
		
	//Bondings
	bondingsList->clear();
	bondingsList->addItems(iface->listBondings());
	
	//Connections
	connectionsList->clear();
	foreach(Solid::Control::BluetoothRemoteDevice remote, iface->listConnections())
		connectionsList->addItem(remote.name() + " (" + remote.ubi() + " )");
	
	buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void LocalDevicesDialog::enableApply()
{
	buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}
void LocalDevicesDialog::applyChanges()
{
// 	kDebug() << k_funcinfo << endl;
	buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	QString ubi = adapterCombo->currentText();
	Q_ASSERT(iface);
	iface->setName(nameLine->text());
	iface->setMinorClass(minorclassCombo->currentText());
	//FIXME This is not i18n compatible.. and ugly
	QString modeString = modeCombo->currentText();
	Solid::Control::BluetoothInterface::Mode mode = ( (modeString=="Off") ? Solid::Control::BluetoothInterface::Off : 
			( (modeString=="Connectable") ? Solid::Control::BluetoothInterface::Connectable : Solid::Control::BluetoothInterface::Discoverable));
	kDebug() << "MODE " << mode << endl;
	iface->setMode(mode);	
	iface->setDiscoverableTimeout(discoverableSlider->value());
}

#include "localDevicesDialog.moc"

/*
 *
 *  BTPM main function
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


#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <QApplication>
#include <QListWidget>

#include "iconfinder.h"
#include "mainwindow.h"
int main(int argc, char **argv)
{
	KComponentData cdata("btpm");

	KAboutData aboutData("btpm",
			     0,
			     ki18n("Bluetooth Presence Manager"),
			     QByteArray("0.1"),
			     ki18n("Use this program to make your computer react to remote bluetooth devices arrival/leaving"),
			     KAboutData::License_GPL,
			     ki18n("(c) 2007, Juan González")
			    );
	aboutData.addAuthor(ki18n("Juan González"),ki18n("Author"),"kde_devel@opsiland.info");

	QApplication app(argc,argv);
	app.setOrganizationName("KDEBluetooth");
	app.setOrganizationName("bluetooth.kmobiletools.org");
	app.setApplicationName("Bluetooth Presence Manager");

	MainWindow window;
	window.show();
	return app.exec();
}

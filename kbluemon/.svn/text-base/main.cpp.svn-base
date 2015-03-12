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


#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <qapplication.h>


#include "monitor.h"
int main(int argc, char *argv[])
{
	KComponentData cdata("kbluemon");
	KAboutData aboutData("kbluemon",
			     0,
			     ki18n("KBlueMon"),
				   QByteArray("0.2"),
			     ki18n("Use this program to monitor your bluetooth neighbourhood"),
			     KAboutData::License_GPL,
			     ki18n("(c) 2007, Tom Patzig")
			     );
	
	aboutData.addAuthor(ki18n("Tom Patzig"), ki18n("Author"), "tpatzig@suse.de");
	aboutData.addAuthor(ki18n("Daniel Gollub"), ki18n(""), "dgollub@suse.de");	
	aboutData.addAuthor(ki18n("Juan González"),ki18n("KDE4 Rewrite"),"kde_devel@opsiland.info");
	
	QApplication a(argc, argv);	
	Monitor mon;
	mon.show();
	return a.exec();

}

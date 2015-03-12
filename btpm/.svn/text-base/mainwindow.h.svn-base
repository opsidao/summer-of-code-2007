/*
 *
 *  BTPM main window
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
#ifndef _BTPM_MAIN_WINDOW_
#define _BTPM_MAIN_WINDOW_

#include "ui_mainwindow.h"
#include <QLabel>
class KnownRemotesModel;
class QListWidgetItem;
class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	private Q_SLOTS:
		void selectedRemoteChanged( QListWidgetItem * current, QListWidgetItem * previous );
	private:
		QLabel selectedDeviceLabel;
		
};

#endif

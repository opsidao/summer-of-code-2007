/*
 *
 *  BTPM icon finder
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
#include <kiconloader.h>
#include <kdebug.h>
#include "iconfinder.h"
// BluetoothIconFinder* BluetoothIconFinder::m_instance = new BluetoothIconFinder;
BluetoothIconFinder::BluetoothIconFinder()
{
	kDebug() << k_funcinfo << endl;
}

// BluetoothIconFinder * BluetoothIconFinder::instance()
// {
// // 	if(m_instance == 0)
// // 		m_instance = ;
// 	return m_instance;
// }

QPixmap BluetoothIconFinder::findIcon(const QString & major, const QString & minor, K3Icon::Group group, K3Icon::StdSizes size,K3Icon::States state)
{
	QString id = major +"/"+ minor;
	QString iconName = "phone";
	if(major=="computer")
	{
		if(minor=="uncategorized" || minor=="desktop"|| minor=="server")
		{
			iconName="computer";
		} else if(minor=="laptop")
		{
			iconName="computer-laptop";
		} else if(minor=="handheld" || minor=="palm" || minor=="wearable")
		{
			iconName="pda";
		}
	} else if(major=="phone")
	{
			/*if(minor=="uncategorized" || minor=="cellular" || minor=="cordless" || minor=="smart phone")
		{
	} else*/ if(minor=="modem")
		 {
			 iconName="modem";
		 }
	} else if(major=="access point")
	{
			//TODO Add emblem indicating power
		iconName="modem";
	} else if(major=="audio/video")
	{
// 			TODO uncategorized,loudspeaker,portable audio, car audio,
// 			set-top box, hifi audio, vcr, video camera, camcorder,
// 			video monitor, video display and loudspeaker,
// 			video conferencing, gaming/toy, unknown
		if(minor=="headset" || minor=="handsfree" || minor=="headphones")
		{
			iconName="audio-headset";
		} else if(minor=="microphone")
		{
			iconName="audio-input-microphone";
		} else
			iconName="audio-headset";
	} else if(major=="peripheral")
	{
		if(minor=="uncategorized" || minor=="keyboard" || minor=="combo")
		{
			iconName="input-keyboard";
		} else if(minor=="pointing")
		{
			iconName="input-mouse";
		}
	} else if(major=="imaging")
	{
		if(minor=="display")
		{
			iconName="video-display";
		} else if(minor=="camera")
		{
			iconName="camera-photo";
		} else if(minor=="scanner")
		{
			iconName="scanner";
		} else if(minor=="printer")
		{
			iconName="printer";
		}
	} else if(major=="wearable")
	{
			//TODO wrist watch, pager, jacket, helmet, glasses
	} else if(major=="toy")
	{
			//TODO robot, vehicle, doll, controller, game
	}
	return KIconLoader::global()->loadIcon(iconName,group,size,state);
}

#include "iconfinder.moc"

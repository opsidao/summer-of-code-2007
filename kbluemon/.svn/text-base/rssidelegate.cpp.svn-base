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

#include "rssidelegate.h"
#include <QPainter>
#include <QProgressBar>
#include <QModelIndex>
#include <kdebug.h>

void RssiDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{	
	QString content = index.data(Qt::DisplayRole).toString();	
	QProgressBar bar;
	bar.setRange(-128,127);
	bar.setValue(content.remove("\%").toInt());
		
	QRect rect = bar.geometry();
	rect.setWidth(option.rect.width());
	rect.setHeight(option.rect.height());
	bar.setGeometry(rect);
		
	QPixmap pxm = QPixmap::grabWidget(&bar,rect);
	painter->drawPixmap(option.rect,pxm,rect);
}

QSize RssiDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &) const
{
	return QSize(option.rect.width(),option.rect.height());
}


RssiDelegate::RssiDelegate(QObject * parent)
	:QItemDelegate(parent)
{
}

#include "rssidelegate.moc"

//
// C++ Implementation: visuals
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "visuals.h"
#include <kdebug.h>
#include <QPainter>
// #include <plasma/widgets/label.h>
RadarItem::RadarItem(QGraphicsItem * parent, Plasma::Svg * theme)
	:QGraphicsItem(parent),theme(theme)
{
}

QRectF RadarItem::boundingRect() const
{
	QSize size = theme->elementSize("radar");
	return QRectF(-size.width()/2,-size.height()/2,size.width(),size.height());
}

void RadarItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	theme->paint(painter,boundingRect(),"radar");
}

RadarTraceItem::RadarTraceItem(QGraphicsItem * parent, Plasma::Svg * theme)
	:QGraphicsItem(parent),theme(theme)
{
}

QRectF RadarTraceItem::boundingRect() const
{
	QSize size = theme->elementSize("radarTrace");
	return QRectF(0,0,size.width(),size.height());
}

void RadarTraceItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	theme->paint(painter,boundingRect(),"radarTrace");
}

DeviceDotItem::DeviceDotItem(Plasma::Applet * parent, Plasma::Svg * theme,const QString &ubi)
	:QObject(parent),QGraphicsItem(parent),theme(theme),ubi(ubi)
{
	setAcceptsHoverEvents(true);
}

QRectF DeviceDotItem::boundingRect() const
{
	QSize size = theme->elementSize("devDot");
	return QRectF(-size.width()/2,-size.height()/2,size.width(),size.height());
}

void DeviceDotItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	theme->paint(painter,boundingRect(),"devDot");
}

InfoPanelItem::InfoPanelItem(QGraphicsItem * parent, Plasma::Svg * theme,Plasma::DataEngine *bluetoothEngine)
	:QGraphicsItem(parent),theme(theme),bluetoothEngine(bluetoothEngine)
{
	QSize size = theme->elementSize("infoPanel");
	setBounds(QRectF(-size.width()/2,-size.height()/2,size.width(),size.height()));
}

QRectF InfoPanelItem::boundingRect() const
{
	return bounds;
}

void InfoPanelItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option)
			Q_UNUSED(widget)
			painter->setRenderHint(QPainter::SmoothPixmapTransform);
	theme->paint(painter,boundingRect(),"infoPanel");
	QString text;
	QRectF bounds = boundingRect();
	QFontMetrics fm = painter->fontMetrics();
	if(!shownDeviceUbi.isEmpty())
	{
		Plasma::DataEngine::Data info = bluetoothEngine->query(shownDeviceUbi);
		
		if(!info.isEmpty())
		{
			if(info.contains("alias"))
				text = info["alias"].toString();
			else if(info.contains("name"))
				text = info["name"].toString();
		}
		if(text.isEmpty())
			text = info["address"].toString();
		int i = 4;
		do
		{
			i++;
			painter->setFont(QFont("Courier",i,QFont::Bold,false));
			fm=painter->fontMetrics();
		}
		while(fm.height() < (bounds.height()/1.5) && fm.boundingRect(text).width() < (bounds.width()));
		painter->setFont(QFont("Courier",i-1,QFont::Bold,false));
		painter->setPen(Qt::green);
		painter->drawText(boundingRect(),Qt::AlignHCenter | Qt::AlignTop,text);
		
		painter->setFont(QFont("Courier",(i-1)*2/3,QFont::Bold,false));
		painter->setPen(Qt::white);
		painter->drawText(boundingRect(),Qt::AlignHCenter | Qt::AlignBottom,"Click to get more info");
	} else
	{
		text = i18n("Hover the green dots to see remote names");
		int i = 4;
		do
		{
			i++;
			painter->setFont(QFont("Courier",i,QFont::Bold,false));
			fm=painter->fontMetrics();
		}
		while(fm.height() < bounds.height() && fm.boundingRect(text).width() < bounds.width());
		painter->setFont(QFont("Courier",i-1,QFont::Bold,false));
		painter->setPen(Qt::green);
		painter->drawText(boundingRect(),Qt::AlignCenter,text);
	}
	
}

void InfoPanelItem::setBounds(const QRectF & bounds)
{
	this->bounds=bounds ;
}

void DeviceDotItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event)
// 	kDebug() << "Mouse entered " << ubi << endl;
	emit mouseOver(true,ubi);
}

void DeviceDotItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
// 	kDebug() << "Mouse left " << ubi << endl;
	Q_UNUSED(event)
	emit mouseOver(false,ubi);
}

void InfoPanelItem::showDeviceName(bool show, const QString & ubi)
{
	shownDeviceUbi = show ? ubi:"";
// 	kDebug() << "Shown device = "<< shownDeviceUbi << endl;
	update();
}


#include "visuals.moc"

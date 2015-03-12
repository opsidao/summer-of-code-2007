//
// C++ Interface: visuals
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BLUETOOTH_APPLET_VISUALS
#define __BLUETOOTH_APPLET_VISUALS

#include <QGraphicsItem>
#include <plasma/svg.h>
#include <plasma/dataengine.h>
#include <plasma/applet.h>

class RadarItem :public QGraphicsItem
{	
	public:
		RadarItem(QGraphicsItem *parent, Plasma::Svg *theme);
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);
	private:
		Plasma::Svg * theme;
};

class RadarTraceItem : public QGraphicsItem
{
	public:
		RadarTraceItem(QGraphicsItem *parent, Plasma::Svg *theme);
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);
	private:
		Plasma::Svg * theme;
};

class DeviceDotItem: public QObject,public QGraphicsItem
{
	Q_OBJECT
	public:
		DeviceDotItem(Plasma::Applet *parent, Plasma::Svg *theme,const QString &ubi);
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);
	public Q_SLOTS:
		void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
		void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
	Q_SIGNALS:
		void mouseOver(bool isOver,const QString & ubi);
	private:
		Plasma::Svg * theme;		
		QString ubi;
};

class InfoPanelItem : public QObject,public QGraphicsItem
{
	Q_OBJECT
	public:
		InfoPanelItem(QGraphicsItem *parent, Plasma::Svg *theme,Plasma::DataEngine *bluetoothEngine);
		void setBounds(const QRectF &bounds);
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);
	public Q_SLOTS:
		void showDeviceName(bool show,const QString & ubi);
	private:
		Plasma::Svg * theme;
		QRectF bounds;
		QString shownDeviceUbi;
		Plasma::DataEngine *bluetoothEngine;
};

#endif

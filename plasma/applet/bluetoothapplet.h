//
// C++ Interface: bluetoothapplet
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __BLUETOOTH_PLASMA_APPLET
#define __BLUETOOTH_PLASMA_APPLET

#include <plasma/applet.h>
#include <plasma/dataengine.h>
#include <plasma/svg.h>

class RadarItem;
class RadarTraceItem;
class DeviceDotItem;
class InfoPanelItem;
class BluetoothApplet : public Plasma::Applet
{
	Q_OBJECT
	public:
		BluetoothApplet(QObject *parent, const QStringList &args);
		~BluetoothApplet();
		//From Plasma::Applet
		void constraintsUpdated();
// 		void showConfigurationInterface();
		void paintInterface(QPainter *painter,
				    const QStyleOptionGraphicsItem *option,
				    QWidget *widget = 0);
		QRectF boundingRect() const;
		QPainterPath shape() const;
		void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
		void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
		void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
	private slots:
		void moveRemotes();
		void mouseOverSlot(bool isOver);
		//To receive updates from the engine
		void updated(const QString &name, const Plasma::DataEngine::Data &data);
		void newSource(const QString& source);
		void sourceRemoved(const QString& source);
	Q_SIGNALS:
		void mouseOver(bool isOver);
	private:
		Plasma::DataEngine* bluetoothEngine;
		
		RadarItem *radar;
		RadarTraceItem *radarTrace;
		InfoPanelItem *infoPanel;
		
		QList<QString> remotesAlive;
		QMap<QString,DeviceDotItem*> remotesItems;
		
		Plasma::Svg* m_theme;
		
		QRect m_bounds;
		
		QString currentName;

		bool mouseIsOver;
};

K_EXPORT_PLASMA_APPLET(bluetooth, BluetoothApplet)

#endif

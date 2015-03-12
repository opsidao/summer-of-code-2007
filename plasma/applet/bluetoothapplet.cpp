//
// C++ Implementation: bluetoothapplet
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bluetoothapplet.h"
#include <QMessageBox>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <cstdlib>
#include <plasma/phase.h>
#include "visuals.h"

BluetoothApplet::BluetoothApplet(QObject * parent, const QStringList & args)
	:Plasma::Applet(parent,args),bluetoothEngine(0),radar(0),radarTrace(0),infoPanel(0),m_theme(0)
{
    setDrawStandardBackground(true);
	bluetoothEngine = dataEngine("bluetooth");
        kDebug() << "Engine created" <<  endl;
        if(!bluetoothEngine->isValid())
        {
/*		QMessageBox::critical(0,
				      i18n("No bluetooth engine available"),
				      i18n("Sorry, you need to have the bluetooth data engine installed in order to use this applet."));*/
             setFailedToLaunch(true,"Sorry, you need to have the bluetooth data engine installed in order to use this applet.");
            return;
        }
        bluetoothEngine->connectSource("adapters",this);
        Plasma::DataEngine::Data adapters = bluetoothEngine->query("adapters");
        if(adapters.isEmpty())
        {
/*            QMessageBox::critical(0,
                                  i18n("No bluetooth adapters available"),
                                  i18n("I can't find any compatible bluetooth adapter, make sure your adapter is detected correctly by the operating system"));*/
             setFailedToLaunch(true,"I can't find any compatible bluetooth adapter, make sure your adapter is detected correctly by the operating system");
            return;
        }

        srand(time(NULL));
        setAcceptsHoverEvents(true);
        m_bounds = QRect(-125,-125,250,250);
	
        m_theme = new Plasma::Svg("widgets/bluetooth-monitor-main", this);
        m_theme->setContentType(Plasma::Svg::ImageSet);
        m_theme->resize(m_bounds.width(),m_bounds.height());

	// Dots go in the 0 z layer
	radar = new RadarItem(this,m_theme);
	radar->setZValue(-1);
		
	radarTrace = new RadarTraceItem(this,m_theme);
//         radarTrace = new QGraphicsEllipseItem(QRectF(m_bounds),this );
//         radarTrace->setPen(QPen(QColor(0,0,0,0)));
//         
//         QRectF traceBounds = radarTrace->boundingRect();
//         QLinearGradient gradient(0,traceBounds.top()/2,0,0);
//         gradient.setColorAt(0.0,QColor(0,128,0,0));
//         gradient.setColorAt(0.7,QColor(0,128,0,50));
//         gradient.setColorAt(1.0,QColor(0,128,0,127));
//         radarTrace->setBrush(gradient);
//         radarTrace->setSpanAngle(720);
 	radarTrace->setZValue(1);
	
	infoPanel = new InfoPanelItem(this,m_theme,bluetoothEngine);
	infoPanel->setZValue(2);
	infoPanel->scale(0,0);
	
	//FIXME Needs deletion?
	QTimeLine *timeLine = new QTimeLine(10000);
	timeLine->setLoopCount(0);
	timeLine->setCurveShape(QTimeLine::LinearCurve);

	QObject::connect(timeLine,SIGNAL(frameChanged( int )),
			 this,SLOT(moveRemotes()));
	
	//FIXME Needs deletion?
	QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(this);
	animation->setItem(radarTrace);
	animation->setTimeLine(timeLine);
	
	for (int i = 0; i < 360; i++)
		animation->setRotationAt(i/360.0,i);
	
	timeLine->start();
	
	//Maybe the engine was started before... check if there are devices available	
	//FIXME This code is repeted in update
	if(bluetoothEngine->sources().contains("devices"))
	{
		bool requestPanelShow=false;
		QStringList newRemotes = bluetoothEngine->query("devices").keys();
		if(mouseIsOver && remotesAlive.size()==0 && newRemotes.size()>0)
			requestPanelShow=true;
		foreach(QString remote, newRemotes)
		{
			if(remotesAlive.contains(remote))
			{
				if(!newRemotes.contains(remote))
				{
					remotesAlive.removeAll(remote);
					remotesItems.remove(remote);
				}
			}
			else 
			{
				remotesAlive << remote;
				DeviceDotItem *dot = new DeviceDotItem(this,m_theme,remote);
				QObject::connect(dot,SIGNAL(mouseOver(bool,const QString&)),
						infoPanel,SLOT(showDeviceName(bool,const QString&)));				
				QRectF traceBounds = radarTrace->boundingRect();
				QPointF point(traceBounds.x()+traceBounds.width()/2,traceBounds.y()+traceBounds.height()/2);
				point = radarTrace->mapToItem(dot,point);
				dot->moveBy(point.x()*((rand()%99+1)/100.0),point.y()*((rand()%99+1)/100.0));
				dot->setZValue(0);
				remotesItems[remote] = dot;
			}
		}
		if(requestPanelShow)
			mouseOverSlot(true);//Force showing the infoPanel
	}
		
	QObject::connect(bluetoothEngine,SIGNAL(newSource( const QString& )),
			 this,SLOT(newSource(const QString&)));
	QObject::connect(bluetoothEngine,SIGNAL(sourceRemoved(const QString&)),
			 this,SLOT(sourceRemoved(const QString&)));
	constraintsUpdated();
}

BluetoothApplet::~ BluetoothApplet()
{	
	delete m_theme;
	delete radar;
	delete radarTrace;
	delete infoPanel;
}

void BluetoothApplet::constraintsUpdated()
{
// 	kDebug() << k_funcinfo << endl;
	update();
}

// void BluetoothApplet::showConfigurationInterface()
// {
// }

void BluetoothApplet::paintInterface(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	Q_UNUSED(painter);
}

void BluetoothApplet::updated(const QString & name, const Plasma::DataEngine::Data & data)
{
// 	kDebug() << "####" << k_funcinfo << " name : " << name << endl;
	if(name == "devices")
	{
		bool requestPanelShow=false;
		QStringList newRemotes = data.keys();
		if(mouseIsOver && remotesAlive.size()==0 && newRemotes.size()>0)
			requestPanelShow=true;
		foreach(QString remote, newRemotes)
		{
			if(!remotesAlive.contains(remote))
			{
				remotesAlive << remote;
				DeviceDotItem *dot = new DeviceDotItem(this,m_theme,remote);
				QObject::connect(dot,SIGNAL(mouseOver(bool,const QString&)),
						infoPanel,SLOT(showDeviceName(bool,const QString&)));				
				QRectF traceBounds = radarTrace->boundingRect();
				QPointF point(traceBounds.x()+traceBounds.width()/2,traceBounds.y()+traceBounds.height()/2);
				point = radarTrace->mapToItem(dot,point);
				dot->moveBy(point.x()*((rand()%99+1)/100.0),point.y()*((rand()%99+1)/100.0));
				dot->setZValue(0);
				remotesItems[remote] = dot;
			}
		}
                foreach(QString remote, remotesAlive)
                {
                    if(!newRemotes.contains(remote))
                    {
                        remotesItems[remote]->setParentItem(0);
                        remotesAlive.removeAll(remote);
                        remotesItems.remove(remote);
                    }
                }
		if(requestPanelShow)
			mouseOverSlot(true);//Force showing the infoPanel
		update();
	}
}

QRectF BluetoothApplet::boundingRect() const
{
    if(infoPanel)
	return m_bounds;
    else
        return Plasma::Applet::boundingRect();
}
QPainterPath BluetoothApplet::shape() const
{
    if(infoPanel)
    {
        QPainterPath path;
        if(!mouseIsOver)
        {
            path.addEllipse(boundingRect());
        } else
        {
            QRectF b = boundingRect();
            b.setHeight(b.height()+infoPanel->boundingRect().height());
            path.addRect(b);
        }
        return path;
    } else
        return Plasma::Applet::shape();
}
void BluetoothApplet::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event)
        if(infoPanel)
        {
            QPointF pos = infoPanel->pos();
            pos.setY(event->pos().y());
	    infoPanel->setPos(pos);
	    mouseOverSlot(true);
        }
}

void BluetoothApplet::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event)
        if(infoPanel)
	   mouseOverSlot(false);
}

void BluetoothApplet::newSource(const QString & source)
{
	bluetoothEngine->connectSource(source,this);
}

void BluetoothApplet::sourceRemoved(const QString & source)
{
	bluetoothEngine->disconnectSource(source,this);
}

void BluetoothApplet::moveRemotes()
{	
	if(!mouseIsOver)
	{
		foreach(DeviceDotItem *dot, remotesItems)
		{
			double step = ((rand()%10+1)/20.0);
			double dx,dy;
			switch(rand()%9)
			{
				case 0:
					dx=-step;
					dy=-step;
					break;
				case 1:
					dx=-step;
					dy=step;
					break;
				case 2:
					dx=step;
					dy=-step;
					break;
				case 3:
					dx=step;
					dy=step;
					break;
				case 4:
					dx=0;
					dy=step;
					break;
				case 5:
					dx=1;
					dy=step;
					break;
				case 6:
					dx=-1;
					dy=step;
					break;
				case 7:
					dx=0;
					dy=-step;
					break;
				case 8:
					dx=0;
					dy=0;
					break;
			}
			if(shape().contains(QPointF(dot->pos().x()+20*dx,dot->pos().y()+20*dy)))
				dot->moveBy(dx,dy);
			else if(shape().contains(QPointF(dot->pos().x()-20*dx,dot->pos().y()-20*dy)))
				dot->moveBy(-dx,-dy);
		}
	}
	update();
}

void BluetoothApplet::mouseOverSlot(bool isOver)
{	
// 	kDebug() << k_funcinfo << " isOver : " << isOver << endl;
	mouseIsOver = isOver;
	emit mouseOver(isOver);
	if(!remotesItems.isEmpty())
	{
		//FIXME Needs to be freed?
		QTimeLine *timeLine = new QTimeLine(1000);
		timeLine->setFrameRange(0, 100);
		timeLine->setLoopCount(1);
		timeLine->setCurveShape(QTimeLine::LinearCurve);
	
		//FIXME Needs to be freed?
		QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(this);
		animation->setItem(infoPanel);
		animation->setTimeLine(timeLine);
	
		if(isOver)
		{
			animation->setScaleAt(0,0,0);
			animation->setScaleAt(1,1,1);
		} else
		{
			animation->setScaleAt(0,1,1);
			animation->setScaleAt(1,0,0);
		}
		timeLine->start();
	}
}

void BluetoothApplet::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
// 	kDebug() << "Mouse on (" << event->pos().x() << "," << event->pos().y() << ")" << endl;
    if(infoPanel)
    {
	QPointF mousePos = infoPanel->pos();	
	mousePos.setY(event->pos().y()+infoPanel->boundingRect().height()/1.5);	
	infoPanel->setPos(mousePos);
    }
}

#include "bluetoothapplet.moc"

#include "levelbacking.h"
#include <QPainter>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

LevelBacking::LevelBacking(QWidget *parent) :
    QWidget(parent),
    levelBackingL(0.0),
    levelBackingR(0.0)
{
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void LevelBacking::setLevelBacking(qreal levelL, qreal levelR)
{
    if(levelBackingL!=levelL)
    {
        levelBackingL = levelL;
        update();
    }
    if(levelBackingR!=levelR)
    {
        levelBackingR = levelR;
        update();
    }
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void LevelBacking::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    qreal width_levelL = levelBackingL * width();
    qreal width_levelR = levelBackingR * width();
    painter.fillRect(0,0,width()*0.8,height(),Qt::darkGreen);
    painter.fillRect(width()*0.8,0,width(),height(),Qt::darkRed);
    if(width_levelL<width()*0.8)
    {
        painter.fillRect(0,0,width_levelL,height()/2,Qt::green);
    }

    if(width_levelL>=width()*0.8)
    {
        painter.fillRect(width()*0.8,0,width_levelL-width()*0.8,height()/2,Qt::red);
        painter.fillRect(0,0,width()*0.8,height()/2,Qt::green);
    }
    if(width_levelR<width()*0.8)
    {
        painter.fillRect(0,height()/2,width_levelR,height(),Qt::green);
    }

    if(width_levelR>=width()*0.8)
    {
        painter.fillRect(width()*0.8,height()/2,width_levelR-width()*0.8,height(),Qt::red);
        painter.fillRect(0,height()/2,width()*0.8,height(),Qt::green);
    }
}

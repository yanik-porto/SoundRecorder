#include "levelrec.h"
#include <QPainter>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

LevelRec::LevelRec(QWidget *parent) :
    QWidget(parent),
    levelRecL(0.0),
    levelRecR(0.0)
{
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void LevelRec::setLevel(qreal levelL, qreal levelR)
{
    if(levelRecL!=levelL)
    {
        levelRecL = levelL;
        update();
    }
    if(levelRecR!=levelR)
    {
        levelRecR = levelR;
        update();
    }
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void LevelRec::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    qreal width_levelL = levelRecL * width();
    qreal width_levelR = levelRecR * width();
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

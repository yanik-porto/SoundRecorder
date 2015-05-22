#include "leveloutput.h"
#include <QPainter>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

LevelOutput::LevelOutput(QWidget *parent) :
    QWidget(parent),
    levelOutputL(0.0),
    levelOutputR(0.0)
{
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void LevelOutput::setLevel(qreal levelL, qreal levelR)
{
    if(levelOutputL!=levelL)
    {
        levelOutputL = levelL;
        update();
    }
    if(levelOutputR!=levelR)
    {
        levelOutputR = levelR;
        update();
    }
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void LevelOutput::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    qreal height_levelL = levelOutputL * height();
    qreal height_levelR = levelOutputR * height();
    painter.fillRect(0,0,width(),height(),Qt::darkGreen);
    painter.fillRect(0,0,width(),height()*0.2,Qt::darkRed);
    if(height_levelL<height()*0.8)
    {
        painter.fillRect(0,height()-height_levelL,width()/2,height(),Qt::green);
    }

    if(height_levelL>=height()*0.8)
    {
        painter.fillRect(0,height()-height_levelL,width()/2,height()*0.2,Qt::red);
        painter.fillRect(0,height()*0.2,width()/2,height(),Qt::green);
    }

    if(height_levelR<height()*0.8)
    {
        painter.fillRect(width()/2,height()-height_levelR,width(),height(),Qt::green);
    }

    if(height_levelR>=height()*0.8)
    {
        painter.fillRect(width()/2,height()-height_levelR,width(),height()*0.2,Qt::red);
        painter.fillRect(width()/2,height()*0.2,width(),height(),Qt::green);
    }

}

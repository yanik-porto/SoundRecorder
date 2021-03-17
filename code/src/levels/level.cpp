#include "include/private/levels/level.h"
#include <QPainter>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Level::Level(QWidget *parent, LevelOrientation levelOrientation) :
    QWidget(parent),
    _levelL(0.0),
    _levelR(0.0),
    _levelOrientation(levelOrientation)
{
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void Level::setLevel(qreal levelL, qreal levelR)
{
    if(_levelL!=levelL)
    {
        _levelL = levelL;
        update();
    }
    if(_levelR!=levelR)
    {
        _levelR = levelR;
        update();
    }
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void Level::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

//    qreal dimSize1, dimSize2;
//    switch(_levelOrientation) {
//        case LevelOrientation::HORIZONTAL:
//            dimSize1 = width();
//            dimSize2 = height();
//        case LevelOrientation::VERTICAL:
//            dimSize1 = height();
//            dimSize2 = width();
//    }

//    QPainter painter(this);
//    qreal dimSize1_levelL = _levelL * dimSize1;
//    qreal dimSize1_levelR = _levelR * dimSize1;
//    painter.fillRect(0, 0, dimSize1*0.8, dimSize2, Qt::darkGreen);
//    painter.fillRect(dimSize1*0.8, 0, dimSize1, dimSize2, Qt::darkRed);
//    if(dimSize1_levelL < dimSize1*0.8)
//    {
//        painter.fillRect(0, 0, dimSize1_levelL, dimSize2/2, Qt::green);
//    }

//    if(dimSize1_levelL >= dimSize1*0.8)
//    {
//        painter.fillRect(dimSize1*0.8, 0, dimSize1_levelL-dimSize1*0.8, dimSize2/2, Qt::red);
//        painter.fillRect(0, 0, dimSize1*0.8, dimSize2/2, Qt::green);
//    }
//    if(dimSize1_levelR < dimSize1*0.8)
//    {
//        painter.fillRect(0, dimSize2/2, dimSize1_levelR, dimSize2,Qt::green);
//    }

//    if(dimSize1_levelR >= dimSize1*0.8)
//    {
//        painter.fillRect(dimSize1*0.8, dimSize2/2, dimSize1_levelR - dimSize1*0.8, dimSize2,Qt::red);
//        painter.fillRect(0, dimSize2/2, dimSize1*0.8, dimSize2, Qt::green);
//    }

    QPainter painter(this);
    if (_levelOrientation == LevelOrientation::HORIZONTAL) {
        qreal width_levelL = _levelL * width();
        qreal width_levelR = _levelR * width();
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
    } else {
        qreal height_levelL = _levelL * height();
        qreal height_levelR = _levelR * height();
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
}


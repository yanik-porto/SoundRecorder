#include "include/private/audio_spectre.h"

audioSpectre::audioSpectre(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(1024,200);
    setBackgroundRole(QPalette::NoRole);

    for(int i=0;i<1024;i++)
        m_intensityArray[i]=0;
}

audioSpectre::~audioSpectre(){}

void audioSpectre::set_intensity(int ind, const qreal &intensity)
{
    m_intensityArray[ind]=intensity;
    update();
}

void audioSpectre::reset()
{
    for(int i=0;i<1024;i++)
        m_intensityArray[i]=0;
}


void audioSpectre::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::black);


    for(int i=0;i<1024;i++)
    {
        painter.drawLine(i,100,i,100+100*m_intensityArray[i]);
        painter.drawLine(i,100,i,100-100*m_intensityArray[i]);
    }

    painter.setPen(Qt::white);
    painter.drawLine(0,0,1023,0);
    painter.drawLine(0,199,1023,199);


}

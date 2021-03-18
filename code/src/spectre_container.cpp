#include "include/private/spectre_container.h"

SpectreContainer::SpectreContainer(QWidget *parent) :
    QFrame(parent),
    cursorPos(0)
{
    setAutoFillBackground(true);
    setPalette(QPalette(QColor(200,200,200)));
    setStatusTip(tr("Clic in the lower or upper part to move the cursor"));
}

void SpectreContainer::mousePressEvent(QMouseEvent *event)
{
    cursorPos=event->pos().x();
    update();
}

void SpectreContainer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setViewport(this->rect());

    painter.setBrush(QBrush(Qt::darkGray));
    painter.drawRect(0,0,width(),26);
    painter.drawRect(0,226,width(),28);
    painter.drawRect(0,453,width(),28);

    painter.setPen(Qt::blue);
    painter.drawLine(cursorPos,height(),cursorPos,0);
}

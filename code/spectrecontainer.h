#ifndef SPECTRECONTAINER_H
#define SPECTRECONTAINER_H

#include <QFrame>
#include <QMouseEvent>
#include <QPainter>

class SpectreContainer : public QFrame
{
    Q_OBJECT
public:
    explicit SpectreContainer(QWidget *parent = 0);

    void set_cursorPos(const int &position){cursorPos=position;
                                            update();}
    const int &get_cursorPos(){return cursorPos;}

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    int cursorPos;


signals:

public slots:

};

#endif // SPECTRECONTAINER_H

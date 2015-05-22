#ifndef AUDIOSPECTRE_H
#define AUDIOSPECTRE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class audioSpectre : public QWidget
{
    Q_OBJECT
public:
    explicit audioSpectre(QWidget *parent = 0);
    ~audioSpectre();

    void set_intensity(int ind, const qreal &intensity);

    void reset();

protected:
    void paintEvent(QPaintEvent *event);

private:
    qreal m_intensityArray[1024];

signals:

public slots:

};

#endif // AUDIOSPECTRE_H

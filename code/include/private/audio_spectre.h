#ifndef AUDIOSPECTRE_H
#define AUDIOSPECTRE_H

#include "include/public/i_audio_spectre.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class audioSpectre : public QWidget, public IAudioSpectre
{
    Q_OBJECT
public:
    explicit audioSpectre(QWidget *parent = 0);
    ~audioSpectre();

    /**
     * @brief set audio spectre intensity
     * @param ind The index of the spectre intensity to set
     * @param intensity The new level for this intensity
     */
    virtual void SetIntensity(int ind, const qreal &intensity) override;

    /**
     * @brief reset audio spectre
     */
    virtual void Reset() override;

protected:
    void paintEvent(QPaintEvent *event);

private:
    qreal m_intensityArray[1024];

signals:

public slots:

};

#endif // AUDIOSPECTRE_H

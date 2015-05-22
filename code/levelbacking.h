/**
 * The LevelBacking class is responsible for painting one of the three
 * intensity bars in the GUI. It receives the intensity level sent by
 * one of the objects of the SoundTrack class through the MainWindow.
 */

#ifndef LEVELBACKING_H
#define LEVELBACKING_H

#include <QWidget>

class LevelBacking : public QWidget
{
    Q_OBJECT

public:
    explicit LevelBacking(QWidget *parent = 0);

    /**
     * Functions which receives the intensity level
     * emitted by the SoundTrack class. The interface
     * is made by the MainWindow.
     */
    void setLevelBacking(qreal levelL, qreal levelR);

protected:
    /**
     * The paintEvent in triggered by the update()
     * inside the setLevel function.
     */
    void paintEvent(QPaintEvent *event);

private:
    qreal levelBackingL;    // This class was designed for dealing
    qreal levelBackingR;    // with stereo tracks. However, only one
                            // intensity (mono) in being used for now.
};

#endif // LEVELBACKING_H

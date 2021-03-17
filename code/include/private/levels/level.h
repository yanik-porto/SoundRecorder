/**
 * The Level class is responsible for painting the Widget
 * representing one of the three intensity bars in the GUI. It receives
 * the intensity level sent by one of the objects of the SoundTrack
 * class through the MainWindow.
 */


#ifndef LEVEL_H
#define LEVEL_H

#include <QWidget>

enum class LevelOrientation {
    HORIZONTAL,
    VERTICAL
};

class Level : public QWidget
{
    Q_OBJECT

public:
    explicit Level(QWidget *parent = 0, LevelOrientation levelOrientation = LevelOrientation::HORIZONTAL);

    /**
     * Functions which receives the intensity level
     * emitted by the SoundTrack class. The interface
     * is made by the MainWindow.
     */
    void setLevel(qreal levelL, qreal levelR);

protected:
    /**
     * The paintEvent in triggered by the update()
     * inside the setLevel function.
     */
    void paintEvent(QPaintEvent *event);

private:
    /**
     * Normalized levels [0 to 1].
     */
    qreal _levelL;    // This class was designed for dealing
    qreal _levelR;    // with stereo tracks. However, only one
                        // intensity (mono) in being used for now.

    /**
     * orientation of the level intensity bar
     */
    LevelOrientation _levelOrientation;
};

#endif // LEVEL_H

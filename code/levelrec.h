/**
 * The LevelBacking class is responsible for painting the Widget
 * representing one of the three intensity bars in the GUI. It receives
 * the intensity level sent by one of the objects of the SoundTrack
 * class through the MainWindow.
 */


#ifndef LEVELREC_H
#define LEVELREC_H

#include <QWidget>

class LevelRec : public QWidget
{
    Q_OBJECT

public:
    explicit LevelRec(QWidget *parent = 0);

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
    qreal levelRecL;    // This class was designed for dealing
    qreal levelRecR;    // with stereo tracks. However, only one
                        // intensity (mono) in being used for now.
};

#endif // LEVELREC_H

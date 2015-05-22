/**
 * The LevelBacking class is responsible for painting one of the three
 * intensity bars in the GUI. It receives the intensity level sent by
 * the SoundEngine class object through the MainWindow. It only used in
 * editing mode.
 */

#ifndef LEVELOUTPUT_H
#define LEVELOUTPUT_H

#include <QWidget>

class LevelOutput : public QWidget
{
    Q_OBJECT

public:
    explicit LevelOutput(QWidget *parent = 0);

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
    qreal levelOutputL;     // This class was designed for dealing
    qreal levelOutputR;     // with stereo tracks. However, only one
                            // intensity (mono) in being (or will be,
                            // in the case of the output) used for now.
};

#endif // LEVELOUTPUT_H

#ifndef SPECTRECONTAINER_H
#define SPECTRECONTAINER_H

#include "include/public/i_spectre_container.h"

#include <QFrame>
#include <QMouseEvent>
#include <QPainter>

class SpectreContainer : public QFrame, public ISpectreContainer
{
    Q_OBJECT
public:
    explicit SpectreContainer(QWidget *parent = 0);

    /**
     * @brief set the cursor position
     * @param position The new postition of the cursor
     */
    virtual void SetCursorPos(const int &position) override;

    /**
     * @return The current cursor position
     */
    virtual const int &GetCursorPos() override;

protected:
    void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;

private:
    int cursorPos;


signals:

public slots:

};

#endif // SPECTRECONTAINER_H

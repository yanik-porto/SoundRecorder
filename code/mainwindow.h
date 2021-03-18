/**
 * The MainWindow class incorporates all the functionaties of the GUI
 * and triggers the functions in SoundEngine and SettingsDialog classes.
 * It is connected to the SoundEngine object and also receive signals
 * from the objecs of SoundTrack class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ISettingsDialog;
class ISoundEngine;
class IAudioSpectre;
class SpectreContainer;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPushButton;
class QComboBox;
class QLabel;
class QProgressBar;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    /**
     * Warning before closing the application.
     */
    void closeEvent(QCloseEvent *event);

private:
    /**
     * Creation and configuration of actions, menus and toolbars.
     */
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    /**
     * Filling the QScrollArea with the container of the audioSpectre.
     */
    void fillScrollArea();

    /**
     * Connection of signals and slots between the MainWindow and the
     * SoundEngine and SounTracks.
     */
    void connectUi();

    /**
     * Displaying audio information in the GUI
     */
    void displayAudioInfo();

private slots:  
    /**
     * Launches a simple "About" window.
     */
    void about();

    /**
     * Launches the Settings dialog.
     */
    void openSettingsDialog();

    /**
     * Launches the File dialog, for importing the wave file
     * for the backing track.
     */
    void openFileDialog();

    /**
     * Connections for some elements of the GUI. Some of them
     * control parameters and trigger actions in the SoundEngine
     * and other in the SoundTrack.
     */
    void on_checkboxRecEnable_stateChanged(int state);
    void on_checkboxMuteRec_stateChanged(int state);
    void on_checkboxMuteBack_stateChanged(int state);
    void on_checkboxMuteOut_stateChanged(int state);
    void on_sliderVolRecTrack_valueChanged(int volume);
    void on_sliderVolBackTrack_valueChanged(int volume);
    void on_sliderVolOutput_valueChanged(int volume);

public slots:
    /**
     * Functions responsible for receiving the levels emitted
     * by the SoundTrack objects and sending to the
     * intensity bar Widgets.
     */
    void receiveLevelRec(const qreal &level);
    void receiveLevelBacking(const qreal &level);

    /**
     * Functions responsible for receiving the levels emitted
     * by the SoundTrack objects and sending to the
     * AudioSpectre Widgets.
     */
    void drawSpectreRec(qreal level);
    void drawSpectreBack(qreal level);

    /**
     * Funtions responsible for updating the status (enabled or disabled)
     * of the buttons of the GUI, according to the status of the
     * SoundEngine.
     */
    void updateButtons();

    /**
     * Funtions responsible for receiving and displaying the messages
     * and status emmited by the SoundEngine and by the SoundTracks.
     */
    void displayMessage(const QString &message);
    void errorMessage(const QString &heading, const QString &detail);

    /**
     * Receives the time position of the recording or playing audio.
     * This time is used for setting the position of the cursor in the
     * QScrollArea and for the displaying the audio duration.
     */
    void receiveTimeLinePosition(const qint64 &time);

    /**
     * Funtions responsible for moving the cursor
     * while playing the tracks
     */
    void moveCursorWhilePlaying();

    /**
     * Send the cursor at the beginning
     */
    void rewind();

    /**
     * Send the cursor at the end of the longest track
     */
    void end();

    /**
     * Reset the WaveForms
     */
    void resetWaveForms();

    /**
     * update the position of the cursor and send it to the soundEngine
     */
    void changePosition();

private:
    Ui::MainWindow *ui;

    /**
     * Instantiation of the classes SoundEngine and SettingsDialog.
     */
    ISoundEngine *m_engine;
    ISettingsDialog *m_settingsDialog;

    /**
     * Instantiation of the attributes and classes to create the audioSpectre.
     */

    struct m_cursorCounter{
        int count;
        int offset;
    }m_count;

    int m_widNum;                       //Number of widget "m_audioSpectre" allocated

    IAudioSpectre *m_audioSpectreRec[10];
    SpectreContainer *m_spectreContainer;
    QHBoxLayout *m_hLayoutRec;
    QHBoxLayout *m_hLayoutBack;
    QVBoxLayout *m_vLayout;
    IAudioSpectre *m_audioSpectreBack[10];


    /**
     * GUI components.
     */
    QMenu *fileMenu;
    QMenu *transportMenu;
    QMenu *editMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *fileToolbar;
    QToolBar *transportToolbar;
    QToolBar *editToolbar;
    QToolBar *optionsToolbar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *importAct;
    QAction *exportAct;
    QAction *quitAct;
    QAction *playAct;
    QAction *recordAct;
    QAction *stopAct;
    QAction *rewindAct;
    QAction *endAct;
    QAction *reviewAct;
    QAction *settingsAct;
    QAction *aboutAct;
    QLabel *statusMode;
    QLabel *statusMessage;
    QProgressBar *statusProgressBar;
};

#endif // MAINWINDOW_H

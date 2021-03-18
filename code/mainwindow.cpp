#include "include/private/sound_engine.h"
#include "include/private/settings_dialog.h"
#include "include/private/sound_track.h"
#include "utilities.h"
#include "include/private/audio_spectre.h"
#include "include/private/spectre_container.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include/public/sound_recorder_exception.h"
#include <QTime>
#include <QtWidgets>


//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_engine(new SoundEngine(this)),
    m_settingsDialog(new SettingsDialog(
                m_engine->AvailableAudioInputDevices(),
                m_engine->AvailableAudioOutputDevices(),
                this)),
    m_widNum(0)
{
    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    connectUi();
    fillScrollArea();

    ui->groupboxBalanceRec->setEnabled(false);
    ui->groupboxBalanceBack->setEnabled(false);
    ui->groupboxBalanceOutput->setEnabled(false);
    ui->groupboxRecTrack->setEnabled(false);
    ui->groupboxBackTrack->setEnabled(false);
    ui->groupboxOutput->setEnabled(false);
    ui->groupboxSettings->setEnabled(false);

    ui->lcdTimeLime->display("00:00");
}

MainWindow::~MainWindow()
{
    while(m_widNum>0){
        delete m_audioSpectreRec[m_widNum];
        delete m_audioSpectreBack[m_widNum];
        m_widNum--;
    }
    delete m_audioSpectreRec[0];
    delete m_audioSpectreBack[0];

    delete m_hLayoutRec;
    delete m_hLayoutBack;
    delete m_vLayout;
    delete m_spectreContainer;
    delete ui;
    delete m_engine;
    delete m_settingsDialog;
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Sound Recorder"),
                     tr("All the data will be lost.\n"
                        "Are you sure you want to quit?"),
                     QMessageBox::Yes | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
        {
            event->accept();
        }
        else if (ret == QMessageBox::Cancel) event->ignore();
}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void MainWindow::createActions()
{
    // New session
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New session"), this);
    newAct->setShortcut(tr("CTRL+N"));
    newAct->setStatusTip(tr("Start a new session"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT());
    newAct->setEnabled(false);

    // Open session
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open session"), this);
    openAct->setShortcut(tr("CTRL+O"));
    openAct->setStatusTip(tr("Open a session"));
//    connect(openAct, SIGNAL(triggered()), this, SLOT());
    openAct->setEnabled(false);

    // Save session
    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save session"), this);
    saveAct->setShortcut(tr("CTRL+S"));
    saveAct->setStatusTip(tr("Save the current session"));
//    connect(saveAct, SIGNAL(triggered()), this, SLOT());
    saveAct->setEnabled(false);

    // Import from a .wav file
    importAct = new QAction(QIcon(":/images/import.png"), tr("&Import .wav"), this);
    importAct->setShortcut(tr("CTRL+I"));;
    importAct->setStatusTip(tr("Import backing track from a .wav file"));
    connect(importAct, SIGNAL(triggered()), this, SLOT(rewind()));
    connect(importAct, SIGNAL(triggered()), this, SLOT(openFileDialog()));
    importAct->setEnabled(false);

    // Export to a .wav file
    exportAct = new QAction(QIcon(":/images/export.png"), tr("&Export .wav"), this);
    exportAct->setShortcut(tr("CTRL+E"));
    exportAct->setStatusTip(tr("Export the current session to a .wav file"));
//    connect(exportAct, SIGNAL(triggered()), this, SLOT());
    exportAct->setEnabled(false);

    // Quit
    quitAct = new QAction(QIcon(":/images/quit.png"), tr("&Quit"), this);
    quitAct->setShortcut(tr("CTRL+Q"));
    quitAct->setStatusTip(tr("Exit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    quitAct->setEnabled(true);



    auto soundEngine = dynamic_cast<SoundEngine*>(m_engine);
    if (!soundEngine) {
        throw SoundRecorderException("Sound Engine is not the correct class");
    }

    // Record
    recordAct = new QAction(QIcon(":/images/record.png"), tr("&Record"), this);
    recordAct->setShortcut(tr("r"));
    recordAct->setStatusTip(tr("Record audio from the input"));
    connect(recordAct, SIGNAL(triggered()), soundEngine, SLOT(record()));
    recordAct->setEnabled(false);

    // Play
    playAct = new QAction(QIcon(":/images/play.png"), tr("&Play"), this);
    playAct->setShortcut(tr("p"));
    playAct->setStatusTip(tr("Play from the current position"));
    connect(playAct, SIGNAL(triggered()), this, SLOT(changePosition()));
    connect(playAct, SIGNAL(triggered()), soundEngine, SLOT(play()));
    playAct->setEnabled(false);

    // Stop
    stopAct = new QAction(QIcon(":/images/stop.png"), tr("&Stop"), this);
    stopAct->setShortcut(tr("s"));
    stopAct->setStatusTip(tr("Stop"));
    connect(stopAct, SIGNAL(triggered()), soundEngine, SLOT(stop()));
    stopAct->setEnabled(false);

    // Rewind
    rewindAct = new QAction(QIcon(":/images/rewind.png"), tr("&Rewind"), this);
    rewindAct->setStatusTip(tr("Go to the begining of the timeline"));
    connect(rewindAct, SIGNAL(triggered()), soundEngine, SLOT(rewind()));
    connect(rewindAct,SIGNAL(triggered()),this,SLOT(rewind()));
    rewindAct->setEnabled(false);

    // End
    endAct = new QAction(QIcon(":/images/end.png"), tr("&End"), this);
    endAct->setStatusTip(tr("Go to the end of the timeline"));
    connect(endAct, SIGNAL(triggered()), soundEngine, SLOT(end()));
    connect(endAct, SIGNAL(triggered()), this, SLOT(end()));
    endAct->setEnabled(false);

    // Review
    reviewAct = new QAction(QIcon(":/images/headphone.png"), tr("&Review"), this);
    reviewAct->setStatusTip(tr("Review and edit before exporting"));
    connect(reviewAct, SIGNAL(triggered()), soundEngine, SLOT(review()));
    reviewAct->setEnabled(false);

    // Audio Setting
    settingsAct = new QAction(QIcon(":/images/settings.png"), tr("&Audio Settings"), this);
    settingsAct->setStatusTip(tr("Open the Audio Settings"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));
    settingsAct->setEnabled(true);

    // About
    aboutAct = new QAction(QIcon(":/images/guitar.png"),tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    aboutAct->setEnabled(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(importAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    transportMenu = menuBar()->addMenu(tr("&Transport"));
    transportMenu->addAction(recordAct);
    transportMenu->addAction(playAct);
    transportMenu->addAction(stopAct);
    transportMenu->addAction(rewindAct);
    transportMenu->addAction(endAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(reviewAct);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(settingsAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolbar = addToolBar(tr("File"));
    fileToolbar->addAction(newAct);
    fileToolbar->addAction(openAct);
    fileToolbar->addAction(saveAct);
    fileToolbar->addAction(importAct);
    fileToolbar->addAction(exportAct);

    optionsToolbar = addToolBar(tr("Options"));
    optionsToolbar->addAction(settingsAct);

    transportToolbar = addToolBar(tr("Transport"));
    transportToolbar->addAction(recordAct);
    transportToolbar->addAction(playAct);
    transportToolbar->addAction(stopAct);
    transportToolbar->addAction(rewindAct);
    transportToolbar->addAction(endAct);

    editToolbar = addToolBar(tr("Edit"));
    editToolbar->addAction(reviewAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Initialize the Audio Device in Options->Audio Settings"));

    statusMessage = new QLabel("");
    statusMessage->setFixedWidth(400);
    statusMessage->setAlignment(Qt::AlignHCenter);

    statusMode = new QLabel("Not initialized");
    statusMode->setFixedWidth(100);
    statusMode->setAlignment(Qt::AlignHCenter);
    statusMode->setStyleSheet("QLabel {background-color: yellow;}");

    statusProgressBar = new QProgressBar(this);
    statusProgressBar->setTextVisible(false);
    statusProgressBar->setFixedWidth(100);
    statusProgressBar->setFixedHeight(15);
    statusProgressBar->setVisible(false);

    statusBar()->insertPermanentWidget(0,statusProgressBar);
    statusBar()->insertPermanentWidget(1,statusMessage);
    statusBar()->insertPermanentWidget(2,statusMode);

    statusProgressBar->setValue(0);
}

void MainWindow::connectUi()
{
    SoundEngine *soundEngine = dynamic_cast<SoundEngine*>(m_engine);
    if (!soundEngine) {
        throw SoundRecorderException("Sound Engine is not the correct class");
    }

    SoundTrack *recTrack = dynamic_cast<SoundTrack*>(m_engine->GetRecordingTrack());
    if (!recTrack) {
        throw SoundRecorderException("Recording Tracking is not a SoundTrack");
    }

    SoundTrack *backTrack = dynamic_cast<SoundTrack*>(m_engine->GetBackingTrack());
    if (!backTrack) {
        throw SoundRecorderException("Backing Tracking is not a SoundTrack");
    }

    connect(ui->checkboxMonitorEnable,SIGNAL(stateChanged(int)),
            recTrack,SLOT(toggleMonitor(int)));

    connect(ui->buttonImport,SIGNAL(clicked()),
            importAct,SIGNAL(triggered()));

//    connect(ui->buttonExport, SIGNAL(clicked()),
//            m_engine, SLOT());

//    connect(ui->dialBalanceRec, SIGNAL(valueChanged(int)),
//            m_engine, SLOT());

//    connect(ui->dialBalanceBack, SIGNAL(valueChanged(int)),
//            m_engine, SLOT());

//    connect(ui->dialBalanceOutput, SIGNAL(valueChanged(int)),
//            m_engine, SLOT());

    connect(soundEngine, SIGNAL(statusMessage(QString)),
            this, SLOT(displayMessage(QString)));

    connect(recTrack, SIGNAL(statusMessage(QString)),
            this, SLOT(displayMessage(QString)));

    connect(backTrack, SIGNAL(statusMessage(QString)),
            this, SLOT(displayMessage(QString)));

    connect(soundEngine,SIGNAL(statusChanged()),
            this,SLOT(updateButtons()));

    connect(backTrack, SIGNAL(errorMessage(QString,QString)),
            this, SLOT(errorMessage(QString,QString)));

    connect(recTrack,SIGNAL(sendLevel(qreal)),
            this,SLOT(receiveLevelRec(qreal)));

    connect(backTrack, SIGNAL(sendLevel(qreal)),
            this, SLOT(receiveLevelBacking(qreal)));

    connect(soundEngine, SIGNAL(timeLinePosition(qint64)),
            this,SLOT(receiveTimeLinePosition(qint64)));

    connect(soundEngine, SIGNAL(drawingRecEnabled(qreal)),
            this, SLOT(drawSpectreRec(qreal)));

    connect(soundEngine, SIGNAL(drawingBackEnabled(qreal)),
            this, SLOT(drawSpectreBack(qreal)));

    connect(soundEngine, SIGNAL(movingCursorEnabled()),
            this, SLOT(moveCursorWhilePlaying()));

    connect(backTrack, SIGNAL(statusMessage(QString)),
            this, SLOT());

    connect(soundEngine, SIGNAL(reset()),
            this, SLOT(resetWaveForms()));
}

void MainWindow::fillScrollArea()
{
    //Create the container for the AudioSpectre of the two tracks
    m_spectreContainer=new SpectreContainer();
    m_vLayout=new QVBoxLayout(m_spectreContainer);
    m_vLayout->setSpacing(0);
    m_vLayout->setContentsMargins(0,10,0,10);

    //Create the first QHBoxLayout for The Recording track
    m_hLayoutRec=new QHBoxLayout();
    m_hLayoutRec->setSpacing(0);
    m_hLayoutRec->setContentsMargins(0,10,0,10);
    m_audioSpectreRec[m_widNum]=new audioSpectre();
    m_hLayoutRec->addWidget(m_audioSpectreRec[m_widNum]);
    m_vLayout->addLayout(m_hLayoutRec);

    //Create the second QHBoxLayout for The Backing track
    m_hLayoutBack=new QHBoxLayout();
    m_hLayoutBack->setSpacing(0);
    m_hLayoutBack->setContentsMargins(0,10,0,10);
    m_audioSpectreBack[m_widNum]=new audioSpectre();
    m_hLayoutBack->addWidget(m_audioSpectreBack[m_widNum]);
    m_vLayout->addLayout(m_hLayoutBack);

    //Send the container inside the QScrollArea
    ui->scrollAreaAudioSpectre->setWidget(m_spectreContainer);

    //initialize the cursor position
    m_count.count=0;
    m_count.offset=0;
}

void MainWindow::displayAudioInfo()
{
    if (m_engine->GetFormat().sampleRate()!=-1)
    {
        ui->displayCurrentInput->setText(m_settingsDialog->InputDevice().deviceName());
        ui->displayCurrentOutput->setText(m_settingsDialog->OutputDevice().deviceName());
        ui->displaySampleRate->setText(QString("%1Hz").arg(m_engine->GetFormat().sampleRate()));
        ui->displayChannels->setText("1/2");
        ui->displaySampleSize->setText(QString("%1bits").arg(m_engine->GetFormat().sampleSize()));
        ui->displayCodec->setText(m_engine->GetFormat().codec());
        ui->displaySampleType->setText("SignedInt");
        ui->displayEndian->setText("LittleEndian");

        ui->displayInputFormat->setText(formatToString(m_engine->GetRecordingTrack()->GetFormat()));
    }
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Sound Recorder"),
                            tr("<b>Sound Recorder</b> is a simple Qt based application "
                               "for recording and editing soundtracks."));
}

void MainWindow::openSettingsDialog()
{
    auto settingsDialog = dynamic_cast<SettingsDialog*>(m_settingsDialog);
    if (!settingsDialog) {
        throw SoundRecorderException("the setting dialog is not from the SettingsDialog class ");
    }

    settingsDialog->exec();
    if (settingsDialog->result() == QDialog::Accepted) {
        m_engine->SetAudioInputDevice(m_settingsDialog->InputDevice());
        m_engine->SetAudioOutputDevice(m_settingsDialog->OutputDevice());
    }
    displayAudioInfo();
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::openFileDialog()
{
    const QString dir;
    const QStringList fileName = QFileDialog::getOpenFileNames(this, tr("Import WAV file"),
                                                               dir, "*.wav");
    if (!fileName.isEmpty()) m_engine->GetBackingTrack()->LoadFile(fileName.front());
    ui->displayFilePath->setText(fileName.front());
    ui->displayFilePath->setAlignment(Qt::AlignRight);
}

void MainWindow::on_checkboxRecEnable_stateChanged(int state)
{
    if (state == 0)
    {
        recordAct->setEnabled(false);
        ui->checkboxMuteRec->setEnabled(true);
    }
    if (state == 2)
    {
        recordAct->setEnabled(true);
        ui->checkboxMuteRec->setChecked(false);
        ui->checkboxMonitorEnable->setChecked(false);
        ui->checkboxMuteRec->setEnabled(false);
    }
}

void MainWindow::on_checkboxMuteRec_stateChanged(int state)
{
    if (state == 0)
    {
        m_engine->GetRecordingTrack()->SetVolume(ui->sliderVolRecTrack->value());
        ui->sliderVolRecTrack->setEnabled(true);
        ui->labelVolRec->setEnabled(true);
    }
    if (state == 2)
    {
        m_engine->GetRecordingTrack()->SetVolume(0);
        ui->sliderVolRecTrack->setEnabled(false);
        ui->labelVolRec->setEnabled(false);
    }
}

void MainWindow::on_checkboxMuteBack_stateChanged(int state)
{
    if (state == 0)
    {
        m_engine->GetBackingTrack()->SetVolume(ui->sliderVolBackTrack->value());
        ui->sliderVolBackTrack->setEnabled(true);
        ui->labelVolBack->setEnabled(true);
    }
    if (state == 2)
    {
        m_engine->GetBackingTrack()->SetVolume(0);
        ui->sliderVolBackTrack->setEnabled(false);
        ui->labelVolBack->setEnabled(false);
    }
}

void MainWindow::on_checkboxMuteOut_stateChanged(int state)
{
    if (state == 0)
    {
        m_engine->SetVolume(ui->sliderVolOutput->value());
        ui->sliderVolOutput->setEnabled(true);
        ui->labelVolOutput->setEnabled(true);
    }
    if (state == 2)
    {
        m_engine->SetVolume(0);
        ui->sliderVolOutput->setEnabled(false);
        ui->labelVolOutput->setEnabled(false);
    }
}

void MainWindow::on_sliderVolRecTrack_valueChanged(int volume)
{
    m_engine->GetRecordingTrack()->SetVolume(volume);
}

void MainWindow::on_sliderVolBackTrack_valueChanged(int volume)
{
    m_engine->GetBackingTrack()->SetVolume(volume);
}

void MainWindow::on_sliderVolOutput_valueChanged(int volume)
{
    m_engine->SetVolume(volume);
}

//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void MainWindow::receiveLevelRec(const qreal &level)
{
    ui->levelRec->setLevel(level, level);
}

void MainWindow::receiveLevelBacking(const qreal &level)
{
    ui->levelBacking->setLevel(level, level);
}

void MainWindow::drawSpectreRec(qreal level)
{
    if(m_count.count<1024)
    {
        m_spectreContainer->set_cursorPos(m_count.count+1024*m_count.offset);
        m_audioSpectreRec[m_count.offset]->set_intensity(m_count.count,level);
        m_count.count++;
    }
    if(m_count.count>=1024)
    {
        m_count.count=0;
        m_count.offset++;

        if(m_widNum<m_count.offset)
        {
            m_widNum++;
            m_audioSpectreRec[m_widNum]=new audioSpectre();
            m_audioSpectreBack[m_widNum]=new audioSpectre();
            m_hLayoutRec->addWidget(m_audioSpectreRec[m_widNum]);
            m_hLayoutBack->addWidget(m_audioSpectreBack[m_widNum]);
        }
    }

    ui->scrollAreaAudioSpectre->ensureVisible(m_count.count+1024*m_count.offset,0,300,0);
}

void MainWindow::drawSpectreBack(qreal level)
{
    if(m_count.count<1024){
        m_audioSpectreBack[m_count.offset]->set_intensity(m_count.count,level);
        m_count.count++;
    }
    else
    {
        m_count.count=0;
        m_count.offset++;
        if(m_widNum<m_count.offset)
        {
            m_widNum++;
            m_audioSpectreRec[m_widNum]=new audioSpectre();
            m_audioSpectreBack[m_widNum]=new audioSpectre();
            m_hLayoutRec->addWidget(m_audioSpectreRec[m_widNum]);
            m_hLayoutBack->addWidget(m_audioSpectreBack[m_widNum]);
        }


    }
}

void MainWindow::updateButtons()
{
    if (m_engine->GetStatus() == SoundEngine::Initialized)
    {
        ui->groupboxRecTrack->setEnabled(true);
        ui->groupboxBackTrack->setEnabled(true);
        ui->groupboxOutput->setEnabled(true);
        ui->groupboxSettings->setEnabled(true);
        ui->sliderVolRecTrack->setEnabled(true);
        ui->sliderVolBackTrack->setEnabled(true);
        ui->sliderVolOutput->setEnabled(true);
        ui->labelVolRec->setEnabled(true);
        ui->labelVolBack->setEnabled(true);
        ui->labelVolOutput->setEnabled(true);
    }

    if (m_engine->GetStatus() == SoundEngine::Stopped)
    {
        statusMode->setText("Stopped");
        statusMode->setStyleSheet("");

        settingsAct->setEnabled(true);

        if (ui->checkboxRecEnable->checkState() == 2)
            recordAct->setEnabled(true);
        stopAct->setEnabled(false);
        if (m_engine->GetBackingTrack()->IsFileDataLoaded() ||
                m_engine->GetRecordingTrack()->IsFileDataLoaded())
        {
            playAct->setEnabled(true);
            rewindAct->setEnabled(true);
            endAct->setEnabled(true);
            reviewAct->setEnabled(true);
        }
        importAct->setEnabled(true);
        ui->buttonImport->setEnabled(true);
        ui->checkboxMonitorEnable->setEnabled(true);
        ui->checkboxRecEnable->setEnabled(true);
        if (ui->checkboxRecEnable->checkState() == 0)
            ui->checkboxMuteRec->setEnabled(true);
        ui->checkboxMuteBack->setEnabled(true);
        ui->checkboxMuteOut->setEnabled(true);

        if (m_engine->GetBackingTrack()->IsFileDataLoaded())
        {
            ui->displayFileInfo->setText(formatToString(m_engine->GetBackingTrack()->GetFormat()));
            ui->displayFilePath->setText(".wav");
        }
    }

    if (m_engine->GetStatus() == SoundEngine::Recording)
    {
        statusMode->setText("Recording");
        statusMode->setStyleSheet("QLabel {background-color: red;}");

        settingsAct->setEnabled(false);

        recordAct->setEnabled(false);
        stopAct->setEnabled(true);
        playAct->setEnabled(false);
        rewindAct->setEnabled(false);
        endAct->setEnabled(false);
        reviewAct->setEnabled(false);

        importAct->setEnabled(false);
        ui->buttonImport->setEnabled(false);

        ui->checkboxRecEnable->setEnabled(false);
    }

    if (m_engine->GetStatus() == SoundEngine::Playing)
    {
        statusMode->setText("Playing");
        statusMode->setStyleSheet("QLabel {background-color: green;}");

        settingsAct->setEnabled(false);

        recordAct->setEnabled(false);
        stopAct->setEnabled(true);
        playAct->setEnabled(false);
        rewindAct->setEnabled(false);
        endAct->setEnabled(false);
        reviewAct->setEnabled(false);

        importAct->setEnabled(false);
        ui->buttonImport->setEnabled(false);
        ui->buttonExport->setEnabled(false);

        if (m_engine->GetRecordingTrack()->IsFileDataLoaded())
        {
            ui->checkboxMonitorEnable->setChecked(false);
            ui->checkboxMonitorEnable->setEnabled(false);
        }
        ui->checkboxRecEnable->setEnabled(false);

    }
}

void MainWindow::displayMessage(const QString &message)
{
    statusMessage->setText(message);
}

void MainWindow::errorMessage(const QString &heading, const QString &detail)
{
    QMessageBox::warning(this, heading, detail, QMessageBox::Close);
}

void MainWindow::receiveTimeLinePosition(const qint64 &time)
{
    if(time==0)rewind();

    short min = time/60000000;
    short sec = time/1000000%60;
    QTime displayTime;
    displayTime.setHMS(00, min, sec, 000);
    QString position = displayTime.toString("mm:ss");
    ui->lcdTimeLime->display(position);
}

void MainWindow::moveCursorWhilePlaying()
{
        if(m_count.count<1024)
            m_count.count++;
        else{
            m_count.count=0;
            m_count.offset++;
        }

        ui->scrollAreaAudioSpectre->ensureVisible(m_count.count+1024*m_count.offset,0,300,0);
        m_spectreContainer->set_cursorPos(m_count.count+1024*m_count.offset);
}

void MainWindow::rewind()
{
    m_count.count=0;
    m_count.offset=0;
    m_spectreContainer->set_cursorPos(m_count.count+1024*m_count.offset);
    //ui->scrollAreaAudioSpectre->horizontalScrollBar()->setSliderPosition(0);
    ui->scrollAreaAudioSpectre->ensureVisible(0,0,0,0);
}

void MainWindow::end()
{
    qint64 endPosCurs = m_engine->GetAudioDuration();
    endPosCurs=endPosCurs/40000;
    m_count.count=endPosCurs%1024;
    m_count.offset=endPosCurs/1024;
    m_spectreContainer->set_cursorPos(endPosCurs);
    ui->scrollAreaAudioSpectre->ensureVisible(endPosCurs,0,300,0);
}

void MainWindow::resetWaveForms()
{
    m_count.offset=m_widNum;
    while(m_count.offset>0)
    {
        m_audioSpectreRec[m_count.offset]->reset();
        m_audioSpectreBack[m_count.offset]->reset();
        m_count.offset--;
    }
    m_audioSpectreRec[0]->reset();
    m_audioSpectreBack[0]->reset();
}

void MainWindow::changePosition()
{
    if(m_engine->GetStatus() == SoundEngine::Stopped)
    {
        qint64 cursorPos=m_spectreContainer->get_cursorPos();
        m_count.count=cursorPos%1024;
        m_count.offset=cursorPos/1024;
        m_engine->SetTimeLinePosition(cursorPos*40000);
    }

}

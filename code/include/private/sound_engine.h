/**
 * The SoundEngine class manages the objects of SoundTrack class, triggering
 * functions and receiving signals. It is also in charge of controlling the
 * time line position and for the editing and exporting tasks (to be implemented
 * for the demo).
 */

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QFile>
#include <QBuffer>
#include <memory>

#include "include/public/i_sound_engine.h"

class InputMonitor;
class SoundTrack;

QT_BEGIN_NAMESPACE
class QAudioOutput;
QT_END_NAMESPACE

class SoundEngine : public QObject, public ISoundEngine
{
    Q_OBJECT

public:
    typedef std::shared_ptr<SoundEngine> Ptr;
    static SoundEngine::Ptr getInstance();
    virtual ~SoundEngine();


private:
    static  SoundEngine::Ptr _singleton;

    explicit SoundEngine(QObject *parent = 0);

    /**
     * Accessors
     */
    virtual const QList<QAudioDeviceInfo> &AvailableAudioInputDevices() const override;

    virtual const QList<QAudioDeviceInfo> &AvailableAudioOutputDevices() const override;

    virtual const QAudioFormat &GetFormat() const override;

    virtual ISoundEngine::Status GetStatus() override;

    virtual qint64 GetAudioDuration() override;

    virtual ISoundTrack *GetRecordingTrack() const override;

    virtual ISoundTrack *GetBackingTrack() const override;

    /**
     * Mutators
     */
    void SetStatus(Status status) override;

    void SetTimeLinePosition(const qint64 &position) override;

    /**
     * These functions receives the chosen IO devices from the SettingsDialog
     * and initializes the objects of the SoundTrack class, as well as the
     * editing/exporting output.
     */
    void SetAudioInputDevice(const QAudioDeviceInfo &device) override;
    void SetAudioOutputDevice(const QAudioDeviceInfo &device) override;

    /**
     * Receive the volume from the Slider in the GUI.
     */
    void SetVolume(int volume) override;

public slots:


    /**
     * Functions triggered by the GUI
     */
    void stop() override;
    void rewind() override;
    void end() override;
    void play() override;
    void record() override;
    void review() override;

    /**
     * Receive the signal emitted by SoundTrack class when the file status
     * is changed.
     */
    void fileStatusChanged();
    void updateAudioDuration();

    /**
     * Receive the signal emitted at each audio notification interval,
     * and processes the time line recording/playing position
     */
    void updateTimeLinePosition();

    /**
     * Sweep the inputFile of the backing track to send the levels to the
     * WaveForm
     */
    void sweepInputFile(const qint64 &audioLength, const QAudioFormat &format);


signals:

    /**
     * Signal emitted when the status is changed.
     */
    void statusChanged();

    /**
     * Signal emitted for displaying the status of
     * the SoundEngine in the GUI
     */
    void statusMessage(const QString &message);

    /**
     * Signal emitted for sending to the GUI the current
     * time line position
     */
    void timeLinePosition(const qint64 &time);

    /**
     * Signal emitted for drawing the spectre of
     * the recording track and backing track
     */
    void drawingRecEnabled(qreal level);
    void drawingBackEnabled(qreal level);

    /**
     * Signal emitted for moving the cursor of
     * the WaveForm container
     */
    void movingCursorEnabled();

    /**
     * Signal telling to reset all tracks
     */
    void reset();


private:
    /**
     * Initialize the audio devices
     */
    void initializeAudio();

    /**
     * Not implemented
     */
    void resetTracks();

    /**
     * Connects the signals and slots of the SoundEngine
     */
    void connectSoundEngine();



private:

    /**
     * Status of the SoundEngine
     */
    Status m_status;

    /**
     * Volume of the output. Only used when editing/reviewing befor exporting.
     */
    qreal m_volumeOutput;

    /**
     * Global information about the playing/recording position and
     * the duration of the SoundTracks.
     */
    qint64 m_audioDuration;
    qint64 m_timeLinePosition;

    /**
     * Two new files, to be used for editing and exporting the
     * recorded data (to be implemented for the demo)
     */
    QFile *m_recordedFile;
    QFile *m_backingFile;

    /**
     * Objects of the SoundTrack class. One for each track.
     */
    SoundTrack *m_recorderTrack;
    SoundTrack *m_backingTrack;

    /**
     * Output audio format, used for the exported file.
     */
    QAudioFormat m_formatOutput;

    /**
     * IO audio devices info. Retrieved from the SettingsDialog and
     * used for initializing the SoundTracks.
     */
    const QList<QAudioDeviceInfo> m_availableAudioInputDevices;
    QAudioDeviceInfo m_audioInputDeviceInfo;
    const QList<QAudioDeviceInfo> m_availableAudioOutputDevices;
    QAudioDeviceInfo m_audioOutputDeviceInfo;

    /**
     * Audio output, to be used when editing/reviewing befor exporting.
     */
    QAudioOutput *m_audioOutput;
    QBuffer       m_audioOutputIODevice;
};

#endif // SOUNDENGINE_H

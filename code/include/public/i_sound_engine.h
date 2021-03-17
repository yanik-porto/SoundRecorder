#ifndef I_SOUND_ENGINE_H
#define I_SOUND_ENGINE_H

#include <QObject>
#include <QAudioDeviceInfo>

class ISoundEngine {
public:
    virtual ~ISoundEngine(){}

    /**
     * Status list
     */
    enum Status{Initialized, Stopped, Playing, Recording, Editing};

    /**
     * @brief get the list of available input devices
     * @return the list of available input devices
     */
    virtual const QList<QAudioDeviceInfo> &AvailableAudioInputDevices() const = 0;

    /**
     * @brief get the list of available output devices
     * @return the list of available output devices
     */
    virtual const QList<QAudioDeviceInfo> &AvailableAudioOutputDevices() const = 0;

    /**
     * @brief get the audio format
     * @return the audio format
     */
    virtual const QAudioFormat &GetFormat() const = 0;

    /**
     * @brief Get sound engine status
     * @return status of the engine
     */
    virtual Status GetStatus() = 0;

    /**
     * @brief get audio duration
     * @return duration time of the audio
     */
    virtual qint64 GetAudioDuration() = 0;

    /**
     * @brief set sound engine status
     * @param[in] status the new sound engine status to set
     */
    virtual void SetStatus(Status status) = 0;

    /**
     * @brief set the time line position
     * @param[in] position the new time line position
     */
    virtual void SetTimeLinePosition(const qint64 &position) = 0;

    /**
     * @brief set the audio input device
     * @param[in] device the new device to set
     */
    virtual void SetAudioInputDevice(const QAudioDeviceInfo &device) = 0;

    /**
     * @brief set the audio output device
     * @param[in] device the new device to set
     */
    virtual void SetAudioOutputDevice(const QAudioDeviceInfo &device) = 0;

public slots:
    /**
     * Functions triggered by the GUI
     */
    virtual void stop() = 0;
    virtual void rewind() = 0;
    virtual void end() = 0;
    virtual void play() = 0;
    virtual void record() = 0;
    virtual void review() = 0;

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
};

#endif // I_SOUND_ENGINE_H

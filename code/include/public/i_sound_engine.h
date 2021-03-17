#ifndef I_SOUND_ENGINE_H
#define I_SOUND_ENGINE_H

#include "include/public/i_sound_track.h"

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

    /**
     * Receive the volume from the Slider in the GUI.
     */
    virtual void SetVolume(int volume) = 0;

    /**
     * @return the track responsible for recording
     */
    virtual ISoundTrack *GetRecordingTrack() const = 0;

    /**
     * @return the track responsible of the backing part
     */
    virtual ISoundTrack *GetBackingTrack() const = 0;

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

};

#endif // I_SOUND_ENGINE_H

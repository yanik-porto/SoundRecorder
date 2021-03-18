#ifndef I_SOUND_TRACK_H
#define I_SOUND_TRACK_H

#include "include/public/tools/wavfile.h"

#include <QAudioFormat>
#include <QAudioDeviceInfo>

class ISoundTrack {
public:
    virtual ~ISoundTrack() {};

    /**
     * @brief get the audio format
     * @return the audio format
     */
    virtual const QAudioFormat &GetFormat() const = 0;

    /**
     * @return Flag for indicating if there is any audio data for playing.
     */
    virtual bool IsFileDataLoaded() const = 0;

    /**
     * @brief Set the track volume
     * @param volume the new volume level
     */
    virtual void SetVolume(const int &volume) = 0;

    /**
    * @brief Triggered by the File dialog, it will analyze and open the
    * wave file, when the SoundTrack object is initialized as
    * BackingTrackMode.
    * @param[in] filename The path to the file to load
    */
    virtual void LoadFile(const QString &fileName) = 0;

    /**
     * @brief start recording at the given position
     * @param position The position where to start recording
     */
    virtual void Record(const qint64 &position) = 0;

    /**
     * @brief start playing at the given position
     * @param position The position where to start playing
     */
    virtual void Play(const qint64 &position) = 0;

    /**
     * @brief stop recording or playing
     */
    virtual void Stop() = 0;

    /**
     * @return the sound track level
     */
    virtual qreal GetLevel() const = 0;

    /**
     * @return the audio duration
     */
    virtual qint64 GetAudioDuration() const = 0;

    /**
     * @return the time position in the track
     */
    virtual qint64 GetTimePosition() const = 0;

    /**
     * Mode list. The mode, set by the SoundEngine when initializing
     * the SoundTracks, will define how the object will be initialized.
     */
     enum Mode {NoMode, RecorderMode, BackingTrackMode};

     /**
      * @brief set the sound track mode
      * @param mode The new mode to set
      */
     virtual void SetMode(Mode mode) = 0;

     /**
      * @return the input wav file
      */
     virtual WavFile *GetInputWavFile() const = 0;

     /**
     * @brief Initialized the SoundTrack with the information given by
     * SoundEngine
     * @param[in] inputDeviceInfo the input device info
     * @param[in] outputDeviceInfo the output device info
     */
     virtual void Initialize(const QAudioDeviceInfo &inputDeviceInfo,
                     const QAudioDeviceInfo &outputDeviceInfo) = 0;

public slots:
     /**
     * @brief Calculation of the level, considering an amount of samples equal the
     * buffer size. This level is used in the intensity bars and in the waveform.
     * @param[in] levelBuffer The buffer level
     * @param[in] samples The samples
     */
     virtual void CalculateLevel(const QByteArray &levelBuffer, const qint64 &samples) = 0;

};

#endif // I_SOUND_TRACK_H

#ifndef I_SOUND_TRACK_H
#define I_SOUND_TRACK_H

#include <QAudioFormat>

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

};

#endif // I_SOUND_TRACK_H

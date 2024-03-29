/**
 * The SoundTrack class is responsible for dealing with inputs and outputs,
 * recording, playing and monitoring the audio. Two objects of this class are
 * created in the SoundEngine. One for Recording and one for Backing Playing. *
 */

#ifndef SOUNDTRACK_H
#define SOUNDTRACK_H

#include "include/public/entities/i_sound_track.h"

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QByteArray>
#include <QFile>

class WavFile;

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioOutput;
QT_END_NAMESPACE

class SoundTrack : public QObject, public ISoundTrack
{
    Q_OBJECT

public:
    explicit SoundTrack(QObject *parent = 0);
    ~SoundTrack();

    /**
     * Accessors
     */
    virtual const QAudioFormat &GetFormat() const override;

    bool IsFileDataLoaded() const override;

    /**
     * @return the sound track level
     */
    virtual qreal GetLevel() const override;

    /**
     * @return the audio duration
     */
    virtual qint64 GetAudioDuration() const override;

    /**
     * @return the time position in the track
     */
    virtual qint64 GetTimePosition() const override;

    /**
     * @return the input wav file
     */
    virtual WavFile *GetInputWavFile() const override;

     /**
     * Mutators
     */
    void SetVolume(const int &volume) override;

    /**
     * @brief set the sound track mode
     * @param mode The new mode to set
     */
    void SetMode(Mode mode) override;

    /**
    * Triggered by the File dialog, it will analyze and open the
    * wave file, when the SoundTrack object is initialized as
    * BackingTrackMode.
    */
    void LoadFile(const QString &fileName) override;


    /**
     * @brief start recording at the given position
     * @param position The position where to start recording
     */
    virtual void Record(const qint64 &position) override;

    /**
     * @brief start playing at the given position
     * @param position The position where to start playing
     */
    virtual void Play(const qint64 &position) override;

    /**
     * @brief stop recording or playing
     */
    virtual void Stop() override;

    /**
    * @brief Initialized the SoundTrack with the information given by
    * SoundEngine
    * @param[in] inputDeviceInfo the input device info
    * @param[in] outputDeviceInfo the output device info
    */
    virtual void Initialize(const QAudioDeviceInfo &inputDeviceInfo,
                    const QAudioDeviceInfo &outputDeviceInfo) override;

private:  
    /**
    * Post initialization, when initialized as BackingTrackMode.
    * The information for the initialization will depend on the
    * format of the imported file.
    */
    void initializePlayer();

    /**
    * Not implemented
    */
    void reset();


public slots:

    /**
    * @brief Calculation of the level, considering an amount of samples equal the
    * buffer size. This level is used in the intensity bars and in the waveform.
    * @param[in] levelBuffer The buffer level
    * @param[in] samples The samples
    */
    virtual void CalculateLevel(const QByteArray &levelBuffer, const qint64 &samples) override;

    /**
    * Functions triggered by the MainWindow. It toggles the state of the
    * audio monitor, when the SoundTrack object is initialized as
    * RecorderMode.
    */
    void toggleMonitor(int state);

private slots:
    /**
    * Function triggered when there are bytes ready in the input buffer.
    */
    void monitor();

    /**
    * Functions triggered when there are bytes ready in the output buffer.
    */
    void readMore();

    /**
    * Handling of the automatic QAudioInput and QAudioOutput notifications.
    * The interval of notifications is specified in the initialize() function.
    * At each event, the time line position is verified and its value is sent to
    * the SoundEngine.
    */
    void handleOutputNotify();
    void handleInputNotify();

    /**
    * For practial reasons, this functions fills the recording file, when a larger
    * backing track file is loaded. It aims the let the two files with the same size.
    */
    void fillRecordingFile(const qint64 &audioLength, const QAudioFormat &format);

signals:
    /**
    * Signal emitted when the file status change. It is used for updating the
    * buttons status in the GUI.
    */
    void fileStatusChanged();

    /**
    * Signal for sending messages to the GUI.
    */
    void statusMessage(const QString &message);
    void errorMessage(const QString &heading, const QString &detail);

    /**
    * Signal emitted when a certain amount of data is ready for level
    * calculation.
    */
    void bufferReady(const QByteArray &levelBuffer, const qint64 &samples);

    /**
    * Signal emitted with the audio level of a certain amount of bytes (buffer size).
    */
    void sendLevel(const qreal &level);

    /**
    * Signal emitted when the audio duration change (new imported file
    * or just recorder file).
    */
    void audioDurationChanged();

    /**
    * Signal emitted at each QAudioInput/Output interval.
    */
    void audioTimePositionChanged();

    /**
    * Signal emitted when a new backing track file is imported.
    * It is used for filling with zeros the recording file.
    */
    void newBackingTrack(const qint64 &audioLength, const QAudioFormat &format);


private:
    /**
    * Initialization mode of the SoundTrack (Recorder or Backing Track).
    */
    Mode m_mode;

    /**
    * Flag for indicating if ther is any audio data for playing.
    */
    bool m_flagFile;

    /**
    * Flag for indicating if the devices of the SoundTrack where initialized.
    */
    bool m_flagDevice;

    /**
    * The buffer size will depend on the number of channels of the SoundTrack.
    * In RecorderMode, it will be always have one channel. It must be compatible
    * with the sample rate, the notify interval and the number of channels.
    */
    int m_bufferSize;
    int m_usedBufferSize;

    /**
    * General data of the SoundTrack.
    */
    qreal m_volume;
    qreal m_level;
    qint64 m_position;
    qint64 m_timePosition;
    qint64 m_audioLength;
    qint64 m_audioDuration;

    /**
    * Files to be used in the SoundTrack. In case of initialization in
    * RecorderMode, the QFile (raw file) will be used. In case of
    * initialization in BackingTrackMode, the WavFile class will be used
    * for importing and retrieving information from the wave file.
    */
    WavFile *m_inputFile;
    QFile *m_rawFile;

    /**
    * Buffers (IO buffer and monitor buffer)
    */
    QByteArray m_buffer;
    QByteArray m_monitorBuffer;

    /**
    * Object which stores the format of the SoundTrack
    */
    QAudioFormat m_format;

    /**
    * Input audio objects
    */
    QAudioDeviceInfo m_audioInputDeviceInfo;    // Chosen hardware or driver.
    QIODevice *m_inputDevice;       // Base interface between hardware and QAudioOutput.
    QAudioInput *m_audioInput;      // Interface between audio data and QIODevice.
    QAudioInput *m_auxInput;        // Same as above. Used for the monitor.

    /**
    * Output audio objects
    */
    QAudioDeviceInfo m_audioOutputDeviceInfo;   // Chosen hardware or driver.
    QIODevice *m_outputDevice;      // Base interface between hardware and QAudioOutput.
    QAudioOutput *m_audioOutput;    // Interface between audio data and QIODevice.
    QAudioOutput *m_auxOutput;      // Same as above. Used for the monitor.
};

#endif // SOUNDTRACK_H

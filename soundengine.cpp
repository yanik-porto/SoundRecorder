#include "soundengine.h"
#include "soundtrack.h"
#include "utilities.h"
#include "wavfile.h"
#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

SoundEngine::SoundEngine(QObject *parent) :
    QObject(parent),
    m_volumeOutput(0.6),
    m_audioDuration(0),
    m_timeLinePosition(0),
    m_recorderTrack(new SoundTrack(this)),
    m_backingTrack(new SoundTrack(this)),
    m_availableAudioInputDevices
        (QAudioDeviceInfo::availableDevices(QAudio::AudioInput)),
    m_audioInputDeviceInfo(QAudioDeviceInfo::defaultInputDevice()),
    m_availableAudioOutputDevices
        (QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)),
    m_audioOutputDeviceInfo(QAudioDeviceInfo::defaultOutputDevice())
{
    connectSoundEngine();
}

SoundEngine::~SoundEngine()
{
    delete m_recorderTrack;
    delete m_backingTrack;
}

void SoundEngine::setTimeLinePosition(const qint64 &position)
{
    m_timeLinePosition=position;
    emit timeLinePosition(m_timeLinePosition);
}

//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void SoundEngine::setAudioInputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioInputDeviceInfo.deviceName())
    {
        m_audioInputDeviceInfo = device;
        initializeAudio();
    }
}

void SoundEngine::setAudioOutputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioOutputDeviceInfo.deviceName())
    {
        m_audioOutputDeviceInfo = device;
        initializeAudio();
    }
}

void SoundEngine::stop()
{
    NOTIFY  << "SoundEngine::stop()";
    if (m_recorderTrack->m_flagFile) m_recorderTrack->stop();
    if (m_backingTrack->m_flagFile) m_backingTrack->stop();
    setStatus(Stopped);
    emit statusChanged();

}

void SoundEngine::rewind()
{
    NOTIFY  << "SoundEngine::rewind()";
    m_timeLinePosition = 0;
    emit timeLinePosition(0);
}

void SoundEngine::end()
{
    NOTIFY  << "SoundEngine::end()";
    m_timeLinePosition = m_audioDuration;
    emit timeLinePosition(m_audioDuration);
}

void SoundEngine::play()
{
    NOTIFY  << "SoundEngine::play()";
    if (m_timeLinePosition<m_audioDuration)
    {
    if (m_recorderTrack->m_flagFile) m_recorderTrack->play(m_timeLinePosition);
    if (m_backingTrack->m_flagFile) m_backingTrack->play(m_timeLinePosition);
    setStatus(Playing);
    emit statusChanged();
    }
}

void SoundEngine::record()
{
    NOTIFY << "SoundEngine::record()";
    if (true)
        m_recorderTrack->record(m_timeLinePosition);
    if (m_backingTrack->m_flagFile)
        m_backingTrack->play(m_timeLinePosition);
    setStatus(Recording);
    emit statusChanged();
}

void SoundEngine::review()
{
    NOTIFY << "SoundEngine::review()";
}

void SoundEngine::fileStatusChanged()
{
    setStatus(Stopped);
    emit statusChanged();
}

void SoundEngine::updateAudioDuration()
{
    m_audioDuration = qMax(m_recorderTrack->m_audioDuration, m_backingTrack->m_audioDuration);
}

void SoundEngine::updateTimeLinePosition()
{
    if (m_status == Playing)
    {
        if (m_recorderTrack->m_audioDuration > m_backingTrack->m_audioDuration){
            if((m_recorderTrack->m_timePosition-m_timeLinePosition)>0)
                 emit movingCursorEnabled();
            m_timeLinePosition = m_recorderTrack->m_timePosition;

        }

        else {
            if((m_backingTrack->m_timePosition-m_timeLinePosition)>0)
                 emit movingCursorEnabled();
             m_timeLinePosition = m_backingTrack->m_timePosition;
        }


        if (m_timeLinePosition >= m_audioDuration)
            stop();
    }

    if (m_status == Recording)
    {
        if((m_recorderTrack->m_timePosition-m_timeLinePosition)>0)
            emit drawingRecEnabled(m_recorderTrack->m_level);
        m_timeLinePosition = m_recorderTrack->m_timePosition;
    }
    emit timeLinePosition(m_timeLinePosition);
}

void SoundEngine::sweepInputFile(const qint64 &audioLength, const QAudioFormat &format)
{
    emit reset();

    int channelCount=format.channelCount();
    QByteArray startBuffer(4410*channelCount,0);
    m_backingTrack->m_inputFile->seek(m_backingTrack->m_inputFile->headerLength());

        while(!m_backingTrack->m_inputFile->atEnd())
        {
            m_backingTrack->m_inputFile->read(startBuffer.data(),3528*channelCount);
            m_backingTrack->calculateLevel(startBuffer,3528*channelCount);
            emit drawingBackEnabled(m_backingTrack->m_level);
        }

    emit timeLinePosition(0);

}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------


void SoundEngine::initializeAudio()
{
    m_formatOutput.setSampleRate(44100);
    m_formatOutput.setChannelCount(2);
    m_formatOutput.setSampleSize(16);
    m_formatOutput.setSampleType(QAudioFormat::SignedInt);
    m_formatOutput.setByteOrder(QAudioFormat::LittleEndian);
    m_formatOutput.setCodec("audio/pcm");

    NOTIFY  << "SoundEngine::initializeAudio()"
            << formatToString(m_formatOutput);

    m_recorderTrack->setMode(SoundTrack::RecorderMode);
    m_recorderTrack->initialize(m_audioInputDeviceInfo, m_audioOutputDeviceInfo);

    m_backingTrack->setMode(SoundTrack::BackingTrackMode);
    m_backingTrack->initialize(m_audioInputDeviceInfo, m_audioOutputDeviceInfo);

    m_audioOutput = new QAudioOutput(m_audioOutputDeviceInfo, m_formatOutput, this);

    setStatus(Initialized);
    emit statusChanged();
    emit statusMessage("Audio initialized");

    setStatus(Stopped);
    emit statusChanged();
}


void SoundEngine::resetTracks()
{
}


void SoundEngine::connectSoundEngine()
{
    connect(m_backingTrack,SIGNAL(fileStatusChanged()),
            this,SLOT(fileStatusChanged()));

    connect(m_recorderTrack,SIGNAL(audioDurationChanged()),
            this,SLOT(updateAudioDuration()));
    connect(m_backingTrack,SIGNAL(audioDurationChanged()),
            this,SLOT(updateAudioDuration()));

    connect(m_recorderTrack,SIGNAL(audioTimePositionChanged()),
            this,SLOT(updateTimeLinePosition()));
    connect(m_backingTrack,SIGNAL(audioTimePositionChanged()),
            this,SLOT(updateTimeLinePosition()));

    connect(m_backingTrack,SIGNAL(newBackingTrack(qint64,QAudioFormat)),
            m_recorderTrack,SLOT(fillRecordingFile(qint64,QAudioFormat)));

    connect(m_backingTrack,SIGNAL(newBackingTrack(qint64,QAudioFormat)),
            this,SLOT(sweepInputFile(qint64,QAudioFormat)));
}


void SoundEngine::setVolume(int volume)
{
    qreal m_volumeOutput = qreal(volume)/100;
    NOTIFY  << "Output volume" << m_volumeOutput;
}

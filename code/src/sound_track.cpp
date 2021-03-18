#include "include/private/sound_track.h"
#include "include/public/tools/utilities.h"
#include "include/public/tools/wavfile.h"
#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const qreal peakValue = 32768;

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

SoundTrack::SoundTrack(QObject *parent) :
    QObject(parent),
    m_mode(NoMode),
    m_flagFile(false),
    m_flagDevice(false),
    m_bufferSize(0),
    m_volume(0.6),
    m_level(0.0),
    m_position(0),
    m_timePosition(0),
    m_audioLength(0),
    m_audioDuration(0),
    m_buffer(4410,0),
    m_monitorBuffer(4410,0),
    m_audioInput(0),
    m_auxInput(0),
    m_audioOutput(0),
    m_auxOutput(0),
    m_usedBufferSize(3528)
{
}

SoundTrack::~SoundTrack()
{
    if (m_mode == BackingTrackMode && m_flagFile) delete m_inputFile;
    if (m_mode == RecorderMode) delete m_rawFile;
    if (m_audioInput) delete m_audioInput;
    if (m_audioOutput) delete m_audioOutput;
    if (m_auxInput) delete m_auxInput;
    if (m_auxOutput) delete m_auxOutput;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
const QAudioFormat &SoundTrack::GetFormat() const {
    return m_format;
}

bool SoundTrack::IsFileDataLoaded() const {
    return m_flagFile;
}

qreal SoundTrack::GetLevel() const {
    return m_level;
}

qint64 SoundTrack::GetAudioDuration() const {
    return m_audioDuration;
}

qint64 SoundTrack::GetTimePosition() const {
    return m_timePosition;
}

WavFile *SoundTrack::GetInputWavFile() const {
    return m_inputFile;
}

void SoundTrack::SetVolume(const int &volume) {
    m_volume = (qreal)volume/100;
}

void SoundTrack::SetMode(ISoundTrack::Mode mode) {
    m_mode = mode;
}

void SoundTrack::LoadFile(const QString &fileName)
{
    NOTIFY  << "BackingTrack::loadFile()"
            << fileName;

    if (!fileName.isEmpty())
    {
        m_inputFile = new WavFile(this);
        if (m_inputFile->open(fileName))
        {
            if (isPCMS16LE(m_inputFile->fileFormat()))
                 initializePlayer();
            else emit errorMessage(tr("Audio format not supported"),
                                   formatToString(m_inputFile->fileFormat()));
        }
        else emit errorMessage(tr("Could not open file"), fileName);
    }
}

//-----------------------------------------------------------------------------
// Private Functions
//-----------------------------------------------------------------------------

void SoundTrack::Initialize(const QAudioDeviceInfo &inputDeviceInfo,
                            const QAudioDeviceInfo &outputDeviceInfo)
{
    m_audioInputDeviceInfo = inputDeviceInfo;
    m_audioOutputDeviceInfo = outputDeviceInfo;

    if (m_mode == NoMode) NOTIFY << "Set the mode of the SoundTrack";

    if (m_mode == RecorderMode)
    {
        m_format.setSampleRate(44100);
        m_format.setChannelCount(1);
        m_format.setSampleSize(16);
        m_format.setSampleType(QAudioFormat::SignedInt);
        m_format.setByteOrder(QAudioFormat::LittleEndian);
        m_format.setCodec("audio/pcm");
        m_bufferSize = (m_format.sampleRate()/10)*m_format.channelCount();
        m_usedBufferSize = 3528*m_format.channelCount();
        m_buffer.resize(m_bufferSize);
        m_monitorBuffer.resize(m_bufferSize);
        m_auxInput = new QAudioInput(m_audioInputDeviceInfo, m_format, this);
        m_auxOutput = new QAudioOutput(m_audioOutputDeviceInfo, m_format, this);
        m_audioInput = new QAudioInput(m_audioInputDeviceInfo, m_format, this);
        m_audioOutput = new QAudioOutput(m_audioOutputDeviceInfo, m_format, this);
        m_audioOutput->setNotifyInterval(40);
        m_audioInput->setNotifyInterval(40);
        connect(m_audioOutput,SIGNAL(notify()),SLOT(readMore()));
        connect(m_audioOutput,SIGNAL(notify()),SLOT(handleOutputNotify()));
        connect(m_audioInput,SIGNAL(notify()),SLOT(handleInputNotify()));
        m_flagDevice = true;
        m_rawFile = new QFile("recording.raw");
        if (m_rawFile->size() > 0) m_rawFile->remove();
        connect(this,SIGNAL(bufferReady(QByteArray,qint64)),
                this,SLOT(CalculateLevel(QByteArray,qint64)));
        NOTIFY  << "SoundTrack::initialize() in RecorderMode";
        NOTIFY  << "Format:" << formatToString(m_format);
    }
}

void SoundTrack::initializePlayer()
{
    if (m_mode == BackingTrackMode)
    {
        m_format = m_inputFile->fileFormat();
        m_bufferSize = (m_format.sampleRate()/10)*m_format.channelCount();
        m_usedBufferSize = 3528*m_format.channelCount();
        m_buffer.resize(m_bufferSize);
        m_audioLength = (m_inputFile->size())-44;
        m_audioDuration = audioDuration(m_format,(m_inputFile->size())-44);
        m_audioOutput = new QAudioOutput(m_audioOutputDeviceInfo, m_format, this);
        m_audioOutput->setNotifyInterval(40);
        connect(m_audioOutput,SIGNAL(notify()),SLOT(readMore()));
        connect(m_audioOutput,SIGNAL(notify()),SLOT(handleOutputNotify()));
        m_flagDevice = true;
        m_flagFile = true;
        emit fileStatusChanged();
        emit audioDurationChanged();
        emit newBackingTrack(m_audioLength, m_format);
        emit audioTimePositionChanged();
        m_timePosition = 0;
        connect(this,SIGNAL(bufferReady(QByteArray,qint64)),
                this,SLOT(CalculateLevel(QByteArray,qint64)));
        NOTIFY  << "SoundTrack::initialize() in BackingTrackMode";
        NOTIFY  << "Format:" << formatToString(m_format);
        NOTIFY  << "Imported audio length:" << m_audioLength << "bytes";
        NOTIFY  << "Imported audio duration:" << m_audioDuration << "us";
    }
}

void SoundTrack::reset()
{
     if (m_mode == RecorderMode)
     {
     }
     if (m_mode == BackingTrackMode)
     {
     }
}

void SoundTrack::Record(const qint64 &position)
{
    m_timePosition = position;
    m_position = audioLength(m_format,m_timePosition);
    m_rawFile->open(QIODevice::WriteOnly | QIODevice::Append);
    m_rawFile->seek(m_position);
    m_audioInput->start(m_rawFile);
    m_flagFile = true;
}

void SoundTrack::Play(const qint64 &position)
{
    if (m_mode == RecorderMode)
    {
        m_timePosition = position;
        m_position = audioLength(m_format,m_timePosition);
        toggleMonitor(0);
        m_rawFile->open(QIODevice::ReadOnly);
        m_rawFile->seek(m_position);
        m_outputDevice = m_audioOutput->start();
        QByteArray startBuffer(m_bufferSize,0);
        m_outputDevice->write(startBuffer.constData(), m_bufferSize);
        emit m_audioOutput->notify();
    }
    if (m_mode == BackingTrackMode)
    {
        m_timePosition = position;
        m_position = audioLength(m_format,m_timePosition);
        m_inputFile->seek(m_position + m_inputFile->headerLength());
        m_outputDevice = m_audioOutput->start();
        QByteArray startBuffer(m_bufferSize,0);
        m_outputDevice->write(startBuffer.constData(), m_bufferSize);
        emit m_audioOutput->notify();
    }
}

void SoundTrack::Stop()
{
    if (m_mode == RecorderMode)
    {
        if (m_rawFile->openMode()==(QIODevice::WriteOnly | QIODevice::Append))
        {
            m_audioInput->stop();
            m_rawFile->close();
            m_position = 0;
            m_audioLength = (m_rawFile->size());
            m_audioDuration = audioDuration(m_format,(m_rawFile->size()));
            emit audioDurationChanged();
            NOTIFY  << "Recorded audio length:" << m_audioLength << "bytes";
            NOTIFY  << "Recorder audio duration:" << m_audioDuration << "us";
        }
        if (m_rawFile->openMode() == QIODevice::ReadOnly)
        {
            m_audioOutput->stop();
            m_rawFile->close();
            m_position = 0;
        }
    }
    if  (m_mode == BackingTrackMode)
    {
        m_audioOutput->stop();
        m_position = 0;
    }
    emit sendLevel(0.0);
}

//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void SoundTrack::toggleMonitor(int state)
{
    if (m_mode == RecorderMode)
    {
        if (state == 2)
        {
            m_outputDevice = m_auxOutput->start();
            m_inputDevice = m_auxInput->start();
            connect(m_inputDevice,SIGNAL(readyRead()),SLOT(monitor()));
            emit statusMessage("Monitoring the input");
        }
        if (state == 0)
        {
            m_auxOutput->stop();
            m_auxInput->stop();
            emit sendLevel(0.0);
            emit statusMessage("");
        }
    }
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void SoundTrack::monitor()
{
    if(!m_auxInput)
        return;
    qint64 availableData = m_auxInput->bytesReady();
    if(availableData > m_bufferSize)
        availableData = m_bufferSize;
    qint64 dataRead = m_inputDevice->read(m_monitorBuffer.data(), availableData);
    if(dataRead > 0)
    {
        short int *ptrBuffer;
        ptrBuffer = (short int*)m_monitorBuffer.data();
        for(int i=0; i<availableData/2; i++) ptrBuffer[i] *= m_volume;
        if (m_rawFile->openMode()!=(QIODevice::ReadOnly))
            emit bufferReady(m_monitorBuffer, availableData);
        m_outputDevice->write(m_monitorBuffer.constData(), dataRead);
    }
}

void SoundTrack::readMore()
{
    if (m_mode == RecorderMode)
    {
        //m_rawFile->seek(m_position);
        m_rawFile->read(m_buffer.data(), m_usedBufferSize);
        short int *ptrBuffer;
        ptrBuffer = (short int*)m_buffer.data();
        for(int i=0; i<m_bufferSize/2; i++) ptrBuffer[i]*=m_volume;
        emit bufferReady(m_buffer, m_usedBufferSize);
        m_outputDevice->write(m_buffer.constData(), m_usedBufferSize);
        m_audioDuration = audioDuration(m_format,(m_position));
        m_position = m_position+m_usedBufferSize;
    }

    if (m_mode == BackingTrackMode)
    {
        //m_inputFile->seek(m_position + m_inputFile->headerLength());
        m_inputFile->read(m_buffer.data(), m_usedBufferSize);
        short int *ptrBuffer;
        ptrBuffer = (short int*)m_buffer.data();
        for(int i=0; i<m_usedBufferSize/2; i++) ptrBuffer[i]*=m_volume;
        emit bufferReady(m_buffer, m_usedBufferSize);
        m_outputDevice->write(m_buffer.constData(), m_usedBufferSize);
        m_position = m_position+m_usedBufferSize;
    }
}

void SoundTrack::CalculateLevel(const QByteArray &levelBuffer, const qint64 &samples)
{
    const char *ptrBuffer = levelBuffer.data();
    int step = 2; // Solution for Sample size equal to 16 bits
    qreal maxValue=0.0;
    for(int i=0; i<samples; i=i+step)
    {
        qreal value = qreal((qAbs(short(ptrBuffer[i+1]))<< 8)|qAbs(short(ptrBuffer[i])));
        if(value>maxValue)
            maxValue = value;
    }
    m_level = (maxValue/peakValue);
    //NOTIFY<<m_level;
    emit sendLevel(m_level);
}

void SoundTrack::handleOutputNotify()
{
    m_timePosition = audioDuration(m_format, m_position);
    emit audioTimePositionChanged();
}

void SoundTrack::handleInputNotify()
{
    m_timePosition = m_audioInput->processedUSecs();
    emit audioTimePositionChanged();
}

void SoundTrack::fillRecordingFile(const qint64 &audioLength, const QAudioFormat &format)
{
    QByteArray fillFile(audioLength/format.channelCount(),0);
    m_rawFile->open(QIODevice::WriteOnly | QIODevice::Append);
    m_rawFile->seek(0);
    m_rawFile->write(fillFile);
    m_rawFile->close();
}

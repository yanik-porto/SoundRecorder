/**
 * The WavFile class is derived from QFile class, and is responsible
 * for retrieving all the information from the imported wave file.
 * It is also responsible for creating the reader for the files being
 * exported (to be implemented for the demo).
 */

#ifndef WAVFILE_H
#define WAVFILE_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = 0);

    using QFile::open;
    bool open(const QString &fileName);

    const QAudioFormat &fileFormat() const;
    qint64 headerLength() const;

    void writeWav(const QByteArray &output, const QAudioFormat &format);

private:
    bool readHeader();

private:
    QAudioFormat m_fileFormat;
    qint64 m_headerLength;
};

#endif // WAVFILE_H

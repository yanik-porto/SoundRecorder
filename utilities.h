/**
 * The Utilities file contains some commom use functions.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtCore/qglobal.h>
#include <QDebug>

QT_FORWARD_DECLARE_CLASS(QAudioFormat)

// Return the audio duration in us considering the format and the size
qint64 audioDuration(const QAudioFormat &format, qint64 bytes);

// Return the audio lenght (bytes) considering the format and the duration
qint64 audioLength(const QAudioFormat &format, qint64 microSeconds);

// Return a string with the format consideren the QAudioFormat
QString formatToString(const QAudioFormat &format);

// Check whether the audio format is PCM
bool isPCM(const QAudioFormat &format);

// Check whether the audio format is signed, little-endian, 16-bit PCM
bool isPCMS16LE(const QAudioFormat &format);

// Scale PCM value to [-1.0, 1.0]
qreal pcmToReal(qint16 pcm);

// Scale real value in [-1.0, 1.0] to PCM
qint16 realToPcm(qreal real);

/*
* Debug output using qWarnigng
*/

#define NOTIFY qWarning()

#endif // UTILITIES_H

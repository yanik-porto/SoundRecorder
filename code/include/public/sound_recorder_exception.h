#ifndef SOUND_RECORDER_EXCEPTION_H
#define SOUND_RECORDER_EXCEPTION_H

#include <exception>
#include <string>


class SoundRecorderException : public std::exception {
private:
    std::string sWhat;

public:
    SoundRecorderException(const std::string &message);

    virtual ~SoundRecorderException();
    virtual const char* what() const throw ();

};


#endif // SOUND_RECORDER_EXCEPTION_H

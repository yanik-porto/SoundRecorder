#include "include/public/sound_recorder_exception.h"

SoundRecorderException::SoundRecorderException(std::string const &message) :
    sWhat(message) {

}

SoundRecorderException::~SoundRecorderException() {
}

const char* SoundRecorderException::what() const throw () {
    return sWhat.c_str();
}

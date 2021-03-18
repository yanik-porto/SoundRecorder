#ifndef I_AUDIO_SPECTRE_H
#define I_AUDIO_SPECTRE_H

#include <QtGlobal>

class IAudioSpectre {
public:
    virtual ~IAudioSpectre() {};

    /**
     * @brief set audio spectre intensity
     * @param ind The index of the spectre intensity to set
     * @param intensity The new level for this intensity
     */
    virtual void SetIntensity(int ind, const qreal &intensity) = 0;

    /**
     * @brief reset audio spectre
     */
    virtual void Reset() = 0;

};

#endif // I_AUDIO_SPECTRE_H

#ifndef I_LEVEL_H
#define I_LEVEL_H

class ILevel {

public:
    virtual ~ILevel(){}

    /**
     * @brief Functions which receives the intensity level
     *          emitted by the SoundTrack class. The interface
     *          is made by the MainWindow.
     * @param[in] levelL level of the left channel
     * @param[in] levelR level of the right channel
     */
    virtual void SetLevel(double levelL, double levelR) = 0;
};

#endif // I_LEVEL_H

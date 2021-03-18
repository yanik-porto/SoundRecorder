#ifndef I_SETTINGS_DIALOG_H
#define I_SETTINGS_DIALOG_H

#include <QAudioDeviceInfo>

class ISettingsDialog {

public:
    virtual ~ISettingsDialog() {};

    /**
     * @return the selected input device
     */
    virtual const QAudioDeviceInfo &InputDevice() const = 0;

    /**
     * @return the selected output device
     */
    virtual const QAudioDeviceInfo &OutputDevice() const = 0;
};

#endif // I_SETTINGS_DIALOG_H

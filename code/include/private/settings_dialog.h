/**
 * The SettingsDialog class is responsible for launching the dialog
 * window in which the IO device is selected. It is triggered by the
 * MainWindow, more precisely by the settings button.
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "include/public/i_settings_dialog.h"

#include <QDialog>
#include <QAudioDeviceInfo>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QSlider;
class QSpinBox;
class QGridLayout;
QT_END_NAMESPACE

class SettingsDialog : public QDialog, public ISettingsDialog
{
    Q_OBJECT

public:

    /**
     * The QComboBoxes are populates with the available IO sound devices.
     */
    SettingsDialog(const QList<QAudioDeviceInfo> &availableInputDevices,
                   const QList<QAudioDeviceInfo> &availableOutputDevices,
                   QWidget *parent = 0);
    ~SettingsDialog();

    /**
     * @return the selected input device
     */
    const QAudioDeviceInfo &InputDevice() const override;

    /**
     * @return the selected outpout device
     */
    const QAudioDeviceInfo &OutputDevice() const override;

private slots:
    /**
     * If there is change in comparison with the previous state, the new
     * information regarding the IOs is then sent to the SoundEngine for
     * audio initialization and for the MainWindow for displaying audio
     * information.
     */
    void inputDeviceChanged(int index);
    void outputDeviceChanged(int index);

private:
    QAudioDeviceInfo m_inputDevice;
    QAudioDeviceInfo m_outputDevice;

    QComboBox *m_inputDeviceComboBox;
    QComboBox *m_outputDeviceComboBox;
};

#endif // SETTINGSDIALOG_H

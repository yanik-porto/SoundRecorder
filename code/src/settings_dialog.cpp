#include "include/private/settings_dialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

SettingsDialog::SettingsDialog(
            const QList<QAudioDeviceInfo> &availableInputDevices,
            const QList<QAudioDeviceInfo> &availableOutputDevices,
            QWidget *parent)
    :   QDialog(parent)
    ,   m_inputDeviceComboBox(new QComboBox(this))
    ,   m_outputDeviceComboBox(new QComboBox(this))
{
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);

    // Populate combo boxes
    QAudioDeviceInfo device;
    foreach (device, availableInputDevices)
        m_inputDeviceComboBox->addItem(device.deviceName(),
                                       QVariant::fromValue(device));
    foreach (device, availableOutputDevices)
        m_outputDeviceComboBox->addItem(device.deviceName(),
                                       QVariant::fromValue(device));

    // Initialize default devices
    if (!availableInputDevices.empty())
        m_inputDevice = availableInputDevices.front();
    if (!availableOutputDevices.empty())
        m_outputDevice = availableOutputDevices.front();

    // Add widgets to layout
    QScopedPointer<QHBoxLayout> inputDeviceLayout(new QHBoxLayout);
    QLabel *inputDeviceLabel = new QLabel(tr("Input device"), this);
    inputDeviceLayout->addWidget(inputDeviceLabel);
    inputDeviceLayout->addWidget(m_inputDeviceComboBox);
    dialogLayout->addLayout(inputDeviceLayout.data());
    inputDeviceLayout.take();

    QScopedPointer<QHBoxLayout> outputDeviceLayout(new QHBoxLayout);
    QLabel *outputDeviceLabel = new QLabel(tr("Output device"), this);
    outputDeviceLayout->addWidget(outputDeviceLabel);
    outputDeviceLayout->addWidget(m_outputDeviceComboBox);
    dialogLayout->addLayout(outputDeviceLayout.data());
    outputDeviceLayout.take();

    // Connect
    connect(m_inputDeviceComboBox, SIGNAL(activated(int)),
                    this, SLOT(inputDeviceChanged(int)));
    connect(m_outputDeviceComboBox, SIGNAL(activated(int)),
                    this, SLOT(outputDeviceChanged(int)));

    // Add standard buttons to layout
    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dialogLayout->addWidget(buttonBox);

    // Connect standard buttons
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
                    this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
                    this, SLOT(reject()));
    setLayout(dialogLayout);
}

SettingsDialog::~SettingsDialog()
{
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void SettingsDialog::inputDeviceChanged(int index)
{
    m_inputDevice = m_inputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
}

void SettingsDialog::outputDeviceChanged(int index)
{
    m_outputDevice = m_outputDeviceComboBox->itemData(index).value<QAudioDeviceInfo>();
}


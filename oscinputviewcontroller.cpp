#include "oscinputviewcontroller.h"

OscInputViewController::OscInputViewController(QObject *parent)
    : QObject{parent}
{

}

bool OscInputViewController::connectDevice() const
{
    return m_connectDevice;
}

void OscInputViewController::setConnectDevice(bool newConnectDevice)
{
    if (m_connectDevice == newConnectDevice)
        return;
    m_connectDevice = newConnectDevice;
    emit connectDeviceChanged(m_connectDevice);
}

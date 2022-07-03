#include "devicestatusmanager.h"

DeviceStatusManager::DeviceStatusManager(QObject *parent)
    : QObject{parent}
{
    m_port = 55600;
    socket = new QUdpSocket(this);
    bool bindSuccess = socket->bind(QHostAddress::Any, this->m_port);

    if(bindSuccess) {

        connect(socket, &QUdpSocket::readyRead, this, &DeviceStatusManager::readIncomingUdpData);
        qInfo() << "SUCESS binding on Port"<< this->m_port;
    }
    else {
        qWarning() << "NO BINDING possible on Port"<< this->m_port;
    }


}

void DeviceStatusManager::setIdForNamedDevice(QString deviceName, QString deviceId)
{
    if (!oscInputDevices.contains(deviceName.toUtf8())) {
        qWarning() << "Please implement oscInput-device creation triggered by backend!";
    }
    else {
        deviceForId.insert(deviceId, deviceName.toUtf8());
        oscInputDevices.value(deviceName.toUtf8())->setUniqueId(deviceId);
    }
}

void DeviceStatusManager::loadMotionDeviceFromDomElement(QDomElement element)
{
    QString _id = element.tagName();
    QString _name = element.attribute("deviceName", "no!!name!!");

    OscInputDevice *newMotionDevice = new OscInputDevice(_name, this);
    oscInputDevices.insert(_name.toUtf8(), newMotionDevice);

    setIdForNamedDevice(_name, _id);

    emit newOscInputDevice(_name, newMotionDevice, _id);

    /*
    QDomNode _attrNode = element.firstChildElement("oscinput-device");
    if(_attrNode.isElement()) {
        QDomElement _attrElement = _attrNode.toElement();
        if(_attrElement.hasAttribute("nSensors")) {

        }
    }*/
}

void DeviceStatusManager::readIncomingUdpData()
{
    QByteArray buffer(socket->pendingDatagramSize(), char());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(),buffer.size(), &sender, &senderPort);


    handleOscPacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));

}

void DeviceStatusManager::handleOscPacket(const OSCPP::Server::Packet &packet)
{
    if(packet.isMessage()) handleOscMessage(OSCPP::Server::Message(packet));
}

void DeviceStatusManager::handleOscMessage(const OSCPP::Server::Message &message)
{

    if(message == "/glove/ping") {
        OSCPP::Server::ArgStream oscArgs(message.args());
        if(oscArgs.tag() == 's') {
            QByteArray pingersName(oscArgs.string());
            if(oscInputDevices.contains(pingersName)) {
                oscInputDevices.value(pingersName)->handlePingMessage(oscArgs);
            }
            else {
                createNewMotionDevice(pingersName, oscArgs);
            }
        }
    }
}

void DeviceStatusManager::createNewMotionDevice(QByteArray name, OSCPP::Server::ArgStream oscArgs)
{
    OscInputDevice *newMotionDevice = new OscInputDevice(name, this);
    oscInputDevices.insert(name, newMotionDevice);

    newMotionDevice->handlePingMessage(oscArgs);

    emit newOscInputDevice(name, newMotionDevice);
}




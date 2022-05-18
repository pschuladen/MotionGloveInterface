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




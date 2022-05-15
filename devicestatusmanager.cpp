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

    qInfo() << "device status object" << objectName() << thread();
}


void DeviceStatusManager::setConnectStatus(bool connect, QString device)
{
    discoveredDevices[device.toUtf8()].connectStatus = connect;
    qInfo() << "Status for" << device << "connection is now" << discoveredDevices[device.toUtf8()].connectStatus;
}

void DeviceStatusManager::readIncomingUdpData()
{
    QByteArray buffer(socket->pendingDatagramSize(), char());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(),buffer.size(), &sender, &senderPort);

//    qInfo() <<"udp read" <<buffer;
    handleOscPacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));

    //    handlePacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));

}

void DeviceStatusManager::handleOscPacket(const OSCPP::Server::Packet &packet)
{
    if(packet.isMessage()) handleOscMessage(OSCPP::Server::Message(packet));
}

void DeviceStatusManager::handleOscMessage(const OSCPP::Server::Message &message)
{
//    qInfo() << "handling osc message";
    if(message == "/glove/ping") {
        OSCPP::Server::ArgStream oscArgs(message.args());
        if(oscArgs.tag() == 's') {
            QByteArray pingersName(oscArgs.string());
            if(oscInputDevices.contains(pingersName)) {

                oscInputDevices.value(pingersName)->handlePingMessage(oscArgs);

//                emit receivedNewDevice(&newDevice);
            }
            else {
                createNewMotionDevice(pingersName, oscArgs);
//                if(discoveredDevices[pingersName].connectStatus) {
//                    pingBackToDevice(pingersName);
//                }
                //send ping back if true in hash
            }
        }
    }
}

void DeviceStatusManager::createNewMotionDevice(QByteArray name, OSCPP::Server::ArgStream oscArgs)
{
    MotionDevice newDevice;
    OscInputDevice *newMotionDevice = new OscInputDevice(name, this);
    oscInputDevices.insert(name, newMotionDevice);

    newMotionDevice->handlePingMessage(oscArgs);

    emit newOscInputDevice(name, newMotionDevice);


//    bool deviceData_ok = true;

//    if(!oscArgs.atEnd() && oscArgs.tag() == 's') {
//        newDevice.m_deviceAddress = oscArgs.string();
//    } else deviceData_ok = false;

//    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
//        newDevice.port = oscArgs.int32();
//    } else deviceData_ok = false;

//    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
//        newDevice.sendIntervall = oscArgs.int32();
//    }

//    if(!deviceData_ok) {
//        qInfo() << "not enough info for device" << name;
//        return;
//    }
//    newDevice.deviceName = name;
//    newDevice.inputHandler = new OscInputDevice(name, 51002+discoveredDevices.size(), 2, &newDevice, this);

//    newDevice.connectStatus = false;
//    discoveredDevices.insert(name, newDevice);
//    qInfo() << "received ping from new device" << name;

//    emit receivedNewDevice(&newDevice);//pingersName, newDevice.inputHandler);
}

void DeviceStatusManager::pingBackToDevice(QByteArray deviceName)
{
    MotionDevice *device = &discoveredDevices[deviceName];
    qInfo() << "pingback";
    QByteArray pongBuf(100, char());
    OSCPP::Client::Packet pongPacket(pongBuf.data(), 40);
    pongPacket.openMessage("/pong", 1)
            .int32(51002)
            .closeMessage();
    size_t pongSize = pongPacket.size();
    qInfo() << "device addr" << device->m_deviceAddress << device->port;
    qint64 sendedBytes = socket->writeDatagram(pongBuf.data(), pongSize, QHostAddress(device->m_deviceAddress), device->port);
}



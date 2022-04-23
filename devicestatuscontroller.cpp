#include "devicestatuscontroller.h"

DeviceStatusController::DeviceStatusController(QObject *parent)
    : QObject{parent}
{
    m_port = 55600;
    socket = new QUdpSocket(this);
    bool bindSuccess = socket->bind(QHostAddress::Any, this->m_port);

    if(bindSuccess) {

        connect(socket, &QUdpSocket::readyRead, this, &DeviceStatusController::readIncomingUdpData);
        qDebug() << "SUCESS binding on Port"<< this->m_port;
    }
    else {
        qDebug() << "NO BINDING possible on Port"<< this->m_port;
    }
    connect(this, &DeviceStatusController::receivedNewDevice, main_backend::Instance(), &MainBackend::createNewInputViews);

}


void DeviceStatusController::setConnectStatus(bool connect, QString device)
{
    discoveredDevices[device].connectStatus = connect;
    qInfo() << "Status for" << device << "connection is now" << discoveredDevices[device].connectStatus;
}

void DeviceStatusController::readIncomingUdpData()
{
    QByteArray buffer(socket->pendingDatagramSize(), char());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(),buffer.size(), &sender, &senderPort);

    qInfo() <<"udp read" <<buffer;
    handleOscPacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));

    //    handlePacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));

}

void DeviceStatusController::handleOscPacket(const OSCPP::Server::Packet &packet)
{
    if(packet.isMessage()) handleOscMessage(OSCPP::Server::Message(packet));
}

void DeviceStatusController::handleOscMessage(const OSCPP::Server::Message &message)
{
    qInfo() << "handling osc message";
    if(message == "/glove/ping") {
        OSCPP::Server::ArgStream oscArgs(message.args());
        if(oscArgs.tag() == 's') {
            QString pingersName(oscArgs.string());
            if(!discoveredDevices.keys().contains(pingersName)) {
                MotionDevice newDevice;
                bool deviceData_ok = true;

                if(!oscArgs.atEnd() && oscArgs.tag() == 's') {
                    newDevice.address = oscArgs.string();
                } else deviceData_ok = false;

                if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
                    newDevice.port = oscArgs.int32();
                } else deviceData_ok = false;

                if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
                    newDevice.sendIntervall = oscArgs.int32();
                }

                if(!deviceData_ok) {
                    qInfo() << "not enough info for device" << pingersName;
                    return;
                }
                newDevice.deviceName = pingersName;
                newDevice.inputHandler = new DeviceDataInput(this, pingersName, 51002+discoveredDevices.size(), 2, &newDevice);

                newDevice.connectStatus = false;
                discoveredDevices.insert(pingersName, newDevice);
                qInfo() << "received ping from new device" << pingersName;


                emit receivedNewDevice(&newDevice);//pingersName, newDevice.inputHandler);
            }
            else {
                if(discoveredDevices[pingersName].connectStatus) {

                    pingBackToDevice(pingersName);
                }
                //send ping back if true in hash
            }
        }
    }
}

void DeviceStatusController::pingBackToDevice(QString deviceName)
{
    MotionDevice *device = &discoveredDevices[deviceName];
    qInfo() << "pingback";
    QByteArray pongBuf(100, char());
    OSCPP::Client::Packet pongPacket(pongBuf.data(), 40);
    pongPacket.openMessage("/pong", 1)
            .int32(51002)
            .closeMessage();
    size_t pongSize = pongPacket.size();
    qInfo() << "device addr" << device->address << device->port;
    qint64 sendedBytes = socket->writeDatagram(pongBuf.data(), pongSize, QHostAddress(device->address), device->port);
}



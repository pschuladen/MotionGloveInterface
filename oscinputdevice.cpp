#include "oscinputdevice.h"

OscInputDevice::OscInputDevice(QObject *parent)
    : QObject{parent}
{

}

OscInputDevice::OscInputDevice(QString deviceName, QObject *parent)
    : QObject{parent}, m_deviceName{deviceName}
{
    m_nSensors = 2;

    setupOscInputBindings();

    quint16 _baseport = 51001;
    m_socket = new QUdpSocket(this);

    quint16 _maxtry = 30;
    quint16 _trials = 0;
    bool bindSuccess = false;
    while(!bindSuccess && _trials < _maxtry) {
            bindSuccess = m_socket->bind(QHostAddress::Any, _baseport+_trials);
            _trials++;
    }
    if(bindSuccess) {
        connect(m_socket, &QUdpSocket::readyRead, this, &OscInputDevice::readIncomingUdpData);
        qDebug() << "SUCESS binding on Port"<< m_rcvPort;
    }
    else {
        qDebug() << "NO BINDING possible on Port"<< m_rcvPort;
    }
}


const QString &OscInputDevice::deviceName() const
{
    return m_deviceName;
}

ValueNotifierClass *OscInputDevice::getNotifier(SensType sensType, int sensIndex, int subIndex)
{
    qWarning() << "WARNING! get notifier for sensType and sensIndex not implemented!!";
return nullptr;
}

void OscInputDevice::readIncomingUdpData()
{
    QByteArray buffer(m_socket->pendingDatagramSize(), char());
    QHostAddress sender;
    quint16 senderPort;

    m_socket->readDatagram(buffer.data(),buffer.size(), &sender, &senderPort);

//    qInfo() <<"udp read" <<buffer;
    handleOscPacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));
}

void OscInputDevice::handleOscPacket(const OSCPP::Server::Packet &packet)
{
    if(packet.isMessage()) {
        handleOscMessage(OSCPP::Server::Message(packet));
    }

    else if(packet.isBundle()) {
        OSCPP::Server::Bundle bundle(packet);
        OSCPP::Server::PacketStream packets(bundle.packets());

        while(!packets.atEnd()) {
            handleOscMessage(packets.next());
        }
    }
}

void OscInputDevice::handleOscMessage(const OSCPP::Server::Message &message)
{

    oscInputParser.value(message.address())->handleOscArgs(OSCPP::Server::ArgStream(message.args()), 0);
}

void OscInputDevice::setupOscInputBindings()
{

    static const QList<SensType> sortOrder={SensType::RotQuat, SensType::Accel, SensType::Grav, SensType::Gyro};

    for(const TypeHelper::SensorType &_type: sortOrder) {
        for (int i = 0; i< nSensors(); i++) {
            oscInputParser.insert(

                        QString("/%1/%2/%3").arg(deviceName(), TypeHelper::getProtoStrForSensor(_type)).arg(i).toUtf8(),
                        new OscInputParser(_type, this));
        }
    }
    oscInputParser.insert(
                QString("/%1/%2").arg(deviceName(), TypeHelper::getProtoStrForSensor(SensType::Touch)).toUtf8(),
                new OscInputParser(SensType::Touch, this));

    qInfo() << "created oscInputParser size" << oscInputParser.size();

    QString oscAddress;

    for(int i = 0; i < m_nSensors; i++) {

        oscAddress = QString("/%1/acc/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Accel, this));

        oscAddress = QString("/%1/gravity/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Grav, this));

        oscAddress = QString("/%1/gyr/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Gyro, this));

        oscAddress = QString("/%1/quat/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::RotQuat, this));
    }

    oscAddress = QString("/%1/touch").arg(deviceName());
    oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Touch, this));

}


bool OscInputDevice::readyToPong()
{
    return m_socket->isValid() && !m_deviceIpAddress.isNull() && (1024 < devicePort() && devicePort() < 65534);
}

void OscInputDevice::sendPongMessage()
{
    QByteArray pongBuf(100, char());
    OSCPP::Client::Packet pongPacket(pongBuf.data(), 40);
    pongPacket.openMessage("/pong", 1)
            .int32(rcvPort())
            .closeMessage();
    size_t pongSize = pongPacket.size();
    qint64 sendedBytes = m_socket->writeDatagram(pongBuf.data(), pongSize, m_deviceIpAddress, devicePort());
    qInfo() << "pong datasize" << sendedBytes;
    qInfo() << "sended to device addr" << deviceAddressStr() << devicePort();
}


void OscInputDevice::setDeviceName(const QString &newDeviceName)
{
    if (m_deviceName == newDeviceName)
        return;
    m_deviceName = newDeviceName;
    emit deviceNameChanged();
}

quint8 OscInputDevice::nSensors() const
{
    return m_nSensors;
}

void OscInputDevice::setNSensors(quint8 newNSensors)
{
    if (m_nSensors == newNSensors)
        return;
    m_nSensors = newNSensors;
    emit nSensorsChanged();
}

quint16 OscInputDevice::rcvPort() const
{
    return m_rcvPort;
}

void OscInputDevice::setRcvPort(quint16 newPort)
{
    if (m_rcvPort == newPort)
        return;
    m_rcvPort = newPort;
    emit rcvPortChanged();
}

bool OscInputDevice::sendPong() const
{
    return m_sendPong;
}

void OscInputDevice::setSendPong(bool newSendPong)
{
    if (m_sendPong == newSendPong)
        return;
    m_sendPong = newSendPong;
    emit sendPongChanged();
}

const QString &OscInputDevice::deviceAddressStr() const
{
    return m_deviceAddressStr;
}

void OscInputDevice::setDeviceAddressStr(const QString &newIpAddress)
{
    if (m_deviceAddressStr == newIpAddress)
        return;
    m_deviceIpAddress.setAddress(newIpAddress);
    m_deviceAddressStr = newIpAddress;
    emit deviceAddressStrChanged();
}

quint16 OscInputDevice::sendIntervall() const
{
    return m_sendIntervall;
}

void OscInputDevice::setSendIntervall(quint16 newSendIntervall)
{
    if (m_sendIntervall == newSendIntervall)
        return;
    m_sendIntervall = newSendIntervall;
    emit sendIntervallChanged();
}

bool OscInputDevice::deviceReady() const
{
    return m_deviceReady;
}

void OscInputDevice::setDeviceReady(bool newDeviceReady)
{
    if (m_deviceReady == newDeviceReady)
        return;
    m_deviceReady = newDeviceReady;
    emit deviceReadyChanged();
}

void OscInputDevice::handlePingMessage(OSCPP::Server::ArgStream oscArgs)
{
    bool deviceData_ok = true;
    if(!oscArgs.atEnd() && oscArgs.tag() == 's') {
        setDeviceAddressStr(oscArgs.string());
    } else deviceData_ok = false;

    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
        setDevicePort(oscArgs.int32());
    } else deviceData_ok = false;

    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
        setSendIntervall(oscArgs.int32());
    }

    if(sendPong() && readyToPong() && deviceData_ok) {
        sendPongMessage();
    }
}

quint16 OscInputDevice::devicePort() const
{
    return m_devicePort;
}

void OscInputDevice::setDevicePort(quint16 newDevicePort)
{
    if (m_devicePort == newDevicePort)
        return;
    m_devicePort = newDevicePort;
    emit devicePortChanged();
}

void OscInputDevice::viewWasCreated()
{
    // gather all keys sorted...
    // TODO: qmap is already sorted but some kind of sortingfunction would be nice
    QList<OscSensorInputStruct> oscStructs;
    qInfo() << "oscStruct size" << oscInputParser.size();
    oscStructs.reserve(oscInputParser.size());
    int ci = 0;
    QMap<QByteArray,OscInputParser*>::const_iterator oscPa;
    for(oscPa = oscInputParser.cbegin(); oscPa != oscInputParser.cend(); ++oscPa) {
        oscStructs.append(OscSensorInputStruct(QString(oscPa.key()), oscPa.value()->sensorType(), oscPa.value()));
        ci++;
    }

    emit sendSensorStructList(deviceName(), oscStructs);
}

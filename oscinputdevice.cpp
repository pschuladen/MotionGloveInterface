#include "oscinputdevice.h"

OscInputDevice::OscInputDevice(QObject *parent)
    : QObject{parent}
{

}

OscInputDevice::OscInputDevice(QString deviceName, QObject *parent)
    : QObject{parent}, m_deviceName{deviceName}
{
    m_nSensors = 2;

//    createNotifier();
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

//OscInputDevice::OscInputDevice(QString identifier, uint16_t port, uint8_t nSensors, MotionDevice* deviceDescription, QObject *parent)
//    : QObject{parent},
//      m_deviceName{identifier}, m_rcvPort{port},
//      m_nSensors{nSensors}, m_deviceDescription{deviceDescription}
//{
////    setupOscInputBindings();
////    deviceDescription->inputs = &oscHandleHash;
//////    createNotifier();

////    m_socket = new QUdpSocket(this);

////    //TODO: bind on other port if no success
////    //TODO: tell devicestatuscontroller the port
////    bool bindSuccess = m_socket->bind(QHostAddress::Any, m_rcvPort);

////    if(bindSuccess) {
////        connect(m_socket, &QUdpSocket::readyRead, this, &OscInputDevice::readIncomingUdpData);
////        qDebug() << "SUCESS binding on Port"<< m_rcvPort;
////    }
////    else {
////        qDebug() << "NO BINDING possible on Port"<< m_rcvPort;
////    }

//}

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
//    qInfo() << "handling osc message" << message.address();

//    const OscInputStruct &oscInput = oscHandleHash.value(message.address(), unmappedOsc);
    oscInputParser.value(message.address())->handleOscArgs(OSCPP::Server::ArgStream(message.args()), 0);
//    oscInput.handleFunction(this, oscInput.sensorIndex, OSCPP::Server::ArgStream(message.args()));
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

//    acceleration.resize(m_nSensors);
//    gyroscope.resize(m_nSensors);
//    gravityVector.resize(m_nSensors);
//    quaternion.resize(m_nSensors);

    QString oscAddress;
//    InputValueViewController::ValueViewMode type;
    for(int i = 0; i < m_nSensors; i++) {

        oscAddress = QString("/%1/acc/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Accel, this));
//        oscHandleHash.insert(oscAddress.toUtf8(), OscInputStruct(&OscInputDevice::oscR_setAcceleration, i, TypeHelper::Accel));

        oscAddress = QString("/%1/gravity/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Grav, this));
//        oscHandleHash.insert(oscAddress.toUtf8(), OscInputStruct(&OscInputDevice::oscR_setGravityVector, i, TypeHelper::Grav));

        oscAddress = QString("/%1/gyr/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Gyro, this));
//        oscHandleHash.insert(oscAddress.toUtf8(), OscInputStruct(&OscInputDevice::oscR_setGyroscope, i, TypeHelper::Gyro));

        oscAddress = QString("/%1/quat/%2").arg(deviceName()).arg(i);
        oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::RotQuat, this));
//        oscHandleHash.insert(oscAddress.toUtf8(), OscInputStruct(&OscInputDevice::oscR_setQuaternion, i, TypeHelper::RotQuat));
    }

    oscAddress = QString("/%1/touch").arg(deviceName());
    oscInputParser.insert(oscAddress.toUtf8(), new OscInputParser(TypeHelper::Touch, this));
//    oscHandleHash.insert(oscAddress.toUtf8(),  OscInputStruct(&OscInputDevice::oscR_setQuaternion, 0, TypeHelper::Touch));

}

//void OscInputDevice::createNotifier()
//{
////    for(SensType t: qAsConst(allSensTypes)) {
////        QList<ValueNotifierClass*> notifyList;
////        notifyList.resize(m_nSensors);
////        valueNotifier.insert(t, notifyList);
////    }
////    for(QHash<QByteArray, OscInputStruct>::const_iterator oscInpIterator = oscHandleHash.cbegin(), end = oscHandleHash.cend(); oscInpIterator != end; ++oscInpIterator) {
////        valueNotifier[oscInpIterator.value().sensorType][oscInpIterator.value().sensorIndex] = new ValueNotifierClass(oscInpIterator.value().sensorType);
////    }
////    for(QByteArray osc : oscHandleHash.keys()) {
////        OscInputStruct oscStruct = oscHandleHash.value(osc);
////        valueNotifier[oscStruct.sensorType][oscStruct.sensorIndex] = new ValueNotifierClass(oscStruct.sensorType, this);
//    //    }
//}

//void OscInputDevice::sendPongToDevice()
//{

//}

//void OscInputDevice::oscR_setAcceleration(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args)
//{
//    who->set3dVector(&who->acceleration[sens_index], &args);
//    who->valueNotifier.value(SensType::Accel).at(sens_index)->callVectorChanged(who->acceleration[sens_index]);
//}

//void OscInputDevice::oscR_setGyroscope(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args)
//{
//    who->set3dVector(&who->gyroscope[sens_index], &args);
//    who->valueNotifier.value(SensType::Gyro).at(sens_index)->callVectorChanged(who->gyroscope[sens_index]);
//}

//void OscInputDevice::oscR_setGravityVector(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args)
//{
//    who->set3dVector(&who->gravityVector[sens_index], &args);
//    who->gravityVector[sens_index].normalize();
//    who->valueNotifier.value(SensType::Grav).at(sens_index)->callVectorChanged(who->gravityVector[sens_index]);
//}

//void OscInputDevice::oscR_setQuaternion(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args)
//{
//    who->setQuat(&who->quaternion[sens_index], &args);
//    who->valueNotifier.value(SensType::RotQuat).at(sens_index)->callQuatChanged(who->quaternion[sens_index]);
//}

//void OscInputDevice::oscR_setTouch(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args)
//{
//    //TODO: implement touch

//}

//void OscInputDevice::oscR_unMapped(OscInputDevice *who, int sens_index, OSCPP::Server::ArgStream args)
//{
////    qInfo() << "osc unmapped";
//    //dummy function as default value for oscFunctionHashMap... does nothing, but prevents crash for unknow osc
//    //could be used for automatically create inputs
//}

//void OscInputDevice::set3dVector(QVector3D *vector, OSCPP::Server::ArgStream *args)
//{
//    uint8_t fc = 0;
//    while(!args->atEnd() && fc < 3) {
//        if(args->tag() == 'f') {
//            //check for NaN?
//            switch (fc) {
//            case 0:
//                vector->setX(args->float32()); break;
//            case 1:
//                vector->setY(args->float32()); break;
//            case 2:
//                vector->setZ(args->float32()); break;
//            }
//            fc++;
//        }
//    }
//}

//void OscInputDevice::setQuat(QQuaternion *quat, OSCPP::Server::ArgStream *args)
//{
//    uint8_t fc = 0;
//    while(!args->atEnd() && fc < 4) {
//        if(args->tag() == 'f') {
//            //check for NaN?
//            switch(fc) {
//            case 0:
//                quat->setX(args->float32()); break;
//            case 1:
//                quat->setY(args->float32()); break;
//            case 2:
//                quat->setZ(args->float32()); break;
//            case 3:
//                quat->setScalar(args->float32()); break;
//            }
//            fc++;
//        }
//    }
//}

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

//QString OscInputDevice::getProtoStrForSensor(TypeHelper::SensorType sensType)
//{
//    qWarning() << "touch is wrong.";
//    switch(sensType) {
//    case TypeHelper::Custom:
//        return QString("/%1/sensor/%2");
//    case TypeHelper::Accel:
//        return QString("/%1/acc/%2");
//    case TypeHelper::Gyro:
//        return QString("/%1/gyr/%2");
//    case TypeHelper::Grav:
//        return QString("/%1/gravity/%2");
//    case TypeHelper::RotQuat:
//        return QString("/%1/qiat/%2");
//    case TypeHelper::Touch:
//        return QString("/%1/touch/%2");

//    }
//}



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
//        setDeviceName(oscArgs.string());
        setDeviceAddressStr(oscArgs.string());
//        newDevice.address = oscArgs.string();
    } else deviceData_ok = false;

    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
//        setRcvPort()
        setDevicePort(oscArgs.int32());
//        newDevice.port = oscArgs.int32();
    } else deviceData_ok = false;

    if(!oscArgs.atEnd() && oscArgs.tag() == 'i') {
        setSendIntervall(oscArgs.int32());
//        newDevice.sendIntervall = oscArgs.int32();
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
        qInfo() << oscPa.key();
        oscStructs.append(OscSensorInputStruct(QString(oscPa.key()), oscPa.value()->sensorType(), oscPa.value()));
        qInfo() << oscPa.key() << "added" << oscPa.value()->sensorType() << oscPa.value();
        qInfo() << ci << oscStructs.at(ci).identifier << oscStructs.at(ci).sensType << oscStructs.at(ci).oscHandler;
        ci++;
    }
    qInfo() << "sended oscsctruct size" << oscStructs.size()<< "counter" << ci;
    for(int i = 0; i < oscStructs.size(); i++)
    {
        qInfo() << i << oscStructs.at(i).identifier << oscStructs.at(i).sensType << oscStructs.at(i).oscHandler;
    }
    emit sendSensorStructList(deviceName(), oscStructs);
}

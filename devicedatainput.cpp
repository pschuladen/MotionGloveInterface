#include "devicedatainput.h"

DeviceDataInput::DeviceDataInput(QObject *parent)
    : QObject{parent}
{

}

DeviceDataInput::DeviceDataInput(QString identifier, uint16_t port, uint8_t nSensors, MotionDevice* deviceDescription, QObject *parent)
    : QObject{parent},
      m_identifier{identifier}, m_port{port},
      m_nSensors{nSensors}, m_deviceDescription{deviceDescription}
{
    setupOscInputBindings();
    deviceDescription->inputs = &oscHandleHash;
    createNotifier();

    socket = new QUdpSocket(this);

    //TODO: bind on other port if no success
    //TODO: tell devicestatuscontroller the port
    bool bindSuccess = socket->bind(QHostAddress::Any, m_port);

    if(bindSuccess) {
        connect(socket, &QUdpSocket::readyRead, this, &DeviceDataInput::readIncomingUdpData);
        qDebug() << "SUCESS binding on Port"<< m_port;
    }
    else {
        qDebug() << "NO BINDING possible on Port"<< m_port;
    }

}

ValueNotifierClass *DeviceDataInput::getNotifier(SensType sensType, int sensIndex, int subIndex)
{
    if(subIndex == -1) {
        return valueNotifier.value(sensType).at(sensIndex);
    }
    else {
        return valueNotifier.value(sensType).at(sensIndex)->subNotifier.at(subIndex);
    }
}

void DeviceDataInput::readIncomingUdpData()
{
    QByteArray buffer(socket->pendingDatagramSize(), char());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(),buffer.size(), &sender, &senderPort);

//    qInfo() <<"udp read" <<buffer;
    handleOscPacket(OSCPP::Server::Packet(buffer.data(), buffer.size()));
}

void DeviceDataInput::handleOscPacket(const OSCPP::Server::Packet &packet)
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

void DeviceDataInput::handleOscMessage(const OSCPP::Server::Message &message)
{
//    qInfo() << "handling osc message" << message.address();

    const OscInputStruct &oscInput = oscHandleHash.value(message.address(), unmappedOsc);
    oscInput.handleFunction(this, oscInput.sensorIndex, OSCPP::Server::ArgStream(message.args()));
}

void DeviceDataInput::setupOscInputBindings()
{
    acceleration.resize(m_nSensors);
    gyroscope.resize(m_nSensors);
    gravityVector.resize(m_nSensors);
    quaternion.resize(m_nSensors);

    QString oscAddress;
    OscInputStruct oscHandle;
    InputValueViewController::ValueViewMode type;
    for(int i = 0; i < m_nSensors; i++) {
        oscHandle.sensorIndex = i;

        oscAddress = QString("/%1/acc/%2").arg(this->m_identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setAcceleration;
        oscHandle.sensorType = TypeHelper::Accel;
        oscHandleHash.insert(oscAddress, oscHandle);

        oscAddress = QString("/%1/gravity/%2").arg(this->m_identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGravityVector;
        oscHandle.sensorType = TypeHelper::Grav;
        oscHandleHash.insert(oscAddress, oscHandle);

        oscAddress = QString("/%1/gyr/%2").arg(this->m_identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGyroscope;
        oscHandle.sensorType = TypeHelper::Gyro;
        oscHandleHash.insert(oscAddress, oscHandle);

        oscAddress = QString("/%1/quat/%2").arg(this->m_identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setQuaternion;
        oscHandle.sensorType = TypeHelper::RotQuat;
        oscHandleHash.insert(oscAddress, oscHandle);
    }

    oscAddress = QString("/%1/touch").arg(this->m_identifier);
    oscHandle.handleFunction = &DeviceDataInput::oscR_setTouch;
    oscHandle.sensorType = TypeHelper::Touch;
    oscHandleHash.insert(oscAddress, oscHandle);

//    qInfo() << "function hash map" << oscHandleHash.keys();
}

void DeviceDataInput::createNotifier()
{
    foreach(const SensType &t, allSensTypes) {
        QList<ValueNotifierClass*> notifyList;
        notifyList.resize(m_nSensors);
        valueNotifier.insert(t, notifyList);
    }
    foreach(const QString &osc, oscHandleHash.keys()) {
        OscInputStruct oscStruct = oscHandleHash.value(osc);
        valueNotifier[oscStruct.sensorType][oscStruct.sensorIndex] = new ValueNotifierClass(oscStruct.sensorType, this);
    }
//    qInfo() << "valuenotifier" << valueNotifier.keys() << valueNotifier.value(allSensTypes[1]);
}

void DeviceDataInput::oscR_setAcceleration(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    who->set3dVector(&who->acceleration[sens_index], &args);
    who->valueNotifier.value(SensType::Accel).at(sens_index)->callVectorChanged(who->acceleration[sens_index]);
}

void DeviceDataInput::oscR_setGyroscope(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    who->set3dVector(&who->gyroscope[sens_index], &args);
    who->valueNotifier.value(SensType::Gyro).at(sens_index)->callVectorChanged(who->gyroscope[sens_index]);
}

void DeviceDataInput::oscR_setGravityVector(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    who->set3dVector(&who->gravityVector[sens_index], &args);
    who->gravityVector[sens_index].normalize();
    who->valueNotifier.value(SensType::Grav).at(sens_index)->callVectorChanged(who->gravityVector[sens_index]);
}

void DeviceDataInput::oscR_setQuaternion(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    who->setQuat(&who->quaternion[sens_index], &args);
    who->valueNotifier.value(SensType::RotQuat).at(sens_index)->callQuatChanged(who->quaternion[sens_index]);
}

void DeviceDataInput::oscR_setTouch(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    //TODO: implement touch

}

void DeviceDataInput::oscR_unMapped(DeviceDataInput *who, int sens_index, OSCPP::Server::ArgStream args)
{
//    qInfo() << "osc unmapped";
    //dummy function as default value for oscFunctionHashMap... does nothing, but prevents crash for unknow osc
    //could be used for automatically create inputs
}

void DeviceDataInput::set3dVector(QVector3D *vector, OSCPP::Server::ArgStream *args)
{
    uint8_t fc = 0;
    while(!args->atEnd() && fc < 3) {
        if(args->tag() == 'f') {
            //check for NaN?
            switch (fc) {
            case 0:
                vector->setX(args->float32()); break;
            case 1:
                vector->setY(args->float32()); break;
            case 2:
                vector->setZ(args->float32()); break;
            }
            fc++;
        }
    }
}

void DeviceDataInput::setQuat(QQuaternion *quat, OSCPP::Server::ArgStream *args)
{
    uint8_t fc = 0;
    while(!args->atEnd() && fc < 4) {
        if(args->tag() == 'f') {
            //check for NaN?
            switch(fc) {
            case 0:
                quat->setX(args->float32()); break;
            case 1:
                quat->setY(args->float32()); break;
            case 2:
                quat->setZ(args->float32()); break;
            case 3:
                quat->setScalar(args->float32()); break;
            }
            fc++;
        }
    }
}







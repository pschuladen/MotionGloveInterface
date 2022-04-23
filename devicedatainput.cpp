#include "devicedatainput.h"


//#include <QFile>
//#include <QThread>
DeviceDataInput::DeviceDataInput(QObject *parent, QString identifier, uint16_t port, uint8_t nSensors, MotionDevice* deviceDescription)
    : QObject{parent}
{
    identifier = identifier;
    m_port = port;
    m_nSensors = nSensors;
    deviceDescription = deviceDescription;

    setupOscInputBindings();

    socket = new QUdpSocket(this);

    bool bindSuccess = socket->bind(QHostAddress::Any, m_port);

    if(bindSuccess) {
        connect(socket, &QUdpSocket::readyRead, this, &DeviceDataInput::readIncomingUdpData);
        qDebug() << "SUCESS binding on Port"<< m_port;
    }
    else {
        qDebug() << "NO BINDING possible on Port"<< m_port;
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
    //    auto hashedFunction = this->oscFunctionHash.value(message.address(), &DeviceDataInput::oscR_unMapped);
    //    int sensorIndex = this->oscSensorIndexHash.value(message.address(), 0);
    const OscInputStruct &oscInput = oscHandleHash.value(message.address(), unmappedOsc);

    //    hashedFunction(this, sensorIndex, OSCPP::Server::ArgStream(message.args()));
    oscInput.handleFunction(this, oscInput.sensorIndex, OSCPP::Server::ArgStream(message.args()));
}

void DeviceDataInput::setupOscInputBindings()
{
    acceleration.resize(m_nSensors);
    gyroscope.resize(m_nSensors);
    gravityVector.resize(m_nSensors);
    quaternion.resize(m_nSensors);

    accelNotify.resize(m_nSensors);
    gyrosNotify.resize(m_nSensors);
    gravityNotify.resize(m_nSensors);
    quaternionNotify.resize(m_nSensors);

    QString oscAddress;
    OscInputStruct oscHandle;
    for(int i = 0; i < m_nSensors; i++) {
        oscHandle.sensorIndex = i;

        oscAddress = QString("/%1/acc/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setAcceleration;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        accelNotify[i] = new ValueNotifierClass(this);
        oscHandle.sensorType = ValueViewBackend::Accel;
//        deviceDescription->inputs.insert(oscAddress, ValueViewBackend::Accel);
        deviceDescription->inputs.insert(oscAddress, MotionDevice::InputDef(ValueViewBackend::Accel, i));


        oscAddress = QString("/%1/gravity/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGravityVector;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        gravityNotify[i] = new ValueNotifierClass(this);
        oscHandle.sensorType = ValueViewBackend::Grav;
        deviceDescription->inputs.insert(oscAddress, MotionDevice::InputDef(ValueViewBackend::Grav, i));


        oscAddress = QString("/%1/gyr/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGyroscope;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        gyrosNotify[i] = new ValueNotifierClass(this);
        oscHandle.sensorType = ValueViewBackend::Gyro;
        deviceDescription->inputs.insert(oscAddress, MotionDevice::InputDef(ValueViewBackend::Gyro, i));


        oscAddress = QString("/%1/quat/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setQuaternion;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        quaternionNotify[i] = new ValueNotifierClass(this);
        oscHandle.sensorType = ValueViewBackend::Quat;
        deviceDescription->inputs.insert(oscAddress, MotionDevice::InputDef(ValueViewBackend::Quat, i));
    }

    oscAddress = QString("/%1/touch").arg(this->identifier);
    oscHandle.handleFunction = &DeviceDataInput::oscR_setTouch;
    oscHandleHash.insert(oscAddress, oscHandle);
    touchNotifier = new ValueNotifierClass(this);
    oscHandle.sensorType = ValueViewBackend::Touch;
    deviceDescription->inputs.insert(oscAddress, MotionDevice::InputDef(ValueViewBackend::Touch, 0));

    qInfo() << "function hash map" << oscHandleHash.keys();
}

void DeviceDataInput::oscR_setAcceleration(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
//    QVector3D *accVector = &who->acceleration[sens_index];
//    who->set3dVector(accVector, &args);
    who->set3dVector(&who->acceleration[sens_index], &args);

    emit who->accelNotify.at(sens_index)->vectorChanged(who->acceleration[sens_index]);
}

void DeviceDataInput::oscR_setGyroscope(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    QVector3D *gyrVector = &who->gyroscope[sens_index];
    who->set3dVector(gyrVector, &args);
//    qInfo() << who->identifier << "gyroscope" << sens_index << gyrVector->x() << gyrVector->y() << gyrVector->z();

    emit who->gyrosNotify.at(sens_index)->vectorChanged(*gyrVector);
}

void DeviceDataInput::oscR_setGravityVector(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    QVector3D *gravVector = &who->gravityVector[sens_index];
    who->set3dVector(gravVector, &args);

    gravVector->normalize();

    emit who->gravityNotify.at(sens_index)->vectorChanged(*gravVector);

}

void DeviceDataInput::oscR_setQuaternion(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    QQuaternion *quat = &who->quaternion[sens_index];
    who->setQuat(quat, &args);
    emit who->quaternionNotify.at(sens_index)->quatChanged(*quat);
}

void DeviceDataInput::oscR_setTouch(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{

}

void DeviceDataInput::oscR_unMapped(DeviceDataInput *who, int sens_index, OSCPP::Server::ArgStream args)
{
//    qInfo() << "osc unmapped";
    //dummy function as default value for oscFunctionHashMap... does nothing, but prevents crash for unknow osc
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







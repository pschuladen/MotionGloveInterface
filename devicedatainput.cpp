#include "devicedatainput.h"


//#include <QFile>
//#include <QThread>
DeviceDataInput::DeviceDataInput(QObject *parent, QString identifier, uint16_t port, uint8_t nSensors)
    : QObject{parent}
{
    this->identifier = identifier;
    this->m_port = port;
    this->m_nSensors = nSensors;

    this->setupOscInputBindings();

    socket = new QUdpSocket(this);

    bool bindSuccess = socket->bind(QHostAddress::Any, this->m_port);

    if(bindSuccess) {
        connect(socket, &QUdpSocket::readyRead, this, &DeviceDataInput::readIncomingUdpData);
        qDebug() << "SUCESS binding on Port"<< this->m_port;
    }
    else {
        qDebug() << "NO BINDING possible on Port"<< this->m_port;
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
    const OscInputStruct &oscInput = this->oscHandleHash.value(message.address(), this->unmappedOsc);

    //    hashedFunction(this, sensorIndex, OSCPP::Server::ArgStream(message.args()));
    oscInput.handleFunction(this, oscInput.sensorIndex, OSCPP::Server::ArgStream(message.args()));
}

void DeviceDataInput::setupOscInputBindings()
{
    this->acceleration.resize(this->m_nSensors);
    this->gyroscope.resize(this->m_nSensors);
    this->gravityVector.resize(this->m_nSensors);
    this->quaternion.resize(this->m_nSensors);

    this->accelNotify.resize(this->m_nSensors);
    this->gyrosNotify.resize(this->m_nSensors);
    this->gravityNotify.resize(this->m_nSensors);
    this->quaternionNotify.resize(this->m_nSensors);

    QString oscAddress;
    OscInputStruct oscHandle;
    for(int i = 0; i < m_nSensors; i++) {
        oscHandle.sensorIndex = i;

        oscAddress = QString("/%1/acc/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setAcceleration;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        accelNotify[i] = new ValueNotifierClass(this);


        oscAddress = QString("/%1/gravity/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGravityVector;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        gravityNotify[i] = new ValueNotifierClass(this);


        oscAddress = QString("/%1/gyr/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setGyroscope;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        gyrosNotify[i] = new ValueNotifierClass(this);


        oscAddress = QString("/%1/quat/%2").arg(this->identifier).arg(i);
        oscHandle.handleFunction = &DeviceDataInput::oscR_setQuaternion;
        this->oscHandleHash.insert(oscAddress, oscHandle);
        quaternionNotify[i] = new ValueNotifierClass(this);
    }
    oscAddress = QString("/%1/touch").arg(this->identifier);
    this->oscFunctionHash.insert(oscAddress, &DeviceDataInput::oscR_setTouch);

    qInfo() << "function hash map" << oscHandleHash.keys();
}

void DeviceDataInput::oscR_setAcceleration(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args)
{
    QVector3D *accVector = &who->acceleration[sens_index];
    who->set3dVector(accVector, &args);


emit who->accelNotify.at(sens_index)->vectorChanged(*accVector);

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







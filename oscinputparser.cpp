#include "oscinputparser.h"

OscInputParser::OscInputParser(QObject *parent)
    : ValueNotifierClass{parent}
{
    setSupportsSubValues(false);
}

OscInputParser::OscInputParser(QByteArray identifier, TypeHelper::SensorType sensorType, QObject *parent)
    : ValueNotifierClass{identifier, 0, TypeHelper::valueTypeForSensor(sensorType), 0, parent}, m_sensorType{sensorType}
{
    switch(sensorType) {
    case TypeHelper::Custom:
        m_oscHandle = &OscInputParser::oscR_unMapped;
        break;
    case TypeHelper::Accel:
        m_oscHandle = &OscInputParser::oscR_vectorType;
        break;
    case TypeHelper::Gyro:
        m_oscHandle = &OscInputParser::oscR_vectorType;
        break;
    case TypeHelper::Grav:
        m_oscHandle = &OscInputParser::oscR_gravityVector;
        break;
    case TypeHelper::RotQuat:
        m_oscHandle = &OscInputParser::oscR_quatType;
        break;
    case TypeHelper::Touch:
        m_valueList.resize(6);
        setValueNumber(6);
        m_oscHandle = &OscInputParser::oscR_touchType;
        break;
    }
    setSupportsSubValues(false);
}

OscInputParser::OscInputParser(QByteArray identifier, TypeHelper::ValueType valType, quint16 nValues, QObject *parent)
    : ValueNotifierClass{identifier, 0, valType, nValues, parent}
{
    switch(valType) {
    case TypeHelper::Undefined:
        m_oscHandle = &OscInputParser::oscR_unMapped;
        break;
    case TypeHelper::Vector:
        m_oscHandle = &OscInputParser::oscR_vectorType;
        break;
    case TypeHelper::Quat:
        m_oscHandle = &OscInputParser::oscR_quatType;
        break;
    case TypeHelper::List:
        m_oscHandle = &OscInputParser::oscR_listType;
        break;
    case TypeHelper::SingleValue:
        m_oscHandle = &OscInputParser::oscR_singleValue;
        break;
    case TypeHelper::BoolValue:
        break;
    case TypeHelper::Trigger:
        break;
    case TypeHelper::BoolList:
        break;
    case TypeHelper::IntValue:
        break;
    }
}

bool OscInputParser::handleUnmappedOsc() const
{
    return m_handleUnmappedOsc;
}

void OscInputParser::setHandleUnmappedOsc(bool newHandleUnmappedOsc)
{
    if (m_handleUnmappedOsc == newHandleUnmappedOsc)
        return;
    m_handleUnmappedOsc = newHandleUnmappedOsc;
    emit handleUnmappedOscChanged();
}

void OscInputParser::handleOscArgs(OSCPP::Server::ArgStream args, quint32 frame)
{
    m_frame = frame;
    (this->*m_oscHandle)(&args); //??? ok...
}

void OscInputParser::oscR_vectorType(OSCPP::Server::ArgStream *args)
{
//    qDebug() << "receiving vector" << args;
    set3dVectorFromArgs(args);
    callVectorChanged(m_vector, m_frame);
}

void OscInputParser::oscR_quatType(OSCPP::Server::ArgStream *args)
{
    setQuaternionFromArgs(args);
    callQuatChanged(m_quat, m_frame);
}

void OscInputParser::oscR_touchType(OSCPP::Server::ArgStream *args)
{
    setFloatListFromIntArgs(args);
//    callTouchChanged(m_valueList, m_frame);
    callValuesChanged(m_valueList, m_frame);
}

void OscInputParser::oscR_floatListType(OSCPP::Server::ArgStream *args)
{
    m_valueList.resize(args->size());
    setFloatListFromFloatArgs(args);
    callValuesChanged(m_valueList, m_frame);
}

void OscInputParser::oscR_intListType(OSCPP::Server::ArgStream *args)
{
    m_valueList.resize(args->size());
    setFloatListFromIntArgs(args);
    callValuesChanged(m_valueList, m_frame);
}

void OscInputParser::oscR_listType(OSCPP::Server::ArgStream *args)
{
    m_valueList.resize(args->size());
    setFloatListFromArgs(args);
    callValuesChanged(m_valueList, m_frame);
}

void OscInputParser::oscR_gravityVector(OSCPP::Server::ArgStream *args)
{
    set3dVectorFromArgs(args);
    m_vector.normalize();
    callVectorChanged(m_vector, m_frame);
}

void OscInputParser::oscR_singleValue(OSCPP::Server::ArgStream *args)
{
    if(!args->atEnd()) {
        try {
            m_value = args->float32();
        } catch (OSCPP::ParseError) {
            emit errorInOscParsing();
        }
    }
}

void OscInputParser::oscR_unMapped(OSCPP::Server::ArgStream *args)
{
//    qDebug() << "got unmapped";
    emit gotUnmappedMessage();
}


void OscInputParser::set3dVectorFromArgs(OSCPP::Server::ArgStream *args)
{

    uint8_t fc = 0;
    while(!args->atEnd() && fc < 3) {
        if(args->tag() == 'f') {
            //check for NaN?
            switch (fc) {
            case 0:
                m_vector.setX(args->float32()); break;
            case 1:
                m_vector.setY(args->float32()); break;
            case 2:
                m_vector.setZ(args->float32()); break;
            }
            fc++;
        }
    }
//    qDebug() << "Did set vector" << m_vector;
}

void OscInputParser::setQuaternionFromArgs(OSCPP::Server::ArgStream *args)
{
//    QQuaternion quat;
    uint8_t fc = 0;
    while(!args->atEnd() && fc < 4) {
        if(args->tag() == 'f') {
            //check for NaN?
            switch(fc) {
            case 0:
                m_quat.setX(args->float32()); break;
            case 1:
                m_quat.setY(args->float32()); break;
            case 2:
                m_quat.setZ(args->float32()); break;
            case 3:
                m_quat.setScalar(args->float32()); break;
            }
            fc++;
        }
    }
//    qDebug() << "Did set quat" << m_quat;
}

void OscInputParser::setFloatListFromFloatArgs(OSCPP::Server::ArgStream *args)
{
    quint8 fc = 0;
    while(!args->atEnd() && fc < m_valueList.size()) {
        if(args->tag() == 'f') {
            m_valueList[fc] = args->float32();
        }
        else {
            args->drop();
        }
        fc++;
    }
}

void OscInputParser::setFloatListFromIntArgs(OSCPP::Server::ArgStream *args)
{
    quint8 fc = 0;
    while(!args->atEnd() && fc < m_valueList.size()) {
        if(args->tag() == 'i') {
            m_valueList[fc] = args->int32();
        }
        else {
            args->drop();
        }
        fc++;
    }
//    qDebug() << "got touch" << m_valueList;
}

void OscInputParser::setFloatListFromArgs(OSCPP::Server::ArgStream *args)
{
    quint8 fc = 0;
    while(!args->atEnd() && fc < m_valueList.size()) {
        try {
            m_valueList[fc] = args->float32();
        } catch (OSCPP::ParseError) {
            emit errorInOscParsing();
        }
        fc++;
    }
}

const TypeHelper::SensorType &OscInputParser::sensorType() const
{
    return m_sensorType;
}

void OscInputParser::setSensorType(const TypeHelper::SensorType &newSensorType)
{
    if (m_sensorType == newSensorType)
        return;
    m_sensorType = newSensorType;
    emit sensorTypeChanged();
}

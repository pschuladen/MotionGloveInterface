#include "valuenotifierclass.h"

ValueNotifierClass::ValueNotifierClass(QObject *parent)
    : QObject{parent}
{
}

ValueNotifierClass::ValueNotifierClass(QByteArray identifier, int objectIdx, QObject *parent)
    : QObject{parent}, m_indexInObject{objectIdx}, m_identifier{identifier}
{
}

ValueNotifierClass::ValueNotifierClass(QByteArray identifier, int objectIdx, TypeHelper::ValueType valType, quint16 valueNumber, QObject *parent)
    : QObject{parent}, m_connectedValueType{valType}, m_valueNumber{valueNumber},
      m_indexInObject{objectIdx}, m_identifier{identifier}
{
    createSubnotifierForValueType(valType, valueNumber);
}

//ValueNotifierClass::ValueNotifierClass(TypeHelper::SensorType sensType, QObject *parent, int valueNumber)
//    : QObject{parent}
//{
//    setParent(parent);
//    int _nVals = 0;
//    switch (sensType) {

//    case SensType::Accel:
//        _nVals = 3;
//        break;
//    case SensType::Gyro:
//        _nVals = 3;
//        break;
//    case SensType::Grav:
//        _nVals = 3;
//        break;
//    case SensType::RotQuat:
//        _nVals = 4;
//        break;
//    case SensType::Touch:
//        _nVals = 6;
//        break;
//    case SensType::Custom:
//        int _nVals = valueNumber;
//        break;
//    }
//    createSubnotifier(_nVals);
//}

void ValueNotifierClass::createSubnotifier(int numberOfSubs)
{
    //TODO: possibly dangerous for existing (sub-)connections...
    subNotifier.reserve(numberOfSubs);
    for(int i = 0; i < numberOfSubs; i++) {
        subNotifier.append(new ValueNotifierClass(this));
    }
}

void ValueNotifierClass::createSubnotifierForValueType(TypeHelper::ValueType valType, int nVals)
{
    if(valType == TypeHelper::List && nVals > 0) createSubnotifier(nVals);
    else createSubnotifier(TypeHelper::getSizeForValueType(valType));
}


ValueNotifierClass *ValueNotifierClass::getNotifier(int idx)
{
    if(idx < 0) return this;

    else if(idx < subNotifier.size()) return subNotifier.at(idx);

    else return nullptr;
}

void ValueNotifierClass::setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType, bool createSubnotifier)
{
    if(newConnectedValueType == m_connectedValueType) return;

    m_connectedValueType = newConnectedValueType;
    setSupportsSubValues(createSubnotifier);
    if(createSubnotifier)
        createSubnotifierForValueType(newConnectedValueType, valueNumber());

    emit connectedValueTypeChanged(newConnectedValueType);
}

void ValueNotifierClass::callQuatChanged(const QQuaternion quat, int frame)
{
    emit quatChanged(quat, frame);
    if(supportsSubValues()) {
        emit subNotifier.at(0)->singleValueChanged(quat.x(), frame);
        emit subNotifier.at(1)->singleValueChanged(quat.y(), frame);
        emit subNotifier.at(2)->singleValueChanged(quat.z(), frame);
        emit subNotifier.at(3)->singleValueChanged(quat.scalar(), frame);
    }
}

void ValueNotifierClass::callVectorChanged(const QVector3D vect, int frame)
{
    emit vectorChanged(vect);
    if(supportsSubValues()) {
        emit subNotifier.at(0)->singleValueChanged(vect.x(), frame);
        emit subNotifier.at(1)->singleValueChanged(vect.y(), frame);
        emit subNotifier.at(2)->singleValueChanged(vect.z(), frame);
    }
}

void ValueNotifierClass::callTouchChanged(const QList<float> touch, int frame)
{
    emit touchChanged(touch);
    if(supportsSubValues()) {
        for(int i = 0; i < m_valueNumber; i++) {
            emit subNotifier.at(i)->singleValueChanged(touch.at(i), frame);
        }
    }
}

void ValueNotifierClass::callValuesChanged(const QList<float> values, int frame)
{
    emit valuesChanged(values);
    if(supportsSubValues()) {
        for(int i= 0; i < values.size(); i++) {
            emit subNotifier.at(i)->singleValueChanged(values[i], frame);
        }
    }
}

bool ValueNotifierClass::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

void ValueNotifierClass::slot_quatChanged(QQuaternion quat, int frame)
{
    if(m_autoEmit) callQuatChanged(quat, frame);
}

void ValueNotifierClass::slot_vectorChanged(QVector3D vect, int frame)
{
    if(m_autoEmit) callVectorChanged(vect, frame);
}

void ValueNotifierClass::slot_touchChanged(QList<float> touch, int frame)
{
    if(m_autoEmit) callTouchChanged(touch, frame);
}

void ValueNotifierClass::slot_valuesChanged(QList<float> values, int frame)
{
    if(m_autoEmit) callValuesChanged(values, frame);
}

void ValueNotifierClass::slot_singleValueChanged(float value, int frame)
{
    if(m_autoEmit) emit singleValueChanged(value, frame);
}

void ValueNotifierClass::slot_boolValueChanged(bool value, int frame)
{
    //TODO: implement
}

void ValueNotifierClass::slot_trigger(int frame)
{
    //TODO: implement
}

const TypeHelper::ValueType &ValueNotifierClass::connectedValueType() const
{
    return m_connectedValueType;
}


bool ValueNotifierClass::connectValueTypeSignalToSlot(const ValueNotifierClass *sender, const ValueNotifierClass *receiver, const TypeHelper::ValueType vType)
{
    if(sender == nullptr || receiver == nullptr) return false;
    QMetaObject::Connection _co;
    typedef ValueNotifierClass _vc;
    switch(vType) {
    case TypeHelper::Undefined:
        return false;
    case TypeHelper::Vector:
        _co = connect(sender, &_vc::vectorChanged, receiver, &_vc::slot_vectorChanged, Qt::QueuedConnection);
        break;
    case TypeHelper::Quat:
        _co = connect(sender, &_vc::quatChanged, receiver, &_vc::slot_quatChanged);
        break;
    case TypeHelper::List:
        _co = connect(sender, &_vc::valuesChanged, receiver, &_vc::slot_valuesChanged);
        break;
    case TypeHelper::SingleValue:
        _co = connect(sender, &_vc::singleValueChanged, receiver, &_vc::slot_singleValueChanged);
        break;
    case TypeHelper::BoolValue:
        _co = connect(sender, &_vc::boolValueChanged, receiver, &_vc::slot_boolValueChanged);
        break;
    case TypeHelper::Trigger:
        _co = connect(sender, &_vc::triggerActivated, receiver, &_vc::slot_trigger);
        break;
    }
    return true;
}

bool ValueNotifierClass::disconnectValueTypeSignalToSlot(const ValueNotifierClass *sender, const ValueNotifierClass *receiver, const TypeHelper::ValueType vType)
{

    typedef ValueNotifierClass _vc;
    switch(vType) {
    case TypeHelper::Undefined:
        return false;
    case TypeHelper::Vector:
        return disconnect(sender, &_vc::vectorChanged, receiver, &_vc::slot_vectorChanged);
        break;
    case TypeHelper::Quat:
        return disconnect(sender, &_vc::quatChanged, receiver, &_vc::slot_quatChanged);
        break;
    case TypeHelper::List:
        return disconnect(sender, &_vc::valuesChanged, receiver, &_vc::slot_valuesChanged);
        break;
    case TypeHelper::SingleValue:
        return disconnect(sender, &_vc::singleValueChanged, receiver, &_vc::slot_singleValueChanged);
        break;
    case TypeHelper::BoolValue:
        return disconnect(sender, &_vc::boolValueChanged, receiver, &_vc::slot_boolValueChanged);
        break;
    case TypeHelper::Trigger:
        return disconnect(sender, &_vc::triggerActivated, receiver, &_vc::slot_trigger);
        break;
    }
    return false;
}

quint32 ValueNotifierClass::indexInObject() const
{
    return m_indexInObject;
}

void ValueNotifierClass::setIndexInObject(quint32 newIndexInObject)
{
    if (m_indexInObject == newIndexInObject)
        return;
    m_indexInObject = newIndexInObject;
    emit indexInObjectChanged();
}

quint16 ValueNotifierClass::valueNumber() const
{
    return m_valueNumber;
}

void ValueNotifierClass::setValueNumber(quint16 newValueNumber)
{
    if (m_valueNumber == newValueNumber)
        return;
    m_valueNumber = newValueNumber;
    emit valueNumberChanged();
}

void ValueNotifierClass::unimplementedValueTypeWarning(TypeHelper::ValueType valType, QString extraMsg)
{
    qInfo() << this << ": valueType" << valType << "not implemented yet" << extraMsg;
}

bool ValueNotifierClass::acceptsInputType(TypeHelper::ValueType typ) const
{
    return false;
}

bool ValueNotifierClass::supportsSubValues() const
{
    return m_supportsSubValues;
}

void ValueNotifierClass::setSupportsSubValues(bool newSupportsSubValues)
{
    if (m_supportsSubValues == newSupportsSubValues)
        return;
    m_supportsSubValues = newSupportsSubValues;
    emit supportsSubValuesChanged();
}

const QByteArray &ValueNotifierClass::identifier() const
{
    return m_identifier;
}

void ValueNotifierClass::setIdentifier(const QByteArray &newIdentifier)
{
    if (m_identifier == newIdentifier)
        return;
    m_identifier = newIdentifier;
    emit identifierChanged();
}

bool ValueNotifierClass::autoEmit() const
{
    return m_autoEmit;
}

void ValueNotifierClass::setAutoEmit(bool newAutoEmit)
{
    if (m_autoEmit == newAutoEmit)
        return;
    m_autoEmit = newAutoEmit;
    emit autoEmitChanged();
}

#include "valuenotifierclass.h"

ValueNotifierClass::ValueNotifierClass(QObject *parent, TypeHelper::ValueType valueType,  int valueNumber, int indexForObject)
    : QObject{parent}
{
    m_indexInObject = indexForObject;
    setConnectedValueType(valueType);

}

ValueNotifierClass::ValueNotifierClass(TypeHelper::SensorType sensType, QObject *parent, int valueNumber)
{
    setParent(parent);
    int _nVals = 0;
    switch (sensType) {

    case SensType::Accel:
        _nVals = 3;
        break;
    case SensType::Gyro:
        _nVals = 3;
        break;
    case SensType::Grav:
        _nVals = 3;
        break;
    case SensType::RotQuat:
        _nVals = 4;
        break;
    case SensType::Touch:
        _nVals = 6;
        break;
    case SensType::Custom:
        int _nVals = valueNumber;
        break;
    }
    createSubnotifier(_nVals);
}

void ValueNotifierClass::createSubnotifier(int numberOfSubs)
{
    //TODO: possibly dangerous for existing (sub-)connections...
    subNotifier.resize(numberOfSubs);
    for(int i = 0; i < numberOfSubs; i++) {
        ValueNotifierClass *subNotify = new ValueNotifierClass(this);
        subNotifier.insert(i, subNotify);
    }
}

void ValueNotifierClass::createSubnotifier(TypeHelper::ValueType valueType)
{
    int _nVals = 0;
    switch(valueType) {
    case TypeHelper::Undefined:
        qWarning() << "undefined value notifier";
        break;
    case TypeHelper::Vector:
        _nVals = 3;
        break;
    case TypeHelper::Quat:
        _nVals = 4;
        break;
    case TypeHelper::List:
        _nVals = 6;
        break;
    case TypeHelper::SingleValue:
        break;
    case TypeHelper::BoolValue:
        break;
    case TypeHelper::Trigger:
        break;
    }
    createSubnotifier(_nVals);

}

ValueNotifierClass *ValueNotifierClass::getNotifier(int idx)
{
    if(idx < 0) return this;

    else if(idx < subNotifier.size()) return subNotifier.at(idx);

    else return nullptr;
}

void ValueNotifierClass::setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType)
{
    if(newConnectedValueType == connectedValueType()) return;
    m_connectedValueType = newConnectedValueType;
    createSubnotifier(newConnectedValueType);
    emit connectedValueTypeChanged(newConnectedValueType);
}

void ValueNotifierClass::callQuatChanged(const QQuaternion quat, int frame)
{
    emit quatChanged(quat, frame);
    emit subNotifier.at(0)->singleValueChanged(quat.x(), frame);
    emit subNotifier.at(1)->singleValueChanged(quat.y(), frame);
    emit subNotifier.at(2)->singleValueChanged(quat.z(), frame);
    emit subNotifier.at(3)->singleValueChanged(quat.scalar(), frame);
}

void ValueNotifierClass::callVectorChanged(const QVector3D vect, int frame)
{
    emit vectorChanged(vect);
    emit subNotifier.at(0)->singleValueChanged(vect.x(), frame);
    emit subNotifier.at(1)->singleValueChanged(vect.y(), frame);
    emit subNotifier.at(2)->singleValueChanged(vect.z(), frame);
}

void ValueNotifierClass::callTouchChanged(const QList<float> touch, int frame)
{
    emit touchChanged(touch);
    for(int i = 0; i < m_numberValues; i++) {
        emit subNotifier.at(i)->singleValueChanged(touch.at(i), frame);
    }
}

void ValueNotifierClass::callValuesChanged(const QList<float> values, int frame)
{
    emit valuesChanged(values);
    for(int i= 0; i < values.size(); i++) {
        emit subNotifier.at(i)->singleValueChanged(values[i], frame);
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


bool ValueNotifierClass::connectValueTypeSignalToSlot(ValueNotifierClass *sender, ValueNotifierClass *receiver, TypeHelper::ValueType vType)
{
    typedef ValueNotifierClass _vc;
    switch(vType) {
    case TypeHelper::Undefined:
        return false;
    case TypeHelper::Vector:
        connect(sender, &_vc::vectorChanged, receiver, &_vc::slot_vectorChanged);
        break;
    case TypeHelper::Quat:
        connect(sender, &_vc::quatChanged, receiver, &_vc::slot_quatChanged);
        break;
    case TypeHelper::List:
        connect(sender, &_vc::valuesChanged, receiver, &_vc::slot_valuesChanged);
        break;
    case TypeHelper::SingleValue:
        connect(sender, &_vc::singleValueChanged, receiver, &_vc::slot_singleValueChanged);
        break;
    case TypeHelper::BoolValue:
        connect(sender, &_vc::boolValueChanged, receiver, &_vc::slot_boolValueChanged);
        break;
    case TypeHelper::Trigger:
        connect(sender, &_vc::triggerActivated, receiver, &_vc::slot_trigger);
        break;
    }
    return true;
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

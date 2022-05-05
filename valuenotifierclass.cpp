#include "valuenotifierclass.h"

ValueNotifierClass::ValueNotifierClass(QObject *parent, TypeHelper::ValueType valueType,  int valueNumber, int indexForObject)
    : QObject{parent}
{
    m_indexInObject = indexForObject;
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
        _nVals = valueNumber;
        break;
    case TypeHelper::SingleValue:
        break;
    case TypeHelper::BoolValue:
        break;
    case TypeHelper::Trigger:
        break;

    }
    m_numberValues = _nVals;
    createSubnotifier(_nVals);
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
        //        default:
        //            break;
    }
    createSubnotifier(_nVals);
}

void ValueNotifierClass::createSubnotifier(int numberOfSubs)
{
    subNotifier.resize(numberOfSubs);
    for(int i = 0; i < numberOfSubs; i++) {
        //        qInfo() << "subnr " << i;
        ValueNotifierClass *subNotify = new ValueNotifierClass(this);
        subNotifier.insert(i, subNotify);
    }
}

void ValueNotifierClass::callQuatChanged(const QQuaternion quat)
{
    emit quatChanged(quat);
    emit subNotifier.at(0)->singleValueChanged(quat.x());
    emit subNotifier.at(1)->singleValueChanged(quat.y());
    emit subNotifier.at(2)->singleValueChanged(quat.z());
    emit subNotifier.at(3)->singleValueChanged(quat.scalar());
}

void ValueNotifierClass::callVectorChanged(const QVector3D vect)
{
    emit vectorChanged(vect);
    emit subNotifier.at(0)->singleValueChanged(vect.x());
    emit subNotifier.at(1)->singleValueChanged(vect.y());
    emit subNotifier.at(2)->singleValueChanged(vect.z());
}

void ValueNotifierClass::callTouchChanged(const QList<float> touch)
{
    emit touchChanged(touch);
    for(int i = 0; i < m_numberValues; i++) {
        emit subNotifier.at(i)->singleValueChanged(touch.at(i));
    }
}

void ValueNotifierClass::callValuesChanged(const QList<float> values)
{
    emit valuesChanged(values);
    for(int i= 0; i < values.size(); i++) {
        emit subNotifier.at(i)->singleValueChanged(values[i]);
    }
}

void ValueNotifierClass::slot_quatChanged(QQuaternion quat, int frame)
{

}

void ValueNotifierClass::slot_vectorChanged(QVector3D vect, int frame)
{

}

void ValueNotifierClass::slot_touchChanged(QList<float> touch, int frame)
{

}

void ValueNotifierClass::slot_valuesChanged(QList<float> values, int frame)
{

}

void ValueNotifierClass::slot_singleValueChanged(float value, int frame)
{

}

void ValueNotifierClass::slot_boolValueChanged(bool value, int frame)
{
    //TODO: implement
}

void ValueNotifierClass::slot_trigger(int frame)
{
    //TODO: implement
}

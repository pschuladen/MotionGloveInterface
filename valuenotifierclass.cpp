#include "valuenotifierclass.h"

ValueNotifierClass::ValueNotifierClass(QObject *parent, SensType sType,  int valueNumber)
    : QObject{parent}
{
    int _nVals = 0;
    switch(sType) {
    case SensType::Accel:
        _nVals = 3;
        break;
    case SensType::Gyro:
        _nVals = 3;
        break;
    case SensType::Grav:
        _nVals = 3;
        break;
    case SensType::Quat:
        _nVals = 4;
        break;
    case SensType::Touch:
        _nVals = 6;
        break;
    case SensType::Custom:
        int _nVals = valueNumber;
        break;
//    default:
//        break;
    }
    m_numberValues = _nVals;
    subNotifier.resize(_nVals);
    for(int i = 0; i < _nVals; i++) {
//        qInfo() << "subnr " << i;
        ValueNotifierClass *subNotify = new ValueNotifierClass(this);
        subNotifier.insert(i, subNotify);
    }
}

void ValueNotifierClass::callQuatChanged(QQuaternion quat)
{
    emit quatChanged(quat);
    emit subNotifier.at(0)->singleValueChanged(quat.x());
    emit subNotifier.at(1)->singleValueChanged(quat.y());
    emit subNotifier.at(2)->singleValueChanged(quat.z());
    emit subNotifier.at(3)->singleValueChanged(quat.scalar());
}

void ValueNotifierClass::callVectorChanged(QVector3D vect)
{
    emit vectorChanged(vect);
    emit subNotifier.at(0)->singleValueChanged(vect.x());
    emit subNotifier.at(1)->singleValueChanged(vect.y());
    emit subNotifier.at(2)->singleValueChanged(vect.z());
}

void ValueNotifierClass::callTouchChanged(QList<float> touch)
{
    emit touchChanged(touch);
    for(int i = 0; i < m_numberValues; i++) {
        emit subNotifier.at(i)->singleValueChanged(touch.at(i));
    }
}

void ValueNotifierClass::callValuesChanged(QList<float> values)
{

}

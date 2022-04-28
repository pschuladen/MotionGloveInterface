#ifndef VALUENOTIFIERCLASS_H
#define VALUENOTIFIERCLASS_H

/*
 * minimalistic class just for sending signals.
 * Was created for handling value updates for lists of values to differen receivers.
*/

#include <QObject>
#include <QQuaternion>
#include <QVector3D>
#include "inputvalueviewcontroller.h"

class ValueNotifierClass : public QObject
{
    Q_OBJECT
public:
    typedef InputValueViewController::ValueViewMode SensType ;
    explicit ValueNotifierClass(QObject *parent = nullptr, SensType sType = SensType::Custom, int valueNumber = 0);
//    void valueChanged(QQuaternion quat);
//    void valueChanged(QQuaternion quat);
    void callQuatChanged(QQuaternion quat);
    void callVectorChanged(QVector3D vect);
    void callTouchChanged(QList<float> touch);
    void callValuesChanged(QList<float> values);

    QList<ValueNotifierClass*> subNotifier;
private:
    int m_numberValues;

signals:
     void quatChanged(QQuaternion quat);
     void vectorChanged(QVector3D vect);
     void touchChanged(QList<float> touch);
     void valuesChanged(QList<float> values);
     void singleValueChanged(float value);
//     void valueChanged()

};

#endif // VALUENOTIFIERCLASS_H

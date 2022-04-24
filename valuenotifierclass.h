#ifndef VALUENOTIFIERCLASS_H
#define VALUENOTIFIERCLASS_H

#include <QObject>
#include <QQuaternion>
#include <QVector3D>
#include "valueviewbackend.h"

class ValueNotifierClass : public QObject
{
    Q_OBJECT
public:
    typedef ValueViewBackend::ValueViewMode sensType ;
    explicit ValueNotifierClass(QObject *parent = nullptr, sensType = sensType::Custom);

//    void valueChanged(QQuaternion quat);
//    void valueChanged(QQuaternion quat);

signals:
     void quatChanged(QQuaternion quat);
     void vectorChanged(QVector3D vect);
     void touchChanged(QList<float> touch);
     void valuesChanged(QList<float> values);

};

#endif // VALUENOTIFIERCLASS_H

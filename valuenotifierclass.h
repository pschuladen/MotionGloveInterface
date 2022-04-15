#ifndef VALUENOTIFIERCLASS_H
#define VALUENOTIFIERCLASS_H

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

class ValueNotifierClass : public QObject
{
    Q_OBJECT
public:
    explicit ValueNotifierClass(QObject *parent = nullptr);

signals:
     void quatChanged(QQuaternion quat);
     void vectorChanged(QVector3D vect);

};

#endif // VALUENOTIFIERCLASS_H

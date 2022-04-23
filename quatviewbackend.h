#ifndef QUATVIEWBACKEND_H
#define QUATVIEWBACKEND_H

#include <QObject>
#include <QQuaternion>

//#include <QtQml>
#include <QtQml/qqmlregistration.h>

class QuatViewBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT


    Q_PROPERTY(float xValue READ xValue NOTIFY xValueChanged)
    Q_PROPERTY(float yValue READ yValue NOTIFY yValueChanged)
    Q_PROPERTY(float zValue READ zValue NOTIFY zValueChanged)
    Q_PROPERTY(float wValue READ wValue NOTIFY wValueChanged)
    Q_PROPERTY(QList<QString> vname READ vname WRITE setVname NOTIFY vnameChanged)
//    QML_NAMED_ELEMENT(QuatViewBackend)

    float m_wValue;

    float m_xValue;

    float m_yValue;

    float m_zValue;

    QQuaternion *quatPointer;
    QQuaternion quat;

    QList<QString> m_vname;

public:
    explicit QuatViewBackend(QObject *parent = nullptr);

    float wValue() const;
    void setwValue(float newWValue);

    float xValue() const;
    void setxValue(float newXValue);

    float yValue() const;
    void setyValue(float newYValue);

    float zValue() const;
    void setzValue(float newZValue);

    void setQuatPointer(QQuaternion *newQuatPointer);

    const QList<QString> &vname() const;
    void setVname(const QList<QString> &newVname);

public slots:
    void vectorUpdatet();
    void quatChanged(QQuaternion newQuat);


signals:

    void wValueChanged();
    void xValueChanged();
    void yValueChanged();
    void zValueChanged();

    void vnameChanged();
};

#endif // QUATVIEWBACKEND_H

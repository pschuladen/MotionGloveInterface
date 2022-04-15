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
//    QML_NAMED_ELEMENT(QuatViewBackend)

    float m_wValue;

    float m_xValue;

    float m_yValue;

    float m_zValue;

    QQuaternion *quatPointer;

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

public slots:
    void vectorUpdatet();

signals:

    void wValueChanged();
    void xValueChanged();
    void yValueChanged();
    void zValueChanged();

};

#endif // QUATVIEWBACKEND_H

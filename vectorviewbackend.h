#ifndef VECTORVIEWBACKEND_H
#define VECTORVIEWBACKEND_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
//#include <QQmlEngine>
//#include <QtQml>
#include <QtQml/qqmlregistration.h>

class VectorViewBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float xValue READ xValue WRITE setxValue NOTIFY xValueChanged)
    Q_PROPERTY(float yValue READ yValue WRITE setyValue NOTIFY yValueChanged)
    Q_PROPERTY(float zValue READ zValue WRITE setzValue NOTIFY zValueChanged)
//    QML_NAMED_ELEMENT(VectorViewBackend)
    QML_ELEMENT

    float m_xValue;

    float m_yValue;

    float m_zValue;


public:
    explicit VectorViewBackend(QObject *parent = nullptr);
    explicit VectorViewBackend(const QVector3D *vecPointer, QObject *parent = nullptr);

    const QVector3D *vectPointer;

    float xValue() const;
    void setxValue(float newXValue);

    float yValue() const;
    void setyValue(float newYValue);

    float zValue() const;
    void setzValue(float newZValue);

    void setVectPointer(const QVector3D *newVectPointer);


public slots:
    void vectorUpdatet();

signals:

    void xValueChanged();
    void yValueChanged();
    void zValueChanged();
};

#endif // VECTORVIEWBACKEND_H

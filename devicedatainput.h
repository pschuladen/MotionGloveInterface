#ifndef DEVICEDATAINPUT_H
#define DEVICEDATAINPUT_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QUdpSocket>
#include <QList>
#include <QHash>

#include "oscpp/client.hpp"
#include "oscpp/server.hpp"
#include "vectorviewbackend.h"
#include "quatviewbackend.h"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlListProperty>
#include <QQuickWindow>
#include <QQmlContext>

class DeviceDataInput : public QObject
{
    Q_OBJECT
public:
    explicit DeviceDataInput(QObject *parent = nullptr, QString identifier = "", uint16_t port = 51002, uint8_t nSensors = 2);

    QString identifier;


    QList<VectorViewBackend*> accelView;
    QList<VectorViewBackend*> gyrosView;
    QList<VectorViewBackend*> gravityView;
    QList<QuatViewBackend*> quaternionView;

    QList<QVector3D> acceleration;
    QList<QVector3D> gyroscope;
    QList<QVector3D> gravityVector;
    QList<QQuaternion> quaternion;

    QList<int> touch;

    void createAllSensorInputViews(QQmlApplicationEngine *engine, QQuickItem *parentView);


signals:
    void accelerationChanged(int sensIndex, QVector3D* acceleration);
    void gyroscopeChanged(int sensIndex,QVector3D* gyroscope);
    void gravityVectorChanged(int sensIndex,QVector3D* gravityVector);
    void quaternionChanged(int sensIndex,QQuaternion* quaternion);


private:
    enum SenTyp {
        accel, gyro, grav, quat
    };
    void createSensorInputView(QQmlApplicationEngine *engine, QQuickItem *parentView, SenTyp typ);

    QUdpSocket *socket;
    uint16_t m_port;
    uint8_t m_nSensors;

//    typedef QHash<QString, QVariant> ;

//    QHash<QString, void(*)(int)> oscFunctionHash;
    typedef void (*OscHandleTypeFun)(DeviceDataInput*, int, OSCPP::Server::ArgStream);
    struct OscInputStruct {
        OscHandleTypeFun handleFunction = &DeviceDataInput::oscR_unMapped;
        int sensorIndex = 0;
    };
    OscInputStruct unmappedOsc;

    QHash<QString, void(*)(DeviceDataInput*, int, OSCPP::Server::ArgStream)> oscFunctionHash;
    QHash<QString, int> oscSensorIndexHash;
    QHash<QString, OscInputStruct> oscHandleHash;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void setupOscInputBindings();

    static void oscR_setAcceleration(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);
    static void oscR_setGyroscope(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);
    static void oscR_setGravityVector(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);
    static void oscR_setQuaternion(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);
    static void oscR_setTouch(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);

    static void oscR_unMapped(DeviceDataInput* who, int sens_index, OSCPP::Server::ArgStream args);

    void set3dVector(QVector3D *vector, OSCPP::Server::ArgStream *args);
    void setQuat(QQuaternion *quat, OSCPP::Server::ArgStream *args);

    QQuickItem *sensorViewContainer = nullptr;

    bool createSensorViewContainer(QQmlApplicationEngine* engine, QQuickItem* parentView);


};

#endif // DEVICEDATAINPUT_H

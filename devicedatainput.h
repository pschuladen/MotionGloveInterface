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
#include "valuenotifierclass.h"

#include "valueviewbackend.h"
//#include "MotionDeviceStruct.h"

struct MotionDevice;

class DeviceDataInput : public QObject
{
    Q_OBJECT
public:
    explicit DeviceDataInput(QObject *parent = nullptr, QString identifier = "", uint16_t port = 51002, uint8_t nSensors = 2, MotionDevice* deviceDescription = nullptr);

    QString identifier;

    QList<ValueNotifierClass*> accelNotify;
    QList<ValueNotifierClass*> gyrosNotify;
    QList<ValueNotifierClass*> gravityNotify;
    QList<ValueNotifierClass*> quaternionNotify;
    ValueNotifierClass *touchNotifier;

    QList<QVector3D> acceleration;
    QList<QVector3D> gyroscope;
    QList<QVector3D> gravityVector;
    QList<QQuaternion> quaternion;
    QList<int> touch;

    uint8_t m_nSensors;

signals:
    void accelerationChanged(int sensIndex, QVector3D* acceleration);
    void gyroscopeChanged(int sensIndex,QVector3D* gyroscope);
    void gravityVectorChanged(int sensIndex,QVector3D* gravityVector);
    void quaternionChanged(int sensIndex,QQuaternion* quaternion);


private:

    MotionDevice* deviceDescription;

    enum SenTyp {
        accel, gyro, grav, quat
    };
//    void createSensorInputView(QQmlApplicationEngine *engine, QQuickItem *parentView, SenTyp typ);

    QUdpSocket *socket;
    uint16_t m_port;

//    typedef QHash<QString, QVariant> ;

//    QHash<QString, void(*)(int)> oscFunctionHash;
    typedef void (*OscHandleTypeFun)(DeviceDataInput*, int, OSCPP::Server::ArgStream);
    struct OscInputStruct {
        OscHandleTypeFun handleFunction = &DeviceDataInput::oscR_unMapped;
        int sensorIndex = 0;
        ValueViewBackend::ValueViewMode sensorType;
    };

    OscInputStruct unmappedOsc;

//    QHash<QString, void(*)(DeviceDataInput*, int, OSCPP::Server::ArgStream)> oscFunctionHash;
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
//    QVector3D readOsc3dVector(OSCPP::Server::ArgStream *args);
    void setQuat(QQuaternion *quat, OSCPP::Server::ArgStream *args);



};

struct MotionDevice {
    struct InputDef {
        ValueViewBackend::ValueViewMode viewMode;
        int sensorIndex;
        InputDef(ValueViewBackend::ValueViewMode _viewMode, int _sensorIndex)
        {
            viewMode = _viewMode;
            sensorIndex = _sensorIndex;
        }
    };

    QString deviceName;
    QString address;
    uint16_t port;
    uint16_t sendIntervall;
    bool connectStatus;
    DeviceDataInput *inputHandler;
    QMap<QString, InputDef> inputs;

//    void getInputs() {
//        for(QString key in inputHandler->)
//    };
//    ValueViewBackend::ValueViewMode viewMode;
//    uint16_t numSensors;
};

#endif // DEVICEDATAINPUT_H

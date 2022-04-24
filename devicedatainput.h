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

    typedef ValueViewBackend::ValueViewMode SensType ;
    QString m_identifier;
    QList<SensType> allSensTypes = {SensType::Custom, SensType::Accel, SensType::Gyro, SensType::Grav, SensType::Quat, SensType::Touch};

    QMap<SensType, QList<ValueNotifierClass*>> valueNotifier;

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

    typedef void (*OscHandleTypeFun)(DeviceDataInput*, int, OSCPP::Server::ArgStream);
    struct OscInputStruct {
        OscHandleTypeFun handleFunction = &DeviceDataInput::oscR_unMapped;
        int sensorIndex = 0;
        ValueViewBackend::ValueViewMode sensorType;
    };

signals:
    void accelerationChanged(int sensIndex, QVector3D* acceleration);
    void gyroscopeChanged(int sensIndex,QVector3D* gyroscope);
    void gravityVectorChanged(int sensIndex,QVector3D* gravityVector);
    void quaternionChanged(int sensIndex,QQuaternion* quaternion);


private:

    MotionDevice* m_deviceDescription;

    enum SenTyp {
        accel, gyro, grav, quat
    };
    //    void createSensorInputView(QQmlApplicationEngine *engine, QQuickItem *parentView, SenTyp typ);

    QUdpSocket *socket;
    uint16_t m_port;

    //    typedef QHash<QString, QVariant> ;

    //    QHash<QString, void(*)(int)> oscFunctionHash;

    OscInputStruct unmappedOsc;

    //    QHash<QString, void(*)(DeviceDataInput*, int, OSCPP::Server::ArgStream)> oscFunctionHash;
    QHash<QString, int> oscSensorIndexHash;
    QHash<QString, OscInputStruct> oscHandleHash;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void setupOscInputBindings();
    void createNotifier();

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
    typedef ValueViewBackend::ValueViewMode SensType ;
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
    QHash<QString, DeviceDataInput::OscInputStruct> *inputs;

    QList<QString> getSortedInputKeys(bool sortByType=true, QList<SensType> sortOrder={SensType::Quat, SensType::Accel, SensType::Grav, SensType::Gyro, SensType::Touch, SensType::Custom}) {
        QMap<SensType, QList<QString>> sortMap;
        QList<QString> sortedKeys;
        foreach(const SensType &_typ, sortOrder) {
            QList<QString> sortedTypeKeys;
            sortedTypeKeys.resize(inputHandler->m_nSensors);
            sortMap[_typ] = sortedTypeKeys;
        }
        foreach(const QString &osc, inputs->keys()) {
            DeviceDataInput::OscInputStruct inputStruct = inputs->value(osc);
            if (sortOrder.contains(inputStruct.sensorType)) {
                sortMap[inputStruct.sensorType][inputStruct.sensorIndex] = osc;
            }
        }
        if (sortByType) {
            foreach(const SensType &_typ, sortOrder) {
                foreach(const QString &osc, sortMap[_typ]) {
                    if(osc != "") {
                        sortedKeys.append(osc);
                    }
                }
            }
        }
        else {
            for(int i = 0; i < inputHandler->m_nSensors; i++) {
                foreach(const SensType &_typ, sortOrder) {
                    if(i < sortMap[_typ].size()) {
                        if(sortMap[_typ].at(i) != "") {
                            sortedKeys.append(sortMap[_typ].at(i));
                        }
                    }
                }
            }
        }
        return sortedKeys;
    }
};

#endif // DEVICEDATAINPUT_H

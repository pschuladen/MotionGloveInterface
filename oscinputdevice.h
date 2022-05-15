#ifndef DEVICEDATAINPUT_H
#define DEVICEDATAINPUT_H

/*
 * This class opens a udp-port and accepts osc-sensor messages.
 * Currently it is especially designed for the gloves of Paul Schuladen
 * and accepts Quaternion, Acceleration, Gyroscope, GravityVector and Touch.
 *
 * The incoming osc-messages were sorted with a hashmap (QHash<QString, OscInputStruct> oscHandleHash)
 * and the data stored in the corresponding QLists.
 *
 * Views and Nodes are notified through signals emitted from the ValueNotifierClass.
 *
*/

//#include <QObject>
//#include <QVector3D>
//#include <QQuaternion>
#include <QUdpSocket>
#include <QList>
#include <QHash>

#include "oscpp/client.hpp"
#include "oscpp/server.hpp"

#include "valuenotifierclass.h"
//#include "inputvalueviewcontroller.h"

#include "typehelper.h"

#include "oscinputparser.h"

struct MotionDevice;

class OscInputDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(quint8 nSensors READ nSensors WRITE setNSensors NOTIFY nSensorsChanged)
    Q_PROPERTY(quint16 rcvPort READ rcvPort WRITE setRcvPort NOTIFY rcvPortChanged)
    Q_PROPERTY(quint16 devicePort READ devicePort WRITE setDevicePort NOTIFY devicePortChanged)
    Q_PROPERTY(QString deviceAddressStr READ deviceAddressStr WRITE setDeviceAddressStr NOTIFY deviceAddressStrChanged)
    Q_PROPERTY(quint16 sendIntervall READ sendIntervall WRITE setSendIntervall NOTIFY sendIntervallChanged)
    Q_PROPERTY(bool sendPong READ sendPong WRITE setSendPong NOTIFY sendPongChanged)
    Q_PROPERTY(bool deviceReady READ deviceReady WRITE setDeviceReady NOTIFY deviceReadyChanged)


public:
    explicit OscInputDevice(QObject *parent = nullptr);
    explicit OscInputDevice(QString deviceName, QObject *parent = nullptr);
//    explicit OscInputDevice(QString identifier = "", uint16_t port = 51002, uint8_t nSensors = 2, MotionDevice* deviceDescription = nullptr, QObject *parent = nullptr);

    QString m_deviceName;
    uint8_t m_nSensors;

//    typedef InputValueViewController::ValueViewMode SensType ;
    typedef TypeHelper::SensorType SensType ;
    const QList<SensType> allSensTypes = {SensType::Custom, SensType::Accel, SensType::Gyro, SensType::Grav, SensType::RotQuat, SensType::Touch};

    // delete soon
    QMap<SensType, QList<ValueNotifierClass*>> valueNotifier;
    ValueNotifierClass *getNotifier(SensType sensType, int sensIndex, int subIndex);
    //

    QMap<QByteArray, OscInputParser*> oscInputParser;

//    QList<QVector3D> acceleration;
//    QList<QVector3D> gyroscope;
//    QList<QVector3D> gravityVector;
//    QList<QQuaternion> quaternion;
//    QList<int> touch;

//    typedef void (*OscHandleTypeFun)(OscInputDevice*, int, OSCPP::Server::ArgStream);
//    struct OscInputStruct {
//        OscHandleTypeFun handleFunction = &OscInputDevice::oscR_unMapped;
//        int sensorIndex = 0;
//        TypeHelper::SensorType sensorType;

//        OscInputStruct(OscHandleTypeFun _handleFunc, int _sensIdx, TypeHelper::SensorType _sensType)
//            : handleFunction{_handleFunc}, sensorIndex{_sensIdx}, sensorType{_sensType} {}
//        OscInputStruct() {}
//    };
    const QString &deviceName() const;

    struct OscSensorInputStruct {
        QString identifier;
        SensType sensType;
        ValueNotifierClass *oscHandler;
        OscSensorInputStruct(QString _identifier, SensType _sensType, ValueNotifierClass *_oscHandler)
            : identifier{_identifier}, sensType{_sensType}, oscHandler{_oscHandler} {}
        OscSensorInputStruct() {}
    };

    void setDeviceName(const QString &newDeviceName);

    quint8 nSensors() const;
    void setNSensors(quint8 newNSensors);

    quint16 rcvPort() const;
    void setRcvPort(quint16 newPort);

    bool sendPong() const;
    void setSendPong(bool newSendPong);

    const QString &deviceAddressStr() const;
    void setDeviceAddressStr(const QString &newIpAddress);

    quint16 sendIntervall() const;
    void setSendIntervall(quint16 newSendIntervall);

    bool deviceReady() const;
    void setDeviceReady(bool newDeviceReady);

    void handlePingMessage(OSCPP::Server::ArgStream oscArgs);

    quint16 devicePort() const;
    void setDevicePort(quint16 newDevicePort);

signals:

    void deviceNameChanged();
    void nSensorsChanged();
    void rcvPortChanged();
    void sendPongChanged();
    void deviceAddressStrChanged();
    void sendIntervallChanged();
    void deviceReadyChanged();
    void devicePortChanged();

//    void sendInputPathsForValueType(QString oscPaths, TypeHelper::ValueType valType, ValueNotifierClass* oscHandler);
//    void sendInputPathsForSensorType(QString oscPaths, TypeHelper::SensorType sensType, ValueNotifierClass* oscHandler);
    void sendSensorStructList(const QString myname, const QList<OscInputDevice::OscSensorInputStruct>);
    void requestViewCreation(QString deviceName);

public slots:
//    void nameRequested();
    void viewWasCreated();


private:

    MotionDevice* m_deviceDescription;

//    enum SenTyp {
//        accel, gyro, grav, quat
//    };

    QUdpSocket *m_socket;
    uint16_t m_rcvPort;

//    OscInputStruct unmappedOsc;

//    QHash<QByteArray, OscInputStruct> oscHandleHash;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void setupOscInputBindings();
    void createNotifier();

//    void sendPongToDevice();

    //OSC Callback functions
//    static void oscR_setAcceleration(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);
//    static void oscR_setGyroscope(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);
//    static void oscR_setGravityVector(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);
//    static void oscR_setQuaternion(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);
//    static void oscR_setTouch(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);

//    static void oscR_unMapped(OscInputDevice* who, int sens_index, OSCPP::Server::ArgStream args);

    void set3dVector(QVector3D *vector, OSCPP::Server::ArgStream *args);
    //    QVector3D readOsc3dVector(OSCPP::Server::ArgStream *args);
    void setQuat(QQuaternion *quat, OSCPP::Server::ArgStream *args);
    bool m_sendPong;
    QString m_deviceAddressStr;
    quint16 m_sendIntervall;
    bool m_deviceReady;
    quint16 m_devicePort;

    QHostAddress m_deviceIpAddress;
    bool readyToPong();
    void sendPongMessage();

//    static QString getProtoStrForSensor(TypeHelper::SensorType sensType);
};

/*
 * The struct MotionDevce serves as description for inputdevices.
 * The DeviceStatusController holds a list of MotionDevice Objects
 * for management.
*/

struct MotionDevice {
//    typedef InputValueViewController::ValueViewMode SensType ;
    typedef TypeHelper::SensorType SensType ;

    QString deviceName;
    QString m_deviceAddress;
    uint16_t port;
    uint16_t sendIntervall;
    bool connectStatus;
    OscInputDevice *inputHandler;
//    QHash<QByteArray, OscInputDevice::OscInputStruct> *inputs;

    ValueNotifierClass *getValueNotifierForPath(const QString oscPath, int valueIndex=-1) const {
//        if(inputHandler != nullptr) {
//            return inputHandler->getNotifier(inputs->value(oscPath.toUtf8()).sensorType,
//                                      inputs->value(oscPath.toUtf8()).sensorIndex,
//                                      valueIndex);
//        }
        return nullptr;
    }

    QList<QByteArray> getSortedInputKeys(bool sortByType=true,
                                      const QList<SensType> sortOrder={SensType::RotQuat, SensType::Accel, SensType::Grav, SensType::Gyro, SensType::Touch, SensType::Custom})
    {
        QList<QByteArray> sortedKeys;
//        QMap<SensType, QList<QByteArray>> sortMap;
//        for(const SensType &_typ: sortOrder) {
//            QList<QByteArray> sortedTypeKeys;
//            sortedTypeKeys.resize(inputHandler->m_nSensors);
//            sortMap[_typ] = sortedTypeKeys;
//        }
//        for(QHash<QByteArray, OscInputDevice::OscInputStruct>::const_iterator oscInpIterator = inputs->cbegin(), end = inputs->cend(); oscInpIterator != end; ++oscInpIterator) {
//            if (sortOrder.contains(oscInpIterator.value().sensorType)) {
//                sortMap[oscInpIterator.value().sensorType][oscInpIterator.value().sensorIndex] = oscInpIterator.key();
//            }
//        }

//        if (sortByType) {
//            for(const SensType &_typ: sortOrder) {
//                for(const QByteArray &osc: sortMap[_typ]) {
//                    if(osc != "") {
//                        sortedKeys.append(osc);
//                    }
//                }
//            }
//        }
//        else {
//            for(int i = 0; i < inputHandler->m_nSensors; ++i) {
//                for(const SensType &_typ: sortOrder) {
//                    if(i < sortMap[_typ].size()) {
//                        if(sortMap[_typ].at(i) != "") {
//                            sortedKeys.append(sortMap[_typ].at(i));
//                        }
//                    }
//                }
//            }
//        }
        return sortedKeys;
    }
};

#endif // DEVICEDATAINPUT_H
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

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QUdpSocket>
#include <QList>
#include <QHash>

#include "oscpp/client.hpp"
#include "oscpp/server.hpp"

#include "valuenotifierclass.h"
#include "inputvalueviewcontroller.h"

struct MotionDevice;

class DeviceDataInput : public QObject
{
    Q_OBJECT
public:
    explicit DeviceDataInput(QObject *parent = nullptr, QString identifier = "", uint16_t port = 51002, uint8_t nSensors = 2, MotionDevice* deviceDescription = nullptr);

    QString m_identifier;
    uint8_t m_nSensors;

    typedef InputValueViewController::ValueViewMode SensType ;

    const QList<SensType> allSensTypes = {SensType::Custom, SensType::Accel, SensType::Gyro, SensType::Grav, SensType::Quat, SensType::Touch};

    QMap<SensType, QList<ValueNotifierClass*>> valueNotifier;

    QList<QVector3D> acceleration;
    QList<QVector3D> gyroscope;
    QList<QVector3D> gravityVector;
    QList<QQuaternion> quaternion;
    QList<int> touch;

    typedef void (*OscHandleTypeFun)(DeviceDataInput*, int, OSCPP::Server::ArgStream);
    struct OscInputStruct {
        OscHandleTypeFun handleFunction = &DeviceDataInput::oscR_unMapped;
        int sensorIndex = 0;
        InputValueViewController::ValueViewMode sensorType;
    };

signals:

private:

    MotionDevice* m_deviceDescription;

    enum SenTyp {
        accel, gyro, grav, quat
    };

    QUdpSocket *socket;
    uint16_t m_port;

    OscInputStruct unmappedOsc;

    QHash<QString, OscInputStruct> oscHandleHash;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void setupOscInputBindings();
    void createNotifier();

    //OSC Callback functions
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

/*
 * The struct MotionDevce serves as description for inputdevices.
 * The DeviceStatusController holds a list of MotionDevice Objects
 * for management.
*/

struct MotionDevice {
    typedef InputValueViewController::ValueViewMode SensType ;

    QString deviceName;
    QString address;
    uint16_t port;
    uint16_t sendIntervall;
    bool connectStatus;
    DeviceDataInput *inputHandler;
    QHash<QString, DeviceDataInput::OscInputStruct> *inputs;

    QList<QString> getSortedInputKeys(bool sortByType=true,
                                      QList<SensType> sortOrder={SensType::Quat, SensType::Accel, SensType::Grav, SensType::Gyro, SensType::Touch, SensType::Custom})
    {
        QList<QString> sortedKeys;
        QMap<SensType, QList<QString>> sortMap;
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

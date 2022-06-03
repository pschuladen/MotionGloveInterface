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

#include <QUdpSocket>
#include <QList>
#include <QHash>

#include "oscpp/client.hpp"
#include "oscpp/server.hpp"

#include "valuenotifierclass.h"

#include "typehelper.h"

#include "oscinputparser.h"


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

    Q_PROPERTY(QString uniqueId READ uniqueId WRITE setUniqueId NOTIFY uniqueIdChanged)


public:
    explicit OscInputDevice(QObject *parent = nullptr);
    explicit OscInputDevice(QString deviceName, QObject *parent = nullptr);

    QString m_deviceName;
    uint8_t m_nSensors;

    typedef TypeHelper::SensorType SensType ;
    const QList<SensType> allSensTypes = {SensType::Custom, SensType::Accel, SensType::Gyro, SensType::Grav, SensType::RotQuat, SensType::Touch};

    ValueNotifierClass *getNotifier(SensType sensType, int sensIndex, int subIndex);
    //

    QMap<QByteArray, OscInputParser*> oscInputParser;
    ValueNotifierClass* getNotifierForOsc(QByteArray oscAddress) const;

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

    const QString &uniqueId() const;
    void setUniqueId(const QString &newUniqueId);

signals:

    void deviceNameChanged();
    void nSensorsChanged();
    void rcvPortChanged();
    void sendPongChanged();
    void deviceAddressStrChanged();
    void sendIntervallChanged();
    void deviceReadyChanged();
    void devicePortChanged();

    void sendSensorStructList(const QString myId, const QList<OscInputDevice::OscSensorInputStruct>);
    void requestViewCreation(QString deviceName);

    void uniqueIdChanged();

public slots:

    void viewWasCreated();


private:

    QUdpSocket *m_socket;
    uint16_t m_rcvPort;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void setupOscInputBindings();
    void createNotifier();


    bool m_sendPong;
    QString m_deviceAddressStr;
    quint16 m_sendIntervall;
    bool m_deviceReady;
    quint16 m_devicePort;

    QHostAddress m_deviceIpAddress;
    bool readyToPong();
    void sendPongMessage();

    QString m_uniqueId;
};

/*
 * The struct MotionDevce serves as description for inputdevices.
 * The DeviceStatusController holds a list of MotionDevice Objects
 * for management.
*/

//    QList<QByteArray> getSortedInputKeys(bool sortByType=true,
//                                      const QList<SensType> sortOrder={SensType::RotQuat, SensType::Accel, SensType::Grav, SensType::Gyro, SensType::Touch, SensType::Custom})
//    {
//        QList<QByteArray> sortedKeys;
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
//        return sortedKeys;
//    }


#endif // DEVICEDATAINPUT_H

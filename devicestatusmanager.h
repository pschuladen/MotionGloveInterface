#ifndef DEVICESTATUSMANAGER_H
#define DEVICESTATUSMANAGER_H

/*
 * Devicestatus controller is a singleton.
 * always call this object through main_devicestatus::Instance()
 *
 * It opens udp-port 55600
 * glove-Devices can register themselves with
 * < /glove/ping name 127.0.0.1 55102 50 >
 * < /glove/ping [deviceName] [deviceAddress] [portDevicesListens] [optional:sensorSendRate] >
 *
 * For new Devices the DeviceStatusController create a new DeviceDataInput object, intialises
 * the creation of views and can answer to /glove/ping -messages with a /pong message if
 * the connection is set to true (by the UI).
*/

#include <QObject>
#include <QUdpSocket>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlProperty>
#include <QQuickWindow>

#include "oscpp/server.hpp"
#include "oscpp/client.hpp"

#include "oscinputdevice.h"

class DeviceStatusManager : public QObject
{
    Q_OBJECT


public:
    explicit DeviceStatusManager(QObject *parent = nullptr);

    void setEngine(QQmlApplicationEngine *engine);

    QMap<QByteArray, OscInputDevice*> oscInputDevices;

signals:

    void receivedNewDevice(MotionDevice *motionDevice);
    void newOscInputDevice(QString name, OscInputDevice *newDevice);

public slots:

private:
    QUdpSocket *socket;

    uint16_t m_port;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    void createNewMotionDevice(QByteArray name, OSCPP::Server::ArgStream msgArgs);

};


#endif // DEVICESTATUSMANAGER_H

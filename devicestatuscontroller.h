#ifndef DEVICESTATUSCONTROLLER_H
#define DEVICESTATUSCONTROLLER_H

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

#include "devicedatainput.h"
#include "Singleton.h"
#include "mainbackend.h"

//struct MotionDevice;

class DeviceStatusController : public QObject
{
    Q_OBJECT


public:
    explicit DeviceStatusController(QObject *parent = nullptr);

    void setEngine(QQmlApplicationEngine *engine);

signals:
    void receivedPing();
    void receivedNewDevice(MotionDevice *motionDevice);

public slots:
    void setConnectStatus(bool connect, QString device);

private:
    QUdpSocket *socket;

    uint16_t m_port;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

    QHash<QString, MotionDevice> discoveredDevices;

    void pingBackToDevice(QString deviceName);
};

//Singleton definition always call this object through main_devicestatus::Instance();
typedef Singleton<DeviceStatusController> main_devicestatus ;

#endif // DEVICESTATUSCONTROLLER_H

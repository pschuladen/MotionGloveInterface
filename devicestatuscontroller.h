#ifndef DEVICESTATUSCONTROLLER_H
#define DEVICESTATUSCONTROLLER_H

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
#include "MotionDeviceStruct.h"

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

typedef Singleton<DeviceStatusController> main_devicestatus ;

#endif // DEVICESTATUSCONTROLLER_H

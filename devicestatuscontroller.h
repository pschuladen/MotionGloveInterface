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

#include "mainbackend.h"

class DeviceStatusController : public QObject
{
    Q_OBJECT


public:
    explicit DeviceStatusController(QObject *parent = nullptr);

    void setEngine(QQmlApplicationEngine *engine);

    struct MotionDevice {
        QString deviceName;
        QString address;
        uint16_t port;
        uint16_t sendIntervall;
        bool connectStatus;
        DeviceDataInput *inputHandler;
    };


signals:
    void receivedPing();
    void receivedNewDevice(MotionDevice *motionDevice);//QString deviceName, DeviceDataInput *inputHandler);

public slots:
    void setConnectStatus(bool connect, QString device);

private:
    QUdpSocket *socket;

    QQmlApplicationEngine *engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;

    uint16_t m_port;

    void readIncomingUdpData();
    void handleOscPacket(const OSCPP::Server::Packet &packet);
    void handleOscMessage(const OSCPP::Server::Message &message);

//    QList<QString> discoveredDevices;
    QHash<QString, MotionDevice> discoveredDevices;

    void createDeviceStatus(QString deviceName);

    void pingBackToDevice(QString deviceName);


};

#endif // DEVICESTATUSCONTROLLER_H
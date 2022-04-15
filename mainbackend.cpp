#include "mainbackend.h"
#include <QDebug>

MainBackend::MainBackend(QObject *parent)
    : QObject{parent}
{
    qInfo() << this->objectName();
}


void MainBackend::setEngine(QQmlApplicationEngine *engine)
{
    this->engine = engine;
    this->mainWindow = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    this->deviceStatusView = mainWindow->findChild<QQuickItem*>("discoveredDevicesView");
}

void MainBackend::createInputStatusView(DeviceStatusController::MotionDevice *motionDevice)
{
    QQmlComponent newDevice(engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/DeviceStatusView.qml")));

    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDevice.create());
    newDeviceItem->setProperty("deviceName", motionDevice->deviceName);
    newDeviceItem->setParentItem(deviceStatusView);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));

    connect(newDeviceItem, SIGNAL(connectButtonChanged(bool,QString)), this, SLOT(setConnectStatus(bool,QString)));
}

void MainBackend::createNewInputViews(DeviceStatusController::MotionDevice *motionDevice)
{

}

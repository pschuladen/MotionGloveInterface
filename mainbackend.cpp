#include "mainbackend.h"
#include <QDebug>

MainBackend::MainBackend(QObject *parent)
    : QObject{parent}
{
    qInfo() << this->objectName();
}


void MainBackend::setEngine(QQmlApplicationEngine *engine)
{
    this->m_engine = engine;
    mainWindow = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
    deviceStatusView = mainWindow->findChild<QQuickItem*>("discoveredDevicesView");
    inputDevicesSidebarView = mainWindow->findChild<QQuickItem*>("inputDevicesView");
}

void MainBackend::createMotionInputDeviceView(MotionDevice *motionDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));
//    qInfo() << newDeviceComponent.errors();
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.create());
    newDeviceItem->setProperty("identifier", motionDevice->deviceName);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));
    newDeviceItem->setParentItem(inputDevicesSidebarView);
    inputDevices.insert(motionDevice->deviceName, newDeviceItem);
    connect(newDeviceItem, SIGNAL(connectButtonChanged(bool,QString)), main_devicestatus::Instance(), SLOT(setConnectStatus(bool,QString)));
}

void MainBackend::createValueInputViewsForDevice(MotionDevice *motionDevice)
{
    QQuickItem *parentForValueView = inputDevices.value(motionDevice->deviceName, new QQuickItem())->findChild<QQuickItem*>("sensorViewContainer");
    if(!parentForValueView) {
        qWarning() << "no suitable view for inputs found";
        return;
    }
    QQmlComponent newInput(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorValuesView.qml")));
    foreach(const QString &osc, motionDevice->getSortedInputKeys(false)) {//inputs->keys()) { //
        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", motionDevice->inputs->value(osc).sensorType},
                                                                                                   {"identifier", osc}}));
        newValDevice->setParentItem(parentForValueView);
        ValueViewBackend *viewBackend = newValDevice->findChild<ValueViewBackend*>("valuebackend");
        if (!viewBackend) {
            qWarning() << "no backend found";
            return;
        }
        sensType sTyp = motionDevice->inputs->value(osc).sensorType;
        int senIdx = motionDevice->inputs->value(osc).sensorIndex;

        if(sTyp == sensType::Accel || sTyp == sensType::Gyro || sTyp==sensType::Grav) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::vectorChanged, viewBackend, &ValueViewBackend::vectorChanged);
        }
        else if(sTyp == sensType::Quat) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::quatChanged, viewBackend, &ValueViewBackend::quatChanged);
        }
        else if(sTyp == sensType::Touch) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::touchChanged, viewBackend, &ValueViewBackend::touchChanged);
        }
        else {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::valuesChanged, viewBackend, &ValueViewBackend::valuesChanged);
        }
    }
}

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);

}

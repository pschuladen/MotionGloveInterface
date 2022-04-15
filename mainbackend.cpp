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

void MainBackend::createDeviceStatusView(MotionDevice *motionDevice)
{
    QQmlComponent newDevice(engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/DeviceStatusView.qml")));

    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDevice.create());
    newDeviceItem->setProperty("deviceName", motionDevice->deviceName);
    newDeviceItem->setParentItem(deviceStatusView);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));

    connect(newDeviceItem, SIGNAL(connectButtonChanged(bool,QString)), main_devicestatus::Instance(), SLOT(setConnectStatus(bool,QString)));
}

void MainBackend::createSensorInputViews(MotionDevice *motionDevice)
{
    QString containerName = QString("%1-inputContainer").arg(motionDevice->deviceName);
    QQuickItem *sensorViewContainer = sensorInputContainer.value(containerName,
                                                                 createSensorViewContainer(motionDevice));
    if(!sensorViewContainer) return;
//    if(!sensorInputContainer.contains(containerName)) {
//        sensorViewContainer = createSensorViewContainer(motionDevice);

//    }
//    else {
//        sensorViewContainer = sensorInputContainer.value(containerName);
//    }
    createSensorInputView(sensorViewContainer, quat, motionDevice);
    createSensorInputView(sensorViewContainer, accel, motionDevice);
    createSensorInputView(sensorViewContainer, gyro, motionDevice);
    createSensorInputView(sensorViewContainer, grav, motionDevice);
}

void MainBackend::createSensorInputView(QQuickItem *parentView, SenTyp typ, MotionDevice *motionDevice)
{
    DeviceDataInput *inputHandler = motionDevice->inputHandler;
    QUrl qmlpath;
    if (typ != quat) {qmlpath = QUrl(QStringLiteral("qrc:/MotionGloveInterface/VectorView.qml"));}
    else if (typ == quat) {qmlpath = QUrl(QStringLiteral("qrc:/MotionGloveInterface/QuatView.qml"));}

    for(int i = 0; i < inputHandler->m_nSensors; i++) {
        QQmlComponent newSensorView(engine, qmlpath);//(QStringLiteral("qrc:/MotionGloveInterface/VectorView.qml")));
        QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newSensorView.create());
        VectorViewBackend *conBackend;// = newDeviceItem->findChild<VectorViewBackend*>("vectBackend");
        QuatViewBackend *quatBackend;

        QString sensName;
        switch (typ) {
        case accel:
            conBackend = newDeviceItem->findChild<VectorViewBackend*>("vectBackend");
            connect(inputHandler->accelNotify.at(i), &ValueNotifierClass::vectorChanged, conBackend, &VectorViewBackend::vectorChanged);
//            conBackend->setVectPointer(&this->acceleration[i]);

            sensName = QString("/%2/acc/%1").arg(i).arg(motionDevice->deviceName);
            break;
        case gyro:
            conBackend = newDeviceItem->findChild<VectorViewBackend*>("vectBackend");
            connect(inputHandler->gyrosNotify.at(i), &ValueNotifierClass::vectorChanged, conBackend, &VectorViewBackend::vectorChanged);
//            conBackend->setVectPointer(&this->gyroscope[i]);
//            this->gyrosView[i] = conBackend;
            sensName = QString("/%2/gyr/%1").arg(i).arg(motionDevice->deviceName);
            break;
        case grav:
            conBackend = newDeviceItem->findChild<VectorViewBackend*>("vectBackend");
            connect(inputHandler->gravityNotify.at(i), &ValueNotifierClass::vectorChanged, conBackend, &VectorViewBackend::vectorChanged);
//            conBackend->setVectPointer(&this->gravityVector[i]);
//            this->gravityView[i] = conBackend;
            sensName = QString("/%2/gravity/%1").arg(i).arg(motionDevice->deviceName);
            break;
        case quat:
            quatBackend = newDeviceItem->findChild<QuatViewBackend*>("quatBackend");
            connect(inputHandler->quaternionNotify.at(i), &ValueNotifierClass::quatChanged, quatBackend, &QuatViewBackend::quatChanged);
//            connect(&inputHandler->quaternionNotify.at(i), &ValueNotifierClass::quatChanged, conBackend, &QuatViewBackend::quatChanged);

//            quatBackend->setQuatPointer(&this->quaternion[i]);
//            this->quaternionView[i] = quatBackend;
            sensName = QString("/%2/quat/%1").arg(i).arg(motionDevice->deviceName);
            break;
        case touch:

            break;
        }
        newDeviceItem->setProperty("sensorName", sensName);
        newDeviceItem->setParentItem(parentView);
    }
}

QQuickItem* MainBackend::createSensorViewContainer(MotionDevice * motionDevice)
{
    QQuickItem *parentView = mainWindow->findChild<QQuickItem*>("sensorInputView");
    QUrl qmlpath = QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml"));
    QQmlComponent newSensorContainer(engine, qmlpath);
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newSensorContainer.create());
    newDeviceItem->setObjectName(QString("%1-inputContainer").arg(motionDevice->deviceName));
    newDeviceItem->setParentItem(parentView);
    QQuickItem *containerObject = newDeviceItem->findChild<QQuickItem*>("objectContainer");
    if(containerObject) {
        //        sensorViewContainer = containerObject;
        return containerObject;
    }
    else {
        qWarning() << "could not Create view container for SensorInput";
        return nullptr;
    }
}



void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
    createDeviceStatusView(motionDevice);
    createSensorInputViews(motionDevice);
}

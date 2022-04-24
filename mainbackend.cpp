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

void MainBackend::createDeviceStatusView(MotionDevice *motionDevice)
{
    QQmlComponent newDevice(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/DeviceStatusView.qml")));

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
        QQmlComponent newSensorView(m_engine, qmlpath);//(QStringLiteral("qrc:/MotionGloveInterface/VectorView.qml")));

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

void MainBackend::createMotionInputDeviceView(MotionDevice *motionDevice)
{


    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));
    qInfo() << newDeviceComponent.errors();
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.create());
    newDeviceItem->setProperty("identifier", motionDevice->deviceName);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));
    newDeviceItem->setParentItem(inputDevicesSidebarView);
    qInfo() << "new Device in engine" << m_engine->objectName();
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

QQuickItem* MainBackend::createSensorViewContainer(MotionDevice * motionDevice)
{
    QQuickItem *parentView = mainWindow->findChild<QQuickItem*>("sensorInputView");
    QUrl qmlpath = QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml"));
    QQmlComponent newSensorContainer(m_engine, qmlpath);
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
    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);
//    createDeviceStatusView(motionDevice);
//    createSensorInputViews(motionDevice);
}

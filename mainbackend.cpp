#include "mainbackend.h"
#include <QDebug>

MainBackend::MainBackend(QObject *parent)
    : QObject{parent}
{
    qInfo() << "mainbackend object name" << this->objectName();
    qInfo() << "mainbackend thread" << this->thread();

}


void MainBackend::setQmlEngine(QQmlApplicationEngine *engine)
{
    m_engine = engine;

    mainWindow = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));

    deviceStatusView = mainWindow->findChild<QQuickItem*>("discoveredDevicesView");
    inputDevicesSidebarView = mainWindow->findChild<QQuickItem*>("inputDevicesView");
    processingGraphView = mainWindow->findChild<QQuickItem*>("processingGraphView");
    outputDevcesSidebarView = mainWindow->findChild<QQuickItem*>("outputDevicesView");
}

void MainBackend::initialSetup()
{
    DeviceStatusManager *_dma = new DeviceStatusManager();
    deviceManager.append(_dma);
//    connect(_dma, &DeviceStatusManager::receivedNewDevice, this, &MainBackend::createNewInputViews);
    connect(_dma, &DeviceStatusManager::newOscInputDevice, this, &MainBackend::createNewMotionDeviceView);
}

void MainBackend::createNewProcessingView(ProcessNodeController::ProcessingType type, QPoint atPosition)//float posX, float posY)
{
    qInfo() << "now i should creat a view" << type << "at" << atPosition.x() << atPosition.y();

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_ScaleView.qml")));
    qInfo() << "errorstring" << newProcessingComponent.errorString();
    QString name = QString("proc-%1").arg(processingNodes.size());
//    ProcessNodeController newC; //= new ProcessNodeController();
    QQuickItem *newProcessingItem = qobject_cast<QQuickItem*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", name},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNode *viewBackend = newProcessingItem->findChild<ProcessNode*>(name+"-viewcontroller");
    newProcessingItem->setParentItem(processingGraphView);
    qInfo() << viewBackend << viewBackend->parent();

    ProcessingNode newNode;

    newNode.controller = viewBackend;
    newNode.qmlView = newProcessingItem;
    processingNodes.insert(name, newNode);//ProcessingNode(newProcessCalculus, viewBackend));
    allConnectableObjects.insert(name, ConnectableObject(viewBackend, newProcessingItem, TypeHelper::Process));

}

bool MainBackend::connectionRequest(const QString senderNodeId,int sourceValueIdx, QQuickItem *senderConnector,
                                    const QString receiverNodeId, int targetValueIdx, QQuickItem *receiverConnector,
                                    TypeHelper::ValueType valueType)
{
    qInfo() << "Connection request" << senderNodeId << sourceValueIdx << senderConnector <<
               receiverNodeId << targetValueIdx << receiverConnector <<
               valueType;

    if(allConnectableObjects.contains(senderNodeId) && allConnectableObjects.contains(receiverNodeId)) {
        //TODO: create ConnectionView/Object
        ValueNotifierClass *sendingNotifier = allConnectableObjects[senderNodeId].notifier->getNotifier(sourceValueIdx);
        ValueNotifierClass *receivingNotifier = allConnectableObjects[receiverNodeId].notifier->getNotifier(targetValueIdx);
        if (sendingNotifier != nullptr && receivingNotifier != nullptr) {
            return receivingNotifier->newConnectionFromSender(sendingNotifier, valueType);
        }
        else {
            qWarning() << "no sending notifier found!";
            return false;
        }
    }
    else {
        qInfo() << "no objects found for connection";
        return false;
    }
}

bool MainBackend::createOscOutputDevice()
{
    qInfo() << "create new osc output device";

    QString uniqueID = QString("oscout-%1").arg(oscOutputDevices.size());
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OscOutputDeviceView.qml")));
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", uniqueID}}));
    OscOutputViewController *viewController = newDeviceItem->findChild<OscOutputViewController*>(OscOutputViewController::standardObjectName());

    OscOutputDevice *oscDevice = new OscOutputDevice(viewController, nullptr);

    if(viewController == nullptr) qInfo() << "no ViewController found!!";
    if(oscDevice == nullptr) qInfo() << "oscDevice Not Created found!!";
    if(newDeviceItem == nullptr) qInfo() << "no VIEW found!!";

    if(oscDevice != nullptr && viewController != nullptr && newDeviceItem != nullptr) {

        oscOutputDevices.insert(uniqueID, OscOutDeviceStruct(oscDevice, viewController, newDeviceItem));
        allConnectableObjects.insert(uniqueID, ConnectableObject(oscDevice, newDeviceItem, TypeHelper::Output));
        newDeviceItem->setParentItem(outputDevcesSidebarView);
        return true;
    }
    else {

        delete oscDevice;
        delete newDeviceItem;
        qWarning() << "Something went wrong while creating osc device";
        return false;
    }
}

void MainBackend::createMotionInputDeviceView(MotionDevice *motionDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));
//    qInfo() << newDeviceComponent.errors();
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.create());
    newDeviceItem->setProperty("identifier", motionDevice->deviceName);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));
    newDeviceItem->setParentItem(inputDevicesSidebarView);
    inputDeviceViews.insert(motionDevice->deviceName, newDeviceItem);
    connect(newDeviceItem, SIGNAL(connectButtonChanged(bool,QString)), deviceManager.at(0), SLOT(setConnectStatus(bool,QString)));
}

void MainBackend::createValueInputViewsForDevice(MotionDevice *motionDevice)
{
//    QQuickItem *parentForValueView = inputDeviceViews.value(motionDevice->deviceName, new QQuickItem())->findChild<QQuickItem*>("sensorViewContainer");
//    if(!parentForValueView) {
//        qWarning() << "no suitable view for inputs found";
//        return;
//    }
//    QQmlComponent newInput(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorValuesView.qml")));
//    foreach(const QByteArray &osc, motionDevice->getSortedInputKeys(true)) {//inputs->keys()) { //
//        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", motionDevice->inputs->value(osc).sensorType},
//                                                                                                   {"identifier", osc},
//                                                                                                   {"sourceObjectId", motionDevice->deviceName}}));
//        newValDevice->setParentItem(parentForValueView);
//        InputValueViewController *viewBackend = newValDevice->findChild<InputValueViewController*>("valuebackend");
//        if (!viewBackend) {
//            qWarning() << "no backend found";
//            return;
//        }
//        TypeHelper::SensorType sTyp = motionDevice->inputs->value(osc).sensorType;
//        TypeHelper::ValueType vTyp = TypeHelper::valueTypeForSensor(sTyp);
//        int senIdx = motionDevice->inputs->value(osc).sensorIndex;

//        ValueNotifierClass::connectValueTypeSignalToSlot(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx),
//                                                         viewBackend, vTyp);

//        allConnectableObjects.insert(osc, ConnectableObject(motionDevice->getValueNotifierForPath(osc), newValDevice, TypeHelper::Input));
//    }
}

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{


//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);
}

void MainBackend::createNewMotionDeviceView(QString name, OscInputDevice *newDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));
//    qInfo() << newDeviceComponent.errors();
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.createWithInitialProperties({{"identifier", name}}));
    OscInputViewController *viewCon = newDeviceItem->findChild<OscInputViewController*>("oscviewcontroller");

    if(viewCon == nullptr) {
        qWarning() << "no osc input viewcontroller found! returning..";
        return;
    }

    newDeviceItem->setObjectName(QString(name+"-view"));
    oscInputDevices.insert(name, OscInDeviceStruct(newDevice, viewCon, newDeviceItem));
    connect(viewCon, &OscInputViewController::connectDeviceChanged, newDevice, &OscInputDevice::setSendPong);

    newDeviceItem->setParentItem(inputDevicesSidebarView);

    connect(this, &MainBackend::inputViewReady, newDevice, &OscInputDevice::viewWasCreated, Qt::SingleShotConnection);
    connect(newDevice, &OscInputDevice::sendSensorStructList, this, &MainBackend::createSensorViewsForMotionDevice, Qt::SingleShotConnection);
    emit inputViewReady();
}

void MainBackend::createSensorViewsForMotionDevice(const QString sendername, const QList<OscInputDevice::OscSensorInputStruct> sensors)
{
    qInfo() << "creating views" << sensors.size();
    if(!oscInputDevices.contains(sendername)) {
        qWarning() << "no parentview found for sender";
    }
    QQmlComponent newInput(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorValuesView.qml")));
    QQuickItem *parentForSensorView = oscInputDevices.value(sendername).view->findChild<QQuickItem*>("sensorViewContainer");

    if(parentForSensorView == nullptr) {
        qWarning() << "no container view for osc device" << sendername << "found!";
    }

//    for(const OscInputDevice::OscSensorInputStruct sensor: sensors) {
    for(QList<OscInputDevice::OscSensorInputStruct>::const_iterator sensor = sensors.cbegin(), end = sensors.cend(); sensor != end; ++sensor) {

        qInfo() << "Sensor" << sensor->identifier << sensor->sensType;

        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", sensor->sensType},
                                                                                                   {"identifier", sensor->identifier},
                                                                                                   {"sourceObjectId", sendername}}));
        InputValueViewController *viewBackend = newValDevice->findChild<InputValueViewController*>("valuebackend");
        viewBackend->setSourceNotifier(sensor->oscHandler);
        newValDevice->setParentItem(parentForSensorView);

    }

//    for(QList<OscInputDevice::OscSensorInputStruct>::const_iterator sensor = sensors.cbegin(), end = sensors.cend(); sensor != end; ++sensor)
//    }

//    for(OscInputDevice::OscSensorInputStruct _sensor: sensors) {

//    }

}

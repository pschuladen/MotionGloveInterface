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
    connect(_dma, &DeviceStatusManager::receivedNewDevice, this, &MainBackend::createNewInputViews);


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

    QString uniqueID = QString("oscout-%1").arg(oscDevices.size());
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OscOutputDeviceView.qml")));
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", uniqueID}}));
    OscViewController *viewController = newDeviceItem->findChild<OscViewController*>(OscViewController::standardObjectName());

    OscOutputDevice *oscDevice = new OscOutputDevice(viewController, nullptr);

    if(viewController == nullptr) qInfo() << "no ViewController found!!";
    if(oscDevice == nullptr) qInfo() << "oscDevice Not Created found!!";
    if(newDeviceItem == nullptr) qInfo() << "no VIEW found!!";

    if(oscDevice != nullptr && viewController != nullptr && newDeviceItem != nullptr) {

        oscDevices.insert(uniqueID, OscDeviceStruct(oscDevice, viewController, newDeviceItem));
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
    QQuickItem *parentForValueView = inputDeviceViews.value(motionDevice->deviceName, new QQuickItem())->findChild<QQuickItem*>("sensorViewContainer");
    if(!parentForValueView) {
        qWarning() << "no suitable view for inputs found";
        return;
    }
    QQmlComponent newInput(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorValuesView.qml")));
    foreach(const QString &osc, motionDevice->getSortedInputKeys(true)) {//inputs->keys()) { //
        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", motionDevice->inputs->value(osc).sensorType},
                                                                                                   {"identifier", osc},
                                                                                                   {"sourceObjectId", motionDevice->deviceName}}));
        newValDevice->setParentItem(parentForValueView);
        InputValueViewController *viewBackend = newValDevice->findChild<InputValueViewController*>("valuebackend");
        if (!viewBackend) {
            qWarning() << "no backend found";
            return;
        }
        TypeHelper::SensorType sTyp = motionDevice->inputs->value(osc).sensorType;
        TypeHelper::ValueType vTyp = TypeHelper::valueTypeForSensor(sTyp);
        int senIdx = motionDevice->inputs->value(osc).sensorIndex;

        ValueNotifierClass::connectValueTypeSignalToSlot(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx),
                                                         viewBackend, vTyp);

        allConnectableObjects.insert(osc, ConnectableObject(motionDevice->getValueNotifierForPath(osc), newValDevice, TypeHelper::Input));
    }
}

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);
}

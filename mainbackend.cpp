#include "mainbackend.h"
#include <QDebug>

MainBackend::MainBackend(QObject *parent)
    : QObject{parent}
{
//    qInfo() << "mainbackend object name" << this->objectName();
//    qInfo() << "mainbackend thread" << this->thread();
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
    QList<ThreadRole> _threadList = {Main, NetIn, NetOut, Process, Audio};
    QList<QString> _threadNames = {"Main-Thread", "Net-In", "Net-Out", "Process-Thread", "Audio-Thread"};
//    foreach(const ThreadRole &_thread, _threadList) {
    for(int i=0; i<_threadList.size(); i++)
    {
        QThread *_newThread = new QThread();
        _newThread->setObjectName(_threadNames.at(i));
        threads.insert(_threadList.at(i), _newThread);
        _newThread->start();
    }

    _dma->moveToThread(threads.value(NetIn));
    connect(_dma, &DeviceStatusManager::newOscInputDevice, this, &MainBackend::createNewMotionDeviceView);
//    connect(_dma, &DeviceStatusManager::receivedNewDevice, this, &MainBackend::createNewInputViews, Qt::QueuedConnection);

//    connect(threads.value(NetIn), &QThread::finished, this, &MainBackend::test_threadAlive);
//    threads.value(NetIn)->start();
}

bool MainBackend::createNewProcessingView(int type, QPoint atPosition)//float posX, float posY)
{

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_ScaleView.qml")));
    if(newProcessingComponent.isError()) {
        qWarning() << "WARNING! New Processing Component:" << newProcessingComponent.errorString();
        return false;
    }
    QString name = QString("proc-%1").arg(processingNodes.size());
    QQuickItem *newProcessingView = qobject_cast<QQuickItem*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", name},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNode *viewBackend = newProcessingView->findChild<ProcessNode*>(name+"-viewcontroller");
    newProcessingView->setParentItem(processingGraphView);

    processingNodes.insert(name, ProcessingNode(viewBackend, newProcessingView));
    allConnectableObjects.insert(name, ConnectableObject(viewBackend, newProcessingView, TypeHelper::Process));
    connect(viewBackend, &ProcessNode::newSubprocessorWasCreated, this, &MainBackend::moveSubprocessorToProcessThread);

    return true;
}

bool MainBackend::connectionRequest(const QString senderNodeId,int sourceValueIdx, QQuickItem *senderConnector,
                                    const QString receiverNodeId, int targetValueIdx, QQuickItem *receiverConnector,
                                    TypeHelper::ValueType valueType)
{
    if(!allConnectableObjects.contains(senderNodeId) || !allConnectableObjects.contains(receiverNodeId)) {
        qWarning() << "Connection request" << senderNodeId << sourceValueIdx << senderConnector <<
                   receiverNodeId << targetValueIdx << receiverConnector <<
                   valueType;
        qWarning() << "no objects found for connection";
        return false;
    }
    //TODO: create ConnectionView/Object
    ValueNotifierClass *sendingNotifier = allConnectableObjects[senderNodeId].notifier->getNotifier(sourceValueIdx);
    ValueNotifierClass *receivingNotifier = allConnectableObjects[receiverNodeId].notifier->getNotifier(targetValueIdx);
//    qDebug() << allConnectableObjects[senderNodeId].notifier << sendingNotifier;
//    qDebug() << valueType;

    if (sendingNotifier == nullptr || receivingNotifier == nullptr) {
        qWarning() << "Connection request" << senderNodeId << sourceValueIdx << senderConnector <<
                   receiverNodeId << targetValueIdx << receiverConnector <<
                   valueType;
        qWarning() << "no sending and/or receiving notifier found!";
        return false;
    }

    return receivingNotifier->newConnectionFromSender(sendingNotifier, valueType);

}

bool MainBackend::createOscOutputDevice()
{
    QString uniqueID = QString("oscout-%1").arg(oscOutputDevices.size());
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OscOutputDeviceView.qml")));

    if(newDeviceComponent.isError()) {
        qWarning() << "osc view errors while creating oscOutputDevice" << newDeviceComponent.errorString();
        return false;
    }

    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", uniqueID}}));
    OscOutputViewController *viewController = newDeviceItem->findChild<OscOutputViewController*>(OscOutputViewController::standardObjectName());

    if(viewController == nullptr) {
        qWarning() << "no ViewController found!!";
        return false;
    }

    OscOutputDevice *oscDevice = new OscOutputDevice(viewController, nullptr);

    if(oscDevice == nullptr || viewController == nullptr || newDeviceItem == nullptr) {
        if(oscDevice == nullptr) qWarning() << "oscDevice Not Created found!!";
        if(newDeviceItem == nullptr) qWarning() << "no VIEW found!!";

        oscDevice->deleteLater();
        newDeviceItem->deleteLater();
        qWarning() << "Something went wrong while creating osc device";
        return false;
    }

    oscOutputDevices.insert(uniqueID, OscOutDeviceStruct(oscDevice, viewController, newDeviceItem));
    allConnectableObjects.insert(uniqueID, ConnectableObject(oscDevice, newDeviceItem, TypeHelper::Output));
    newDeviceItem->setParentItem(outputDevcesSidebarView);
    oscDevice->moveToThread(threads.value(NetOut));
    return true;
}

MainBackend::ThreadRole MainBackend::threadRoleForNodeType(TypeHelper::NodeType nodeType)
{
    switch(nodeType) {
    case TypeHelper::Audiocontroller:
        return Subthread;
    case TypeHelper::Generic:
        return Subthread;
    case TypeHelper::Gui:
        return Main;
    case TypeHelper::Input:
        return NetIn;
    case TypeHelper::Process:
        return Process;
    case TypeHelper::Output:
        return NetOut;
    case TypeHelper::Audio:
        return Audio;
    }
}


void MainBackend::createNewMotionDeviceView(QString name, OscInputDevice *newDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));

    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating motionInputDevice" << newDeviceComponent.errorString();
        return;
    }

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
    if(!oscInputDevices.contains(sendername)) {
        qWarning() << "no parentview found for sender";
        return;
    }
    QQmlComponent newInput(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorValuesView.qml")));

    if(newInput.isError()) {
        qWarning() << "ERROR! while creating motionInputDevice" << newInput.errorString();
        return;
    }

    QQuickItem *parentForSensorView = oscInputDevices.value(sendername).view->findChild<QQuickItem*>("sensorViewContainer");

    if(parentForSensorView == nullptr) {
        qWarning() << "no container view for osc device" << sendername << "found!";
        return;
    }

    for(QList<OscInputDevice::OscSensorInputStruct>::const_iterator sensor = sensors.cbegin(), end = sensors.cend(); sensor != end; ++sensor) {

        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", sensor->sensType},
                                                                                                   {"identifier", sensor->identifier},
                                                                                                   {"sourceObjectId", sendername}}));
        InputValueViewController *viewBackend = newValDevice->findChild<InputValueViewController*>("valuebackend");
        viewBackend->setSourceNotifier(sensor->oscHandler);
        newValDevice->setParentItem(parentForSensorView);
        allConnectableObjects.insert(sensor->identifier, ConnectableObject(sensor->oscHandler, newValDevice, TypeHelper::Input));
    }
}

void MainBackend::moveObjectToThread(QObject *obj, TypeHelper::NodeType nodeType)
{
    obj->moveToThread(threads.value(threadRoleForNodeType(nodeType)));
}

void MainBackend::moveSubprocessorToProcessThread(ProcessNode *processor)
{
    moveObjectToThread(processor, TypeHelper::Process);
}

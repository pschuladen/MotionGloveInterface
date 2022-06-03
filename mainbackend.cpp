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
    QList<ThreadRole> _threadList = {Main, NetIn, NetOut, Process};//, Audio};
    QList<QString> _threadNames = {"Main-Thread", "Net-In", "Net-Out", "Process-Thread"};//, "Audio-Thread"};
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
    connect(this, &MainBackend::deviceWithNameCreated, _dma, &DeviceStatusManager::setIdForNamedDevice);

//    connect(_dma, &DeviceStatusManager::receivedNewDevice, this, &MainBackend::createNewInputViews, Qt::QueuedConnection);

//    connect(threads.value(NetIn), &QThread::finished, this, &MainBackend::test_threadAlive);
//    threads.value(NetIn)->start();
}

bool MainBackend::createProcessingNodeDrop(QPoint atPosition, int type)//float posX, float posY)
{

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_ScaleView.qml")));
    if(newProcessingComponent.isError()) {
        qWarning() << "WARNING! New Processing Component:" << newProcessingComponent.errorString();
        return false;
    }
    QString _id = createUniqueId(TypeHelper::Process); //name = QString("proc-%1").arg(processingNodes.size());
    QQuickItem *newProcessingView = qobject_cast<QQuickItem*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", _id},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNode *viewBackend = newProcessingView->findChild<ProcessNode*>(_id+"-viewcontroller");
    newProcessingView->setParentItem(processingGraphView);

    processingNodes.insert(_id.toUtf8(), ProcessingNode(viewBackend, newProcessingView));
    allConnectableObjects.insert(_id.toUtf8(), ConnectableObject(viewBackend, newProcessingView, TypeHelper::Process));
    connect(viewBackend, &ProcessNode::newSubprocessorWasCreated, this, &MainBackend::moveSubprocessorToProcessThread);

    return true;
}

bool MainBackend::createOutputNodeDrop(QPoint atPoint, QString targetDevice, quint16 outputIndex, TypeHelper::ValueType valType, int subIdx)
{
    qInfo() << atPoint << targetDevice << outputIndex << valType << subIdx;
//    qWarning() << "create ouput node drop not implemented";
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OutputNodeView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating output node" << newDeviceComponent.errorString();
        newDeviceComponent.deleteLater();
        return false;
    }
    QString _id = createUniqueId(TypeHelper::Output);


    OutputNodeController *newOutPutNode = qobject_cast<OutputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueId", _id},
                                                                                                          {"outputAddress", oscOutputDevices.value(targetDevice).viewController->oscPaths().at(outputIndex)},
                                                                                                          {"x", atPoint.x()-30},
                                                                                                          {"y", atPoint.y()-10}}));
    if(newOutPutNode == nullptr) {
        qWarning() << "ERROR! could not create outputnode view";
        return false;
    }

    OscPacketBuilder *outputNotifier = qobject_cast<OscPacketBuilder*>(oscOutputDevices.value(targetDevice).oscSender->getNotifier(outputIndex));
    if(outputNotifier == nullptr) {
        qWarning() << "ERROR! could not find output notifier";
        return false;
    }

    connect(outputNotifier, &ValueNotifierClass::connectedValueTypeChanged, newOutPutNode, &OutputNodeController::setValueType);
    connect(outputNotifier, &OscPacketBuilder::oscAddressChanged, newOutPutNode, &OutputNodeController::setOutputAddress);
    connect(outputNotifier, &OscPacketBuilder::oscMessIdxChanged, newOutPutNode, &OutputNodeController::setTargetIdx);
    emit outputNotifier->oscMessIdxChanged(outputNotifier->oscMessIdx());
//    newOutPutNode->setTargetIdx(outputNotifier->oscMessIdx());

    connect(oscOutputDevices.value(targetDevice).viewController, &OscOutputViewController::sig_viewAtIndexHovered, newOutPutNode, &OutputNodeController::mouseAtTargetHoveredAtIdx);
    connect(newOutPutNode, &OutputNodeController::mouseHoveredOnNode, oscOutputDevices.value(targetDevice).viewController, &OscOutputViewController::viewAtIndexHovered);


    allConnectableObjects.insert(_id, ConnectableObject(outputNotifier, newOutPutNode, TypeHelper::Output));
    outputNodes.insert(_id, OutputNode(newOutPutNode, targetDevice, outputIndex));
//    oscOutputDevices[targetDevice].;

    newOutPutNode->setParentItem(processingGraphView);


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

//TODO: for now the controller (living in main thread is responsible for managing connections...
    // so no sig/slot necessary
    //    connect(this, &MainBackend::sig_connectRequestFromSender, receivingNotifier, &ValueNotifierClass::newConnectionFromSender, Qt::SingleShotConnection);
    //    emit sig_connectRequestFromSender(sendingNotifier, valueType);

    //    bool acceptsConnection = receivingNotifier->acceptsInputType(valueType);
    bool acceptsConnection = receivingNotifier->newConnectionFromSender(sendingNotifier, valueType);

    allConnections.insert(senderNodeId+"-"+receiverNodeId, ValueConnection(
                              senderNodeId, sourceValueIdx, receiverNodeId, targetValueIdx,
                              valueType));

    return acceptsConnection;
}

bool MainBackend::createOscOutputDevice()
{
    QString uniqueID = createUniqueId(TypeHelper::OscOutput);//QString("oscout-%1").arg(oscOutputDevices.size());
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
//    allConnectableObjects.insert(uniqueID, ConnectableObject(oscDevice, newDeviceItem, TypeHelper::Output));
    newDeviceItem->setParentItem(outputDevcesSidebarView);
    oscDevice->moveToThread(threads.value(NetOut));
    return true;
}


bool MainBackend::createInputNodeDrop(QPoint atPoint, QString sourceDevice, QString inputPath, TypeHelper::ValueType valType, int subIdx)
{   
//    qInfo() << "create input" << atPoint << sourceDevice << identifier << valType << subIdx;

    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/InputNodeView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating input node" << newDeviceComponent.errorString();
        newDeviceComponent.deleteLater();
        return false;
    }
    QString _id = createUniqueId(TypeHelper::Input);

    InputNodeController *inputNodeViewCtrl = qobject_cast<InputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueId", _id},
                                                                                                          {"inputPath", inputPath},
                                                                                                          {"sourceDevice", sourceDevice},
                                                                                                          {"valueType", valType},
                                                                                                          {"x", atPoint.x()-30},
                                                                                                          {"y", atPoint.y()-10}}));
    if(inputNodeViewCtrl == nullptr) {
        qWarning() << "ERROR casting inputNodeController";
        newDeviceComponent.deleteLater();
        return false;
    }

    ValueNotifierClass *inNodeNoti = new ValueNotifierClass(_id.toUtf8(), -1, valType);
    inNodeNoti->setAutoEmit(true);

    if(!ValueNotifierClass::connectValueTypeSignalToSlot(oscInputDevices.value(sourceDevice).oscReceiver->getNotifierForOsc(inputPath.toUtf8()),
                                                         inNodeNoti, valType))
    {
        inputNodeViewCtrl->deleteLater();
        inNodeNoti->deleteLater();
        newDeviceComponent.deleteLater();
        qWarning() << "ERROR! while connecting input node";
        return false;
    }

    moveObjectToThread(inNodeNoti, TypeHelper::Process);
    inputNodeViewCtrl->setInputValueController(oscInputDevices.value(sourceDevice).inputViewController.value(inputPath));

    inputNodes.insert(_id, InputNode(inputNodeViewCtrl, sourceDevice, inputPath));
    allConnectableObjects.insert(_id, ConnectableObject(inNodeNoti, inputNodeViewCtrl, TypeHelper::Input));
    inputNodeViewCtrl->setParentItem(processingGraphView);

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
        return Process;
    case TypeHelper::Process:
        return Process;
    case TypeHelper::Output:
        return Process;
    case TypeHelper::Audio:
        return Audio;
    default:
        return Subthread;
    }
}

QString MainBackend::createUniqueId(TypeHelper::NodeType nodeType)
{
    QString _tmpId = TypeHelper::getPrefixForNodetype(nodeType).append("-%1");

    QString _tmpId2;

    bool idOk = false;
    quint32 exitCounter = 0;

    while(!idOk) {
        _tmpId2 = _tmpId.arg(QRandomGenerator::global()->bounded(0, 99999));
        if(nodeType == TypeHelper::Process) {
            idOk = !processingNodes.contains(_tmpId2) && !allConnectableObjects.contains(_tmpId2);
        }
        else if(nodeType == TypeHelper::Input) {
            idOk = !inputNodes.contains(_tmpId2) && !allConnectableObjects.contains(_tmpId2);
        }
        else if(nodeType == TypeHelper::Output) {
            idOk = !outputNodes.contains(_tmpId2) && !allConnectableObjects.contains(_tmpId2);
        }
        else if(nodeType == TypeHelper::OscOutput) {
            idOk = !oscOutputDevices.contains(_tmpId2) && !allConnectableObjects.contains(_tmpId2);
        }
        else if(nodeType == TypeHelper::OscInput) {
            idOk = !oscInputDevices.contains(_tmpId2) && !allConnectableObjects.contains(_tmpId2);
        }
        else {
            idOk = !allConnectableObjects.contains(_tmpId2);
        }
        if(exitCounter++ > 90909) return "";
    }
//    qInfo() << _tmpId2;
    return _tmpId2;
}


void MainBackend::createNewMotionDeviceView(QString name, OscInputDevice *newDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));

    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating motionInputDevice" << newDeviceComponent.errorString();
        return;
    }

    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.createWithInitialProperties({{"deviceName", name}}));
    OscInputViewController *viewCon = newDeviceItem->findChild<OscInputViewController*>("oscviewcontroller");

    if(viewCon == nullptr) {
        qWarning() << "no osc input viewcontroller found! returning..";
        return;
    }

    QString _id = createUniqueId(TypeHelper::OscInput);
    newDeviceItem->setObjectName(QString(name+"-view"));
    oscInputDevices.insert(_id, OscInDeviceStruct(name, newDevice, viewCon, newDeviceItem));
    connect(viewCon, &OscInputViewController::connectDeviceChanged, newDevice, &OscInputDevice::setSendPong);

    newDeviceItem->setParentItem(inputDevicesSidebarView);

    connect(this, &MainBackend::inputViewReady, newDevice, &OscInputDevice::viewWasCreated, Qt::SingleShotConnection);
    connect(newDevice, &OscInputDevice::sendSensorStructList, this, &MainBackend::createSensorViewsForMotionDevice, Qt::SingleShotConnection);

    emit deviceWithNameCreated(name, _id);
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
    QMap<QString, InputValueViewController*> _inputViewController;

    for(QList<OscInputDevice::OscSensorInputStruct>::const_iterator sensor = sensors.cbegin(), end = sensors.cend(); sensor != end; ++sensor) {

        QQuickItem *newValDevice = qobject_cast<QQuickItem*>(newInput.createWithInitialProperties({{"viewmode", sensor->sensType},
                                                                                                   {"identifier", sensor->identifier},
                                                                                                   {"sourceObjectId", sendername},
                                                                                                   {"oscInputPath", sendername}}));

        InputValueViewController *viewBackend = newValDevice->findChild<InputValueViewController*>("valuebackend");
        oscInputDevices[sendername].inputViewController[sensor->identifier] = viewBackend;//.insert(sensor->identifier, viewBackend);
        _inputViewController.insert(sensor->identifier, viewBackend);
        viewBackend->setSourceNotifier(sensor->oscHandler);
        newValDevice->setParentItem(parentForSensorView);
//        allConnectableObjects.insert(sensor->identifier, ConnectableObject(sensor->oscHandler, newValDevice, TypeHelper::Input));
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

void MainBackend::newConnectionEstablished()
{

}

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
    NodeViewController *newProcessingView = qobject_cast<NodeViewController*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", _id},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNode *viewBackend = newProcessingView->findChild<ProcessNode*>(_id+"-viewcontroller");
    newProcessingView->setParentItem(processingGraphView);

    processingNodes.insert(_id.toUtf8(), ProcessingNode(viewBackend, newProcessingView));
    allConnectableObjects.insert(_id.toUtf8(), ConnectableObject(viewBackend, newProcessingView, TypeHelper::Process));
    connect(viewBackend, &ProcessNode::newSubprocessorWasCreated, this, &MainBackend::moveSubprocessorToProcessThread);
    qDebug() << "going out of scope";
    return true;
}

bool MainBackend::createOutputNodeDrop(QPoint atPoint, QString targetDevice, quint16 outputIndex, TypeHelper::ValueType valType, int subIdx)
{
    qInfo() << atPoint << targetDevice << outputIndex << valType << subIdx;

    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OutputNodeView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating output node" << newDeviceComponent.errorString();
        newDeviceComponent.deleteLater();
        return false;
    }
    QString _id = createUniqueId(TypeHelper::Output);


    OutputNodeController *newOutPutNode = qobject_cast<OutputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", _id},
                                                                                                          {"outputAddress", oscOutputDevices.value(targetDevice).viewController->oscPaths().at(outputIndex)},
                                                                                                          {"x", atPoint.x()-30},
                                                                                                          {"y", atPoint.y()-10}}));
    if(newOutPutNode == nullptr) {
        qWarning() << "ERROR! could not create outputnode view";
        return false;
    }
    qDebug() << "oscoutputdevices" << oscOutputDevices.keys();
    qDebug() << "oscsender" << oscOutputDevices.value(targetDevice).oscSender;
    OscPacketBuilder *oscPackBuilder = qobject_cast<OscPacketBuilder*>(oscOutputDevices.value(targetDevice).oscSender->getNotifier(outputIndex));
    if(oscPackBuilder == nullptr) {
        qWarning() << "ERROR! could not find output notifier";
        return false;
    }
    qDebug() << "outputnode connecting to packetbuilder";
    ValueNotifierClass *_outputNotifier = new ValueNotifierClass(_id.toUtf8(), -1);
    _outputNotifier->setAutoEmit(true);

    ValueNotifierClass::completeConnectValueNotifier(_outputNotifier, oscPackBuilder);

    connect(oscPackBuilder, &ValueNotifierClass::connectedValueTypeChanged, newOutPutNode, &OutputNodeController::setValueType);
    connect(oscPackBuilder, &OscPacketBuilder::oscAddressChanged, newOutPutNode, &OutputNodeController::setOutputAddress);
    connect(oscPackBuilder, &OscPacketBuilder::oscMessIdxChanged, newOutPutNode, &OutputNodeController::setTargetIdx);
    connect(this, &MainBackend::sig_trigger, oscPackBuilder, &OscPacketBuilder::outputNodeWasDropped, Qt::SingleShotConnection);
    emit sig_trigger();
//    emit oscPackBuilder->oscMessIdxChanged(oscPackBuilder->oscMessIdx());
//    newOutPutNode->setTargetIdx(outputNotifier->oscMessIdx());

    connect(oscOutputDevices.value(targetDevice).viewController, &OscOutputViewController::sig_viewAtIndexHovered, newOutPutNode, &OutputNodeController::mouseAtTargetHoveredAtIdx);
    connect(newOutPutNode, &OutputNodeController::mouseHoveredOnNode, oscOutputDevices.value(targetDevice).viewController, &OscOutputViewController::viewAtIndexHovered);


    allConnectableObjects.insert(_id, ConnectableObject(_outputNotifier, newOutPutNode, TypeHelper::Output));
    outputNodes.insert(_id, OutputNode(newOutPutNode, targetDevice, outputIndex));
//    oscOutputDevices[targetDevice].;
    qDebug() << "outputnode created, setting parentItem";

    moveObjectToThread(_outputNotifier, TypeHelper::Process);

    newOutPutNode->setParentItem(processingGraphView);


    return true;
}

bool MainBackend::deleteObjectWithId(const QString uniqueID)
{
    qDebug() << "delete request" << uniqueID << allConnectableObjects.contains(uniqueID);
    qDebug() << "Pointers" << allConnectableObjects.value(uniqueID).qmlView << allConnectableObjects.value(uniqueID).notifier;

    if(!allConnectableObjects.contains(uniqueID)) {
        qWarning() << "Object to delete not found" << uniqueID;
        return false;
    }

    const ConnectableObject &_delObj = allConnectableObjects.value(uniqueID);

    for(const QString &_cid: _delObj.receivingConnections) {
        deleteConnectionWithId(_cid);
//        allConnections.value(_cid).connectionView->deleteLater();
//        allConnections.remove(_cid);
    }
    for (const QSet<QString> &_cset: _delObj.sendConnections) {
        for(const QString &_cid: _cset) {
            deleteConnectionWithId(_cid);
//            allConnections.value(_cid).connectionView->deleteLater();
//            allConnections.remove(_cid);
        }
    }
    switch (_delObj.nodeType) {

    case TypeHelper::Input:
        inputNodes.remove(uniqueID);
        break;
    case TypeHelper::Process:
        processingNodes.remove(uniqueID);
        break;
    case TypeHelper::Output:
        outputNodes.remove(uniqueID);
        break;
    default:
        break;

    }


    _delObj.qmlView->deleteLater();

    if(_delObj.nodeType != TypeHelper::Output)
        _delObj.notifier->deleteLater();

    allConnectableObjects.remove(uniqueID);

    qDebug() << "all connectable objects size" << allConnectableObjects.size();

    return true;
}

bool MainBackend::deleteConnectionWithId(const QString uniqueID)
{
    const ValueConnection &_co = allConnections.value(uniqueID);
    ConnectableObject &_sendObj = allConnectableObjects[_co.sourceId];
    ConnectableObject &_rcvObj = allConnectableObjects[_co.receiverId];

    if(_sendObj.notifier->getNotifier(_co.sourceIdx) && _rcvObj.notifier->getNotifier(_co.receiverIdx))
    {
        ValueNotifierClass::disconnectValueTypeSignalToSlot(_sendObj.notifier->getNotifier(_co.sourceIdx),
                                                            _rcvObj.notifier->getNotifier(_co.receiverIdx),
                                                            _co.valType);
    }

    QSet<QString> &_sendCon = _sendObj.sendConnections[_co.sourceIdx];

    _sendCon.remove(uniqueID);
    qDebug() << "removed send";
    _rcvObj.receivingConnections[_co.receiverIdx] = "";
    qDebug() << "removed receive";
    if(_sendObj.nodeType == TypeHelper::Process) {
        if(_sendCon.isEmpty() && _sendObj.receivingConnections.value(_co.sourceIdx, "") == "") {
            qDebug() << "remove stuff at sendobject";
            _sendObj.receivingConnections.removeAt(_co.sourceIdx);
            _sendObj.sendConnections.removeAt(_co.sourceIdx);

            updateConnectionIndices(_sendObj, _co.sourceIdx);
            qobject_cast<ProcessNode*>(_sendObj.notifier)->deleteSubprocessorAtIdx(_co.sourceIdx);
        }
    }

    if(_rcvObj.nodeType == TypeHelper::Process) {
        if(_rcvObj.receivingConnections.value(_co.receiverIdx, "") == "" && _rcvObj.sendConnections.value(_co.receiverIdx).isEmpty()) {
            qDebug() << "remove stuff at receiveobject";
            qDebug() << "rcvObj rcvConnections" << _rcvObj.receivingConnections;
            _rcvObj.sendConnections.removeAt(_co.receiverIdx);
            _rcvObj.receivingConnections.removeAt(_co.receiverIdx);
            qDebug() << "rcvObj rcvConnections" << _rcvObj.receivingConnections;

            updateConnectionIndices(_rcvObj, _co.receiverIdx);
            qobject_cast<ProcessNode*>(_rcvObj.notifier)->deleteSubprocessorAtIdx(_co.receiverIdx);
        }
    }
    if(_rcvObj.nodeType == TypeHelper::Output) {
        if(_rcvObj.receivingConnections.size() > _co.receiverIdx) {
            _rcvObj.receivingConnections.removeAt(_co.receiverIdx);
            connect(this, &MainBackend::sig_connectionDisconnected, _rcvObj.notifier, &ValueNotifierClass::inputsDisconnected, Qt::SingleShotConnection);
            emit sig_connectionDisconnected();
        }
    }

    _co.connectionView->deleteLater();
    allConnections.remove(uniqueID);

    return true;
}

bool MainBackend::deleteSendConnectionsForObjectAtIdx(const QString uniqueID, int idx)
{
    if(!allConnectableObjects.contains(uniqueID)) return false;
    ConnectableObject _cObj = allConnectableObjects[uniqueID];

    qDebug() << "deleting connections" << uniqueID << idx;
    qDebug() << _cObj.sendConnections;
    int _idx = qMax(idx, 0);
    if(_idx >= _cObj.sendConnections.length()) return false;

//    QSet<QString> _connections = _cObj.sendConnections[idx];
    for(const QString &_conId: _cObj.sendConnections.at(_idx)) {
//    for(const QString &_conId: _connections) {
        qDebug() << "deleting" << _conId;
        deleteConnectionWithId(_conId);
    }
    return true;
}

bool MainBackend::deleteReceiveConnectionForObjectAtIdx(const QString uniqueID, int idx)
{
    if(!allConnectableObjects.contains(uniqueID)) return false;
    ConnectableObject _cObj = allConnectableObjects[uniqueID];
    int _idx = qMax(idx, 0);
    if(_idx >= _cObj.receivingConnections.length()) return false;

    deleteConnectionWithId(_cObj.receivingConnections.at(_idx));

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


    ConnectableObject &_srcObj = allConnectableObjects[senderNodeId];
    ConnectableObject &_rcvObj = allConnectableObjects[receiverNodeId];
    ValueNotifierClass *sendingNotifier = _srcObj.notifier->getNotifier(sourceValueIdx);
    ValueNotifierClass *receivingNotifier = _rcvObj.notifier->getNotifier(targetValueIdx);
    qDebug() << "receiving notifier" << receivingNotifier;

    if (sendingNotifier == nullptr || receivingNotifier == nullptr) {
        qWarning() << "Connection request" << senderNodeId << sourceValueIdx << senderConnector <<
                   receiverNodeId << targetValueIdx << receiverConnector <<
                   valueType;
        qWarning() << "no sending and/or receiving notifier found!";
        return false;
    }
    QString connectionId = createUniqueId(TypeHelper::Connection);//senderNodeId+"-*-"+receiverNodeId;
    if(allConnections.contains(connectionId)) {
        qInfo() << "connection already exists";
        return false;
    }

    int _rcvIdx = receivingNotifier->newConnectionFromSender(sendingNotifier, valueType);
    qDebug() << "receive idx" << _rcvIdx;
    if(_rcvIdx < 0) {
        qWarning() << "receiver does not accept connection";
        return false;
    }
    int _srcIdx = qMax(sourceValueIdx, 0);

    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/ConnectionView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "connectionview error" << newDeviceComponent.errorString();
        return false;
    }
    ConnectionViewController *newConnectionView = qobject_cast<ConnectionViewController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", connectionId},
                                                                                                                                          {"valueType", valueType}}));
    if(newConnectionView == nullptr) {
        qWarning() << "ERROR! while creating connecitonview";
        return false;
    }


    if(!newConnectionView->initialSetup(_srcObj.qmlView, _srcIdx,
                                    _rcvObj.qmlView, _rcvIdx))
    {
        qWarning() << "Initial connection setup failed";
        return false;
    }

    allConnections.insert(connectionId, ValueConnection(
                              senderNodeId, _srcIdx, receiverNodeId, _rcvIdx,
                              valueType, newConnectionView));

    if(_srcObj.sendConnections.size() <= _srcIdx) _srcObj.sendConnections.resize(_srcIdx+1);
    _srcObj.sendConnections[_srcIdx].insert(connectionId);

    if(_rcvObj.receivingConnections.size() <= _rcvIdx ) _rcvObj.receivingConnections.resize(_rcvIdx+1);
    _rcvObj.receivingConnections[_rcvIdx] =  connectionId;

    if(_rcvObj.nodeType == TypeHelper::Process && _rcvObj.sendConnections.size() <= _rcvIdx) {
         _rcvObj.sendConnections.resize(_rcvIdx+1);
    }

    newConnectionView->setParentItem(processingGraphView);

    qDebug() << "connection list size" << allConnectableObjects[receiverNodeId].sendConnections.size() << allConnectableObjects[receiverNodeId].receivingConnections.size();
    qDebug() << "sender node size" << allConnectableObjects.value(senderNodeId).sendConnections;


    return true;
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

    InputNodeController *inputNodeViewCtrl = qobject_cast<InputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", _id},
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

void MainBackend::updateConnectionIndices(ConnectableObject &cobject, quint16 startIdx)
{
    qDebug() << "update receive indices for object"<< cobject.receivingConnections.length() << startIdx;
    qDebug() << "all connections" << allConnections.keys();
    qDebug() << "receiving connections" << cobject.receivingConnections;
    for(int i = startIdx; i < cobject.receivingConnections.length(); i++) {
        allConnections[cobject.receivingConnections.at(i)].setReceiverIdx(i);
    }
    qDebug() << "update send indices for object" << cobject.sendConnections.length() << startIdx;
    for(int i = startIdx; i < cobject.sendConnections.length(); i++) {
        for(const QString &_cid: cobject.sendConnections.at(i)) {
            allConnections[_cid].setSourceIdx(i);
        }
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
        else if(nodeType == TypeHelper::Connection) {
            idOk = !allConnections.contains(_tmpId2);
        }
        else {
            idOk = !allConnectableObjects.contains(_tmpId2);
        }
        if(exitCounter++ > 90909) return "";
    }
//    qInfo() << _tmpId2;
    return _tmpId2;
}

void MainBackend::showConnectionLists(QString objectID)
{
    ConnectableObject _obj = allConnectableObjects[objectID];
    qDebug() << "send connections for objects" << objectID << _obj.sendConnections;
    qDebug() << "receive connections for obejct" << objectID << _obj.receivingConnections;
}

void MainBackend::saveAsButtonPressed(QUrl fileUrl)
{
    qDebug() << "save function " << fileUrl;

}

void MainBackend::loadButtonPressed()
{

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

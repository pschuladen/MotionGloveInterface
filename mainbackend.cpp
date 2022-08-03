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

//    deviceStatusView = mainWindow->findChild<QQuickItem*>("discoveredDevicesView");
    inputDevicesSidebarView = mainWindow->findChild<QQuickItem*>("inputDevicesView");
    processingGraphView = mainWindow->findChild<QQuickItem*>("processingGraphView");
    outputDevcesSidebarView = mainWindow->findChild<QQuickItem*>("outputDevicesView");
    if(!inputDevicesSidebarView || !processingGraphView || !outputDevcesSidebarView) {
        qWarning() << "ERROR! did not found all views";
    }
}

void MainBackend::initialSetup()
{
    DeviceStatusManager *_dma = new DeviceStatusManager();
    deviceManager.append(_dma);

    projectManager = new ProjectFileManager();

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
    connect(this, &MainBackend::loadedMotionDeviceFromFile, _dma, &DeviceStatusManager::loadMotionDeviceFromDomElement);

    projectManager = projManager::Instance();
    connect(this, &MainBackend::sig_increase, projectManager, &ProjectFileManager::increaseSaveObjectCounter);
    connect(this, &MainBackend::sig_sendProjectFileUrl, projectManager, &ProjectFileManager::setXmlFilePath);
    connect(this, &MainBackend::sig_sceneCleared, projectManager, &ProjectFileManager::loadSceneFromCurrentFile);
    connect(projectManager, &ProjectFileManager::selectedXmlFileReadyLoad, this, &MainBackend::prepareForNewProject);
    connect(projectManager, &ProjectFileManager::qDomElemToScene, this, &MainBackend::qDomElementFromFile);
    connect(projectManager, &ProjectFileManager::finishedSendingCategory, this, &MainBackend::loadCategoryFinished);
    connect(this, &MainBackend::requestObjectCategory, projectManager, &ProjectFileManager::extractNodesWithType);

    //tes qdomstufft
    connect(this, &MainBackend::testSendDomNode, projectManager, &ProjectFileManager::testReceiveDomeNode);
    QDomDocument tmpDoc;
    QDomElement xxx = tmpDoc.createElement("blabla");
//    xxx.setTagName("blabla");
    xxx.setAttribute("hase", "loeffel");
    emit testSendDomNode(xxx);
    //


//    connect(this, &MainBackend::sig_triggerLoadFromFile, projectManager, &ProjectFileManager::setXmlFilePath);

//    connect(_dma, &DeviceStatusManager::receivedNewDevice, this, &MainBackend::createNewInputViews, Qt::QueuedConnection);

//    connect(threads.value(NetIn), &QThread::finished, this, &MainBackend::test_threadAlive);
//    threads.value(NetIn)->start();
}

bool MainBackend::createProcessingNodeDrop(QPoint atPosition, TypeHelper::ProcessorType type, QString id)//float posX, float posY)
{

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/Pn_ScaleView.qml")));
    if(newProcessingComponent.isError()) {
        qWarning() << "WARNING! New Processing Component:" << newProcessingComponent.errorString();
        return false;
    }
    if(id == "") id = createUniqueId(TypeHelper::Process); //name = QString("proc-%1").arg(processingNodes.size());
    NodeViewController *newProcessingView = qobject_cast<NodeViewController*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", id},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNode *viewBackend = newProcessingView->findChild<ProcessNode*>(id+"-viewcontroller");
    newProcessingView->setParentItem(processingGraphView);

    processingNodes.insert(id.toUtf8(), ProcessingNode(type, viewBackend, newProcessingView));
    allConnectableObjects.insert(id.toUtf8(), ConnectableObject(viewBackend, newProcessingView, TypeHelper::Process));
    connect(viewBackend, &ProcessNode::newSubprocessorWasCreated, this, &MainBackend::moveSubprocessorToProcessThread);
    qDebug() << "going out of scope";
    return true;
}

bool MainBackend::createOutputNodeDrop(QPoint atPoint, QString targetDevice, quint16 outputIndex, TypeHelper::ValueType valType, QString id,int subIdx)
{
    qInfo() << atPoint << targetDevice << outputIndex << valType << subIdx;

    if(!oscOutputDevices.contains(targetDevice)) {
        qWarning() << "Outputdevice with name" << targetDevice << "does not exists!";
        return false;
    }

    for(auto iter = outputNodes.constBegin(); iter != outputNodes.constEnd(); ++iter) {
        if(iter->outputDevice == targetDevice && iter->outputIndex == outputIndex) return false;
    }

    if(id == "") id = createUniqueId(TypeHelper::Output);
    pendingOutputNodes.insert(id, OutputNode(targetDevice, outputIndex));
    pendingPointForId.insert(id, atPoint);

    connect(this, &MainBackend::sig_requestNotifierPtr, oscOutputDevices[targetDevice].oscSender, &OscOutputDevice::getNotifierRequest, Qt::SingleShotConnection);
    connect(oscOutputDevices[targetDevice].oscSender, &OscOutputDevice::sig_sendNotiferPtr, this, &MainBackend::slot_getNotifierPtrForOutputNode);
    emit sig_requestNotifierPtr(outputIndex, id);

    return true;


}

void MainBackend::slot_getNotifierPtrForOutputNode(ValueNotifierClass *notifier, QString pendingNodeId)
{
    if(!notifier || !pendingOutputNodes.contains(pendingNodeId) || !pendingPointForId.contains(pendingNodeId)) {
        qWarning() << "no notifier for ouput found or pending node not registered. returning home";
        return;
    }

    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/OutputNodeView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating output node" << newDeviceComponent.errorString();
        newDeviceComponent.deleteLater();
        return;
    }
    QPoint &atPoint = pendingPointForId[pendingNodeId];
    QString &targetDevice = pendingOutputNodes[pendingNodeId].outputDevice;
    quint16 &outputIndex = pendingOutputNodes[pendingNodeId].outputIndex;

    OutputNodeController *newOutPutNode = qobject_cast<OutputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", pendingNodeId},
                                                                                                          {"outputAddress", oscOutputDevices.value(targetDevice).viewController->oscPaths().at(outputIndex)},
                                                                                                          {"x", atPoint.x()-30},
                                                                                                          {"y", atPoint.y()-10}}));
    if(newOutPutNode == nullptr) {
        qWarning() << "ERROR! could not create outputnode view";
        return;
    }
    qDebug() << "oscoutputdevices" << oscOutputDevices.keys();
    qDebug() << "oscsender" << oscOutputDevices.value(targetDevice).oscSender;

    OscPacketBuilder *oscPackBuilder = qobject_cast<OscPacketBuilder*>(notifier);

    if(oscPackBuilder == nullptr) {
        qWarning() << "ERROR! could not find output notifier";
        return;
    }
    qDebug() << "outputnode connecting to packetbuilder";

    ValueNotifierClass *_outputNotifier = new ValueNotifierClass(pendingNodeId.toUtf8(), -1);
    _outputNotifier->setAutoEmit(true);
    moveObjectToThread(_outputNotifier, TypeHelper::Process);

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


    allConnectableObjects.insert(pendingNodeId, ConnectableObject(_outputNotifier, newOutPutNode, TypeHelper::Output));
    pendingOutputNodes[pendingNodeId].qmlView = newOutPutNode;
    outputNodes.insert(pendingNodeId, OutputNode(pendingOutputNodes[pendingNodeId]));
//    oscOutputDevices[targetDevice].;

    pendingOutputNodes.remove(pendingNodeId);
    pendingPointForId.remove(pendingNodeId);
    qDebug() << "outputnode created, setting parentItem";

    newOutPutNode->setParentItem(processingGraphView);

    objectDidFinishLoad(pendingNodeId);
    return;
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

    QList<QString> _rCons = _delObj.receivingConnections;
    for(const QString &_cid: _rCons) {// _delObj.receivingConnections) {
        deleteConnectionWithId(_cid);
//        allConnections.value(_cid).connectionView->deleteLater();
//        allConnections.remove(_cid);
    }
    QList<QSet<QString>> _sCons = _delObj.sendConnections;
    for (const QSet<QString> &_cset: _sCons) { //_delObj.sendConnections) {
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

//    if(_delObj.nodeType != TypeHelper::Output)
//        _delObj.notifier->deleteLater();

    allConnectableObjects.remove(uniqueID);

    qDebug() << "all connectable objects size" << allConnectableObjects.size();

    return true;
}

bool MainBackend::deleteConnectionWithId(const QString uniqueID)
{
    if(!allConnections.contains(uniqueID)) return false;
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

bool MainBackend::connectionRequest(const QString senderNodeId,int sourceValueIdx,
                                    const QString receiverNodeId, int targetValueIdx,
                                    TypeHelper::ValueType valueType, QString conId)
{
    if(!allConnectableObjects.contains(senderNodeId) || !allConnectableObjects.contains(receiverNodeId)) {
        qWarning() << "Connection request" << senderNodeId << sourceValueIdx <<
                   receiverNodeId << targetValueIdx  <<
                   valueType;
        qWarning() << "no objects found for connection";
        return false;
    }


    ConnectableObject &_srcObj = allConnectableObjects[senderNodeId];
    ConnectableObject &_rcvObj = allConnectableObjects[receiverNodeId];

    ValueNotifierClass *sendingNotifier;
    if(_srcObj.nodeType == TypeHelper::Input) sendingNotifier = _srcObj.notifier;
    else sendingNotifier = _srcObj.notifier->getNotifier(sourceValueIdx);
    ValueNotifierClass *receivingNotifier = _rcvObj.notifier;//->getNotifier(targetValueIdx);

    qDebug() << "receiving notifier" << receivingNotifier;

    if (sendingNotifier == nullptr || receivingNotifier == nullptr) {
        qWarning() << "Connection request" << senderNodeId << sourceValueIdx <<
                   receiverNodeId << targetValueIdx  <<
                   valueType;
        qWarning() << "no sending and/or receiving notifier found!";
        return false;
    }
    if(conId == "") conId = createUniqueId(TypeHelper::Connection);//senderNodeId+"-*-"+receiverNodeId;
    if(allConnections.contains(conId)) {
        qInfo() << "connection already exists";
        return false;
    }

//    if(targetValueIdx < 0) {
//    }

//    if(_rcvObj.nodeType == TypeHelper::Process) {
//        _rcvIdx = _rcvObj.receivingConnections.size();
//    }
    int _rcvIdx = 0;
    if(_rcvObj.nodeType == TypeHelper::Output) {
        if(_rcvObj.receivingConnections.value(0, "") == "") {
            _rcvIdx = 0;
        }
        else return false;
    }
    else if(_rcvObj.nodeType == TypeHelper::Process) {
        if(targetValueIdx < 0) {
            _rcvIdx = _rcvObj.receivingConnections.size();
        }
        else {
            if(_rcvObj.receivingConnections.value(targetValueIdx, "") == "") {
                _rcvIdx = targetValueIdx;
            }
            else {
                return false;
            }
        }
    }
    if(!_rcvObj.notifier) {
        qWarning() << "RECEIVER DOES NOT EXIST!!!!";
        return false;
    }
    qDebug() << "setting up connection requests";
    connect(this, &MainBackend::sig_connectRequestFromSender, _rcvObj.notifier, &ValueNotifierClass::connectionRequestFromSender, Qt::SingleShotConnection);
    connect(_rcvObj.notifier, &ValueNotifierClass::connectionAccepted, this, &MainBackend::newConnectionEstablished, Qt::SingleShotConnection);
    pendingConnectionRequests.insert(conId, ValueConnection(senderNodeId, qMax(sourceValueIdx, 0),receiverNodeId, _rcvIdx, valueType));
    emit sig_connectRequestFromSender(sendingNotifier, conId, valueType, _rcvIdx);
    return true;


//    if(_rcvObj.nodeType == TypeHelper::Output) {
//        if(_rcvObj.receivingConnections.value(0, "") == "") {
//            _rcvIdx = 0;
//            connect(this, &MainBackend::newConnectionRequest, receivingNotifier, &ValueNotifierClass::newConnectionFromSender, Qt::SingleShotConnection);
//            emit newConnectionRequest(sendingNotifier, valueType, targetValueIdx);
//        }
//        else return false;
//    }
//    else {
//        _rcvIdx = receivingNotifier->newConnectionFromSender(sendingNotifier, valueType, -1);
//    }
//    qDebug() << "receive idx" << _rcvIdx;
//    if(_rcvIdx < 0) {
//        qWarning() << "receiver does not accept connection";
//        return false;
//    }
//    int _srcIdx = qMax(sourceValueIdx, 0);

//    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/ConnectionView.qml")));
//    if(newDeviceComponent.isError()) {
//        qWarning() << "connectionview error" << newDeviceComponent.errorString();
//        return false;
//    }
//    ConnectionViewController *newConnectionView = qobject_cast<ConnectionViewController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", conId},
//                                                                                                                                          {"valueType", valueType}}));
//    if(newConnectionView == nullptr) {
//        qWarning() << "ERROR! while creating connecitonview";
//        return false;
//    }


//    if(!newConnectionView->initialSetup(_srcObj.qmlView, _srcIdx,
//                                    _rcvObj.qmlView, _rcvIdx))
//    {
//        qWarning() << "Initial connection setup failed";
//        return false;
//    }

//    qDebug() << "registering connection";
//    if(_srcObj.sendConnections.size() <= _srcIdx) _srcObj.sendConnections.resize(_srcIdx+1);
//    _srcObj.sendConnections[_srcIdx].insert(conId);

//    if(_rcvObj.receivingConnections.size() <= _rcvIdx ) _rcvObj.receivingConnections.resize(_rcvIdx+1);
//    _rcvObj.receivingConnections[_rcvIdx] =  conId;

//    if(_rcvObj.nodeType == TypeHelper::Process && _rcvObj.sendConnections.size() <= _rcvIdx) {
//         _rcvObj.sendConnections.resize(_rcvIdx+1);
//    }

//    newConnectionView->setParentItem(processingGraphView);

//    allConnections.insert(conId, ValueConnection(
//                              senderNodeId, _srcIdx, receiverNodeId, _rcvIdx,
//                              valueType, newConnectionView));

//    qDebug() << "connection list size" << allConnectableObjects[receiverNodeId].sendConnections.size() << allConnectableObjects[receiverNodeId].receivingConnections.size();
//    qDebug() << "sender node size" << allConnectableObjects.value(senderNodeId).sendConnections;


//    return true;

}

void MainBackend::newConnectionEstablished(QString connectionId, bool accepted, TypeHelper::ValueType type, int atIdx)
{
    if(pendingConnectionRequests.contains(connectionId)) {
        if(accepted) {
            ValueConnection &_newCon = pendingConnectionRequests[connectionId];
            if(!allConnectableObjects.contains(_newCon.receiverId) || !allConnectableObjects.contains(_newCon.sourceId)) {
                qWarning() << "objects with id" << _newCon.sourceId << "and/or" << _newCon.receiverId << "does not exist anymore";
            }

            QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/ConnectionView.qml")));
            if(newDeviceComponent.isError()) {
                qWarning() << "connectionview error" << newDeviceComponent.errorString();
                return;
            }
            ConnectionViewController *newConnectionView = qobject_cast<ConnectionViewController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", connectionId},
                                                                                                                                                  {"valueType", _newCon.valType}}));
            if(newConnectionView == nullptr) {
                qWarning() << "ERROR! while creating connecitonview";
                return;
            }

            ConnectableObject &_srcObj = allConnectableObjects[_newCon.sourceId];
            ConnectableObject &_rcvObj = allConnectableObjects[_newCon.receiverId];
            int _rcvIdx = _newCon.receiverIdx;
            int _srcIdx = _newCon.sourceIdx;

            if(!newConnectionView->initialSetup(_srcObj.qmlView, _srcIdx,
                                            _rcvObj.qmlView, _rcvIdx))
            {
                qWarning() << "Initial connection setup failed";
                return;
            }
            _newCon.connectionView = newConnectionView;

            qDebug() << "registering connection" << "sIdx:" << _srcIdx << "rIdx" << _rcvIdx;
            if(_srcObj.sendConnections.size() <= _srcIdx) _srcObj.sendConnections.resize(_srcIdx+1);
            _srcObj.sendConnections[_srcIdx].insert(connectionId);

            if(_rcvObj.receivingConnections.size() <= _rcvIdx ) _rcvObj.receivingConnections.resize(_rcvIdx+1);
            _rcvObj.receivingConnections[_rcvIdx] =  connectionId;

            if(_rcvObj.nodeType == TypeHelper::Process && _rcvObj.sendConnections.size() <= _rcvIdx) {
                 _rcvObj.sendConnections.resize(_rcvIdx+1);
            }

            newConnectionView->setParentItem(processingGraphView);

            qDebug() << "final insert connection in mape";
            allConnections.insert(connectionId, ValueConnection(_newCon));


        }

        pendingConnectionRequests.remove(connectionId);
    }
}

bool MainBackend::createOscOutputDevice()
{
    QString uniqueID = createUniqueId(TypeHelper::OscOutput);//QString("oscout-%1").arg(oscOutputDevices.size());
    return createOscOutputDevice(uniqueID);
}

bool MainBackend::createOscOutputDevice(QString uniqueID)
{
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

    OscOutputDevice *oscDevice = new OscOutputDevice(uniqueID, viewController, nullptr);
    oscDevice->moveToThread(threads.value(NetOut));

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
    connect(this, &MainBackend::sig_trigger, oscDevice, &OscOutputDevice::initialiseOscDevice, Qt::SingleShotConnection);
    emit sig_trigger();

    return true;
}


bool MainBackend::createInputNodeDrop(QPoint atPoint, QString sourceDevice, QString inputPath, TypeHelper::ValueType valType,
                                      QString id, int subIdx)
{   
//    qInfo() << "create input" << atPoint << sourceDevice << identifier << valType << subIdx;

    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/InputNodeView.qml")));
    if(newDeviceComponent.isError()) {
        qWarning() << "ERROR! while creating input node" << newDeviceComponent.errorString();
        newDeviceComponent.deleteLater();
        return false;
    }
    if(id == "") id = createUniqueId(TypeHelper::Input);

    InputNodeController *inputNodeViewCtrl = qobject_cast<InputNodeController*>(newDeviceComponent.createWithInitialProperties({{"uniqueID", id},
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

    ValueNotifierClass *inNodeNoti = new ValueNotifierClass(id.toUtf8(), -1, valType);
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

    inputNodes.insert(id, InputNode(inputNodeViewCtrl, sourceDevice, inputPath));
    allConnectableObjects.insert(id, ConnectableObject(inNodeNoti, inputNodeViewCtrl, TypeHelper::Input));
    inputNodeViewCtrl->setParentItem(processingGraphView);

    objectDidFinishLoad(id);
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
//    for(const QString &_rcId: cobject.receivingConnections) {
        if(cobject.receivingConnections.at(i) != "")
            allConnections[cobject.receivingConnections.at(i)].setReceiverIdx(i);
    }
    qDebug() << "update send indices for object" << cobject.sendConnections.length() << startIdx;
    for(int i = startIdx; i < cobject.sendConnections.length(); i++) {
        for(const QString &_cid: cobject.sendConnections.at(i)) {
            allConnections[_cid].setSourceIdx(i);
        }
    }
}

void MainBackend::errorWarningCreate(QString id, TypeHelper::NodeType ntype)
{
    qWarning() << "error while creating" << TypeHelper::getPrefixForNodetype(ntype) << id;
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
            idOk = !allConnections.contains(_tmpId2) && !pendingConnectionRequests.contains(_tmpId2);
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

void MainBackend::createNewMotionDeviceView(QString name, OscInputDevice *newDevice, QString uniqueID)
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

    bool updateIdInStatusManager = false;
    if(uniqueID == "") {
        uniqueID = createUniqueId(TypeHelper::OscInput);
        updateIdInStatusManager = true;
    }

    newDeviceItem->setObjectName(QString(name+"-view"));
    oscInputDevices.insert(uniqueID, OscInDeviceStruct(name, newDevice, viewCon, newDeviceItem));
    connect(viewCon, &OscInputViewController::connectDeviceChanged, newDevice, &OscInputDevice::setSendPong);

    newDeviceItem->setParentItem(inputDevicesSidebarView);

    connect(this, &MainBackend::inputViewReady, newDevice, &OscInputDevice::viewWasCreated, Qt::SingleShotConnection);
    connect(newDevice, &OscInputDevice::sendSensorStructList, this, &MainBackend::createSensorViewsForMotionDevice, Qt::SingleShotConnection);
    connect(newDevice, &OscInputDevice::didFinishLoad, this, &MainBackend::objectDidFinishLoad, Qt::SingleShotConnection);

    if(updateIdInStatusManager) emit deviceWithNameCreated(name, uniqueID);

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


void MainBackend::saveAsButtonPressed(QUrl fileUrl)
{
    qDebug() << "save function " << fileUrl;
    emit sig_sendProjectFileUrl(fileUrl);

    projectManager->prepareXmlDoc();
    int saveCount = 0;

    for (auto iter = oscInputDevices.constBegin(); iter != oscInputDevices.constEnd(); ++iter) {
        projectManager->oscInputDeviceElement(iter.key(), iter->deviceName);
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->oscReceiver, &OscInputDevice::initSaveData, Qt::SingleShotConnection);
        saveCount += 1;
    }
    for (auto iter = oscOutputDevices.constBegin(); iter != oscOutputDevices.constEnd(); ++iter) {
        projectManager->oscOutputdeviceElement(iter.key());
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->oscSender, &OscOutputDevice::initSaveData, Qt::SingleShotConnection);
        saveCount += 1;
    }
    for (auto iter = inputNodes.constBegin(); iter != inputNodes.constEnd(); ++iter) {
        projectManager->inputNodeElement(iter.key(), iter->inputDevice, iter->inputAddress);
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->qmlView, &InputNodeController::initSaveData, Qt::SingleShotConnection);
        saveCount += 1;
    }
    for(auto iter = outputNodes.constBegin(); iter != outputNodes.constEnd(); ++iter) {
        projectManager->outputNodeElement(iter.key(), iter->outputDevice, iter->outputIndex);
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->qmlView, &OutputNodeController::initSaveData);
        saveCount += 1;
    }
    for(auto iter = processingNodes.constBegin(); iter != processingNodes.constEnd(); ++iter) {
        projectManager->processingNodeElement(iter.key(), iter->procType);
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->qmlView, &NodeViewController::initSaveData, Qt::SingleShotConnection);
        connect(this, &MainBackend::sig_triggerSaveToFile, iter->controller, &ProcessNode::initSaveData, Qt::SingleShotConnection);
        saveCount += 2;
    }
    for(auto iter = allConnections.constBegin(); iter != allConnections.constEnd(); ++iter) {
        projectManager->valueConnectionElement(iter.key(), iter->sourceId, iter->sourceIdx, iter->receiverId, iter->receiverIdx, iter->valType);
    }

    emit sig_increase(saveCount);

    emit sig_triggerSaveToFile();

//    for(QString &_id: oscInputDevices.keys()) {
//        projectManager->oscInputDevice(_id, oscInputDevices[_id].deviceName);
//    }
//    for(QString &_id: oscOutputDevices.keys()) {
//        projectManager->oscOutputdevice(_id);
//    }
//    for(QString &_id: oscInputDevices.keys()) {
////        projectManager.
//    }

//    projectManager->writeXmlFile(fileUrl);

}

void MainBackend::loadButtonPressed(QUrl fileUrl)
{
    qDebug() << "load file" << fileUrl;

    emit sig_sendProjectFileUrl(fileUrl, true);
}

void MainBackend::prepareForNewProject()
{
    //TODO: delete everything that is there (but not Input/Output? )

    qDebug() << "scene cleared";

//    loadSceneStatus = 1;
//    emit requestObjectCategory(TypeHelper::OscInput);
    requestNextCategory();

//    emit sig_sceneCleared();
}

void MainBackend::qDomElementFromFile(QDomElement element, TypeHelper::NodeType type)
{

    switch(type) {

    case TypeHelper::Input:
    {
//        QString _id = element.tagName();
        QString _sDev = element.attribute("inputDevice", "");
        QString _inPath = element.attribute("inputAddress", "");
        if(_sDev != "" && _inPath != "") {
            QPoint _p = getCreatePosition(element);//(10,10);
            bool _typeOk;
            quint16 _typeint = getAttributeFromViewNode(element, "valueType").toUInt(&_typeOk);
            qDebug() << "creating inputnode "<< element.tagName();
            if(_typeOk) {
                TypeHelper::ValueType _vtyp = static_cast<TypeHelper::ValueType>(_typeint);
                pendingObjectCreation.insert(element.tagName());
                qDebug() << "creating input" << element.tagName();
                if(!createInputNodeDrop(_p, _sDev, _inPath, _vtyp, element.tagName())) qWarning() << "could not create inputnode";
            }
            else {
                qWarning() << "type for input not ok" << _typeint;
            }
        }
    }
        break;

    case TypeHelper::Process:
    {
        bool _typeOk;
        int _sType = element.attribute("processor-type", "").toInt(&_typeOk);
        if(_typeOk) {
            TypeHelper::ProcessorType _pType = static_cast<TypeHelper::ProcessorType>(_sType);
            if(createProcessingNodeDrop(getCreatePosition(element), _pType, element.tagName())) {
                QDomElement _proc = element.firstChildElement("processor");
                pendingObjectCreation.insert(element.tagName());
                if(!_proc.isNull()) {


                    connect(this, &MainBackend::sendQdomElement,
                            processingNodes[element.tagName()].controller,
                            &ProcessNode::loadDataFromQdomElement, Qt::SingleShotConnection);
                    connect(processingNodes[element.tagName()].controller,
                            &ProcessNode::didFinishLoad,
                            this, &MainBackend::objectDidFinishLoad,
                            Qt::SingleShotConnection);
                    emit sendQdomElement(element);
                }
                else {
                    objectDidFinishLoad(element.tagName());
                }
            }
        }

    }
        break;

    case TypeHelper::Output:
    {
        QString _tDev = element.attribute("ouputDevice", "");
        bool idxOk;
        int _tIdx = element.attribute("outputIdx", "").toInt(&idxOk);
        if(idxOk) {
            pendingObjectCreation.insert(element.tagName());
            createOutputNodeDrop(getCreatePosition(element),
                                 _tDev, _tIdx, TypeHelper::Undefined,
                                 element.tagName());
        }
    }
        break;

    case TypeHelper::OscInput:
        pendingObjectCreation.insert(element.tagName());
        emit loadedMotionDeviceFromFile(element);
        break;

    case TypeHelper::OscOutput:
    {
        if(createOscOutputDevice(element.tagName())) {
            QDomElement devSetElement = element.firstChildElement("device-settings");
            if(!devSetElement.isNull()) {
                pendingObjectCreation.insert(element.tagName());
                connect(this, &MainBackend::sendQdomElement,
                        oscOutputDevices[element.tagName()].oscSender,
                        &OscOutputDevice::loadDataFromQdomElement,
                        Qt::SingleShotConnection);
                emit sendQdomElement(devSetElement);
                connect(oscOutputDevices[element.tagName()].oscSender,
                        &OscOutputDevice::didFinishLoad,
                        this, &MainBackend::objectDidFinishLoad,
                        Qt::SingleShotConnection);
            }
        }
        else errorWarningCreate(element.tagName(), type);
    }
        break;

    case TypeHelper::Connection:
    {
        QString _sId = element.attribute("sourceId", "");
        QString _rId = element.attribute("receiverId", "");
        bool _sOk, _rOk, _vOk;
        int _sIdx = element.attribute("srcIdx", "-1").toInt(&_sOk);
        int _rIdx = element.attribute("rcvIdx", "-1").toInt(&_rOk);
        int _vTypi = element.attribute("valueType", "0").toInt(&_vOk);
        if(_sOk && _rOk && _vOk) {
            TypeHelper::ValueType _vTyp = static_cast<TypeHelper::ValueType>(_vTypi);
            connectionRequest(_sId, _sIdx, _rId, _rIdx, _vTyp);
        }
    }
        break;
    default:
        break;
    }
}

QPoint MainBackend::getCreatePosition(QDomElement element)
{
    QPoint _p(10,10);
    QDomElement _viewnode = element.firstChildElement("view-node");
    if(!_viewnode.isNull()) {
        QDomElement _pos = _viewnode.firstChildElement("position");
        if(!_pos.isNull()) {
            _p.setX(_pos.attribute("x", "10").toFloat());
            _p.setY(_pos.attribute("y", "10").toFloat());
        }
    }
    return _p;
}

QString MainBackend::getAttributeFromViewNode(QDomElement element, QString attr)
{
    QDomElement _viewnode = element.firstChildElement("view-node");
    if(!_viewnode.isNull()) {
        QDomElement _attr = _viewnode.firstChildElement("attributes");
        if(!_attr.isNull()) {
            return _attr.attribute(attr, "");
        }
    }
    return "";
}

void MainBackend::objectDidFinishLoad(QString uniqueID)
{

    qDebug() << "finished loading object" << uniqueID;
    if(pendingObjectCreation.remove(uniqueID)) {
        requestNextCategory();
    }
}

void MainBackend::loadCategoryFinished(TypeHelper::NodeType type)
{
    qDebug() << "finished read category" << type;
    categoryPending = false;
    requestNextCategory();
}


void MainBackend::requestNextCategory()
{
    qDebug() << "load scene status:" << loadSceneStatus;
    if(pendingObjectCreation.isEmpty() && !categoryPending) {
        typedef TypeHelper _th;
        const QList<_th::NodeType> _cats = {_th::OscInput, _th::OscOutput,
                                            _th::Input, _th::Output,
                                            _th::Process, _th::Connection};
        if(loadSceneStatus < _cats.length()) {
            loadSceneStatus++;
            categoryPending = true;
            emit requestObjectCategory(_cats.at(loadSceneStatus-1));
        }
        else {
            loadSceneStatus = 0;
        }
    }
    else {
        qDebug() << "pending objects" << pendingObjectCreation;
    }
}






//        qDebug() << "read to load oscouput" << element.tagName();
//        QDomNodeList _nodes = element.childNodes();
//        for(int i = 0; i < _nodes.count(); i++) {
//            if(_nodes.at(i).isElement()) {
//                const QDomElement _subElement = _nodes.at(i).toElement();
//            }
//        }
//    }

//    qDebug() << "receiving nodeElement";
//    QDomNodeList _subNodes = element.childNodes();
//    for(int i = 0; i < _subNodes.count(); i++) {
//        if(_subNodes.at(i).isElement()) {
//            const QDomElement _subElement = _subNodes.at(i).toElement();
//            qDebug() << "getting tags from devie" << element.tagName() << ":" <<_subElement.tagName();
//            QDomNodeList sssubs = _subElement.childNodes();
//            for(int i = 0; i < sssubs.length(); i++) {
//                if(sssubs.at(i).isElement()) {
//                    QDomElement xx = sssubs.at(i).toElement();
//                    qDebug() << xx.tagName();
//                    for(int i = 0; i < xx.childNodes().length(); i++) {
//                        QDomElement xxx = xx.childNodes().at(i).toElement();
//                        qDebug() << xxx.tagName();
//                        for (int i = 0; i < xxx.attributes().length(); i++) {
//                            QDomNode _aa = xxx.attributes().item(i);
//                            QDomAttr _a = _aa.toAttr();
//                            qDebug() << _a.name() << _a.value();
//                        }
//                        if(xxx.childNodes().at(i).isAttr()) {
//                            QDomAttr _a = xxx.childNodes().at(i).toAttr();
//                            qDebug() << _a.name() << _a.value();
//                        }
//                    }
//                }
//            }
//        }
//    }

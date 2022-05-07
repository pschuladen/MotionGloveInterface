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
    processingGraphView = mainWindow->findChild<QQuickItem*>("processingGraphView");
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
    ProcessNode *processController = viewBackend->createProcessControl(name);
    qInfo() << processController << processController->parent();


//    newProcessingItem->setObjectName(name);
//    ProcessNodeController *newProcessCalculus = new ProcessNodeController();
//    newProcessCalculus->setObjectName(name+"-calculus");
    ProcessingNode newNode;
    newNode.processingController = processController;
    newNode.viewController = viewBackend;
    newNode.qmlView = newProcessingItem;
    processingNodes.insert(name, newNode);//ProcessingNode(newProcessCalculus, viewBackend));
    allConnectableObjects.insert(name, ConnectableObject(processController, newProcessingItem));

    qInfo() << "Node Hash size" << processingNodes.size();
    qInfo() << "node" << name << processingNodes.values().size();
//    ProcessNodeController *baba = processingNodes[name].viewController;
//    qInfo() << "viewba" << baba->objectName();

//    qInfo() << "processing nodes" << processingNodes[name].processingObject->objectName() << processingNodes[name].viewBackend->objectName();
    //    QQuickitem *theView = processingNodes.value(name).viewBackend;
}

bool MainBackend::connectionRequest(const QString sourceObjectId, const QString senderNodeId,int sourceValueIdx,
                                    const QString receiverNodeId, int targetValueIdx, TypeHelper::ValueType valueType)
{
    qInfo() << "Connection request" << sourceObjectId << senderNodeId << sourceValueIdx << receiverNodeId << targetValueIdx << valueType;
    qInfo() << "registered objects for" << receiverNodeId
            << processingNodes.value(receiverNodeId).processingController
            << processingNodes.value(receiverNodeId).viewController
           << processingNodes.value(receiverNodeId).qmlView;

    if(allConnectableObjects.contains(senderNodeId) && allConnectableObjects.contains(receiverNodeId)) {
        //TODO: create ConnectionView/Object

        return allConnectableObjects[receiverNodeId].notifier->newConnectionFromSender(allConnectableObjects[senderNodeId].notifier->getNotifier(sourceValueIdx), valueType);
    }
    return false;


//    const MotionDevice &motionDevice = main_devicestatus::Instance()->discoveredDevices.value(sourceObjectId);
//    ProcessNode *prcObject = processingNodes.value(receiverNodeId).processingController;
////    const ProcessNode *prcViewCon = processingNodes.value(receiverNodeId).viewController;
//    const DeviceDataInput::OscInputStruct &oscStr = motionDevice.inputs->value(senderNodeId);
//    ValueNotifierClass *valueNotifier = motionDevice.getValueNotifierForPath(senderNodeId, sourceValueIdx);
//    bool connectionAccepted = prcObject->newConnectionFromSender(valueNotifier, valueType);
////    if (valueType == TypeHelper::SingleValue) {
////        valueNotifier = motionDevice.inputHandler->valueNotifier.value(oscStr.sensorType).at(oscStr.sensorIndex)->subNotifier.at(sourceIdx);

////        connect(valueNotifier, &ValueNotifierClass::singleValueChanged, prcObject, &ProcessNodeController::singleValueChanged);
////        connect(valueNotifier, &ValueNotifierClass::singleValueChanged, prcViewCon, &ProcessNodeController::singleValueChanged);
////    }
////    const ProcessNodeController &proces = processingNodes.value(receiverNodeId).processingObject;

//    return connectionAccepted;
}

bool MainBackend::createOscOutputDevice()
{
    qInfo() << "create new osc output device";
    return true;
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
    connect(newDeviceItem, SIGNAL(connectButtonChanged(bool,QString)), main_devicestatus::Instance(), SLOT(setConnectStatus(bool,QString)));
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

        if(vTyp == TypeHelper::Vector) {//sTyp == sensType::Accel || sTyp == sensType::Gyro || sTyp==sensType::Grav) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::vectorChanged, viewBackend, &InputValueViewController::vectorChanged);
        }
        else if(vTyp == TypeHelper::Quat) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::quatChanged, viewBackend, &InputValueViewController::quatChanged);
        }
        else if(vTyp == TypeHelper::List) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::touchChanged, viewBackend, &InputValueViewController::touchChanged);
        }
        else {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::valuesChanged, viewBackend, &InputValueViewController::viewValuesChanged);
        }
        allConnectableObjects.insert(osc, ConnectableObject(motionDevice->getValueNotifierForPath(osc), newValDevice));
    }
}

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);

}

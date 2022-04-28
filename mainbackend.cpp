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

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/ProcessingNodeBase.qml")));
    qInfo() << "errorstring" << newProcessingComponent.errorString();
    QString name = QString("proc-%1").arg(processingNodes.size());
//    ProcessNodeController newC; //= new ProcessNodeController();
    QQuickItem *newProcessingItem = qobject_cast<QQuickItem*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", name},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));

    ProcessNodeController *viewBackend = newProcessingItem->findChild<ProcessNodeController*>(name+"-viewcontroller");
    newProcessingItem->setParentItem(processingGraphView);
//    newProcessingItem->setObjectName(name);
    ProcessNodeController *newProcessCalculus = new ProcessNodeController();
    newProcessCalculus->setObjectName(name+"-calculus");
    ProcessingNode newNode;
    newNode.processingObject = newProcessCalculus;
    newNode.viewController = viewBackend;
    processingNodes.insert(name, newNode);//ProcessingNode(newProcessCalculus, viewBackend));

    qInfo() << "Node Hash size" << processingNodes.size();
    qInfo() << "node" << name << processingNodes.values().size();
    ProcessNodeController *baba = processingNodes[name].viewController;
    qInfo() << "viewba" << baba->objectName();

//    qInfo() << "processing nodes" << processingNodes[name].processingObject->objectName() << processingNodes[name].viewBackend->objectName();
    //    QQuickitem *theView = processingNodes.value(name).viewBackend;
}

bool MainBackend::connectionRequest(QString sourceObjectId, QString senderNodeId, QString receiverNodeId, ProcessNodeController::ValueType valueType, int idx)
{

    qInfo() << "Connection request" << senderNodeId << receiverNodeId << valueType << idx;
    qInfo() << "";
    const MotionDevice &motionDevice = main_devicestatus::Instance()->discoveredDevices.value(sourceObjectId);
    const ProcessNodeController *prcObject = processingNodes.value(receiverNodeId).processingObject;
    const ProcessNodeController *prcViewCon = processingNodes.value(receiverNodeId).viewController;
    const DeviceDataInput::OscInputStruct &oscStr = motionDevice.inputs->value(senderNodeId);
    ValueNotifierClass *valueNotifier;
    if (valueType == ProcessNodeController::Single) {
        valueNotifier = motionDevice.inputHandler->valueNotifier.value(oscStr.sensorType).at(oscStr.sensorIndex)->subNotifier.at(idx);
        connect(valueNotifier, &ValueNotifierClass::singleValueChanged, prcObject, &ProcessNodeController::singleValueChanged);
        connect(valueNotifier, &ValueNotifierClass::singleValueChanged, prcViewCon, &ProcessNodeController::singleValueChanged);
    }
//    const ProcessNodeController &proces = processingNodes.value(receiverNodeId).processingObject;

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
        sensType sTyp = motionDevice->inputs->value(osc).sensorType;
        int senIdx = motionDevice->inputs->value(osc).sensorIndex;

        if(sTyp == sensType::Accel || sTyp == sensType::Gyro || sTyp==sensType::Grav) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::vectorChanged, viewBackend, &InputValueViewController::vectorChanged);
        }
        else if(sTyp == sensType::Quat) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::quatChanged, viewBackend, &InputValueViewController::quatChanged);
        }
        else if(sTyp == sensType::Touch) {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::touchChanged, viewBackend, &InputValueViewController::touchChanged);
        }
        else {
            connect(motionDevice->inputHandler->valueNotifier.value(sTyp).at(senIdx), &ValueNotifierClass::valuesChanged, viewBackend, &InputValueViewController::valuesChanged);
        }
    }
}

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);

}

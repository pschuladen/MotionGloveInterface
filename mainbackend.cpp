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

void MainBackend::createNewProcessingView(DataProcessingNode::ProcessingType type, QPoint atPosition)//float posX, float posY)
{
    qInfo() << "now i should creat a view" << type << "at" << atPosition.x() << atPosition.y();

    QQmlComponent newProcessingComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/ProcessingNodeBase.qml")));

    QString name = QString("proc-%1").arg(processingNodes.size());
    QQuickItem *newProcessingItem = qobject_cast<QQuickItem*>(newProcessingComponent.createWithInitialProperties({{"uniqueID", name},
                                                                                                                  {"x", atPosition.x()-40},
                                                                                                                  {"y", atPosition.y()-20}}));
    ValueViewBackend *viewBackend = newProcessingItem->findChild<ValueViewBackend*>(name+"-backend");
    newProcessingItem->setParentItem(processingGraphView);
//    newProcessingItem->setObjectName(name);
    DataProcessingNode *newProcessCalculus = new DataProcessingNode();
    newProcessCalculus->setObjectName(name+"-calculus");
    ProcessingNode newNode;
    newNode.processingObject = newProcessCalculus;
    newNode.viewBackend = viewBackend;
    processingNodes.insert(name, newNode);//ProcessingNode(newProcessCalculus, viewBackend));

    qInfo() << "Node Hash size" << processingNodes.size();
    qInfo() << "node" << name << processingNodes.values().size();
    ValueViewBackend *baba = processingNodes[name].viewBackend;
    qInfo() << "viewba" << baba->objectName();

//    qInfo() << "processing nodes" << processingNodes[name].processingObject->objectName() << processingNodes[name].viewBackend->objectName();
//    QQuickitem *theView = processingNodes.value(name).viewBackend;
}

void MainBackend::createMotionInputDeviceView(MotionDevice *motionDevice)
{
    QQmlComponent newDeviceComponent(m_engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));
//    qInfo() << newDeviceComponent.errors();
    QQuickItem *newDeviceItem = qobject_cast<QQuickItem*>(newDeviceComponent.create());
    newDeviceItem->setProperty("identifier", motionDevice->deviceName);
    newDeviceItem->setObjectName(QString(motionDevice->deviceName+"-view"));
    newDeviceItem->setParentItem(inputDevicesSidebarView);
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

void MainBackend::createNewInputViews(MotionDevice *motionDevice)
{
//    qInfo() << "begin creating views";
    createMotionInputDeviceView(motionDevice);
    createValueInputViewsForDevice(motionDevice);

}

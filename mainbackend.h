#ifndef MAINBACKEND_H
#define MAINBACKEND_H

/*
 * This class is a Singleton.
 * always call it through main_backend::Instance()
 *
 * It is responsible for creating views and managing connection
 * between UI and DataBackend.
 * It keeps track for all created views and corresponding data-objects.
 * Therefore it also initialises node-connection requests.
 *
 * It also keeps track of the processing objects and connections.
 *
*/

#include <QObject>
#include "Singleton.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlregistration.h>

#include "devicestatusmanager.h" //recursive inclusion und so...
#include "devicedatainput.h"
#include "valuenotifierclass.h" //kann weg
#include "processnode.h"

#include "inputvalueviewcontroller.h"
#include "processnodecontroller.h"
#include "dataprocessingnode.h"

#include "typehelper.h"

class MainBackend : public QObject
{
    Q_OBJECT

public:

    explicit MainBackend(QObject *parent = nullptr);
    typedef InputValueViewController::ValueViewMode sensType ;

    void setEngine(QQmlApplicationEngine *engine);

    Q_INVOKABLE void createNewProcessingView(ProcessNodeController::ProcessingType type, QPoint atPosition=QPoint(10,10));
    Q_INVOKABLE bool connectionRequest(QString senderNodeId,int sourceIdx, QQuickItem *senderConnector,
                                       QString receiverNodeId, int targetIdx, QQuickItem *receiverConnector,
                                       TypeHelper::ValueType valueType);

    Q_INVOKABLE bool createOscOutputDevice();


private:
    struct NodesData {
        QQuickItem* view;
        QString container;
        QString oscPath; //TODO: implement
    };
    QHash<QString, NodesData> inputNodes; //TODO: implement?

    QQmlApplicationEngine *m_engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;
    QQuickItem *inputDevicesSidebarView;
    QQuickItem *processingGraphView;

    QHash<QString, QQuickItem*> inputDeviceViews; //store devices here
//    QMap<QString, DeviceDataInput*> inputDevices;
    struct ProcessingNode {
        ProcessNode* controller;
        QList<ProcessNode*> processor;
        QQuickItem* qmlView;
        ProcessingNode(ProcessNode* _viewContr, QQuickItem *_qmlV, QList<ProcessNode*> _processor=QList<ProcessNode*>())
            : controller{_viewContr}, processor{_processor}, qmlView{_qmlV} {}
        ProcessingNode() {}
//        ProcessingNode(DataProcessingNode* _processingObject, ValueViewBackend* _viewBackend) : viewBackend(_viewBackend), processingObject(_processingObject){};
    };
    QHash<QString, ProcessingNode> processingNodes;

    struct ConnectableObject {
        ValueNotifierClass *notifier;
        QQuickItem *qmlView;
        TypeHelper::NodeType nodeType;
        ConnectableObject(ValueNotifierClass *_notifier, QQuickItem *_qmlView, TypeHelper::NodeType _nodeType)
            :notifier{_notifier}, qmlView{_qmlView}, nodeType{_nodeType} {}
        ConnectableObject() {}
    };
    QMap<QString, ConnectableObject> allConnectableObjects;

    struct ValueConnection {
        QQuickItem *connectionView;
        QQuickItem *sourceConnector;
        QQuickItem *receiveConnector;
        ValueConnection(QQuickItem *_conView, QQuickItem *_sourCon, QQuickItem *_recCon)
            : connectionView{_conView}, sourceConnector{_sourCon}, receiveConnector{_recCon} {}
    };
    QMap<QString, ValueConnection> allConnections;

    void createMotionInputDeviceView(MotionDevice *motionDevice);
    void createValueInputViewsForDevice(MotionDevice *motionDevice);

signals:


public slots:
    void createNewInputViews(MotionDevice *motionDevice);


};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

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
#include "oscinputdevice.h"
#include "valuenotifierclass.h" //kann weg
#include "processnode.h"

#include "inputvalueviewcontroller.h"
#include "oscoutputdevice.h"
#include "oscoutputviewcontroller.h"
#include "oscinputviewcontroller.h"
#include "outputnodecontroller.h"
#include "inputnodecontroller.h"
#include "connectionviewcontroller.h"
#include "nodeviewcontroller.h"

#include "typehelper.h"


class MainBackend : public QObject
{
    Q_OBJECT

public:

    explicit MainBackend(QObject *parent = nullptr);

    void setQmlEngine(QQmlApplicationEngine *engine);
    void initialSetup();

    Q_INVOKABLE bool connectionRequest(QString senderNodeId,int sourceIdx, QQuickItem *senderConnector,
                                       QString receiverNodeId, int targetIdx, QQuickItem *receiverConnector,
                                       TypeHelper::ValueType valueType);

    Q_INVOKABLE bool createOscOutputDevice();

//    Q_INVOKABLE bool newNodeDropped(QString mimeData);
    Q_INVOKABLE bool createInputNodeDrop(QPoint atPoint, QString sourceDevice, QString inputPath,
                                         TypeHelper::ValueType valType=TypeHelper::Undefined, int subIdx=-1);
    Q_INVOKABLE bool createProcessingNodeDrop(QPoint atPosition, int processorType);
    Q_INVOKABLE bool createOutputNodeDrop(QPoint atPoint, QString targetDevice, quint16 outputIndex,
                                         TypeHelper::ValueType valType=TypeHelper::Undefined, int subIdx=-1);


    Q_INVOKABLE QString createUniqueId(TypeHelper::NodeType forNodeType);

private:
    struct NodesData {
        QQuickItem* view;
        QString container;
        QString oscPath; //TODO: implement
    };
//    QHash<QString, NodesData> inputNodes; //TODO: implement?

    QQmlApplicationEngine *m_engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;
    QQuickItem *inputDevicesSidebarView;
    QQuickItem *processingGraphView;
    QQuickItem *outputDevcesSidebarView;

    QList<DeviceStatusManager*> deviceManager;

    enum ThreadRole {
        Main, NetIn, NetOut, Process, Audio, Subthread
    };
    QMap<ThreadRole, QThread*> threads;

    ThreadRole threadRoleForNodeType(TypeHelper::NodeType nodeType);


    struct ProcessingNode {
        ProcessNode* controller;
        QList<ProcessNode*> processor;
        NodeViewController* qmlView;
        ProcessingNode(ProcessNode* _viewContr, NodeViewController *_qmlV, QList<ProcessNode*> _processor=QList<ProcessNode*>())
            : controller{_viewContr}, processor{_processor}, qmlView{_qmlV} {}
        ProcessingNode() {}
//        ProcessingNode(DataProcessingNode* _processingObject, ValueViewBackend* _viewBackend) : viewBackend(_viewBackend), processingObject(_processingObject){};
    };
    QHash<QString, ProcessingNode> processingNodes;

    struct InputNode {
        InputNodeController *qmlView;
        QString inputDevice;
        QString inputAddress;
        InputNode(InputNodeController *_qmlView, QString _inputDevice, QString _inputAddress)
            : qmlView{_qmlView}, inputDevice{_inputDevice}, inputAddress{_inputAddress} {}
        InputNode() {}
    };
    QMap<QString, InputNode> inputNodes;

    struct OutputNode {
        OutputNodeController *qmlView;
        //QList<QString> outputDevice;
        QString outputDevice;
        //QList<QString> outputAddress;
        quint16 outputIndex;
        OutputNode(OutputNodeController *_qmlView, QString _outputDevice, quint16 _outputIndex)
            : qmlView{_qmlView}, outputDevice{_outputDevice}, outputIndex{_outputIndex} {}
        OutputNode() {}
    };
    QMap<QString, OutputNode> outputNodes;


    struct ConnectableObject {
        ValueNotifierClass *notifier;
        NodeViewController *qmlView;
        TypeHelper::NodeType nodeType;
        QList<QString> receivingConnections;
        QList<QSet<QString>> sendConnections;
        ConnectableObject(ValueNotifierClass *_notifier, NodeViewController *_qmlView, TypeHelper::NodeType _nodeType)
            :notifier{_notifier}, qmlView{_qmlView}, nodeType{_nodeType} {}
        ConnectableObject() {}
    };
    QMap<QString, ConnectableObject> allConnectableObjects;


    struct ValueConnection {
        QString sourceId;
        int sourceIdx;
        QString receiverId;
        int receiverIdx;
        TypeHelper::ValueType valType;
        ConnectionViewController *connectionView;
        ValueConnection(QString _sourceId, int _sourceIdx,
                        QString _receiverId, int _receiverIdx,
                        TypeHelper::ValueType _valType, ConnectionViewController *_connectionView=nullptr)
            : sourceId{_sourceId}, sourceIdx{_sourceIdx},
              receiverId{_receiverId}, receiverIdx{_receiverIdx},
              valType{_valType}, connectionView{_connectionView} {}
        ValueConnection() {}
    };
    QMap<QString, ValueConnection> allConnections;


    struct OscOutDeviceStruct {
        QQuickItem *view;
        OscOutputDevice *oscSender;
        OscOutputViewController *viewController;

        OscOutDeviceStruct(OscOutputDevice*_oscSender, OscOutputViewController* _viewController ,QQuickItem *_view)
            :oscSender{_oscSender}, viewController{_viewController}, view{_view} {}
        OscOutDeviceStruct(){};
    };
    QMap<QString, OscOutDeviceStruct> oscOutputDevices;

    struct OscInDeviceStruct {
        QString deviceName;
        QQuickItem *view;
        OscInputDevice *oscReceiver;
        OscInputViewController *viewController;
//        QQuickItem *conntainerView;
        QMap<QString, InputValueViewController*> inputViewController;// = QMap<QString, InputValueViewController*>();
        OscInDeviceStruct(QString _deviceName, OscInputDevice*_oscReceiver, OscInputViewController* _viewController ,QQuickItem *_view)
            :deviceName{_deviceName}, oscReceiver{_oscReceiver}, viewController{_viewController}, view{_view} {}
        OscInDeviceStruct(){};
    };
    QMap<QString, OscInDeviceStruct> oscInputDevices;

//    QMetaObject::Connection *pendingRequestConnection;
    QMap<QString, QMetaObject::Connection *> pendingRequestConnections;






signals:
    void inputViewReady();
    void sig_connectRequestFromSender(ValueNotifierClass *sender, TypeHelper::ValueType valType, quint16 nValues=0);
    void deviceWithNameCreated(QString deviceName, QString deviceId);

public slots:

    void createNewMotionDeviceView(QString name, OscInputDevice* newDevice);
    void createSensorViewsForMotionDevice(const QString sendername, const QList<OscInputDevice::OscSensorInputStruct> sensors);
    //QString identifier, TypeHelper::SensorType sensType, ValueNotifierClass *oscHandler);

    void moveObjectToThread(QObject *objToMove, TypeHelper::NodeType nodeType);
    void moveSubprocessorToProcessThread(ProcessNode* processor);

    void newConnectionEstablished();



};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

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
#include "projectfilemanager.h"

#include "typehelper.h"

#include <QtXml>


class MainBackend : public QObject
{
    Q_OBJECT

public:

    explicit MainBackend(QObject *parent = nullptr);

    void setQmlEngine(QQmlApplicationEngine *engine);
    void initialSetup();

    Q_INVOKABLE bool connectionRequest(QString senderNodeId,int sourceIdx,
                                       QString receiverNodeId, int targetIdx,
                                       TypeHelper::ValueType valueType, QString conId="");

    Q_INVOKABLE bool createOscOutputDevice();
    bool createOscOutputDevice(QString uniqueID);

//    Q_INVOKABLE bool newNodeDropped(QString mimeData);
    Q_INVOKABLE bool createInputNodeDrop(QPoint atPoint, QString sourceDevice, QString inputPath,
                                         TypeHelper::ValueType valType=TypeHelper::Undefined, QString id = "", int subIdx=-1);
    Q_INVOKABLE bool createProcessingNodeDrop(QPoint atPosition, TypeHelper::ProcessorType processorType, QString id = "");
    Q_INVOKABLE bool createOutputNodeDrop(QPoint atPoint, QString targetDevice, quint16 outputIndex,
                                         TypeHelper::ValueType valType=TypeHelper::Undefined, QString id = "", int subIdx=-1);

    Q_INVOKABLE bool deleteObjectWithId(const QString uniqueID);
    Q_INVOKABLE bool deleteConnectionWithId(const QString uniqueID);
    Q_INVOKABLE bool deleteSendConnectionsForObjectAtIdx(const QString uniqueID, int idx);
    Q_INVOKABLE bool deleteReceiveConnectionForObjectAtIdx(const QString uniqueID, int idx);


    Q_INVOKABLE QString createUniqueId(TypeHelper::NodeType forNodeType);


    Q_INVOKABLE void saveAsButtonPressed(QUrl fileUrl);
    Q_INVOKABLE void loadButtonPressed(QUrl fileUrl);

    // for debugging
    Q_INVOKABLE void showConnectionLists(QString objectID);

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
    ProjectFileManager *projectManager;

    enum ThreadRole {
        Main, NetIn, NetOut, Process, Audio, Subthread
    };
    QMap<ThreadRole, QThread*> threads;

    ThreadRole threadRoleForNodeType(TypeHelper::NodeType nodeType);


    struct ProcessingNode {
        ProcessNode* controller;
        QList<ProcessNode*> processor;
        NodeViewController* qmlView;
        TypeHelper::ProcessorType procType;
        ProcessingNode(TypeHelper::ProcessorType _procType, ProcessNode* _viewContr, NodeViewController *_qmlV, QList<ProcessNode*> _processor=QList<ProcessNode*>())
            : procType{_procType}, controller{_viewContr}, processor{_processor}, qmlView{_qmlV} {}
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
        const QList<QString> getRcvCon() {return receivingConnections;}
        QList<QSet<QString>> sendConnections;
        ConnectableObject(ValueNotifierClass *_notifier, NodeViewController *_qmlView, TypeHelper::NodeType _nodeType)
            :notifier{_notifier}, qmlView{_qmlView}, nodeType{_nodeType} {}
        ConnectableObject() {}
//        ~ConnectableObject() {
//            qDebug() << "deleting connectable object" << notifier << qmlView;

////            qmlView->deleteLater();
////            notifier->deleteLater();
//        };
    };
    QMap<QString, ConnectableObject> allConnectableObjects;


    struct ValueConnection {
        QString sourceId;
        int sourceIdx;
        void setSourceIdx(int idx) {
            sourceIdx = idx;
            connectionView->setSourceIdx(idx);
        }
        QString receiverId;
        int receiverIdx;
        void setReceiverIdx(int idx) {
            receiverIdx = idx;
            connectionView->setTargetIdx(idx);
        }

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

    void updateConnectionIndices(ConnectableObject &cobject, quint16 startIdx = 0);


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


    void errorWarningCreate(QString id, TypeHelper::NodeType ntype);
//    QMetaObject::Connection *pendingRequestConnection;


// loading from file
signals:
    void requestObjectCategory(TypeHelper::NodeType category);
    void loadedMotionDeviceFromFile(QDomElement element);

public slots:
    void objectDidFinishLoad(QString uniqueID);
    void prepareForNewProject();
    void qDomElementFromFile(QDomElement element, TypeHelper::NodeType type);
    void loadCategoryFinished(TypeHelper::NodeType type);

private:


    QSet<QString> pendingObjectCreation;
    quint16 loadSceneStatus = 0;
    void requestNextCategory();
    QPoint getCreatePosition(QDomElement element);
    QString getAttributeFromViewNode(QDomElement element, QString attr);
    bool categoryPending = false;



signals:
    void inputViewReady();
    void sig_connectRequestFromSender(ValueNotifierClass *sender, TypeHelper::ValueType valType, quint16 nValues=0);
    void deviceWithNameCreated(QString deviceName, QString deviceId);

    void newConnectionRequest(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList = 0);

    void sig_connectionDisconnected();
    void sig_trigger();
    void sig_triggerSaveToFile();
    void sig_increase(int = 1);
    void sig_sendProjectFileUrl(QUrl fileUrl, bool loadIt=false);
    void sig_triggerLoadFromFile();
    void sig_sceneCleared();

    void sendQdomElement(QDomElement dElement);

    void testSendDomNode(QDomNode);


public slots:

    void createNewMotionDeviceView(QString name, OscInputDevice* newDevice, QString uniqueID);
    void createSensorViewsForMotionDevice(const QString sendername, const QList<OscInputDevice::OscSensorInputStruct> sensors);
    //QString identifier, TypeHelper::SensorType sensType, ValueNotifierClass *oscHandler);

    void moveObjectToThread(QObject *objToMove, TypeHelper::NodeType nodeType);
    void moveSubprocessorToProcessThread(ProcessNode* processor);



    //connection related
    void newConnectionEstablished(QString connectionId, bool accepted);

private:
    QMap<QString, ValueConnection> pendingConnectionRequests;


};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

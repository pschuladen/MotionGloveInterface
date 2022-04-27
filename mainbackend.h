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

#include "devicestatuscontroller.h"
#include "devicedatainput.h"
#include "valuenotifierclass.h"

#include "valueviewbackend.h"
#include "dataprocessingnode.h"

class MainBackend : public QObject
{
    Q_OBJECT

public:

    explicit MainBackend(QObject *parent = nullptr);
    typedef ValueViewBackend::ValueViewMode sensType ;

    void setEngine(QQmlApplicationEngine *engine);

    Q_INVOKABLE void createNewProcessingView(DataProcessingNode::ProcessingType type, QPoint point=QPoint(10,10));

private:
    struct NodesData {
        QQuickItem* view;
        QString container;
        QString oscPath; //TODO: implement
    };

    QQmlApplicationEngine *m_engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;
    QQuickItem *inputDevicesSidebarView;
    QQuickItem *processingGraphView;

    QHash<QString, QQuickItem*> inputDevices; //store devices here
    QHash<QString, NodesData> inputNodes; //TODO: implement?
    struct ProcessingNode {
        ValueViewBackend* viewBackend;
        DataProcessingNode* processingObject;
//        ProcessingNode(DataProcessingNode* _processingObject, ValueViewBackend* _viewBackend) : viewBackend(_viewBackend), processingObject(_processingObject){};
    };
    QHash<QString, ProcessingNode> processingNodes;

    void createMotionInputDeviceView(MotionDevice *motionDevice);
    void createValueInputViewsForDevice(MotionDevice *motionDevice);

signals:


public slots:
    void createNewInputViews(MotionDevice *motionDevice);


};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

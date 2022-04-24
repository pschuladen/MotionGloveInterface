#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "Singleton.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlApplicationEngine>

#include "devicestatuscontroller.h"
#include "devicedatainput.h"
#include "valuenotifierclass.h"

#include "valueviewbackend.h"

class MainBackend : public QObject
{
    Q_OBJECT

public:
    explicit MainBackend(QObject *parent = nullptr);
    typedef ValueViewBackend::ValueViewMode sensType ;

    void setEngine(QQmlApplicationEngine *engine);

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

    QHash<QString, QQuickItem*> inputDevices; //store devices here

    QHash<QString, NodesData> inputNodes; //TODO: implement?


    void createMotionInputDeviceView(MotionDevice *motionDevice);
    void createValueInputViewsForDevice(MotionDevice *motionDevice);

signals:


public slots:
    void createNewInputViews(MotionDevice *motionDevice);
};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

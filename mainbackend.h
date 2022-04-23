#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "Singleton.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlApplicationEngine>

#include "devicestatuscontroller.h"
#include "devicedatainput.h"
#include "MotionDeviceStruct.h"
#include "valuenotifierclass.h"
#include "vectorviewbackend.h"
#include "quatviewbackend.h"

#include "valueviewbackend.h"

class MainBackend : public QObject
{
    Q_OBJECT



public:
    explicit MainBackend(QObject *parent = nullptr);

    void setEngine(QQmlApplicationEngine *engine);

private:
    struct NodesData {
        QQuickItem* view;
        QString container;
        QString oscPath; //TODO: implement
    };

    QQmlApplicationEngine *engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;
    QQuickItem *inputDevicesSidebarView;

//    QQuickItem *sensorViewContainer = nullptr;
    QHash<QString, QQuickItem*> inputDevices; //store devices here
    QHash<QString, QQuickItem*> sensorInputContainer; // can be deleted?
    QHash<QString, NodesData> inputNodes;

    void createDeviceStatusView(MotionDevice *motionDevice);
    void createSensorInputViews(MotionDevice *motionDevice);

    enum SenTyp {
        accel, gyro, grav, quat, touch
    };
    void createSensorInputView(QQuickItem *parentView, SenTyp typ, MotionDevice *motionDevice);

    void createMotionInputDeviceView(MotionDevice *motionDevice);
    void createValueInputViewsForDevice(MotionDevice *motionDevice);

    QQuickItem *createSensorViewContainer(MotionDevice * motionDevice);


signals:


public slots:
    void createNewInputViews(MotionDevice *motionDevice);//QString deviceName, DeviceDataInput *inputHandler);

};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

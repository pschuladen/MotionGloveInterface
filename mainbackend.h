#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "Singleton.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlApplicationEngine>

#include "devicestatuscontroller.h"
#include "devicedatainput.h"

class MainBackend : public QObject
{
    Q_OBJECT



public:
    explicit MainBackend(QObject *parent = nullptr);

    void setEngine(QQmlApplicationEngine *engine);

private:

    QQmlApplicationEngine *engine;
    QQuickWindow *mainWindow;
    QQuickItem *deviceStatusView;

    void createInputStatusView(DeviceStatusController::MotionDevice *motionDevice);

signals:


public slots:
    void createNewInputViews(DeviceStatusController::MotionDevice *motionDevice);//QString deviceName, DeviceDataInput *inputHandler);

};

//global variable
typedef Singleton<MainBackend> main_backend;

#endif // MAINBACKEND_H

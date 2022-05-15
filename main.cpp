#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicestatusmanager.h"

#include <QQmlContext>
#include "mainbackend.h"

#include "oscoutputdevice.h"
#include "pn_scale.h"
#include "oscoutputviewcontroller.h"
#include "oscinputviewcontroller.h"


//for test purposes
//#include <QQmlComponent>
//#include <QQuickItem>
//#include "valueviewbackend.h"
//#include <QQuickWindow>
void registerQmlTypes(QQmlApplicationEngine *engine) {

    qmlRegisterType<InputValueViewController>("MotionGloveInterface", 1, 0, "InputValueViewController");
    qmlRegisterType<PN_Scale>("MotionGloveInterface", 1, 0, "PN_Scale");
    qmlRegisterUncreatableType<TypeHelper>("MotionGloveInterface", 1, 0, "TypeHelper", "enum-value types only");
    qmlRegisterType<OscOutputViewController>("MotionGloveInterface", 1, 0, "OscOutputViewController");
    qmlRegisterType<OscInputViewController>("MotionGloveInterface", 1, 0, "OscInputViewController");

//    qmlRegisterType<ProcessNodeController>("MotionGloveInterface", 1, 0, "ProcessNodeController"); //obsolet

    engine->rootContext()->setContextProperty("_mbackend", main_backend::Instance());

    //necessary for calling static functions from typehelper
    TypeHelper typhelper;
    engine->rootContext()->setContextProperty("_typehelper", QVariant::fromValue<TypeHelper>(typhelper));
}


int main(int argc, char *argv[])
{
    //    qputenv("QT_FATAL_WARNINGS","1");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/MotionGloveInterface/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QThread::currentThread()->setObjectName("main thread");

    // Somehow necessary to gt rid of qml-warnings with module. Documentation states this is kind of registration is not necessary ?!
    registerQmlTypes(&engine);

    engine.load(url);
    engine.setObjectName("MAIN_QML_ENGINE");
    qInfo() << "engine thread name" << engine.thread()->objectName();//->setObjectName("qml engine thread");

    main_backend::Instance()->setQmlEngine(&engine);
    main_backend::Instance()->initialSetup();


    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicestatusmanager.h"

#include <QQmlContext>
#include "mainbackend.h"

#include "oscoutputdevice.h"
#include "pn_scale.h"
#include "oscviewcontroller.h"


//for test purposes
//#include <QQmlComponent>
//#include <QQuickItem>
//#include "valueviewbackend.h"
//#include <QQuickWindow>
void registerQmlTypes(QQmlApplicationEngine *engine) {

    qmlRegisterType<InputValueViewController>("MotionGloveInterface", 1, 0, "InputValueViewController");
    qmlRegisterType<PN_Scale>("MotionGloveInterface", 1, 0, "PN_Scale");
    qmlRegisterUncreatableType<TypeHelper>("MotionGloveInterface", 1, 0, "TypeHelper", "enum-value types only");
    qmlRegisterType<OscViewController>("MotionGloveInterface", 1, 0, "OscViewController");
    //necessary for calling static functions from typehelper
    TypeHelper typhelper;
    engine->rootContext()->setContextProperty("_typehelper", QVariant::fromValue<TypeHelper>(typhelper));

    qmlRegisterType<ProcessNodeController>("MotionGloveInterface", 1, 0, "ProcessNodeController"); //obsolet
    engine->rootContext()->setContextProperty("_mbackend", main_backend::Instance());
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

    // Somehow necessary to gt rid of qml-warnings with module. Documentation states this is kind of registration is not necessary ?!

    registerQmlTypes(&engine);

    engine.load(url);
    engine.setObjectName("MAIN_QML_ENGINE");

    main_backend::Instance()->setEngine(&engine);
    main_devicestatus::Instance();

    OscOutputDevice test;



    //    QQmlComponent testValView(&engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));

    //    QQuickItem *newValDevice = qobject_cast<QQuickItem*>(testValView.createWithInitialProperties({{"viewmode", ValueViewBackend::ValueViewMode::Quat},
    //                                                                                                  {"identifier", "testNodeXY"}}));
    //    QQuickWindow *wind = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    //    QQuickItem *rootItem = wind->findChild<QQuickItem*>("mainRootItem");
    //    newValDevice->setParentItem(rootItem);

    return app.exec();
}

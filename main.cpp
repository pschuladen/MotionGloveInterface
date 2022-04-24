#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicestatuscontroller.h"
#include "vectorviewbackend.h"
#include "quatviewbackend.h"
#include <QQmlContext>

#include <QQmlComponent>
#include <QQuickItem>
#include "valueviewbackend.h"
#include <QQuickWindow>

#include "mainbackend.h"

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
    engine.rootContext()->setContextProperty("_mbackend", main_backend::Instance());
    engine.load(url);
    engine.setObjectName("MAIN_QML_ENGINE");

    main_backend::Instance()->setEngine(&engine);
    main_devicestatus::Instance();

//    QQmlComponent testValView(&engine, QUrl(QStringLiteral("qrc:/MotionGloveInterface/SensorInputContainer.qml")));

//    QQuickItem *newValDevice = qobject_cast<QQuickItem*>(testValView.createWithInitialProperties({{"viewmode", ValueViewBackend::ValueViewMode::Quat},
//                                                                                                  {"identifier", "testNodeXY"}}));
//    QQuickWindow *wind = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
//    QQuickItem *rootItem = wind->findChild<QQuickItem*>("mainRootItem");
//    newValDevice->setParentItem(rootItem);

    return app.exec();
}

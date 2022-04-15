#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicestatuscontroller.h"
#include "vectorviewbackend.h"
#include "quatviewbackend.h"

#include "mainbackend.h"

int main(int argc, char *argv[])
{
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
    main_backend::Instance()->setEngine(&engine);
    main_devicestatus::Instance();

    return app.exec();
}

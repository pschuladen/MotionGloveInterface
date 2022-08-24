#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicestatusmanager.h"

#include <QQmlContext>
#include "mainbackend.h"

//#include "oscoutputdevice.h"
#include "pn_scale.h"
#include "pn_sumcomponents.h"
#include "pn_absvalue.h"
#include "pn_diffvalue.h"
#include "pn_splitcomponents.h"
//#include "oscoutputviewcontroller.h"
//#include "oscinputviewcontroller.h"



//for test purposes
//#include <QQmlComponent>
//#include <QQuickItem>
//#include "valueviewbackend.h"
//#include <QQuickWindow>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
     QByteArray localMsg = msg.toLocal8Bit();
     const char *file = context.file ? context.file : "";
     const char *function = context.function ? context.function : "";
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s \n(%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
         break;
     case QtInfoMsg:
         fprintf(stderr, "Info: %s\n\n", localMsg.constData());
//         fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s \n(%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s \n(%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s \n(%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
         break;
     }
 }


void registerQmlTypes(QQmlApplicationEngine *engine) {

    qmlRegisterType<InputValueViewController>("MotionGloveInterface", 1, 0, "InputValueViewController");

    qmlRegisterType<PN_Scale>("MotionGloveInterface", 1, 0, "PN_Scale");
    qmlRegisterType<PN_SumComponents>("MotionGloveInterface", 1, 0, "PN_SumComponents");
    qmlRegisterType<PN_AbsValue>("MotionGloveInterface", 1, 0, "PN_AbsValue");
    qmlRegisterType<PN_DiffValue>("MotionGloveInterface", 1, 0, "PN_DiffValue");
    qmlRegisterType<PN_SplitComponents>("MotionGloveInterface", 1, 0, "PN_SplitComponents");

    qmlRegisterUncreatableType<TypeHelper>("MotionGloveInterface", 1, 0, "TypeHelper", "enum-value types only");
    qmlRegisterType<OscOutputViewController>("MotionGloveInterface", 1, 0, "OscOutputViewController");
    qmlRegisterType<OscInputViewController>("MotionGloveInterface", 1, 0, "OscInputViewController");
    qmlRegisterType<OutputNodeController>("MotionGloveInterface", 1, 0, "OutputNodeController");
    qmlRegisterType<InputNodeController>("MotionGloveInterface", 1, 0, "InputNodeController");
    qmlRegisterType<NodeViewController>("MotionGloveInterface", 1, 0, "NodeViewController");
    qmlRegisterType<ConnectionViewController>("MotionGloveInterface", 1, 0, "ConnectionViewController");

//    qmlRegisterType<ProcessNodeController>("MotionGloveInterface", 1, 0, "ProcessNodeController"); //obsolet

    engine->rootContext()->setContextProperty("_mbackend", main_backend::Instance());
    qWarning();

    //necessary for calling static functions from typehelper
    TypeHelper typhelper;
    engine->rootContext()->setContextProperty("_typehelper", QVariant::fromValue<TypeHelper>(typhelper));
}


int main(int argc, char *argv[])
{
    //    qputenv("QT_FATAL_WARNINGS","1");
    qInstallMessageHandler(myMessageOutput);

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

    main_backend::Instance()->setQmlEngine(&engine);
    main_backend::Instance()->initialSetup();


//    main_backend::Instance()->loadButtonPressed(QUrl("file:///Users/psch/Desktop/saveTest/b.xml"));
    return app.exec();
}

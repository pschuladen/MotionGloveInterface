#ifndef OSCOUTPUTDEVICE_H
#define OSCOUTPUTDEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "oscpp/client.hpp"
//#include "valuenotifierclass.h"
#include "oscviewcontroller.h"

class OscOutputDevice : public OscViewController
{
    Q_OBJECT

public:
    explicit OscOutputDevice(QObject *parent = nullptr);

    bool setViewControllerObject(OscViewController* viewController);

private:
    bool m_bindSocket;

    QUdpSocket *socket;
    uint16_t destinationPort;
    QHostAddress destinationAddress;

    OscViewController *m_viewController;

//    QList<QString> oscPaths;

//    QList<QVariant> values;

    struct OscOutput {
        QString ocsAddress;
        QByteArray typeTags;
    };
    QList<OscOutput> outputPaths;

public slots:
    virtual void addOscPath() override;

signals:

};

#endif // OSCOUTPUTDEVICE_H

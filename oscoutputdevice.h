#ifndef OSCOUTPUTDEVICE_H
#define OSCOUTPUTDEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "oscpp/client.hpp"

class OscOutputDevice : public QObject
{
    Q_OBJECT
public:
    explicit OscOutputDevice(QObject *parent = nullptr);

private:
    bool m_bindSocket;

    QUdpSocket *socket;
    uint16_t destinationPort;
    QHostAddress destinationAddress;

//    QList<QString> oscPaths;

//    QList<QVariant> values;

    struct OscOutputPaths {
        QString ocsAddress;
        QByteArray typeTags;
    };

signals:

};

#endif // OSCOUTPUTDEVICE_H

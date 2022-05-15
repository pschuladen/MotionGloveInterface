#ifndef OSCOUTPUTDEVICE_H
#define OSCOUTPUTDEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "oscpp/client.hpp"
//#include "valuenotifierclass.h"
#include "oscoutputviewcontroller.h"
#include "oscpacketbuilder.h"

class OscOutputDevice : public OscOutputViewController
{
    Q_OBJECT

public:
    explicit OscOutputDevice(QObject *parent = nullptr);
    explicit OscOutputDevice(OscOutputViewController *viewController, QObject *parent = nullptr);

    bool setViewControllerObject(OscOutputViewController* viewController);

private:
    void initialiseOscDevice();

    bool m_bindSocket;

    QUdpSocket *socket;

    QHostAddress m_destinationAddress;

    OscOutputViewController *m_viewController;

    QList<OscPacketBuilder*> packetBuilder;

public slots:
    virtual void addOscPath() override;

signals:


    // OscViewController interface
public slots:
    void setDestIp(const QString &newDestIp) override;
    void setDestPort(quint16 newDestPort) override;
    void setOscPaths(const QList<QString> &newOscPaths) override;
    void setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes) override;
    void addOscPath(QString newPath) override;
    void slot_addOscPath();
    void setOscPathAtIndex(QString newPath, quint32 idx) override;

    void newConnectionAtIndex(int idx, TypeHelper::ValueType valueType) override;
    void sendOscMsgBuffer(const QByteArray &oscBuffer, size_t msgSize, int frame=-1 );

    // ValueNotifierClass interface
public:
    ValueNotifierClass *getNotifier(int idx) override;
    const QHostAddress &destinationAddress() const;
};

#endif // OSCOUTPUTDEVICE_H

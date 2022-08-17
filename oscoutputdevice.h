#ifndef OSCOUTPUTDEVICE_H
#define OSCOUTPUTDEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "oscpp/client.hpp"
//#include "valuenotifierclass.h"
#include "oscoutputviewcontroller.h"
#include "oscpacketbuilder.h"
#include "saveloadinterfaceclass.h"

class OscOutputDevice : public OscOutputViewController, public SaveLoadInterfaceClass
{
    Q_OBJECT

public:
    explicit OscOutputDevice(QObject *parent = nullptr);
    explicit OscOutputDevice(QString uniqueID, OscOutputViewController *viewController, QObject *parent = nullptr);

    bool setViewControllerObject(OscOutputViewController* viewController);

private:


    bool m_bindSocket;

    QUdpSocket *socket;

    QHostAddress m_destinationAddress;

    OscOutputViewController *m_viewController;

    QList<OscPacketBuilder*> packetBuilder;

    static inline QString ps_destinationPort = "destinationPort";
    static inline QString ps_destinationAddress = "destinationAddress";
    static inline QString ps_oscPaths = "oscPaths";

public slots:
    virtual void addOscPath() override;

signals:
    void sig_sendNotiferPtr(ValueNotifierClass* notifier, QString nodId);

    void sig_deleteOscOutputView(QString outDevice, int idx);

    // OscViewController interface
public slots:
    void initialiseOscDevice();

    void setDestIp(const QString &newDestIp) override;
    void setDestPort(quint16 newDestPort) override;
    void setOscPaths(const QList<QString> &newOscPaths) override;
    void setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes) override;
    void addOscPath(QString newPath) override;
    void slot_addOscPath();
    void setOscPathAtIndex(QString newPath, quint32 idx) override;
    void slot_deleteOscPathAtIdx(int idx);

    void newConnectionAtIndex(int idx, TypeHelper::ValueType valueType) override;
    void sendOscMsgBuffer(const QByteArray oscBuffer, size_t msgSize, int frame=-1 );

    void getNotifierRequest(int atIdx, QString nodeId);

    // ValueNotifierClass interface
public:
    ValueNotifierClass *getNotifier(int idx) override;
    const QHostAddress &destinationAddress() const;

    // SaveLoadInterfaceClass interface
signals:
//    void sendPropertyMapElement(QString uniqueID, QString subtype, QVariantMap) override;
    void announceAdditionalData(int add = 1) override;
//    void registerSaveableObject(QObject *obj) override;
    void sendSubNodeTree(QString uniqueID, QDomDocument subTreeDocument) override;
    void didFinishLoad(QString uniqueID) override;

    // SaveLoadInterfaceClass interface
public slots:
    void initSaveData() override;
    void loadDataFromQdomElement(QDomElement domElement) override;
};

#endif // OSCOUTPUTDEVICE_H

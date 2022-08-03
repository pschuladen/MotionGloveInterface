#ifndef OSCPACKETBUILDER_H
#define OSCPACKETBUILDER_H

#include "valuenotifierclass.h"
#include "oscpp/client.hpp"

class OscPacketBuilder : public ValueNotifierClass
{
    Q_OBJECT
    Q_PROPERTY(quint32 oscMessIdx READ oscMessIdx WRITE setOscMessIdx NOTIFY oscMessIdxChanged)
    Q_PROPERTY(QByteArray oscAddress READ oscAddress WRITE setOscAddress NOTIFY oscAddressChanged)

public:
    explicit OscPacketBuilder(QObject *parent = nullptr);
    explicit OscPacketBuilder(QByteArray addr, quint32 idx=0, QObject *parent = nullptr);


    const QByteArray &oscAddress() const;
    void setOscAddress(const QByteArray &newOscAddress);

    quint32 oscMessIdx() const;
    void setOscMessIdx(quint32 newOscMessIdx);

    bool valueInputConnected = false;

signals:
    void oscMessageBufferReady(const QByteArray oscBuffer, size_t msgSize, int frame=-1);

    void oscAddressChanged(QByteArray oscAddress);
    void oscMessIdxChanged(quint32 msgIdx);

private:
    QByteArray m_oscAddress;
    quint32 m_oscMessIdx;

    QByteArray m_msgBuffer;
    OSCPP::Client::Packet m_oscPacket;

    size_t m_reservedSize;
    qsizetype m_nValuesInMsg = 0;



    size_t valueListSize = 0;

    // ValueNotifierClass interface
public:


    size_t reservedSize() const;

    qsizetype nValuesInMsg() const;

    bool acceptsInputType(TypeHelper::ValueType typ, int atIdx=0) const override;

public slots:
    void slot_quatChanged(QQuaternion quat, int frame) override;
    void slot_vectorChanged(QVector3D vect, int frame) override;
    void slot_touchChanged(QList<float> touch, int frame) override;
    void slot_valuesChanged(QList<float> values, int frame) override;
    void slot_singleValueChanged(float value, int frame) override;
    void slot_boolValueChanged(bool value, int frame) override;
    void slot_trigger(int frame) override;

    int newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList) override;
    void inputsDisconnected() override;


    void outputNodeWasDropped();
    void setMsgBufferSize();

signals:
    void gotNewConnectionWithType(int idx, TypeHelper::ValueType vtype);

    // ValueNotifierClass interface
protected:


    // ValueNotifierClass interface
public slots:
    void setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType) override;
};

#endif // OSCPACKETBUILDER_H

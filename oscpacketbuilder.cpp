#include "oscpacketbuilder.h"

OscPacketBuilder::OscPacketBuilder(QObject *parent)
    : ValueNotifierClass{parent}
{
    setOscAddress("/u");
    setOscMessIdx(0);
}

OscPacketBuilder::OscPacketBuilder(QByteArray addr, quint32 idx, QObject *parent)
    : ValueNotifierClass{parent},
      m_oscAddress{addr}, m_oscMessIdx{idx}
{
}

const QByteArray &OscPacketBuilder::oscAddress() const
{
    return m_oscAddress;
}

void OscPacketBuilder::setOscAddress(const QByteArray &newOscAddress)
{
    if (m_oscAddress == newOscAddress)
        return;
    m_oscAddress = newOscAddress;
    emit oscAddressChanged();
}

quint32 OscPacketBuilder::oscMessIdx() const
{
    return m_oscMessIdx;
}

void OscPacketBuilder::setOscMessIdx(quint32 newOscMessIdx)
{
    if (m_oscMessIdx == newOscMessIdx)
        return;
    m_oscMessIdx = newOscMessIdx;
    emit oscMessIdxChanged();
}

qsizetype OscPacketBuilder::nValuesInMsg() const
{
    return m_nValuesInMsg;
}

size_t OscPacketBuilder::reservedSize() const
{
    return m_reservedSize;
}

size_t OscPacketBuilder::setMsgBufferSize(size_t valueSize)
{
    //TODO: implement different list sizes
    qsizetype _addrSize = m_oscAddress.size();
    qsizetype _size = _addrSize + TypeHelper::getSizeForValueType(connectedValueType())*4 + 60 + valueListSize; //+10 as safety.. TODO: das macht noch nicht so viel Sinn
    if(connectedValueType() == TypeHelper::List) m_nValuesInMsg = valueListSize;
    else m_nValuesInMsg = TypeHelper::getSizeForValueType(connectedValueType());
    m_msgBuffer.resize(_size);
    m_reservedSize = _size;

    m_oscPacket.reset(m_msgBuffer.data(), _size);
    return _size;
}


bool OscPacketBuilder::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    if (valueInputConnected) return false;

    typedef TypeHelper::ValueType _vt;
    if(type == _vt::BoolValue || type==_vt::Undefined || type==_vt::Trigger ) {
        unimplementedValueTypeWarning(type);
        return false;
    }
//    qDebug() << "connection request in thread" << QThread::currentThread();
//    qDebug() << "this object thread" << this->thread();

    setConnectedValueType(type, false);
    valueInputConnected = true;
    valueListSize = nValuesInList;
    setMsgBufferSize();

    emit gotNewConnectionWithType(oscMessIdx(), type);
    connectValueTypeSignalToSlot(sender, this, type);

    return true;
}

void OscPacketBuilder::slot_quatChanged(QQuaternion quat, int frame)
{
    m_oscPacket.reset();
    m_oscPacket.openMessage(oscAddress(), nValuesInMsg())
            .float32(quat.x())
            .float32(quat.y())
            .float32(quat.z())
            .float32(quat.scalar())
            .closeMessage();
    emit oscMessageBufferReady(m_msgBuffer, m_oscPacket.size(), frame);
}

void OscPacketBuilder::slot_vectorChanged(QVector3D vect, int frame)
{
    m_oscPacket.reset();

    m_oscPacket.openMessage(oscAddress(), nValuesInMsg())
            .float32(vect.x())
            .float32(vect.y())
            .float32(vect.z())
            .closeMessage();
    emit oscMessageBufferReady(m_msgBuffer, m_oscPacket.size(), frame);
}

void OscPacketBuilder::slot_touchChanged(QList<float> touch, int frame)
{
    m_oscPacket.reset();
    m_oscPacket.openMessage(oscAddress(), nValuesInMsg());
    for(int i = 0; i < touch.size(); i++) {
        m_oscPacket.float32(touch[i]);
    }
    m_oscPacket.closeMessage();
    emit oscMessageBufferReady(m_msgBuffer, m_oscPacket.size(), frame);
}

void OscPacketBuilder::slot_valuesChanged(QList<float> values, int frame)
{
    m_oscPacket.reset();
    m_oscPacket.openMessage(oscAddress(), nValuesInMsg());
    for(int i = 0; i < values.size(); i++) {
        m_oscPacket.float32(values[i]);
    }
    m_oscPacket.closeMessage();
    emit oscMessageBufferReady(m_msgBuffer, m_oscPacket.size(), frame);
}

void OscPacketBuilder::slot_singleValueChanged(float value, int frame)
{
    m_oscPacket.reset();
    m_oscPacket.openMessage(oscAddress(), nValuesInMsg())
            .float32(value).closeMessage();
    emit oscMessageBufferReady(m_msgBuffer, m_oscPacket.size(), frame);
}

void OscPacketBuilder::slot_boolValueChanged(bool value, int frame)
{
    //TODO: implement
    unimplementedValueTypeWarning(TypeHelper::BoolValue);
}

void OscPacketBuilder::slot_trigger(int frame)
{
    //TODO: implement
    unimplementedValueTypeWarning(TypeHelper::Trigger);
}

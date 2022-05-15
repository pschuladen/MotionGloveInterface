#include "processnode.h"

ProcessNode::ProcessNode(QObject *parent)
    :ValueNotifierClass(parent)
{
}

bool ProcessNode::acceptsInputType(TypeHelper::ValueType typ)
{
    if(typ == TypeHelper::Undefined) return false;
    else return true;
}

bool ProcessNode::setConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    if(!acceptsInputType(type) && m_connectedValueType != TypeHelper::Undefined) return false;

    setValueNumber(nValuesInList);
    setConnectedValueType(type);

    return connectValueTypeSignalToSlot(sender, this, type);
}

float ProcessNode::process(float value)
{
    return value;
}

quint8 ProcessNode::process(quint8 value)
{
    return value;
}


bool ProcessNode::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

bool ProcessNode::connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

ProcessNode *ProcessNode::createProcessControl(QString objectname_id)
{
    return nullptr;
}

ProcessNode *ProcessNode::createSubprocessor(QString objectname_id)
{
    return nullptr;
}

void ProcessNode::slot_quatChanged(QQuaternion quat, int frame)
{
    if(m_connectedValueType == TypeHelper::Quat) {
        quat.setX(process(quat.x()));
        quat.setY(process(quat.y()));
        quat.setZ(process(quat.z()));
        quat.setScalar(process(quat.scalar()));
        callQuatChanged(quat, frame);
    }
}

void ProcessNode::slot_vectorChanged(QVector3D vect, int frame)
{
    if(m_connectedValueType == TypeHelper::Vector) {
        vect.setX(process(vect.x()));
        vect.setY(process(vect.y()));
        vect.setZ(process(vect.z()));
        qInfo() << "vector print" << vect;
        callVectorChanged(vect, frame);
    }
}

void ProcessNode::slot_touchChanged(QList<float> touch, int frame)
{
    if(m_connectedValueType == TypeHelper::List) {
        for(int i = 0; i < touch.size(); i++) {
            touch[i] = process(touch[i]);
        }
        callTouchChanged(touch, frame);
    }
}

void ProcessNode::slot_valuesChanged(QList<float> values, int frame)
{
    if(m_connectedValueType == TypeHelper::List) {
        for(int i = 0; i < values.size(); i++) {
            values[i] = process(values[i]);
        }
        callValuesChanged(values, frame);
    }
}

void ProcessNode::slot_singleValueChanged(float value, int frame)
{
    if(m_connectedValueType == TypeHelper::SingleValue) {
        emit singleValueChanged(process(value), frame);
    }
}

void ProcessNode::slot_boolValueChanged(bool value, int frame)
{

}

void ProcessNode::slot_trigger(int frame)
{

}

const QList<TypeHelper::ValueType> &ProcessNode::connectedTypes() const
{
    return m_connectedTypes;
}

void ProcessNode::setConnectedTypes(const QList<TypeHelper::ValueType> &newConnectedTypes)
{
    if (m_connectedTypes == newConnectedTypes)
        return;
    m_connectedTypes = newConnectedTypes;
    emit connectedTypesChanged(m_connectedTypes);
}

void ProcessNode::appendToConnectedTypes(TypeHelper::ValueType appendType)
{
    m_connectedTypes.append(appendType);
    emit connectedTypesChanged(m_connectedTypes);
}

ValueNotifierClass *ProcessNode::getNotifier(int idx)
{
    qInfo() << this << "getNotifier in ProcessNode" << idx;
    qInfo() << subProcessor;
    if(idx < 0) return this;
    else if(idx < subProcessor.size()) return subProcessor.at(idx);
    else return nullptr;
}



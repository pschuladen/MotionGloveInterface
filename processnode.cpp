#include "processnode.h"

ProcessNode::ProcessNode(QObject *parent)
    :ValueNotifierClass(parent)
{
}

ProcessNode::ProcessNode(QByteArray identifier, int idxInController, TypeHelper::ValueType valType, quint16 valueNumber, QObject *parent)
    : ValueNotifierClass{identifier, idxInController, valType, valueNumber, parent}
{

}

bool ProcessNode::acceptsInputType(TypeHelper::ValueType typ, int atIdx) const
{
    if(typ == TypeHelper::Undefined) return false;
    else return true;
}

bool ProcessNode::setConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    if(!acceptsInputType(type)) return false;

    setValueNumber(nValuesInList);
    setConnectedValueType(type);

    emit connectedTypeAtIdxChanged(indexInObject(), type);
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


int ProcessNode::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList)
{
    return -1;
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

void ProcessNode::setConnectedTypesAtIdx(int idx, TypeHelper::ValueType valType)
{
    if(idx < connectedTypes().size()) {
        m_connectedTypes[idx] = valType;
        emit connectedTypesChanged(m_connectedTypes);
    }
}

void ProcessNode::appendToConnectedTypes(TypeHelper::ValueType appendType)
{
    m_connectedTypes.append(appendType);
    emit connectedTypesChanged(m_connectedTypes);
}

ValueNotifierClass *ProcessNode::getNotifier(int idx)
{
    return this;
//    if(idx < 0) return this;
//    else if(idx < subProcessor.size()) return subProcessor.at(idx);
//    else return nullptr;
}

void ProcessNode::deleteSubprocessorAtIdx(quint16 idx)
{
    qDebug() << this << subProcessor;
    subProcessor.at(idx)->deleteLater();
    subProcessor.removeAt(idx);
    m_connectedTypes.removeAt(idx);
    emit connectedTypesChanged(connectedTypes());
}




void ProcessNode::initSaveData()
{
}

void ProcessNode::loadDataFromQdomElement(QDomElement domElement)
{
}
TypeHelper::ProcessorType ProcessNode::processorType() const
{
    return m_processorType;
}

void ProcessNode::setProcessorType(TypeHelper::ProcessorType newProcessorType)
{
    if (m_processorType == newProcessorType)
        return;
    m_processorType = newProcessorType;
    emit processorTypeChanged();
}


void ProcessNode::connectionRequestFromSender(ValueNotifierClass *sender, QString connectionId, TypeHelper::ValueType type, int connectToIdx, quint16 nValuesInList)
{

    if(!acceptsInputType(type)) {
        emit connectionAccepted(connectionId, false);
    }
    if(connectToIdx < 0)
    while(connectToIdx >= subProcessor.size()) {
        subProcessor.append(createSubprocessor(identifier()));
        appendToConnectedTypes(TypeHelper::Undefined);
    }
    connect(this, &ProcessNode::sig_connectToSender, subProcessor[connectToIdx], &ProcessNode::slot_connectToSender, Qt::SingleShotConnection);
    connect(subProcessor[connectToIdx], &ProcessNode::connectionAccepted, this, &ProcessNode::connectionAccepted, Qt::SingleShotConnection);
    emit sig_connectToSender(sender, connectionId, type);
}

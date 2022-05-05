#include "processnode.h"

ProcessNode::ProcessNode(QObject *parent)
{
    setParent(parent);
}

bool ProcessNode::acceptsInputType(TypeHelper::ValueType typ)
{
    if(typ == TypeHelper::Undefined) return false;
    else return true;
}

bool ProcessNode::setConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    if(!acceptsInputType(type) && connectedValueType != TypeHelper::Undefined) return false;

    connectedValueType = type;
    createSubnotifier(type);

    switch(type) {
    case TypeHelper::Undefined:
        return false;
        break;
    case TypeHelper::Vector:
        connect(sender, &ValueNotifierClass::vectorChanged, this, &ProcessNode::slot_vectorChanged);
        break;
    case TypeHelper::Quat:
        connect(sender, &ValueNotifierClass::quatChanged, this, &ProcessNode::slot_quatChanged);
        break;
    case TypeHelper::List:
        connect(sender, &ValueNotifierClass::valuesChanged, this, &ProcessNode::slot_valuesChanged);
        break;
    case TypeHelper::SingleValue:
        connect(sender, &ValueNotifierClass::singleValueChanged, this, &ProcessNode::slot_singleValueChanged);
        break;
    case TypeHelper::BoolValue:
        connect(sender, &ValueNotifierClass::boolValueChanged, this, &ProcessNode::slot_boolValueChanged);
        break;
    case TypeHelper::Trigger:
        connect(sender, &ValueNotifierClass::triggerActivated, this, &ProcessNode::slot_trigger);
        break;
    }
    return true;
}

float ProcessNode::process(float value)
{
    return value;
}

void ProcessNode::slot_quatChanged(QQuaternion quat, int frame)
{
    if(connectedValueType == TypeHelper::Quat) {
        quat.setX(process(quat.x()));
        quat.setY(process(quat.y()));
        quat.setZ(process(quat.z()));
        quat.setScalar(process(quat.scalar()));
        callQuatChanged(quat, frame);
    }
}

void ProcessNode::slot_vectorChanged(QVector3D vect, int frame)
{
    if(connectedValueType == TypeHelper::Vector) {
        vect.setX(process(vect.x()));
        vect.setY(process(vect.y()));
        vect.setZ(process(vect.z()));
        callVectorChanged(vect, frame);
    }
}

void ProcessNode::slot_touchChanged(QList<float> touch, int frame)
{
    if(connectedValueType == TypeHelper::List) {
        for(int i = 0; i < touch.size(); i++) {
            touch[i] = process(touch[i]);
        }
        callTouchChanged(touch, frame);
    }
}

void ProcessNode::slot_valuesChanged(QList<float> values, int frame)
{
    if(connectedValueType == TypeHelper::List) {
        for(int i = 0; i < values.size(); i++) {
            values[i] = process(values[i]);
        }
        callValuesChanged(values, frame);
    }
}

void ProcessNode::slot_singleValueChanged(float value, int frame)
{
    if(connectedValueType == TypeHelper::SingleValue) {
        emit singleValueChanged(process(value), frame);
    }
}

void ProcessNode::slot_boolValueChanged(bool value, int frame)
{

}

void ProcessNode::slot_trigger(int frame)
{

}

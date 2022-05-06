#include "pn_scale.h"

PN_Scale::PN_Scale(QObject *parent)
{
    setParent(parent);
}

PN_Scale::PN_Scale(TypeHelper::ValueType type, QObject *parent)
{
    setParent(parent);
    setConnectedValueType(type);

}

float PN_Scale::inLow() const
{
    return m_inLow;
}

void PN_Scale::setInLow(float newInLow)
{
    qInfo() << "setInLow was called" << newInLow;
    if (qFuzzyCompare(m_inLow, newInLow))
        return;
    m_inLow = newInLow;
    evalMultip();
    emit inLowChanged(newInLow);
}

float PN_Scale::inHigh() const
{
    return m_inHigh;
}

void PN_Scale::setInHigh(float newInHigh)
{
    if (qFuzzyCompare(m_inHigh, newInHigh))
        return;
    m_inHigh = newInHigh;
    evalMultip();
    emit inHighChanged(newInHigh);
}

float PN_Scale::outLow() const
{
    return m_outLow;
}

void PN_Scale::setOutLow(float newOutLow)
{
    if (qFuzzyCompare(m_outLow, newOutLow))
        return;
    m_outLow = newOutLow;
    evalMultip();
    emit outLowChanged(newOutLow);
}

float PN_Scale::outHigh() const
{
    return m_outHigh;
}

void PN_Scale::setOutHigh(float newOutHigh)
{
    if (qFuzzyCompare(m_outHigh, newOutHigh))
        return;
    m_outHigh = newOutHigh;
    evalMultip();
    emit outHighChanged(newOutHigh);
}

bool PN_Scale::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    if(!acceptsInputType(type)) return false;

    PN_Scale *newSub = new PN_Scale(type, this);
    setInitialProperties(this, newSub);
    connectPropertiesToProcessor(this, newSub);
    subProcessor.append(newSub);

    return true;
}

bool PN_Scale::connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

ProcessNode *PN_Scale::createProcessControl(QString objectname_id)
{
    PN_Scale *scale = new PN_Scale();
    scale->setObjectName(objectname_id+"-processcontroller");
    setInitialProperties(this, scale);
    connectPropertiesToProcessor(this, scale);
    return qobject_cast<ProcessNode*>(scale);
}

float PN_Scale::process(float value)
{
    if(clipOutput()) {
//        return qBound(((value - inLow()) * multi()) + outLow());
        float result = ((value - inLow()) * multi()) + outLow();
        if(result < outLow()) return outLow();
        else if(result > outHigh()) return outHigh();
        else return result;

    }
    return ((value-inLow()) * multi()) + outLow();
}

quint8 PN_Scale::process(quint8 value)
{
    return value;
}

void PN_Scale::evalMultip()
{
    float inputDiff = inHigh()-inLow();
    if(inputDiff==0) inputDiff = 1;
    setMulti((outHigh()-outLow()) / (inputDiff));
    qInfo() << "multipli" << multi();
}

float PN_Scale::multi() const
{
    return m_multi;
}



void PN_Scale::setMulti(float newMulti)
{
    m_multi = newMulti;
}

bool PN_Scale::acceptsInputType(TypeHelper::ValueType typ)
{

        if(typ == TypeHelper::Undefined || typ == TypeHelper::Quat) return false;
        else return true;

}

void PN_Scale::connectPropertiesToProcessor(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
    connect(propertyMaster, &PN_Scale::inLowChanged, propertySlave, &PN_Scale::setInLow);
    connect(propertyMaster, &PN_Scale::inHighChanged, propertySlave, &PN_Scale::setInHigh);
    connect(propertyMaster, &PN_Scale::outLowChanged, propertySlave, &PN_Scale::setOutLow);
    connect(propertyMaster, &PN_Scale::outHighChanged, propertySlave, &PN_Scale::setOutHigh);
    connect(propertyMaster, &PN_Scale::clipOutputChanged, propertySlave, &PN_Scale::setClipOutput);
}

void PN_Scale::setInitialProperties(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
//    propertySlave->setProperty()

    propertySlave->setInLow(propertyMaster->inLow());
    propertySlave->setInHigh(propertyMaster->inHigh());
    propertySlave->setOutLow(propertyMaster->outLow());
    propertySlave->setOutHigh(propertyMaster->outHigh());
    propertySlave->setClipOutput(propertyMaster->clipOutput());
}

bool PN_Scale::clipOutput() const
{
    return m_clipOutput;
}

void PN_Scale::setClipOutput(bool newClipOutput)
{
    if (m_clipOutput == newClipOutput)
        return;
    m_clipOutput = newClipOutput;
    emit clipOutputChanged(newClipOutput);
}

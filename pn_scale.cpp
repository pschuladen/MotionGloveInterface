#include "pn_scale.h"

PN_Scale::PN_Scale(QObject *parent)
{
    setParent(parent);
}

float PN_Scale::inLow() const
{
    return m_inLow;
}

void PN_Scale::setInLow(float newInLow)
{
    if (qFuzzyCompare(m_inLow, newInLow))
        return;
    m_inLow = newInLow;
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
    emit outHighChanged(newOutHigh);
}

bool PN_Scale::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList)
{
    PN_Scale *newSub = new PN_Scale(this);
    setInitialProperties(this, newSub);
    connectPropertiesToProcessor(this, newSub);
    subProcessor.append(newSub);

    return true;
}

bool PN_Scale::connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList)
{
    return false;
}

float PN_Scale::process(float value)
{
    return value;
}

quint8 PN_Scale::process(quint8 value)
{
    return value;
}

void PN_Scale::connectPropertiesToProcessor(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
    connect(propertyMaster, &PN_Scale::inLowChanged, propertySlave, &PN_Scale::setInLow);
    connect(propertyMaster, &PN_Scale::inHighChanged, propertySlave, &PN_Scale::setInHigh);
    connect(propertyMaster, &PN_Scale::outLowChanged, propertySlave, &PN_Scale::setOutLow);
    connect(propertyMaster, &PN_Scale::outHighChanged, propertySlave, &PN_Scale::setOutHigh);
}

void PN_Scale::setInitialProperties(PN_Scale *propertyMaster, PN_Scale *propertySlave)
{
    propertySlave->setInLow(propertyMaster->inLow());
    propertySlave->setInHigh(propertyMaster->inHigh());
    propertySlave->setOutLow(propertyMaster->outLow());
    propertySlave->setOutHigh(propertyMaster->outHigh());
}

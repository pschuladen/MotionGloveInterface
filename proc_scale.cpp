#include "proc_scale.h"

Proc_Scale::Proc_Scale(QObject *parent, bool isProcessing)
{
    setParent(parent);
    m_processType = ProcessingType::Scale;
}

//QVector3D Proc_Scale::processVector(QVector3D vector, int frame)
//{
//    Q_UNUSED(frame)
//    return vector;
//}

//QQuaternion Proc_Scale::processQuat(QQuaternion quat, int frame)
//{
//    Q_UNUSED(frame)
//    return quat;
//}

//float Proc_Scale::processSingleValue(float value, int frame)
//{
//    Q_UNUSED(frame)
//    return value;
//}

//QList<float> Proc_Scale::processValueList(QList<float> values, int frame)
//{
//    Q_UNUSED(frame)
//    return values;
//}

float Proc_Scale::inLow() const
{
    return m_inLow;
}

void Proc_Scale::setInLow(float newInLow)
{
    if (qFuzzyCompare(m_inLow, newInLow))
        return;
    m_inLow = newInLow;
    emit inLowChanged();
}

float Proc_Scale::inHigh() const
{
    return m_inHigh;
}

void Proc_Scale::setInHigh(float newInHigh)
{
    if (qFuzzyCompare(m_inHigh, newInHigh))
        return;
    m_inHigh = newInHigh;
    emit inHighChanged();
}

float Proc_Scale::outLow() const
{
    return m_outLow;
}

void Proc_Scale::setOutLow(float newOutLow)
{
    if (qFuzzyCompare(m_outLow, newOutLow))
        return;
    m_outLow = newOutLow;
    emit outLowChanged();
}

float Proc_Scale::outHigh() const
{
    return m_outHigh;
}

void Proc_Scale::setOutHigh(float newOutHigh)
{
    if (qFuzzyCompare(m_outHigh, newOutHigh))
        return;
    m_outHigh = newOutHigh;
    emit outHighChanged();
}

float Proc_Scale::simpleProcessingFunction(float value)
{
return value;
}

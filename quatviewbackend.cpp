#include "quatviewbackend.h"

QuatViewBackend::QuatViewBackend(QObject *parent)
    : QObject{parent}
{
    this->quatPointer = new QQuaternion(1, 0,0,0);

    setVname(QList<QString>({"hallo", "mr", "einstein", "undso"}));

}

void QuatViewBackend::setQuatPointer(QQuaternion *newQuatPointer)
{
    quatPointer = newQuatPointer;
}

void QuatViewBackend::vectorUpdatet()
{
    emit xValueChanged();
    emit yValueChanged();
    emit zValueChanged();
    emit wValueChanged();
}

void QuatViewBackend::quatChanged(QQuaternion newQuat)
{
//    qInfo() << "received quat" << newQuat;
    quat = newQuat;
    emit xValueChanged();
    emit yValueChanged();
    emit zValueChanged();
    emit wValueChanged();
}


float QuatViewBackend::wValue() const
{
    return quat.scalar();
//    return quatPointer->scalar();//m_wValue;
}

void QuatViewBackend::setwValue(float newWValue)
{
    if (qFuzzyCompare(m_wValue, newWValue))
        return;
    m_wValue = newWValue;
    emit wValueChanged();
}

float QuatViewBackend::xValue() const
{
    return quat.x();
//    return quatPointer->x();//m_xValue;
}

void QuatViewBackend::setxValue(float newXValue)
{
    if (qFuzzyCompare(m_xValue, newXValue))
        return;
    m_xValue = newXValue;
    emit xValueChanged();
}

float QuatViewBackend::yValue() const
{
    return quat.y();
//    return quatPointer->y();//m_yValue;
}

void QuatViewBackend::setyValue(float newYValue)
{
    if (qFuzzyCompare(m_yValue, newYValue))
        return;
    m_yValue = newYValue;
    emit yValueChanged();
}

float QuatViewBackend::zValue() const
{
    return quat.z();
//    return quatPointer->z();//m_zValue;
}

void QuatViewBackend::setzValue(float newZValue)
{
    if (qFuzzyCompare(m_zValue, newZValue))
        return;
    m_zValue = newZValue;
    emit zValueChanged();
}

const QList<QString> &QuatViewBackend::vname() const
{
    return m_vname;
}

void QuatViewBackend::setVname(const QList<QString> &newVname)
{
    if (m_vname == newVname)
        return;
    m_vname = newVname;
    emit vnameChanged();
}

#include "vectorviewbackend.h"

VectorViewBackend::VectorViewBackend(QObject *parent)
    : QObject{parent}
{
    vectPointer = new QVector3D(0, 0, 0);
}

VectorViewBackend::VectorViewBackend(const QVector3D *vecPointer, QObject *parent) : QObject{parent}
{
    this->vectPointer = vecPointer;
}

void VectorViewBackend::setVectPointer(const QVector3D *newVectPointer)
{
    vectPointer = newVectPointer;
}

float VectorViewBackend::xValue() const
{
    return vect.x();
//    return vectPointer->x();//m_xValue;
}

void VectorViewBackend::setxValue(float newXValue)
{
    if (qFuzzyCompare(m_xValue, newXValue))
        return;
    m_xValue = newXValue;
    emit xValueChanged();
}

float VectorViewBackend::yValue() const
{
    return vect.y();
//    return vectPointer->y();//m_yValue;
}

void VectorViewBackend::setyValue(float newYValue)
{
    if (qFuzzyCompare(m_yValue, newYValue))
        return;
    m_yValue = newYValue;
    emit yValueChanged();
}

float VectorViewBackend::zValue() const
{
    return vect.z();
//    return vectPointer->z();//m_zValue;
}

void VectorViewBackend::setzValue(float newZValue)
{
    if (qFuzzyCompare(m_zValue, newZValue))
        return;
    m_zValue = newZValue;
    emit zValueChanged();
}

void VectorViewBackend::vectorUpdatet()
{
    emit xValueChanged();
    emit yValueChanged();
    emit zValueChanged();
}

void VectorViewBackend::vectorChanged(QVector3D vec)
{
    vect = vec;
    emit xValueChanged();
    emit yValueChanged();
    emit zValueChanged();
}

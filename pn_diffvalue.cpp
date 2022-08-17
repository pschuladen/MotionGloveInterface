#include "pn_diffvalue.h"

PN_DiffValue::PN_DiffValue(QObject *parent)
    : ProcessNode{parent}
{

}

PN_DiffValue::PN_DiffValue(QByteArray identifier, int idxInControlller, PN_DiffValue *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent}
{
    m_processorType = TypeHelper::DiffValue;
    m_lastTouch.resize(6);
    m_lastValues.resize(valueNumber);

}


void PN_DiffValue::slot_quatChanged(QQuaternion quat, int frame)
{
    callQuatChanged(quat - m_lastQuat, frame);
    m_lastQuat = quat;
}

void PN_DiffValue::slot_vectorChanged(QVector3D vect, int frame)
{
    callVectorChanged(vect - m_lastVect, frame);
    m_lastVect = vect;
}

void PN_DiffValue::slot_touchChanged(QList<float> touch, int frame)
{
    for(int i = 0; i < touch.size(); i++) {
        m_lastTouch[i] = touch[i] - m_lastTouch[i];
    }
    callTouchChanged(m_lastTouch, frame);
    m_lastTouch = touch;
}

void PN_DiffValue::slot_valuesChanged(QList<float> values, int frame)
{
    if(values.size() != m_lastValues.size()) m_lastValues.resize(values.size());
    for(int i = 0; i < values.size(); i++) {
        m_lastValues[i] = values[i] - m_lastValues[i];
    }
    callTouchChanged(m_lastValues, frame);
    m_lastValues = values;
}

void PN_DiffValue::slot_singleValueChanged(float value, int frame)
{
    emit singleValueChanged(value - m_lastValue, frame);
    m_lastValue = value;
}

void PN_DiffValue::slot_boolValueChanged(bool value, int frame)
{
    emit boolValueChanged(value != m_lastBool, frame);
    m_lastBool = value;
}

ProcessNode *PN_DiffValue::createSubprocessor(QString objectname_id)
{
    PN_DiffValue *newSubprocessor = new PN_DiffValue(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

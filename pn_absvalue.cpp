#include "pn_absvalue.h"

PN_AbsValue::PN_AbsValue(QObject *parent)
    : ProcessNode{parent}
{
}

PN_AbsValue::PN_AbsValue(QByteArray identifier, int idxInControlller, PN_AbsValue *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent}
{
    m_processorType = TypeHelper::AbsValue;
}


//void PN_AbsValue::initSaveData()
//{
//}

//void PN_AbsValue::loadDataFromQdomElement(QDomElement domElement)
//{
//}

ProcessNode *PN_AbsValue::createSubprocessor(QString objectname_id)
{
    PN_AbsValue *newSubprocessor = new PN_AbsValue(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

float PN_AbsValue::process(float value)
{

    return qAbs(value);
}





















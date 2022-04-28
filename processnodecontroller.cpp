#include "processnodecontroller.h"
#include <QDebug>

ProcessNodeController::ProcessNodeController(QObject *parent)
    : QObject{parent}
{
    m_objectIsView = true;
}

const QList<QColor> &ProcessNodeController::connectorColor() const
{
    return m_connectorColor;
}

void ProcessNodeController::setConnectorColor(const QList<QColor> &newConnectorColor)
{
//    if (m_connectorColor == newConnectorColor)
//        return;
    m_connectorColor = newConnectorColor;
    emit connectorColorChanged();
}

int ProcessNodeController::numberInputs() const
{
    return m_numberInputs;
}

void ProcessNodeController::setNumberInputs(int newNumberInputs)
{
    if (m_numberInputs == newNumberInputs)
        return;
    m_numberInputs = newNumberInputs;
    emit numberInputsChanged();
}

bool ProcessNodeController::acceptMultiInput() const
{
    return m_acceptMultiInput;
}

void ProcessNodeController::setAcceptMultiInput(bool newAcceptMultiInput)
{
    if (m_acceptMultiInput == newAcceptMultiInput)
        return;
    m_acceptMultiInput = newAcceptMultiInput;
    emit acceptMultiInputChanged();
}

const QList<QList<float> > &ProcessNodeController::multiOutputValues() const
{
    return m_multiOutputValues;
}

void ProcessNodeController::setMultiOutputValues(const QList<QList<float> > &newMultiOutputValues)
{
    if (m_multiOutputValues == newMultiOutputValues)
        return;
    m_multiOutputValues = newMultiOutputValues;
    emit multiOutputValuesChanged();
}

const QList<QList<float> > &ProcessNodeController::multiInputValues() const
{
    return m_multiInputValues;
}

void ProcessNodeController::setMultiInputValues(const QList<QList<float> > &newMultiInputValues)
{
    if (m_multiInputValues == newMultiInputValues)
        return;
    m_multiInputValues = newMultiInputValues;
    emit multiInputValuesChanged();
}

const QList<float> &ProcessNodeController::outputValues() const
{
    return m_outputValues;
}

void ProcessNodeController::setOutputValues(const QList<float> &newOutputValues)
{
    if (m_outputValues == newOutputValues)
        return;
    m_outputValues = newOutputValues;
    emit outputValuesChanged();
}

const QList<float> &ProcessNodeController::inputValues() const
{
    return m_inputValues;
}

void ProcessNodeController::setInputValues(const QList<float> &newInputValues)
{
    if (m_inputValues == newInputValues)
        return;
    m_inputValues = newInputValues;
    emit inputValuesChanged();
}

int ProcessNodeController::activeIndex() const
{
    return m_activeIndex;
}

void ProcessNodeController::setActiveIndex(int newActiveIndex)
{
    if (m_activeIndex == newActiveIndex)
        return;
    m_activeIndex = newActiveIndex;
    emit activeIndexChanged();
}

const ProcessNodeController::ProcessingType &ProcessNodeController::processType() const
{
    return m_processType;
}

void ProcessNodeController::setProcessType(const ProcessNodeController::ProcessingType &newProcessType)
{
    if (m_processType == newProcessType)
        return;
    m_processType = newProcessType;
    emit processTypeChanged();
}

bool ProcessNodeController::objectIsView() const
{
    return m_objectIsView;
}

void ProcessNodeController::setObjectIsView(bool newObjectIsView)
{
    m_objectIsView = newObjectIsView;
}

float ProcessNodeController::singleInputValue() const
{
    return m_singleInputValue;
}

void ProcessNodeController::setSingleInputValue(float newSingleInputValue)
{
    if (qFuzzyCompare(m_singleInputValue, newSingleInputValue))
        return;
    m_singleInputValue = newSingleInputValue;
    emit singleInputValueChanged();
}

void ProcessNodeController::singleValueChanged(float value)
{
    setSingleInputValue(value);
    qInfo() << "single value set" << value;

}

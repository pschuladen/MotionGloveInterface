#include "outputnodecontroller.h"

OutputNodeController::OutputNodeController()
{

}

//const QString &OutputNodeController::uniqueId() const
//{
//    return m_uniqueId;
//}

//void OutputNodeController::setUniqueId(const QString &newUniqueId)
//{
//    if (m_uniqueId == newUniqueId)
//        return;
//    m_uniqueId = newUniqueId;
//    emit uniqueIdChanged();
//}

const QString &OutputNodeController::outputAddress() const
{
    return m_outputAddress;
}

void OutputNodeController::setOutputAddress(const QString &newOutputAddress)
{
    if (m_outputAddress == newOutputAddress)
        return;
    m_outputAddress = newOutputAddress;
    emit outputAddressChanged();
}

const TypeHelper::ValueType &OutputNodeController::valueType() const
{
    return m_valueType;
}

void OutputNodeController::setValueType(const TypeHelper::ValueType &newValueType)
{
    if (m_valueType == newValueType)
        return;
    m_valueType = newValueType;
    emit valueTypeChanged();
}

quint32 OutputNodeController::targetIdx() const
{
    return m_targetIdx;
}

void OutputNodeController::setTargetIdx(quint32 newTargetIdx)
{
    if (m_targetIdx == newTargetIdx)
        return;
    m_targetIdx = newTargetIdx;
    emit targetIdxChanged();
}

bool OutputNodeController::mouseHover() const
{
    return m_mouseHover;
}

void OutputNodeController::setMouseHover(bool newMouseHover)
{
    if (m_mouseHover == newMouseHover)
        return;
    m_mouseHover = newMouseHover;
    emit mouseHoverChanged();
    emit mouseHoveredOnNode(newMouseHover, targetIdx());
}

void OutputNodeController::mouseAtTargetHoveredAtIdx(bool hover, int idx)
{
    if(idx == targetIdx()) {
        setMouseRemoteHover(hover);
    }
    else setMouseRemoteHover(false);
}

bool OutputNodeController::mouseRemoteHover() const
{
    return m_mouseRemoteHover;
}

void OutputNodeController::setMouseRemoteHover(bool newMouseRemoteHover)
{
    if (m_mouseRemoteHover == newMouseRemoteHover)
        return;
    m_mouseRemoteHover = newMouseRemoteHover;
    emit mouseRemoteHoverChanged();
}

#include "connectionviewcontroller.h"

ConnectionViewController::ConnectionViewController()
{

}

const TypeHelper::ValueType &ConnectionViewController::valueType() const
{
    return m_valueType;
}

void ConnectionViewController::setValueType(const TypeHelper::ValueType &newValueType)
{
    if (m_valueType == newValueType)
        return;
    m_valueType = newValueType;
    emit valueTypeChanged();
}

quint16 ConnectionViewController::sourceIdx() const
{
    return m_sourceIdx;
}

void ConnectionViewController::setSourceIdx(quint16 newSourceIdx)
{
    if (m_sourceIdx == newSourceIdx)
        return;
    m_sourceIdx = newSourceIdx;

    if(sourceView() != nullptr) {
//        qDebug() << uniqueID() <<"idx" << newSourceIdx << "listsize" << sourceView()->outConOffsets().size();
        setSourceYOffset(sourceView()->outConOffsets().at(newSourceIdx));
    }

    emit sourceIdxChanged();
}

quint16 ConnectionViewController::targetIdx() const
{
    return m_targetIdx;
}

void ConnectionViewController::setTargetIdx(quint16 newTargetIdx)
{
    if (m_targetIdx == newTargetIdx)
        return;
    m_targetIdx = newTargetIdx;

    if(targetView() != nullptr) {
//        qDebug() << uniqueID() << "idx" << newTargetIdx << "listsize" << targetView()->inConOffsets().size();
        setTargetYOffset(targetView()->inConOffsets().at(newTargetIdx));
    }

    emit targetIdxChanged();
}

NodeViewController *ConnectionViewController::targetView() const
{
    return m_targetView;
}

void ConnectionViewController::setTargetView(NodeViewController *newTargetView)
{
    if (m_targetView == newTargetView)
        return;
    m_targetView = newTargetView;

//    setTargetYOffset(newTargetView->inConOffsets().value(targetIdx(), 0));
//    m_targetYOffset = newTargetView->inConOffsets().value(targetIdx(), 0);
//    connect(newTargetView, &NodeViewController::inConOffsetsChanged, this, &ConnectionViewController::targetOffsetsChanged);
//    connect(newTargetView, &NodeViewController::xChanged, this, &ConnectionViewController::targetPositionChanged);
//    connect(newTargetView, &NodeViewController::yChanged, this, &ConnectionViewController::targetPositionChanged);

//    if(m_sourceView != nullptr) setPositionAndSize();

    emit targetViewChanged();
}

NodeViewController *ConnectionViewController::sourceView() const
{
    return m_sourceView;
}

void ConnectionViewController::setSourceView(NodeViewController *newSourceView)
{
    if (m_sourceView == newSourceView)
        return;
    m_sourceView = newSourceView;

//    setSourceYOffset(newSourceView->outConOffsets().value(sourceIdx(), 0));
//    m_sourceYOffset = newSourceView->outConOffsets().value(sourceIdx(), 0);
//    connect(newSourceView, &NodeViewController::outConOffsetsChanged, this, &ConnectionViewController::sourceOffsetsChanged);
//    connect(newSourceView, &NodeViewController::xChanged, this, &ConnectionViewController::sourcePositionChanged);
//    connect(newSourceView, &NodeViewController::yChanged, this, &ConnectionViewController::sourcePositionChanged);

//    if(m_targetView != nullptr) setPositionAndSize();

    emit sourceViewChanged();
}

int ConnectionViewController::targetYOffset() const
{
    return m_targetYOffset;
}

void ConnectionViewController::setTargetYOffset(int newTargetYOffset)
{
    if (m_targetYOffset == newTargetYOffset)
        return;
    m_targetYOffset = newTargetYOffset;
    setPositionAndSize();
    emit targetYOffsetChanged();
}


int ConnectionViewController::sourceYOffset() const
{
    return m_sourceYOffset;
}

void ConnectionViewController::setSourceYOffset(int newSourceYOffset)
{
    if (m_sourceYOffset == newSourceYOffset)
        return;
    m_sourceYOffset = newSourceYOffset;
    setPositionAndSize();
    emit sourceYOffsetChanged();
}

void ConnectionViewController::targetPositionChanged()
{
    setPositionAndSize();
}

void ConnectionViewController::sourcePositionChanged()
{
    setPositionAndSize();
}

void ConnectionViewController::targetOffsetsChanged()
{
//    qDebug() << uniqueID() << "targetoffsetlist" << targetView()->inConOffsets().size() << targetIdx();
    setTargetYOffset(targetView()->inConOffsets().value(targetIdx(), 0));
}

void ConnectionViewController::sourceOffsetsChanged()
{
//    qDebug() << uniqueID() << "sourceoffsetlist" << sourceView()->outConOffsets().size() << sourceIdx();
    setSourceYOffset(sourceView()->outConOffsets().value(sourceIdx(), 0));
}

void ConnectionViewController::setPositionAndSize()
{
    if(!m_targetView || !m_sourceView) return;

    int _targetY = targetY();
    int _sourceY = sourceY();
    int _targetX = targetX();
    int _sourceX = sourceX();
    setY(qMin(_targetY, _sourceY));
    setX(qMin(_targetX, _sourceX));

    setHeight(qAbs(_sourceY - _targetY));
    setWidth(qAbs(_sourceX - _targetX));

    setTargetYhigher(_targetY > _sourceY);
    setTargetXhigher(_targetX > _sourceX);
}

int ConnectionViewController::targetY()
{
    return targetView()->y() + targetYOffset() + addYoff;
}

int ConnectionViewController::targetX()
{
    return targetView()->x();
}

int ConnectionViewController::sourceY()
{
    return sourceView()->y() + sourceYOffset() + addYoff;
}

int ConnectionViewController::sourceX()
{
    return sourceView()->x() + sourceView()->width();
}

bool ConnectionViewController::targetYhigher() const
{
    return m_targetYhigher;
}

void ConnectionViewController::setTargetYhigher(bool newTargetYhigher)
{
    if (m_targetYhigher == newTargetYhigher)
        return;
    m_targetYhigher = newTargetYhigher;
    emit targetYhigherChanged();
}

const QString &ConnectionViewController::uniqueID() const
{
    return m_uniqueID;
}

void ConnectionViewController::setUniqueID(const QString &newUniqueID)
{
    if (m_uniqueID == newUniqueID)
        return;
    m_uniqueID = newUniqueID;
    emit uniqueIDChanged();
}

bool ConnectionViewController::initialSetup(NodeViewController *sourceView, quint16 sIdx, NodeViewController *targetView, quint16 tIdx)
{
    if(!sourceView || !targetView) return false;

    m_sourceView = sourceView;
    m_targetView = targetView;
    m_sourceIdx = sIdx;
    m_targetIdx = tIdx;

    setSigSlotConnections();

    sourceOffsetsChanged();
    targetOffsetsChanged();
//    emit m_sourceView->outConOffsetsChanged();
//    emit m_targetView->inConOffsetsChanged();

    return true;
}

bool ConnectionViewController::setSigSlotConnections()
{
    if(!sourceView() || !targetView()) return false;

    connect(sourceView(), &NodeViewController::outConOffsetsChanged, this, &ConnectionViewController::sourceOffsetsChanged);
    connect(sourceView(), &NodeViewController::xChanged, this, &ConnectionViewController::sourcePositionChanged);
    connect(sourceView(), &NodeViewController::yChanged, this, &ConnectionViewController::sourcePositionChanged);

    connect(targetView(), &NodeViewController::inConOffsetsChanged, this, &ConnectionViewController::targetOffsetsChanged);
    connect(targetView(), &NodeViewController::xChanged, this, &ConnectionViewController::targetPositionChanged);
    connect(targetView(), &NodeViewController::yChanged, this, &ConnectionViewController::targetPositionChanged);

    return true;
}

bool ConnectionViewController::targetXhigher() const
{
    return m_targetXhigher;
}

void ConnectionViewController::setTargetXhigher(bool newTargetXhigher)
{
    if (m_targetXhigher == newTargetXhigher)
        return;
    m_targetXhigher = newTargetXhigher;
    emit targetXhigherChanged();
}

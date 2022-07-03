#include "nodeviewcontroller.h"

NodeViewController::NodeViewController()
{

}

void NodeViewController::setInConOffsetAtIndex(int idx, int offset)
{
    if(idx < 0) return;
    if(m_inConOffsets.length() <= idx) {
        m_inConOffsets.resize(idx+1);
//        qDebug() << "resizing offset list" << idx << offset;
    }
//    qDebug() << uniqueID() << "inoffset" << idx << offset;
    m_inConOffsets[idx] = offset;
    emit inConOffsetsChanged();
}

void NodeViewController::setOutConOffsetAtIndex(int idx, int offset)
{
    if(idx < 0) return;
    if(m_outConOffsets.length() <= idx) {
        m_outConOffsets.resize(idx+1);
    }
//    qDebug() << uniqueID() << "outoffset" << idx << offset;
    m_outConOffsets[idx] = offset;
    emit outConOffsetsChanged();
}

void NodeViewController::setConOffsetAtIndex(int idx, int offset)
{
    setInConOffsetAtIndex(idx, offset);
    setOutConOffsetAtIndex(idx, offset);
}

//QPoint NodeViewController::getPositionForInputAtIndex(int idx) const
//{
//    return QPoint(x(), y() + inConOffsets().at(idx));
//}

//QPoint NodeViewController::getPositionForOutputAtIndex(int idx) const
//{
//    return QPoint(x() + width(), y() + outConOffsets().at(idx));
//}

const QList<int> &NodeViewController::inConOffsets() const
{
    return m_inConOffsets;
}

void NodeViewController::setInConOffsets(const QList<int> &newInConOffsets)
{
    if (m_inConOffsets == newInConOffsets)
        return;
    m_inConOffsets = newInConOffsets;
    emit inConOffsetsChanged();
}

const QList<int> &NodeViewController::outConOffsets() const
{
    return m_outConOffsets;
}

void NodeViewController::setOutConOffsets(const QList<int> &newOutConOffsets)
{
    if (m_outConOffsets == newOutConOffsets)
        return;
    m_outConOffsets = newOutConOffsets;
    emit outConOffsetsChanged();
}

const QString &NodeViewController::uniqueID() const
{
    return m_uniqueID;
}

void NodeViewController::setUniqueID(const QString &newUniqueID)
{
    if (m_uniqueID == newUniqueID)
        return;
    m_uniqueID = newUniqueID;
    emit uniqueIDChanged();
}


void NodeViewController::initSaveData()
{
    QDomDocument _doc("tmpdoc");
    QDomElement root = _doc.createElement("view-node");
    _doc.appendChild(root);
    QDomElement _pos = _doc.createElement("position");
    _pos.setAttribute("x", x());
    _pos.setAttribute("y", y());
    root.appendChild(_pos);

    connect(this, &NodeViewController::sendSubNodeTree, projManager::Instance(), &ProjectFileManager::addSubtree, Qt::SingleShotConnection);
    emit sendSubNodeTree(uniqueID(), _doc);
}

void NodeViewController::loadDataFromQdomElement(QDomElement domElement)
{
}

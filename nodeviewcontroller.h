#ifndef NODEVIEWCONTROLLER_H
#define NODEVIEWCONTROLLER_H

#include <QQuickItem>
#include <QtQml/qqmlregistration.h>

class NodeViewController : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<int> inConOffsets READ inConOffsets WRITE setInConOffsets NOTIFY inConOffsetsChanged)
    Q_PROPERTY(QList<int> outConOffsets READ outConOffsets WRITE setOutConOffsets NOTIFY outConOffsetsChanged)

    Q_PROPERTY(QString uniqueID READ uniqueID WRITE setUniqueID NOTIFY uniqueIDChanged)

public:
    NodeViewController();

    Q_INVOKABLE void setInConOffsetAtIndex(int idx, int offset);
    Q_INVOKABLE void setOutConOffsetAtIndex(int idx, int offset);
    Q_INVOKABLE void setConOffsetAtIndex(int idx, int offset);

//    QPoint getPositionForInputAtIndex(int idx) const;
//    QPoint getPositionForOutputAtIndex(int idx) const;

    const QList<int> &inConOffsets() const;
    void setInConOffsets(const QList<int> &newInConOffsets);

    const QList<int> &outConOffsets() const;
    void setOutConOffsets(const QList<int> &newOutConOffsets);

    const QString &uniqueID() const;
    void setUniqueID(const QString &newUniqueID);

signals:

    void inConOffsetsChanged();
    void outConOffsetsChanged();

    void uniqueIDChanged();

private:
    QList<int> m_inConOffsets = {0};
    QList<int> m_outConOffsets = {0};
    QString m_uniqueID;
};

#endif // NODEVIEWCONTROLLER_H

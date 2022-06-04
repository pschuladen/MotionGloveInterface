#ifndef CONNECTIONVIEWCONTROLLER_H
#define CONNECTIONVIEWCONTROLLER_H

#include <QQuickItem>
#include "typehelper.h"
#include "nodeviewcontroller.h"

class ConnectionViewController : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString uniqueID READ uniqueID WRITE setUniqueID NOTIFY uniqueIDChanged)
    Q_PROPERTY(TypeHelper::ValueType valueType READ valueType WRITE setValueType NOTIFY valueTypeChanged)

    Q_PROPERTY(quint16 sourceIdx READ sourceIdx WRITE setSourceIdx NOTIFY sourceIdxChanged)
    Q_PROPERTY(quint16 targetIdx READ targetIdx WRITE setTargetIdx NOTIFY targetIdxChanged)

    Q_PROPERTY(NodeViewController* targetView READ targetView WRITE setTargetView NOTIFY targetViewChanged)
    Q_PROPERTY(NodeViewController* sourceView READ sourceView WRITE setSourceView NOTIFY sourceViewChanged)

    Q_PROPERTY(int targetYOffset READ targetYOffset WRITE setTargetYOffset NOTIFY targetYOffsetChanged)
    Q_PROPERTY(int sourceYOffset READ sourceYOffset WRITE setSourceYOffset NOTIFY sourceYOffsetChanged)

    Q_PROPERTY(bool targetYhigher READ targetYhigher WRITE setTargetYhigher NOTIFY targetYhigherChanged)
    Q_PROPERTY(bool targetXhigher READ targetXhigher WRITE setTargetXhigher NOTIFY targetXhigherChanged)

public:
    ConnectionViewController();

    const TypeHelper::ValueType &valueType() const;
    void setValueType(const TypeHelper::ValueType &newValueType);

    quint16 sourceIdx() const;
    void setSourceIdx(quint16 newSourceIdx);

    quint16 targetIdx() const;
    void setTargetIdx(quint16 newTargetIdx);

    NodeViewController *targetView() const;
    void setTargetView(NodeViewController *newTargetView);

    NodeViewController *sourceView() const;
    void setSourceView(NodeViewController *newSourceView);

    int targetYOffset() const;
    void setTargetYOffset(int newTargetYOffset);

    int sourceYOffset() const;
    void setSourceYOffset(int newSourceYOffset);

    bool targetYhigher() const;
    void setTargetYhigher(bool newTargetYhigher);

    const QString &uniqueID() const;
    void setUniqueID(const QString &newUniqueID);

    bool initialSetup(NodeViewController* sourceView, quint16 sIdx, NodeViewController* targetView, quint16 tIdx);



    bool targetXhigher() const;
    void setTargetXhigher(bool newTargetXhigher);

public slots:
    void targetPositionChanged();
    void sourcePositionChanged();
    void targetOffsetsChanged();
    void sourceOffsetsChanged();

signals:

    void valueTypeChanged();
    void sourceIdxChanged();

    void targetIdxChanged();

    void targetViewChanged();

    void sourceViewChanged();

    void targetYOffsetChanged();


    void sourceYOffsetChanged();

    void targetYhigherChanged();

    void uniqueIDChanged();

    void targetXhigherChanged();

private:
    TypeHelper::ValueType m_valueType = TypeHelper::Undefined;
    quint16 m_sourceIdx = 0;
    quint16 m_targetIdx = 0;
    NodeViewController *m_targetView;
    NodeViewController *m_sourceView;
    int m_targetYOffset = 0;
    int m_sourceYOffset = 0;

    void setPositionAndSize();
    int targetY();
    int targetX();
    int sourceY();
    int sourceX();
    bool m_targetYhigher;
    QString m_uniqueID;

    int addYoff = 5; // half connector width
    bool setSigSlotConnections();
    bool m_targetXhigher;
};

#endif // CONNECTIONVIEWCONTROLLER_H

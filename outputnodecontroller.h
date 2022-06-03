#ifndef OUTPUTNODECONTROLLER_H
#define OUTPUTNODECONTROLLER_H

#include <QQuickItem>
#include <QtQml/qqmlregistration.h>

#include "typehelper.h"

class OutputNodeController : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString uniqueId READ uniqueId WRITE setUniqueId NOTIFY uniqueIdChanged)
    Q_PROPERTY(QString outputAddress READ outputAddress WRITE setOutputAddress NOTIFY outputAddressChanged)
    Q_PROPERTY(TypeHelper::ValueType valueType READ valueType WRITE setValueType NOTIFY valueTypeChanged)
    Q_PROPERTY(quint32 targetIdx READ targetIdx WRITE setTargetIdx NOTIFY targetIdxChanged)

    Q_PROPERTY(bool mouseHover READ mouseHover WRITE setMouseHover NOTIFY mouseHoverChanged)
    Q_PROPERTY(bool mouseRemoteHover READ mouseRemoteHover WRITE setMouseRemoteHover NOTIFY mouseRemoteHoverChanged)

public:
    OutputNodeController();

    const QString &uniqueId() const;
    void setUniqueId(const QString &newUniqueId);

    const QString &outputAddress() const;

    const TypeHelper::ValueType &valueType() const;

    quint32 targetIdx() const;

    bool mouseHover() const;
    void setMouseHover(bool newMouseHover);

    bool mouseRemoteHover() const;
    void setMouseRemoteHover(bool newMouseRemoteHover);

public slots:
    void mouseAtTargetHoveredAtIdx(bool hover, int idx);
    void setTargetIdx(quint32 newTargetIdx);
    void setOutputAddress(const QString &newOutputAddress);
    void setValueType(const TypeHelper::ValueType &newValueType);


signals:
    void mouseHoveredOnNode(bool hover, int idx);

    void uniqueIdChanged();
    void outputAddressChanged();

    void valueTypeChanged();

    void targetIdxChanged();

    void mouseHoverChanged();

    void mouseRemoteHoverChanged();

private:
    QString m_uniqueId;
    QString m_outputAddress;
    TypeHelper::ValueType m_valueType = TypeHelper::Undefined;
    quint32 m_targetIdx;
    bool m_mouseHover;
    bool m_mouseRemoteHover;
};

#endif // OUTPUTNODECONTROLLER_H

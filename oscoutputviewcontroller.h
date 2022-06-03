#ifndef OSCVIEWCONTROLLER_H
#define OSCVIEWCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
//#include "typehelper.h"

#include "valuenotifierclass.h"
//#include <QQuickItem>
//#include "typehelper.h"

class OscOutputViewController : public ValueNotifierClass
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString destIp READ destIp WRITE setDestIp NOTIFY destIpChanged)
    Q_PROPERTY(quint16 destPort READ destPort WRITE setDestPort NOTIFY destPortChanged)
    Q_PROPERTY(QList<QString> oscPaths READ oscPaths WRITE setOscPaths NOTIFY oscPathsChanged)
    Q_PROPERTY(QList<TypeHelper::ValueType> valueTypes READ valueTypes WRITE setValueTypes NOTIFY valueTypesChanged)
    Q_PROPERTY(int nPaths READ nPaths WRITE setNPaths NOTIFY nPathsChanged)
    Q_PROPERTY(QString uniqueID READ uniqueID WRITE setUniqueID NOTIFY uniqueIDChanged)

    Q_PROPERTY(int indexHovered READ indexHovered WRITE setIndexHovered NOTIFY indexHoveredChanged)

    Q_PROPERTY(QList<bool> outputNodeCreated READ outputNodeCreated WRITE setOutputNodeCreated NOTIFY outputNodeCreatedChanged)
    Q_PROPERTY(QSet<int> createdOutputNodes READ createdOutputNodes WRITE setCreatedOutputNodes NOTIFY createdOutputNodesChanged)


public:
    explicit OscOutputViewController(QObject *parent = nullptr);

    const QString &destIp() const;

    quint16 destPort() const;

    const QList<QString> &oscPaths() const;

    int nPaths() const;

    const QList<TypeHelper::ValueType> &valueTypes() const;

    static const QString standardObjectName();

    Q_INVOKABLE bool nodeAtIndexWasCreated(int idx);

    void nodeWithIdxWasCreated(int idx);
    void removeNodeWithIdx(int idx);



public slots:

    virtual void setDestIp(const QString &newDestIp);
    virtual void setDestPort(quint16 newDestPort);
    virtual void setOscPaths(const QList<QString> &newOscPaths);
    void setNPaths(int newNPaths);
    virtual void setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes);


    virtual void addOscPath();
    virtual void addOscPath(QString newPath);
    virtual void setOscPathAtIndex(QString newPath, quint32 idx);
    virtual void newConnectionAtIndex(int idx, TypeHelper::ValueType valueType);

    void viewAtIndexHovered(bool hovered, int idx);


signals:

    void destIpChanged(QString destIp);
    void destPortChanged(quint16 port);
    void oscPathsChanged(QList<QString> oscPaths);
    void oscPathAtIndexChanged(QString oscPath, quint32 idx);
    void oscPathAdded(QString oscPath);
    void nPathsChanged();
    void valueTypesChanged(QList<TypeHelper::ValueType> newValueTypes);

    void sig_addOscPath();

    void outputNodeCreatedChanged();

    void uniqueIDChanged();

    void createdOutputNodesChanged();

    void sig_viewAtIndexHovered(bool hovered, int idx);

    void indexHoveredChanged();

protected:
    QString m_destIp;
    int m_destPort;
    QList<QString> m_oscPaths;

    int m_nPaths = 0;
    QList<TypeHelper::ValueType> m_valueTypes;

public:
//    ValueNotifierClass *getNotifier(int idx) override;


    const QString &uniqueID() const;
    void setUniqueID(const QString &newUniqueID);

    const QList<bool> &outputNodeCreated() const;
    void setOutputNodeCreated(const QList<bool> &newOutputNodeCreated);

    const QSet<int> &createdOutputNodes() const;
    void setCreatedOutputNodes(const QSet<int> &newCreatedOutputNodes);

    int indexHovered() const;
    void setIndexHovered(int newIndexHovered);

private:

    QString m_uniqueID;
    QList<bool> m_outputNodeCreated;
    QSet<int> m_createdOutputNodes;
    int m_indexHovered;
};

#endif // OSCVIEWCONTROLLER_H

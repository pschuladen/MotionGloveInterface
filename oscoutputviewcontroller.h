#ifndef OSCVIEWCONTROLLER_H
#define OSCVIEWCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
//#include "typehelper.h"

#include "valuenotifierclass.h"

class OscOutputViewController : public ValueNotifierClass
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString destIp READ destIp WRITE setDestIp NOTIFY destIpChanged)
    Q_PROPERTY(quint16 destPort READ destPort WRITE setDestPort NOTIFY destPortChanged)
    Q_PROPERTY(QList<QString> oscPaths READ oscPaths WRITE setOscPaths NOTIFY oscPathsChanged)
    Q_PROPERTY(QList<TypeHelper::ValueType> valueTypes READ valueTypes WRITE setValueTypes NOTIFY valueTypesChanged)
    Q_PROPERTY(int nPaths READ nPaths WRITE setNPaths NOTIFY nPathsChanged)


public:
    explicit OscOutputViewController(QObject *parent = nullptr);

    const QString &destIp() const;

    quint16 destPort() const;

    const QList<QString> &oscPaths() const;

    int nPaths() const;

    const QList<TypeHelper::ValueType> &valueTypes() const;

    static const QString standardObjectName();

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


signals:

    void destIpChanged(QString destIp);
    void destPortChanged(quint16 port);
    void oscPathsChanged(QList<QString> oscPaths);
    void oscPathAtIndexChanged(QString oscPath, quint32 idx);
    void oscPathAdded(QString oscPath);
    void nPathsChanged();
    void valueTypesChanged(QList<TypeHelper::ValueType> newValueTypes);

    void sig_addOscPath();

protected:
    QString m_destIp;
    int m_destPort;
    QList<QString> m_oscPaths;

    int m_nPaths = 0;
    QList<TypeHelper::ValueType> m_valueTypes;
};

#endif // OSCVIEWCONTROLLER_H

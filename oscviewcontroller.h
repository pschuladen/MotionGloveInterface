#ifndef OSCVIEWCONTROLLER_H
#define OSCVIEWCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
//#include "typehelper.h"

#include "valuenotifierclass.h"

class OscViewController : public ValueNotifierClass
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString destIp READ destIp WRITE setDestIp NOTIFY destIpChanged)
    Q_PROPERTY(int destPort READ destPort WRITE setDestPort NOTIFY destPortChanged)
    Q_PROPERTY(QList<QString> oscPaths READ oscPaths WRITE setOscPaths NOTIFY oscPathsChanged)
    Q_PROPERTY(QList<TypeHelper::ValueType> valueTypes READ valueTypes WRITE setValueTypes NOTIFY valueTypesChanged)
    Q_PROPERTY(int nPaths READ nPaths WRITE setNPaths NOTIFY nPathsChanged)


public:
    explicit OscViewController(QObject *parent = nullptr);

    const QString &destIp() const;

    int destPort() const;

    const QList<QString> &oscPaths() const;

    int nPaths() const;

    const QList<TypeHelper::ValueType> &valueTypes() const;

    static const QString standardObjectName();

public slots:

    void setDestIp(const QString &newDestIp);
    void setDestPort(int newDestPort);
    void setOscPaths(const QList<QString> &newOscPaths);
    void setNPaths(int newNPaths);
    void setValueTypes(const QList<TypeHelper::ValueType> &newValueTypes);


    virtual void addOscPath();
    virtual void addOscPath(QString newPath);
    virtual void setOscPathAtIndex(QString newPath, quint32 idx);
    virtual void newConnectionAtIndex(int idx, TypeHelper::ValueType valueType);



signals:

    void destIpChanged(QString destIp);
    void destPortChanged(int port);
    void oscPathsChanged(QList<QString> oscPaths);
    void oscPathAtIndexChanged(QString oscPath, quint32 idx);
    void oscPathAdded(QString oscPath);
    void nPathsChanged();
    void valueTypesChanged();

    void sig_addOscPath();

protected:
    QString m_destIp;
    int m_destPort;
    QList<QString> m_oscPaths;

    int m_nPaths;
    QList<TypeHelper::ValueType> m_valueTypes;
};

#endif // OSCVIEWCONTROLLER_H

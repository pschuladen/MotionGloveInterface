#ifndef SAVELOADINTERFACECLASS_H
#define SAVELOADINTERFACECLASS_H

//#include <QObject>
//#include <QtXml>
#include <QString>
#include <QMap>
#include "projectfilemanager.h"
#include <QObject>


class SaveLoadInterfaceClass
{
//    Q_OBJECT

public:
//    explicit saveLoadInterfaceClass(QObject *parent = nullptr);
    virtual ~SaveLoadInterfaceClass(){} //unneeded since inheriting classes all inherit from QObject?
    static void connectToProjectFileManager(QObject* saveableObj);


private:

protected:



signals:
//    void initSaveToFile();
//    void dataForSaving(QString nodeID, QMap<QString, float>);//QVariant<float>>);
//    virtual void registerSaveableObject(QObject *obj) = 0;
//    virtual void sendPropertyMapElement(QString uniqueID, QString subtype, QVariantMap) = 0;
    virtual void sendSubNodeTree(QString uniqueID, QDomDocument subTree) = 0;
    virtual void announceAdditionalData(int add = 1) = 0;
    virtual void didFinishLoad(QString uniqueID) = 0;

public slots:
    virtual void initSaveData() = 0;
    virtual void loadDataFromQdomElement(QDomElement domElement) = 0;
//    void forwardPropertyMapElement(QString uniqueID, QMap<QString, QString> propMap);


//    void createPropertyMap();
//    void getDataForSaving(QString nodeID, QMap<QString,float>);

};

#endif // SAVELOADINTERFACECLASS_H

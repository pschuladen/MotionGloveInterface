#ifndef SAVELOADINTERFACECLASS_H
#define SAVELOADINTERFACECLASS_H

#include <QObject>
#include <QMap>


class saveLoadInterfaceClass : public QObject
{
    Q_OBJECT

public:
    explicit saveLoadInterfaceClass(QObject *parent = nullptr);
//    virtual ~saveLoadInterfaceClass(){} //unneeded since inheriting classes all inherit from QObject?

private:


signals:
    void initSaveToFile();
    void dataForSaving(QString nodeID, QMap<QString, float>);//QVariant<float>>);

public slots:
    void createPropertyMap();
    void getDataForSaving(QString nodeID, QMap<QString,float>);

};

#endif // SAVELOADINTERFACECLASS_H

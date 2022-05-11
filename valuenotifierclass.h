#ifndef VALUENOTIFIERCLASS_H
#define VALUENOTIFIERCLASS_H

/*
 * (minimalistic class just for sending signals.)
 * Was created for handling value updates for lists of values to different receivers.
 * Includes subnotifier which are used for connecting single components of e.g. 3d-vectors
 * and value lists to receivers.
 * Also acts as Interface/Base class for processnodes
*/

#include <QObject>
#include <QQuaternion>
#include <QVector3D>
//#include "inputvalueviewcontroller.h"
#include "typehelper.h"



class ValueNotifierClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TypeHelper::ValueType connectedValueType READ connectedValueType WRITE setConnectedValueType NOTIFY connectedValueTypeChanged)
    Q_PROPERTY(quint32 indexInObject READ indexInObject WRITE setIndexInObject NOTIFY indexInObjectChanged)

public:
    typedef TypeHelper::SensorType SensType;
    explicit ValueNotifierClass(QObject *parent = nullptr, TypeHelper::ValueType valueType=TypeHelper::SingleValue, int numberOfValues = 0, int indexForObject = 0);
    explicit ValueNotifierClass(TypeHelper::SensorType sensType, QObject *parent = nullptr, int valueNumber=0);
    //    typedef InputValueViewController::ValueViewMode SensType ;
    //    explicit ValueNotifierClass(QObject *parent = nullptr, SensType sType = SensType::Custom, int indexForObject = 0, int valueNumber = 0);
    //    explicit ValueNotifierClass(QObject *parent = nullptr, SensType sType = SensType::Custom, int indexForObject = 0, int valueNumber = 0);
    //    void valueChanged(QQuaternion quat);
    //    void valueChanged(QQuaternion quat);
    void callQuatChanged(const QQuaternion quat, int frame=-1);
    void callVectorChanged(const QVector3D vect, int frame=-1);
    void callTouchChanged(const QList<float> touch, int frame=-1);
    void callValuesChanged(const QList<float> values, int frame=-1);

    virtual bool newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0);

    QList<ValueNotifierClass*> subNotifier; //TODO: make this private
    void createSubnotifier(TypeHelper::ValueType valueType);

    virtual ValueNotifierClass* getNotifier(int idx=-1);


    virtual void setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType);
    const TypeHelper::ValueType &connectedValueType() const;

    static bool connectValueTypeSignalToSlot(ValueNotifierClass* sender, ValueNotifierClass *receiver, TypeHelper::ValueType vType);

    quint32 indexInObject() const;
    void setIndexInObject(quint32 newIndexInObject);

protected:
    TypeHelper::ValueType m_connectedValueType;

private:
    int m_numberValues;
    int m_indexInObject;
    bool m_autoEmit = false;
    void createSubnotifier(int numberOfSubs);



signals:
     void quatChanged(QQuaternion quat, int frame=-1);
     void vectorChanged(QVector3D vect, int frame=-1);
     void touchChanged(QList<float> touch, int frame=-1);
     void valuesChanged(QList<float> values, int frame=-1);
     void singleValueChanged(float value, int frame=-1);
     void boolValueChanged(bool value, int frame=-1);
     void triggerActivated(int frame=-1);
     //     void valueChanged()
     void connectedValueTypeChanged(TypeHelper::ValueType vType);

     void indexInObjectChanged();

public slots:
     virtual void slot_quatChanged(QQuaternion quat, int frame=-1);
     virtual void slot_vectorChanged(QVector3D vect, int frame=-1);
     virtual void slot_touchChanged(QList<float> touch, int frame=-1);
     virtual void slot_valuesChanged(QList<float> values, int frame=-1);
     virtual void slot_singleValueChanged(float value, int frame=-1);
     virtual void slot_boolValueChanged(bool value, int frame=-1);
     virtual void slot_trigger(int frame=-1);
};

#endif // VALUENOTIFIERCLASS_H

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

#include "typehelper.h"



class ValueNotifierClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TypeHelper::ValueType connectedValueType READ connectedValueType WRITE setConnectedValueType NOTIFY connectedValueTypeChanged)
    Q_PROPERTY(quint32 indexInObject READ indexInObject WRITE setIndexInObject NOTIFY indexInObjectChanged)
    Q_PROPERTY(quint16 valueNumber READ valueNumber WRITE setValueNumber NOTIFY valueNumberChanged)
    Q_PROPERTY(bool supportsSubValues READ supportsSubValues WRITE setSupportsSubValues NOTIFY supportsSubValuesChanged)
    Q_PROPERTY(QByteArray identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(bool autoEmit READ autoEmit WRITE setAutoEmit NOTIFY autoEmitChanged)


public:
    typedef TypeHelper::SensorType SensType;
    explicit ValueNotifierClass(QObject *parent = nullptr);
    explicit ValueNotifierClass(QByteArray identifier, int objectIdx, QObject *parent = nullptr);
    explicit ValueNotifierClass(QByteArray identifier, int objectIdx, TypeHelper::ValueType valType, quint16 valueNumber=0, QObject *parent = nullptr);
//    explicit ValueNotifierClass(QObject *parent = nullptr, TypeHelper::ValueType valueType=TypeHelper::SingleValue, int numberOfValues = 0, int indexForObject = 0);
//    explicit ValueNotifierClass(TypeHelper::SensorType sensType, QObject *parent = nullptr, int valueNumber=0); // deprecated... moved to oscInputParser practically

    void callQuatChanged(const QQuaternion quat, int frame=-1);
    void callVectorChanged(const QVector3D vect, int frame=-1);
    void callTouchChanged(const QList<float> touch, int frame=-1);
    void callValuesChanged(const QList<float> values, int frame=-1);


    QList<ValueNotifierClass*> subNotifier; //TODO: make this private
//    void createSubnotifier(TypeHelper::ValueType valueType);

    virtual ValueNotifierClass *getNotifier(int idx=-1);

    const TypeHelper::ValueType &connectedValueType() const;

    static bool completeConnectValueNotifier(const ValueNotifierClass* sender, const ValueNotifierClass *receiver);
    static bool connectValueTypeSignalToSlot(const ValueNotifierClass* sender, const ValueNotifierClass *receiver, const TypeHelper::ValueType vType);
    static bool disconnectValueTypeSignalToSlot(const ValueNotifierClass* sender, const ValueNotifierClass *receiver, const TypeHelper::ValueType vType);

    quint32 indexInObject() const;
    void setIndexInObject(quint32 newIndexInObject);

    quint16 valueNumber() const;
    void setValueNumber(quint16 newValueNumber);

    bool supportsSubValues() const;
    void setSupportsSubValues(bool newSupportsSubValues);
    virtual bool acceptsInputType(TypeHelper::ValueType typ, int atIdx=0) const;

    const QByteArray &identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    bool autoEmit() const;
    void setAutoEmit(bool newAutoEmit);

protected:
    TypeHelper::ValueType m_connectedValueType = TypeHelper::Undefined;
    void unimplementedValueTypeWarning(TypeHelper::ValueType valType, QString extraMsg="");



private:
//    int m_valueNumber;
    int m_indexInObject;
    bool m_autoEmit = false;
    void createSubnotifier(int numberOfSubs);
    void createSubnotifierForValueType(TypeHelper::ValueType valType, int nVals=0);
//    void createSubnotifier();

    quint16 m_valueNumber;

    bool m_supportsSubValues = false;

    QByteArray m_identifier;

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

     void valueNumberChanged();

     void supportsSubValuesChanged();

     void identifierChanged();

     void autoEmitChanged();

     void connectionAccepted(QString connectionId, bool accepted=true, TypeHelper::ValueType type=TypeHelper::Undefined, int atIdx=0);

     void sig_connectToSender(ValueNotifierClass *sender, QString connectionId, TypeHelper::ValueType type, int atIdx=0);



public slots:
     virtual void slot_quatChanged(QQuaternion quat, int frame=-1);
     virtual void slot_vectorChanged(QVector3D vect, int frame=-1);
     virtual void slot_touchChanged(QList<float> touch, int frame=-1);
     virtual void slot_valuesChanged(QList<float> values, int frame=-1);
     virtual void slot_singleValueChanged(float value, int frame=-1);
     virtual void slot_boolValueChanged(bool value, int frame=-1);
     virtual void slot_trigger(int frame=-1);

     virtual void setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType);
     virtual int newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx=0, quint16 nValuesInList=0);

     virtual void connectionRequestFromSender(ValueNotifierClass *sender, QString connectionId, TypeHelper::ValueType type, int connectToIdx=0, quint16 nValuesInList=0);
     virtual void slot_connectToSender(ValueNotifierClass *sender, QString connectionId, TypeHelper::ValueType type, int atIdx=0);
     virtual void slot_subConnectionAccepted(QString connectionId, bool accepted=true, TypeHelper::ValueType type=TypeHelper::Undefined, int atIdx=0);


     virtual void inputsDisconnected();
};

#endif // VALUENOTIFIERCLASS_H

#ifndef PROCESSNODE_H
#define PROCESSNODE_H

#include "valuenotifierclass.h"
#include "QtQml/qqmlregistration.h"

class ProcessNode : public ValueNotifierClass
{
    Q_OBJECT
public:
    explicit ProcessNode(QObject *parent = nullptr);

    virtual bool acceptsInputType(TypeHelper::ValueType typ); //override for asking if connection allowed
    virtual bool setConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0); //use this function for setting up connections!

    enum ProcessRole {
        ViewController, ProcessController, Processor
    };
    ProcessRole processRole = ViewController;

private:
    virtual float process(float value); //child classes override this for the processing
    virtual quint8 process(quint8 value);

    //this should in theory already be used in ValueNotifierClass
    TypeHelper::ValueType connectedValueType = TypeHelper::Undefined; //has to be set otherwise the process function will not be called. this is usually done by the setConnectionFromSender function.

public slots:
    virtual void slot_quatChanged(QQuaternion quat, int frame=-1) override;
    virtual void slot_vectorChanged(QVector3D vect, int frame=-1) override;
    virtual void slot_touchChanged(QList<float> touch, int frame=-1) override;
    virtual void slot_valuesChanged(QList<float> values, int frame=-1) override;
    virtual void slot_singleValueChanged(float value, int frame=-1) override;
    virtual void slot_boolValueChanged(bool value, int frame=-1) override;
    virtual void slot_trigger(int frame=-1) override;
};

#endif // PROCESSNODE_H

#ifndef PROCESSNODE_H
#define PROCESSNODE_H

#include "valuenotifierclass.h"
#include "QtQml/qqmlregistration.h"

/*
 * This is the base class for processing nodes. It
 * should be seen as virtual class and not actually be instantiated.
 * It inherits from ValueNotifierClass which basicallly provides Signal/Slots
 * for all Data-types used in this program.
 * Child classes start with "PN_" and implement processing functions.
 *
 * Usually several objects of childclasses were created:
 * ViewControler -> ProcessController -> (multiple)Processor
 * Parameter for processing were set in the ViewController signaled
 * to the ProcessController which can hold several Processors that
 * do the processing e.g. of different source-signals.
 * Make sure that all relevant parameter were reached through.
 *
 * Childclasses with simple functions just have to override the process
 * function, handle there properties and newConnections (create subProcessors).
 *
 * This slots were automatically mapped to the process function (splitting the
 * types to single values) and emit the result as signal.
*/

class ProcessNode : public ValueNotifierClass
{
    Q_OBJECT
public:
    explicit ProcessNode(QObject *parent = nullptr);

//    Q_PROPERTY(QList<TypeHelper::ValueType> connectedTypes READ connectedTypes WRITE setConnectedTypes NOTIFY connectedTypesChanged)
    Q_PROPERTY(QList<TypeHelper::ValueType> connectedTypes READ connectedTypes WRITE setConnectedTypes NOTIFY connectedTypesChanged);

    enum ProcessRole {
        ViewController, ProcessController, Processor
    };
    ProcessRole processRole = ViewController;

    void setConnectedValueType(const TypeHelper::ValueType &newConnectedValueType) override;

    virtual bool acceptsInputType(TypeHelper::ValueType typ); //override for asking if connection allowed
    virtual bool setConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0); //use this function for setting up connections!
    // there is something doppeltgemoppelt
    virtual bool newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0) override; //this is currently used !!
    virtual bool connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList=0); //TODO: implement

    virtual ProcessNode* createProcessControl(QString objectname_id); //can be deleted maybe
    virtual ProcessNode* createSubprocessor(QString objectname_id);

    const QList<TypeHelper::ValueType> &connectedTypes() const;
    void appendToConnectedTypes(TypeHelper::ValueType appendType);

    virtual ValueNotifierClass* getNotifier(int idx=-1) override;



protected:
    QList<ProcessNode*> subProcessor;
    TypeHelper::ValueType m_connectedValueType = TypeHelper::Undefined; //has to be set otherwise the process function will not be called. this is usually done by the setConnectionFromSender function.

private:
    virtual float process(float value); //child classes override this for the processing
    virtual quint8 process(quint8 value);

    //this should in theory already be used in ValueNotifierClass

    QList<TypeHelper::ValueType> m_connectedTypes;

public slots:
    void setConnectedTypes(const QList<TypeHelper::ValueType> &newConnectedTypes);
    virtual void slot_quatChanged(QQuaternion quat, int frame=-1) override;
    virtual void slot_vectorChanged(QVector3D vect, int frame=-1) override;
    virtual void slot_touchChanged(QList<float> touch, int frame=-1) override;
    virtual void slot_valuesChanged(QList<float> values, int frame=-1) override;
    virtual void slot_singleValueChanged(float value, int frame=-1) override;
    virtual void slot_boolValueChanged(bool value, int frame=-1) override;
    virtual void slot_trigger(int frame=-1) override;
signals:
    void connectedTypesChanged(QList<TypeHelper::ValueType> connectedTypes);
};

#endif // PROCESSNODE_H

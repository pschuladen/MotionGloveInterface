#ifndef PN_SCALE_H
#define PN_SCALE_H

//#include <QObject>
//#include "processnodecontroller.h"
#include "processnode.h"

class PN_Scale : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float inLow READ inLow WRITE setInLow NOTIFY inLowChanged)
    Q_PROPERTY(float inHigh READ inHigh WRITE setInHigh NOTIFY inHighChanged)
    Q_PROPERTY(float outLow READ outLow WRITE setOutLow NOTIFY outLowChanged)
    Q_PROPERTY(float outHigh READ outHigh WRITE setOutHigh NOTIFY outHighChanged)

    float m_inLow;

    float m_inHigh;

    float m_outLow;

    float m_outHigh;

public:
    explicit PN_Scale(QObject *parent = nullptr);

    float inLow() const;
    void setInLow(float newInLow);

    float inHigh() const;
    void setInHigh(float newInHigh);

    float outLow() const;
    void setOutLow(float newOutLow);

    float outHigh() const;
    void setOutHigh(float newOutHigh);

    bool newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0);
    bool connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList=0);

private:
    virtual float process(float value) override;
    virtual quint8 process(quint8 value) override;

    QList<PN_Scale*> subProcessor;
    static void connectPropertiesToProcessor(PN_Scale *propertyMaster, PN_Scale *propertySlave);
    static void setInitialProperties(PN_Scale *propertyMaster, PN_Scale *propertySlave);


signals:

    void inLowChanged();
    void inHighChanged();
    void outLowChanged();
    void outHighChanged();

};

#endif // PN_SCALE_H

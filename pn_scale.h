#ifndef PN_SCALE_H
#define PN_SCALE_H


#include "processnode.h"
//#include <QtGlobal>

class PN_Scale : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float inLow READ inLow WRITE setInLow NOTIFY inLowChanged)// BINDABLE true)
    Q_PROPERTY(float inHigh READ inHigh WRITE setInHigh NOTIFY inHighChanged)
    Q_PROPERTY(float outLow READ outLow WRITE setOutLow NOTIFY outLowChanged)
    Q_PROPERTY(float outHigh READ outHigh WRITE setOutHigh NOTIFY outHighChanged)
    Q_PROPERTY(bool clipOutput READ clipOutput WRITE setClipOutput NOTIFY clipOutputChanged)


    float m_inLow;
    float m_inHigh;
    float m_outLow;
    float m_outHigh;

public:
    explicit PN_Scale(QObject *parent = nullptr);
    explicit PN_Scale(int idxInControlller, PN_Scale *controller, TypeHelper::ValueType type, quint16 valueNumber=0, QObject *parent = nullptr);

    float inLow() const;
    float inHigh() const;
    float outLow() const;
    float outHigh() const;

    bool clipOutput() const;

    float multi() const;
    void setMulti(float newMulti);

    bool acceptsInputType(TypeHelper::ValueType typ) override;
    bool newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, quint16 nValuesInList=0) override;
    bool connectToSubProcessorAtIndex(int index, TypeHelper::ValueType type, quint16 nValuesInList=0) override; //TODO: implement

    ProcessNode* createProcessControl(QString objectname_id) override;


public slots:
    void setInLow(float newInLow);
    void setInHigh(float newInHigh);
    void setOutLow(float newOutLow);
    void setOutHigh(float newOutHigh);
    void setClipOutput(bool newClipOutput);

private:
    virtual float process(float value) override;
    virtual quint8 process(quint8 value) override;

    void evalMultip();
    float m_multi;

    static void connectPropertiesToProcessor(PN_Scale *propertyMaster, PN_Scale *propertySlave);
    static void setInitialProperties(PN_Scale *propertyMaster, PN_Scale *propertySlave);

    bool m_clipOutput;

signals:

    void inLowChanged(float newValue);
    void inHighChanged(float newValue);
    void outLowChanged(float newValue);
    void outHighChanged(float newValue);
    void clipOutputChanged(bool newClipbool);

};

#endif // PN_SCALE_H

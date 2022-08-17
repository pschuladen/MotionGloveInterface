#ifndef PN_ABSVALUE_H
#define PN_ABSVALUE_H

#include "processnode.h"

class PN_AbsValue : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit PN_AbsValue(QObject *parent = nullptr);
    explicit PN_AbsValue(QByteArray identifier, int idxInControlller,
                              PN_AbsValue *controller,
                              TypeHelper::ValueType type=TypeHelper::Undefined, quint16 valueNumber=0,
                              QObject *parent = nullptr);

    // SaveLoadInterfaceClass interface
//public slots:
//    void initSaveData() override;
//    void loadDataFromQdomElement(QDomElement domElement) override;

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;

private:
    float process(float value) override;
};

#endif // PN_ABSVALUE_H

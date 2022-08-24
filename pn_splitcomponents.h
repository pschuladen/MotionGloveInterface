#ifndef PN_SPLITCOMPONENTS_H
#define PN_SPLITCOMPONENTS_H

#include "processnode.h"

class PN_SplitComponents : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PN_SplitComponents(QObject *parent = nullptr);
    explicit PN_SplitComponents(QByteArray identifier, int idxInControlller,
                              PN_SplitComponents *controller,
                              TypeHelper::ValueType type=TypeHelper::Undefined, quint16 valueNumber=0,
                              QObject *parent = nullptr);

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;

    // ValueNotifierClass interface
public:
    ValueNotifierClass *getNotifier(int idx) override;

    // ProcessNode interface
public:
    void deleteSubprocessorAtIdx(quint16 idx) override;

    // SaveLoadInterfaceClass interface
public slots:
    void initSaveData() override;
    void loadDataFromQdomElement(QDomElement domElement) override;
};

#endif // PN_SPLITCOMPONENTS_H

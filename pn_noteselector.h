#ifndef PN_NOTESELECTOR_H
#define PN_NOTESELECTOR_H

#include "processnode.h"

class PN_NoteSelector : public ProcessNode
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit PN_NoteSelector(QObject *parent = nullptr);
    explicit PN_NoteSelector(QByteArray identifier, int idxInControlller,
                              PN_NoteSelector *controller,
                              TypeHelper::ValueType type=TypeHelper::Undefined, quint16 valueNumber=0,
                              QObject *parent = nullptr);

    // ProcessNode interface
public:
    ProcessNode *createSubprocessor(QString objectname_id) override;

    // ValueNotifierClass interface
public slots:
    void connectionRequestFromSender(ValueNotifierClass *sender, QString connectionId, TypeHelper::ValueType type, int connectToIdx, quint16 nValuesInList) override;

    // ValueNotifierClass interface
public slots:
    void slot_singleValueChanged(float value, int frame) override;
    void slot_boolListChanged(QList<bool> boolList, int frame) override;


    // ValueNotifierClass interface
public slots:
    int newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList) override;

private:
    int getNoteEventForBoolList(QList<bool> &bl);
    int getNoteIndexForNoteEvent(int noteEve);
//    int m_touchedNote = 0;
//    int m_octaveRegister = 0;
    int m_octUpper = 17;
    int m_octLower = 11;

    void fillMidiNoteList(int baseMidiNote, bool dur);
    QList<int> midiNotes = QList<int>(30);

    void debug_noteTest();


    // ProcessNode interface
public:
    void deleteSubprocessorAtIdx(quint16 idx) override;
};

#endif // PN_NOTESELECTOR_H

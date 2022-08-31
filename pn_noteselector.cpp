#include "pn_noteselector.h"

PN_NoteSelector::PN_NoteSelector(QObject *parent)
    : ProcessNode{parent}
{
    m_processorType = TypeHelper::NoteSelector;
    subProcessor.resize(2);
    subProcessor[0] = PN_NoteSelector::createSubprocessor(identifier());
//    debug_noteTest();
}

PN_NoteSelector::PN_NoteSelector(QByteArray identifier, int idxInControlller, PN_NoteSelector *controller, TypeHelper::ValueType type, quint16 valueNumber, QObject *parent)
    : ProcessNode{identifier, idxInControlller, type, valueNumber, parent}
{
    m_processorType = TypeHelper::NoteSelector;
    fillMidiNoteList(60, true);
}


ProcessNode *PN_NoteSelector::createSubprocessor(QString objectname_id)
{
    PN_NoteSelector *newSubprocessor = new PN_NoteSelector(identifier(), subProcessor.size(), this);
    connect(this, &QObject::destroyed, newSubprocessor, &QObject::deleteLater);
    emit newSubprocessorWasCreated(newSubprocessor);
    return newSubprocessor;
}

void PN_NoteSelector::connectionRequestFromSender(ValueNotifierClass *sender, QString connectionId,
                                                  TypeHelper::ValueType type, int connectToIdx, quint16 nValuesInList)
{
    if((type == TypeHelper::SingleValue && connectToIdx == 0) ||
            (type == TypeHelper::BoolList && connectToIdx == 1 && sender->valueNumber() >= 4))
    {
        connect(this, &ProcessNode::sig_connectToSender, subProcessor.first(), &ProcessNode::slot_connectToSender, Qt::SingleShotConnection);
        connect(subProcessor.first(), &ProcessNode::connectionAccepted, this, &ProcessNode::slot_subConnectionAccepted, Qt::SingleShotConnection);
        emit sig_connectToSender(sender, connectionId, type, connectToIdx);
    }
    else {
        emit connectionAccepted(connectionId, false);
    }
}

int PN_NoteSelector::newConnectionFromSender(ValueNotifierClass *sender, TypeHelper::ValueType type, int atIdx, quint16 nValuesInList)
{
    if(type == TypeHelper::BoolList || type == TypeHelper::SingleValue)
    {
        connectValueTypeSignalToSlot(sender, this, type);
        return 0;
    }
    else {
        return -1;
    }
}

int PN_NoteSelector::getNoteEventForBoolList(QList<bool> &bl)
{
    //
    if(bl[0] && bl[1]) return 1;
    if(bl[0]) return 0;
    if(bl[1] && bl[2]) return 3;
    if(bl[1]) return 2;
    if(bl[2] && bl[3]) return 5;
    if(bl[2]) return 4;
    if(bl[3] && bl[0]) return 7;
    if(bl[3]) return 6;
    return -1;
}

int PN_NoteSelector::getNoteIndexForNoteEvent(int noteEve)
{
    while(noteEve <= m_octLower) {
        noteEve += 7;
    }
    return noteEve;
}

void PN_NoteSelector::fillMidiNoteList(int baseMidiNote, bool dur)
{
    const static QList<int> durIntervals = {0, 2, 4, 5, 7, 9, 11};
    const static QList<int> mollIntervals = {0, 2, 3, 5, 7, 8, 10};
    const QList<int> &_intervall = dur ? durIntervals : mollIntervals;

    for(int i = 0; i < 30; i++) {
        midiNotes[i] = baseMidiNote - 24 + _intervall[i % 7] + int(i / 7) * 12;
    }
}

/*
 * @parameter
 *
 */
void PN_NoteSelector::slot_singleValueChanged(float value, int frame)
{
    int _v1 = (qBound(-1., value, 1.) + 1) * 14.5;
    m_octLower = qMax(0, _v1 - 3);
    m_octUpper = qMin(29, _v1 + 3);

//    qDebug() << "value" << _v1 << m_octLower << m_octUpper;

}

void PN_NoteSelector::slot_boolListChanged(QList<bool> boolList, int frame)
{
    int _touchEvent = getNoteEventForBoolList(boolList);
    if(_touchEvent > -1) {
        int _midiNote = midiNotes[getNoteIndexForNoteEvent(_touchEvent)];
        qDebug() << "midi-NOTE:" << _midiNote;
        emit intValueChanged(_midiNote);
    }
//    else emit intValueChanged(-1);
//    qDebug() << "bools:" <<  boolList;
}

void PN_NoteSelector::deleteSubprocessorAtIdx(quint16 idx)
{
}


void PN_NoteSelector::debug_noteTest()
{
    connect(this, &PN_NoteSelector::boolListChanged, subProcessor.first(), &ProcessNode::slot_boolListChanged);
    emit boolListChanged({true, false, false, false});
    emit boolListChanged({true, true, false, false});
    emit boolListChanged({false, true, false, false});
    emit boolListChanged({false, true, true, false});
    emit boolListChanged({false, false, true, false});
    emit boolListChanged({false, false, true, true});
    emit boolListChanged({false, false, false, true});
    emit boolListChanged({true, false, false, true});
    disconnect(this, &PN_NoteSelector::boolListChanged, subProcessor.first(), &ProcessNode::slot_boolListChanged);
//    emit boolListChanged({true, false, false, false});
}

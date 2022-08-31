#ifndef PN_NOTESELECTOR_H
#define PN_NOTESELECTOR_H

#include "processnode.h"

class PN_NoteSelector : public ProcessNode
{
    Q_OBJECT
public:
    explicit PN_NoteSelector(QObject *parent = nullptr);
};

#endif // PN_NOTESELECTOR_H

#ifndef INPUTVALUEVIEWCONTROLLER_H
#define INPUTVALUEVIEWCONTROLLER_H

/*
 * Backend for ValueView. It acts similar to a DataModel.
 * The class can be used in qml and be used for valueUpdates
 * The number of values is flexible. When used in qml it should be
 * created with createWithInitialProperties({{"viewmode", ValueViewBackend::ValueViewMode}})
 * when used with Vectors, Quaternions or Touch.
 * For a more general version use ValueViewMode::Custom
*/

#include <QObject>
#include <QQuickItem>
#include <QQuaternion>
#include <QVector3D>
#include <QtQml/qqmlregistration.h>
#include <QColor>
#include <QQmlEngine>
#include "typehelper.h"
#include "valuenotifierclass.h"

class InputValueViewController : public ValueNotifierClass
{
    Q_OBJECT
    QML_ELEMENT

//    Q_PROPERTY(QString nodeIdentifier READ nodeIdentifier WRITE setNodeIdentifier NOTIFY nodeIdentifierChanged)
    Q_PROPERTY(QList<QString> valname READ valname WRITE setValname NOTIFY valnameChanged)
    Q_PROPERTY(QList<float> values READ values WRITE setValues NOTIFY viewValuesChanged)
    Q_PROPERTY(float minvalue READ minvalues WRITE setMinvalue NOTIFY minvalueChanged)
    Q_PROPERTY(float maxvalue READ maxvalues WRITE setMaxvalue NOTIFY maxvalueChanged)
    Q_PROPERTY(int valueCount READ valueCount WRITE setValueCount NOTIFY valueCountChanged)
    Q_PROPERTY(TypeHelper::SensorType viewmode READ viewmode WRITE setViewmode NOTIFY viewmodeChanged)
    Q_PROPERTY(QColor dotColor READ dotColor WRITE setDotColor NOTIFY dotColorChanged)


    Q_PROPERTY(bool emitvalues READ emitvalues WRITE setEmitvalues NOTIFY emitvaluesChanged)
    Q_PROPERTY(bool mouseHover READ mouseHover WRITE setMouseHover NOTIFY mouseHoverChanged)
    Q_PROPERTY(bool canDragInput READ canDragInput WRITE setCanDragInput NOTIFY canDragInputChanged)

    Q_PROPERTY(QString sourceObjectID READ sourceObjectID WRITE setSourceObjectID NOTIFY sourceObjectIDChanged)


public:
    explicit InputValueViewController(QObject *parent = nullptr);

    TypeHelper::SensorType viewmode() const;
    void setViewmode(TypeHelper::SensorType newViewmode);

    int valueCount() const;
    void setValueCount(int newValueCount);

    const QList<float> &values() const;
    void setValues(const QList<float> &newValues);

    const QList<QString> &valname() const;
    void setValname(const QList<QString> &newValname);

//    const QString &nodeIdentifier() const;
//    void setNodeIdentifier(const QString &newNodeIdentifier);


    float minvalues() const;
    void setMinvalue(float newMinvalue);

    float maxvalues() const;
    void setMaxvalue(float newMaxvalue);

    bool emitvalues() const;
    void setEmitvalues(bool newEmitvalues);



    const QColor &dotColor() const;
    void setDotColor(const QColor &newDotColor);
    ValueNotifierClass *sourceNotifier() const;

    ValueNotifierClass *getNotifier(int idx) override;
    void setSourceNotifier(ValueNotifierClass *newSourceNotifier);

    bool mouseHover() const;

    bool canDragInput() const;
    void setCanDragInput(bool newCanDragInput);

    const QString &sourceObjectID() const;
    void setSourceObjectID(const QString &newSourceObjectID);

public slots:

    void setMouseHover(bool newMouseHover);
    void slot_vectorChanged(QVector3D vec, int frame=-1) override;
    void slot_quatChanged(QQuaternion newQuat, int frame=-1) override;
    void slot_touchChanged(QList<float> touch, int frame=-1) override;
    void slot_valuesChanged(QList<float> newValues, int frame=-1) override;

private:

    void checkAndEmitValuesChanged();

    ValueNotifierClass *m_sourceNotifier = nullptr;
    TypeHelper::SensorType m_viewmode;

    int m_valueCount;

    QList<float> m_values;

    QList<QString> m_valname;

//    QString m_nodeIdentifier;

    float m_minvalue;

    float m_maxvalue;

    bool m_emitvalues;

    int m_valuecount;

    QColor m_dotColor;

    bool m_mouseHover;

    bool m_canDragInput = true;

    QString m_sourceObjectID;

signals:

    void viewmodeChanged();
    void valueCountChanged();

    void viewValuesChanged();
    void valnameChanged();
//    void nodeIdentifierChanged();
    void minvalueChanged();
    void maxvalueChanged();
    void emitvaluesChanged();
    void valuecountChanged();
    void dotColorChanged();

    // ValueNotifierClass interface
    void mouseHoverChanged(bool mouseHover);
    void canDragInputChanged();
    void sourceObjectIDChanged();
};

#endif // INPUTVALUEVIEWCONTROLLER_H

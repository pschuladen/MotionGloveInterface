#ifndef VALUEVIEWBACKEND_H
#define VALUEVIEWBACKEND_H

/*
 * Backend for ValueView. It acts similar to a DataModel.
 * The class can be used in qml and be used for valueUpdates
 * The number of values is flexible. When used in qml it should be
 * created with createWithInitialProperties({{"viewmode", ValueViewBackend::ValueViewMode}})
 * when used with Vectors, Quaternions or Touch.
 * For a more general version use ValueViewMode::Custom
*/

#include <QObject>
#include <QQuaternion>
#include <QVector3D>
#include <QtQml/qqmlregistration.h>
#include <QColor>

class ValueViewBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString nodeIdentifier READ nodeIdentifier WRITE setNodeIdentifier NOTIFY nodeIdentifierChanged)
    Q_PROPERTY(QList<QString> valname READ valname WRITE setValname NOTIFY valnameChanged)
    Q_PROPERTY(QList<float> values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(float minvalue READ minvalues WRITE setMinvalue NOTIFY minvalueChanged)
    Q_PROPERTY(float maxvalue READ maxvalues WRITE setMaxvalue NOTIFY maxvalueChanged)
    Q_PROPERTY(int valueCount READ valueCount WRITE setValueCount NOTIFY valueCountChanged)
    Q_PROPERTY(ValueViewMode viewmode READ viewmode WRITE setViewmode NOTIFY viewmodeChanged)
    Q_PROPERTY(QColor dotColor READ dotColor WRITE setDotColor NOTIFY dotColorChanged)

    Q_PROPERTY(int valuecount READ valuecount WRITE setValuecount NOTIFY valuecountChanged)

    Q_PROPERTY(bool emitvalues READ emitvalues WRITE setEmitvalues NOTIFY emitvaluesChanged)

public:
    explicit ValueViewBackend(QObject *parent = nullptr);

    enum ValueViewMode {
        Custom, Accel, Gyro, Grav, Quat, Touch
    };
    Q_ENUM(ValueViewMode);

    ValueViewMode viewmode() const;
    void setViewmode(ValueViewMode newViewmode);

    int valueCount() const;

    const QList<float> &values() const;
    void setValues(const QList<float> &newValues);

    const QList<QString> &valname() const;
    void setValname(const QList<QString> &newValname);

    const QString &nodeIdentifier() const;
    void setNodeIdentifier(const QString &newNodeIdentifier);

    void setValueCount(int newValueCount);

    float minvalues() const;
    void setMinvalue(float newMinvalue);

    float maxvalues() const;
    void setMaxvalue(float newMaxvalue);

    bool emitvalues() const;
    void setEmitvalues(bool newEmitvalues);

    int valuecount() const;
    void setValuecount(int newValuecount);

    const QColor &dotColor() const;
    void setDotColor(const QColor &newDotColor);

public slots:
    void vectorChanged(QVector3D vec);
    void quatChanged(QQuaternion newQuat);
    void touchChanged(QList<float> touch);
    void newValues(QList<float> newValues);

private:

    void checkAndEmitValuesChanged();

    ValueViewMode m_viewmode;

    int m_valueCount;

    QList<float> m_values;

    QList<QString> m_valname;

    QString m_nodeIdentifier;

    float m_minvalue;

    float m_maxvalue;

    bool m_emitvalues;

    int m_valuecount;

    QMap<ValueViewMode, QColor> modeColorMap = {{ValueViewMode::Custom, QColor("mediumvioletred")},
                                               {ValueViewMode::Accel, QColor("red")},
                                               {ValueViewMode::Gyro, QColor("red")},
                                               {ValueViewMode::Grav, QColor("red")},
                                               {ValueViewMode::Quat, QColor("blue")},
                                               {ValueViewMode::Touch, QColor("yellow")}};

    QColor m_dotColor;

signals:

    void viewmodeChanged();
    void valueCountChanged();

    void valuesChanged();
    void valnameChanged();
    void nodeIdentifierChanged();
    void minvalueChanged();
    void maxvalueChanged();
    void emitvaluesChanged();
    void valuecountChanged();
    void dotColorChanged();
};

#endif // VALUEVIEWBACKEND_H

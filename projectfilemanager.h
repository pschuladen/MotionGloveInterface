#ifndef PROJECTFILEMANAGER_H
#define PROJECTFILEMANAGER_H

#include <QObject>
#include <QtXml>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include "typehelper.h"
#include "Singleton.h"

class ProjectFileManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectFileManager(QObject *parent = nullptr);

    struct SLI_element {
        QString cNodeName;
        QVariantMap attributes;
        SLI_element(QString _name, QVariantMap attrs)
            : cNodeName(_name), attributes(attrs) {};
        SLI_element() {};
    };




public slots:

    void addSaveableObject(QObject *obj);
    void removeSaveableObject(QObject *obj);
    void increaseSaveObjectCounter(int inc = 1);

    void setXmlFilePath(QUrl fileUrl, bool loadIt);
    void prepareXmlDoc();
    void writeXmlFile(QUrl fileUrl);
    void loadSceneFromCurrentFile();
    void loadNodeCategoryFromCurrentFile(TypeHelper::NodeType type);

    void valueConnectionElement(QString uniqueID, QString sourceId, int srcIdx, QString rcvId, int rcvIdx, TypeHelper::ValueType vType);
    void inputNodeElement(QString uniqueID, QString inputDevice, QString inputAddress);
    void outputNodeElement(QString uniqueID, QString outputDevice, quint16 outputIdx);
    void processingNodeElement(QString uniqueID, TypeHelper::ProcessorType procType);
    void oscOutputdeviceElement(QString uniqueID);
    void oscInputDeviceElement(QString uniqueID, QString deviceName);

    void addSubNode(QString uniqueID, QString subType, QVariantMap subMap);
    void addSubtree(QString uniqueID, QDomDocument subNodeTreeDocument);

    void extractNodesWithType(TypeHelper::NodeType type);


    void testReceiveDomeNode(QDomNode _dom);


private:
    QString getFilePathNameFromUrl(QUrl fileUrl);

    QDomDocument xmlDoc;
    QString m_xmlPath = "";
    bool m_docReadyRead = false;

    QList<TypeHelper::NodeType> m_nodeCategories = {TypeHelper::Input, TypeHelper::Output, TypeHelper::Process, TypeHelper::OscInput, TypeHelper::OscOutput, TypeHelper::Connection};
    QMap<TypeHelper::NodeType, QDomElement> categoryElems;

    int m_saveObjectCounter = 0;
    void setSaveObjectCounter(int newSaveObjectCounter);


    struct SingleElement {
        QDomElement qDomElement;
        TypeHelper::NodeType nodeType;
        SingleElement(QDomElement _qDomElem, TypeHelper::NodeType _nodeType)
            :qDomElement{_qDomElem}, nodeType{_nodeType} {}
        SingleElement() {}
    };
    QMap<QString, SingleElement> singleElems;

    void createClearXmlElems();

    QSet<QObject*> registeredObjects;





signals:
    void readyForWritingXml();
    void selectedXmlFileReadyLoad();

    void loadedOscInputDevice();
    void loadedOscOuputDevice();
    void loadedInputNode();
    void loadedOutputNode();
    void loadedProcessNode();

    void qDomElemToScene(QDomElement element, TypeHelper::NodeType type);
    void finishedSendingCategory(TypeHelper::NodeType cat);

};
Q_DECLARE_METATYPE(ProjectFileManager::SLI_element);

typedef Singleton<ProjectFileManager> projManager;

#endif // PROJECTFILEMANAGER_H

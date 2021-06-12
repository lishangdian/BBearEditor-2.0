#ifndef BBHIERARCHYTREEWIDGET_H
#define BBHIERARCHYTREEWIDGET_H

#include "BBTreeWidget.h"

class BBGameObject;
class BBGameObjectSet;
class BBHierarchyTreeWidget : public BBTreeWidget
{
    Q_OBJECT

public:
    explicit BBHierarchyTreeWidget(QWidget *parent = 0);
    QString getMimeType() { return BB_MIMETYPE_HIERARCHYTREEWIDGET; }

private:
    void setMenu() override;
    void pasteOne(QTreeWidgetItem *pSource, QTreeWidgetItem* pTranscript) override;
    void deleteOne(QTreeWidgetItem *pItem) override;

    bool moveItem() override;
    bool moveItemFromOthers(const QMimeData *pMimeData) override;
    void moveItemToIndicator();
    QIcon getClassIcon(const QString &className);
    int getDragIconColumnIndex() override { return 1; }

signals:
    void createModel(const QString &filePath);
    void setCoordinateSystemSelectedObject(BBGameObject *pGameObject);
    void setCoordinateSystemSelectedObjects(const QList<BBGameObject*> &gameObjects, BBGameObjectSet *pSet);
    void showGameObjectProperty(BBGameObject *pGameObject);
    void showGameObjectSetProperty(BBGameObject *pCenterGameObject,
                                   const QList<BBGameObject*> &gameObjectSet);
    void deleteGameObject(BBGameObject *pGameObject);
    void copyGameObject(BBGameObject *pSourceObject, QTreeWidgetItem *pTranscriptItem);
    void lookAtGameObject(BBGameObject *pGameObject);

private slots:
    void addGameObject(BBGameObject *pGameObject);
    void addGameObject(BBGameObject *pGameObject, QTreeWidgetItem *pItem);
    void selectPickedItem(BBGameObject *pGameObject);
    void selectPickedItems(const QList<BBGameObject*> &gameObjects);
    void updateMultipleSelectedItems(BBGameObject *pGameObject);
    void changeSelectedItems();
    void doubleClickItem(QTreeWidgetItem *pItem, int nColumn);
    void deleteAction() override;





//private slots:
//    void itemChangedSlot(QTreeWidgetItem *item, int column);
//    void renameItemName(GameObject *gameObject);
//    void changeGameObjectActivation(GameObject *gameObject, bool isActive);
//    void copyGameObjectInsertMap(QTreeWidgetItem *item, GameObject *gameObject);
//    void updateMultipleSelectObjects(GameObject *gameObject);
//    void finishRename() override;
//    void cancelSelectedItems();

//signals:
//    void showSetProperty(QList<GameObject*> gameObjects, CenterPoint *center);
//    void updateNameInInspector(GameObject *gameObject, QString newName);
//    void changeButtonActiveCheckStateInInspector(GameObject *gameObject, bool isActive);
//    void createLight(QString fileName);
//    void cancelFileListSelectedItems();

//private:
//    void pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript) override;
//    bool moveItemFromFileList(const QMimeData *mimeData) override;
//    void focusInEvent(QFocusEvent *event) override;
};

#endif // BBHIERARCHYTREEWIDGET_H

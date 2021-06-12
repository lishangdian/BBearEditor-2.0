#ifndef BBSCENEMANAGER_H
#define BBSCENEMANAGER_H


#include <QMap>

class QTreeWidgetItem;
class BBGameObject;

class BBSceneManager
{
public:
    BBSceneManager();
    ~BBSceneManager();

    static QString getCurrentSceneFilePath() { return m_CurrentSceneFilePath; }

    static void insertObjectMap(QTreeWidgetItem *pItem, BBGameObject *pGameObject);
    static void removeObjectMap(QTreeWidgetItem *pItem);
    static QTreeWidgetItem* getSceneTreeItem(BBGameObject *pGameObject);
    static QList<QTreeWidgetItem*> getSceneTreeItems(const QList<BBGameObject*> &gameObjects);
    static BBGameObject* getGameObject(QTreeWidgetItem *pItem);

    static bool isSceneSwitched(const QString &filePath);
    static void openScene(const QString &filePath);
    static void saveScene(const QString &filePath = m_CurrentSceneFilePath);

private:
    // Save the mapping between each scene tree item and its corresponding GameObject
    static QMap<QTreeWidgetItem*, BBGameObject*> m_ObjectMap;
    static QString m_CurrentSceneFilePath;
};

#endif // BBSCENEMANAGER_H
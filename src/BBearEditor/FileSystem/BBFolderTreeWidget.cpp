#include "BBFolderTreeWidget.h"
#include <QDir>
#include <QQueue>
#include <QMenu>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QDragMoveEvent>
#include <QMimeData>


BBFolderTreeWidget::BBFolderTreeWidget(QWidget *pParent)
    : BBTreeWidget(pParent)
{
    m_pCurrentShowFolderContentItem = NULL;
//    //初始时 需要加载材质
//    isLoadMaterial = true;

    setMenu();

    QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                     this, SLOT(clickItem(QTreeWidgetItem*, int)));
}

void BBFolderTreeWidget::loadTopLevelItems(const QList<QTreeWidgetItem*> &items)
{
    addTopLevelItems(items);
}

void BBFolderTreeWidget::setCurrentItemByPath(const QString &folderPath)
{
    QTreeWidgetItem *pItem = getItemByPath(folderPath);
    setCurrentItem(pItem);
    setItemExpanded(pItem, true);
}

QTreeWidgetItem* BBFolderTreeWidget::getItemByPath(const QString &absolutePath)
{
    // Find the corresponding tree item according to the path
    QString path = absolutePath.mid(BBConstant::BB_PATH_PROJECT_USER.length());
    if (path.length() == 0)
    {
        // "contents" folder
        return NULL;
    }
    else
    {
        QTreeWidgetItem *pItem = NULL;
        // remove "/" at the beginning
        path = path.mid(1);
        QStringList list = path.split('/');
        // Find the item that is at the top level corresponds to item 0 in the list
        // Folders at the same level cannot have the same name
        for (int i = 0; i < topLevelItemCount(); i++)
        {
            if (topLevelItem(i)->text(0) == list.at(0))
            {
                pItem = topLevelItem(i);
                break;
            }
        }
        // Start from item 1 to find non-top items
        for (int i = 1; i < list.count(); i++)
        {
            for (int j = 0; j < pItem->childCount(); j++)
            {
                QTreeWidgetItem *pChild = pItem->child(j);
                if (list.at(i) == pChild->text(0))
                {
                    pItem = pChild;
                    break;
                }
            }
        }
        return pItem;
    }
}

void BBFolderTreeWidget::pressRootButton()
{
    setCurrentItem(NULL);
    // show the files in the root dir in the file list on the right
    // use NULL to indicate root item
    updateCorrespondingWidget(NULL);
}

void BBFolderTreeWidget::pressSettingButton()
{
    setCurrentItem(NULL);
    m_pMenu->exec(cursor().pos());
}

void BBFolderTreeWidget::clickItem(QTreeWidgetItem *pItem, int nColumn)
{
    Q_UNUSED(nColumn);
    updateCorrespondingWidget(pItem);
}

void BBFolderTreeWidget::setMenu()
{
    // first level menu
    m_pMenu = new QMenu(this);
    QAction *pActionNewFolder = new QAction(tr("New Folder"));
    QAction *pActionShowInFolder = new QAction(tr("Show In Folder"));
    QAction *pActionCopy = new QAction(tr("Copy"));
    pActionCopy->setShortcut(QKeySequence(tr("Ctrl+C")));
    QAction *pActionPaste = new QAction(tr("Paste"));
    pActionPaste->setShortcut(QKeySequence(tr("Ctrl+V")));
    QAction *pActionRename = new QAction(tr("Rename"));
#if defined(Q_OS_WIN32)
    pActionRename->setShortcut(Qt::Key_F2);
#elif defined(Q_OS_MAC)
    pActionRename->setShortcut(Qt::Key_Return);
#endif
    QAction *pActionDelete = new QAction(tr("Delete"));
    // second level menu
    QMenu *pMenuNewAsset = new QMenu(tr("New Asset"), m_pMenu);
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "scene.png", tr("Scene")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "material.png", tr("Material")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "animation.png", tr("Animation")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "particle.png", tr("Particle")));
    pMenuNewAsset->addAction(createWidgetAction(pMenuNewAsset, QString(BB_PATH_RESOURCE_ICON) + "script.png", tr("Script")));
    // first level menu
    m_pMenu->addAction(pActionNewFolder);
    m_pMenu->addMenu(pMenuNewAsset);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionShowInFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionCopy);
    m_pMenu->addAction(pActionPaste);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionRename);
    m_pMenu->addAction(pActionDelete);

    QObject::connect(pActionNewFolder, SIGNAL(triggered()), this, SLOT(newFolder()));
    QObject::connect(pActionShowInFolder, SIGNAL(triggered()), this, SLOT(showInFolder()));
    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openRenameEditor()));
    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

QWidgetAction* BBFolderTreeWidget::createWidgetAction(QMenu *pParent, const QString &iconPath, const QString &name)
{
    QWidgetAction *pAction = new QWidgetAction(pParent);
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName("widgetAction");
    pWidget->setStyleSheet("#widgetAction:hover {background: #0ebf9c;}");
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setContentsMargins(6, 2, 6, 2);
    QLabel *pIcon = new QLabel(pWidget);
    QPixmap pix(iconPath);
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(13 * devicePixelRatio(), 13 * devicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pIcon->setPixmap(pix);
    pLayout->addWidget(pIcon);
    QLabel *pText = new QLabel(pWidget);
    pText->setText(name);
    pText->setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\";");
    pLayout->addWidget(pText, Qt::AlignLeft);
    pAction->setDefaultWidget(pWidget);
    return pAction;
}

void BBFolderTreeWidget::updateCorrespondingWidget(QTreeWidgetItem *pItem)
{
    QString folderPath = getAbsolutePath(pItem);
    accessFolder(pItem, folderPath);
    m_pCurrentShowFolderContentItem = pItem;
}

QString BBFolderTreeWidget::getAbsolutePath(const QString &relativePath)
{
    if (relativePath.isEmpty())
    {
        return BBConstant::BB_PATH_PROJECT_USER;
    }
    else
    {
        return BBConstant::BB_PATH_PROJECT_USER + "/" + relativePath;
    }
}

QString BBFolderTreeWidget::getAbsolutePath(QTreeWidgetItem *pItem)
{
    return getAbsolutePath(getLevelPath(pItem));
}



//void BBFolderTreeWidget::loadProject()
//{
//    // it is also invoked when the tree needs to be rebuilt
//    // m_pCurrentShowFolderContentItem will be cleared, record corresponding path
//    // find new m_pCurrentShowFolderContentItem, after rebuilding
//    QString currentShowFolderContentPath;
//    if (m_pCurrentShowFolderContentItem)
//    {
//        currentShowFolderContentPath = getAbsolutePath(m_pCurrentShowFolderContentItem);
//    }
//    else
//    {
//        currentShowFolderContentPath = BBConstant::BB_PATH_PROJECT_USER;
//    }
//    // clear tree, and create new tree
//    clear();

//....................................................................
//    sortItems(0, Qt::AscendingOrder);

//    // find new m_pCurrentShowFolderContentItem, after rebuilding
//    if (m_pCurrentShowFolderContentItem)
//    {
//        m_pCurrentShowFolderContentItem = getItemByPath(currentShowFolderContentPath);
//        setItemExpanded(m_pCurrentShowFolderContentItem, true);
//    }
//    setCurrentItem(m_pCurrentShowFolderContentItem);
//    showFolderContent(currentShowFolderContentPath);
////    //之后重新加载工程时 无需加载材质
////    isLoadMaterial = false;
//}





//void BBFolderTreeWidget::newFolder()
//{
//    QTreeWidgetItem *pParent = currentItem();
//    QString parentPath = getAbsolutePath(pParent);
//    QString fileName = "new folder";
//    QString filePath = BBUtils::getExclusiveFolderPath(parentPath, fileName);

//    QDir dir;
//    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath));

//    // create tree item
//    QTreeWidgetItem *pItem = new QTreeWidgetItem({fileName});
//    pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));

//    if (pParent)
//    {
//        pParent->addChild(pItem);
//        setItemExpanded(pParent, true);
//    }
//    else
//    {
//        addTopLevelItem(pItem);
//    }
//    setCurrentItem(pItem);
//    updateCorrespondingWidget(m_pCurrentShowFolderContentItem);
//    // Open the edit box to let the user set name
//    openRenameEditor();
//}

//void BBFolderTreeWidget::addItem(const QString &parentPath, const QString &name)
//{
//    QTreeWidgetItem *pParent = getItemByPath(parentPath);

//    QTreeWidgetItem *pItem = new QTreeWidgetItem({name});
//    pItem->setIcon(0, QIcon(QString(BB_PATH_RESOURCE_ICON) + "folder5.png"));

//    if (pParent)
//    {
//        pParent->addChild(pItem);
//        setItemExpanded(pParent, true);
//    }
//    else
//    {
//        addTopLevelItem(pItem);
//    }
//    sortItems(0, Qt::AscendingOrder);
//}

//void BBFolderTreeWidget::renameItem(const QString &oldName, const QString &newName)
//{
//    // the name of child of m_pCurrentShowFolderContentItem is changed
//    if (m_pCurrentShowFolderContentItem)
//    {
//        for (int i = 0; i < m_pCurrentShowFolderContentItem->childCount(); i++)
//        {
//            QTreeWidgetItem *pItem = m_pCurrentShowFolderContentItem->child(i);
//            if (pItem->text(0) == oldName)
//            {
//                pItem->setText(0, newName);
//                break;
//            }
//        }
//    }
//    else
//    {
//        // at the top level
//        for (int i = 0; i < topLevelItemCount(); i++)
//        {
//            QTreeWidgetItem *pItem = topLevelItem(i);
//            if (pItem->text(0) == oldName)
//            {
//                pItem->setText(0, newName);
//                break;
//            }
//        }
//    }
//    sortItems(0, Qt::AscendingOrder);
//}

//void BBFolderTreeWidget::moveItem(const QString &oldPath, const QString &newPath)
//{
//    // source item
//    QTreeWidgetItem *pItem = getItemByPath(oldPath);
//    // parent of new position
//    QString parentPath = BBUtils::getParentPath(newPath);
//    QTreeWidgetItem *pParent = getItemByPath(parentPath);
//    QString fileName = BBUtils::getFileNameByPath(newPath);
//    // remove child items of old parent
//    if (pItem->parent())
//    {
//        pItem->parent()->removeChild(pItem);
//    }
//    else
//    {
//        takeTopLevelItem(indexOfTopLevelItem(pItem));
//    }
//    // insert to new parent
//    if (pParent)
//    {
//        pParent->addChild(pItem);
//    }
//    else
//    {
//        addTopLevelItem(pItem);
//    }
//    // rename
//    pItem->setText(0, fileName);
//    sortItems(0, Qt::AscendingOrder);
//}

//void BBFolderTreeWidget::deleteItem(const QString &folderPath)
//{
//    QTreeWidgetItem *pItem = getItemByPath(folderPath);
//    // remove from clipBoard
////    if (clipBoardItems.contains(pItem))
////    {
////        clipBoardItems.removeOne(pItem);
////    }
//    BB_SAFE_DELETE(pItem);
//}

//void BBFolderTreeWidget::showInFolder()
//{
//    QTreeWidgetItem *pItem = currentItem();
//    QString filePath = getAbsolutePath(pItem);

//    BB_PROCESS_ERROR_RETURN(BBUtils::showInFolder(filePath));
//}

//void BBFolderTreeWidget::finishRename()
//{
//    BB_PROCESS_ERROR_RETURN(m_pEditingItem);

//    // whether name is changed
//    // new name cannot be null
//    QString name = m_pRenameEditor->text();
//    QString preName = m_pEditingItem->text(0);
//    if (preName != name && !name.isEmpty())
//    {
//        // previous path
//        QString previous = getAbsolutePath(m_pEditingItem);
//        QString parentPath = BBUtils::getParentPath(previous);
//        // Check if there are the same names
//        QString current = BBUtils::getExclusiveFolderPath(parentPath, name);

//        BB_PROCESS_ERROR_RETURN(QFile::rename(previous, current));
//        m_pEditingItem->setText(0, name);

//        // handle corresponding folder in the engine folder
//        QFile::rename(BBUtils::getEngineAuxiliaryFolderPath(previous),
//                      BBUtils::getEngineAuxiliaryFolderPath(current));

//        // After renaming, remove it from the clipboard
//        if (m_ClipBoardItems.contains(m_pEditingItem))
//        {
//            m_ClipBoardItems.removeOne(m_pEditingItem);
//        }
//        // After renaming, sort
//        sortItems(0, Qt::AscendingOrder);

//        updateCorrespondingWidget(m_pEditingItem);
//    }
//    BBTreeWidget::finishRename();
//}

//void BBFolderTreeWidget::deleteAction()
//{
//    BBTreeWidget::deleteAction();

//    updateCorrespondingWidget(m_pCurrentShowFolderContentItem);

////    //刷新材质文件的映射 被删除的材质文件的映射不再占用内存
////    Material::updateMap();
////    //清空属性栏 包括场景 层级视图选中
////    cancelHierarchyTreeSelectedItems();
////    clearPropertyWidget();
//}







//void BBFolderTreeWidget::deleteOne(QTreeWidgetItem *pItem)
//{
//    // delete folder
//    QString path = getAbsolutePath(pItem);
//    QDir dir(path);
//    BB_PROCESS_ERROR_RETURN(dir.removeRecursively());

//    // delete corresponding folder in the engine folder
//    dir = QDir(BBUtils::getEngineAuxiliaryFolderPath(path));
//    BB_PROCESS_ERROR_RETURN(dir.removeRecursively());

//    // if showing its content, clear, and show root folder
//    if (pItem == m_pCurrentShowFolderContentItem)
//    {
//        m_pCurrentShowFolderContentItem = NULL;
//    }

//    BBTreeWidget::deleteOne(pItem);
//}

//void BBFolderTreeWidget::dragMoveEvent(QDragMoveEvent *event)
//{
//    if (!event->mimeData()->hasFormat(getMimeType()) && !event->mimeData()->hasFormat(BB_MIMETYPE_FILELISTWIDGET))
//    {
//        event->ignore();
//        return;
//    }
//    BBTreeWidget::dragMoveEvent(event);
//}

//bool BBFolderTreeWidget::moveItem()
//{
//    QList<QTreeWidgetItem*> items = selectedItems();
//    // Calculate the original hierarchical position of the selected items for moving or undoing the move
//    // save the last data into queue of undoing (to do ...)
//    QList<QString> originalItemLocations;
//    for (int i = 0; i < items.count(); i++)
//    {
//        originalItemLocations.append(getLevelPath(items.at(i)));
//    }

//    BB_PROCESS_ERROR_RETURN_FALSE(BBTreeWidget::moveItem());

//    for (int i = 0; i < originalItemLocations.count(); i++)
//    {
//        QString oldPath = getAbsolutePath(originalItemLocations.at(i));
//        QString newPath = getAbsolutePath(items.at(i));
//        // The path is not changed
//        // it just changes the order among the siblings, no need to perform follow-up operation
//        if (oldPath == newPath)
//            continue;
//        // check duplication of name
//        QString newName = BBUtils::getFileNameByPath(newPath);
//        newPath = BBUtils::getExclusiveFolderPath(BBUtils::getParentPath(newPath), newName);
//        // corresponding item in the tree needs to rename
//        items.at(i)->setText(0, newName);

//        BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::moveFolder(oldPath, newPath, false));
//    }
//    sortItems(0, Qt::AscendingOrder);
//    // show m_pCurrentShowFolderContentItem
//    // it may be moved, path needs to be computed again
//    QString updatePath = getAbsolutePath(m_pCurrentShowFolderContentItem);
//    showFolderContent(updatePath);
//    return true;
//}


//bool BBFolderTreeWidget::moveItemFromFileList(const QMimeData *pMimeData)
//{
//    BB_PROCESS_ERROR_RETURN_FALSE(m_pIndicatorItem);
//    // new parent path
//    QString destPath;
//    if (m_eIndicatorPos == BBIndicatorPos::CENTER)
//    {
//        // become the children of m_pIndicatorItem
//        destPath = getAbsolutePath(m_pIndicatorItem);
//    }
//    else
//    {
//        // become the siblings of m_pIndicatorItem
//        destPath = getAbsolutePath(m_pIndicatorItem->parent());
//    }
//    // get all file paths from drag
//    QList<QString> sourceFilePaths;
//    QByteArray data = pMimeData->data(BB_MIMETYPE_FILELISTWIDGET);
//    QDataStream dataStream(&data, QIODevice::ReadOnly);
//    QString sourceFilePath;
//    // the first is the path of current item, which we do not need
//    // we need the paths of selected items which are saved behind
//    dataStream >> sourceFilePath;
//    dataStream >> sourceFilePath;
//    while (!sourceFilePath.isEmpty())
//    {
//        sourceFilePaths.append(sourceFilePath);
//        // check whether the movement is legal
//        BBUtils::isMovablePath(sourceFilePath, destPath);
//        dataStream >> sourceFilePath;
//    }
//    // when the movement is legal, move all files into destPath
//    for (int i = 0; i < sourceFilePaths.count(); i++)
//    {
//        QString oldPath = sourceFilePaths.at(i);
//        QString fileName = BBUtils::getFileNameByPath(oldPath);
//        if (QFileInfo(oldPath).isDir())
//        {
//            QString newPath = BBUtils::getExclusiveFolderPath(destPath, fileName);
//            BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::moveFolder(oldPath, newPath, false));

//            // move folder tree items at the same time
//            moveItem(oldPath, newPath);
//        }
//        else
//        {
////            QString newPath = BBUtils::getExclusiveFilePath(destPath, fileName);
////            BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::moveFile(oldPath, newPath, pFileInfo->m_eFileType, false));
//        }
//    }
//    updateCorrespondingWidget(m_pCurrentShowFolderContentItem);
//    return true;
//}











//-------------------------------------------




//void ProjectTree::copyAction()
//{
//    //重置剪贴板
//    BaseTree::copyAction();
//    //遍历剪贴板的项 算出对应的路径
//    QList<QString> folderPaths;
//    int count = clipBoardItems.count();
//    for (int i = 0; i < count; i++)
//    {
//        QString folderPath = getFilePath(getLevelPath(clipBoardItems.at(i)));
//        folderPath = folderPath.mid(0, folderPath.length() - 1);
//        folderPaths.append(folderPath);
//    }
//    //发送信号给文件列表 修改文件列表的剪贴板
//    copyToFileList(folderPaths);
//}

//void ProjectTree::copyByFileList(QList<QString> filePaths)
//{
//    //在文件列表中复制 修改文件夹树的剪贴板
//    //清空上一次剪贴板存下的复制内容
//    clipBoardItems.clear();
//    clipBoardFilePaths.clear();
//    //父亲孩子不会同时被选中
//    int count = filePaths.count();
//    for (int i = 0; i < count; i++)
//    {
//        QString path = filePaths.at(i);
//        QFileInfo *fileInfo = new QFileInfo(path);
//        if (fileInfo->isDir())
//        {
//            clipBoardItems.append(getItemByPath(path));
//        }
//        else
//        {
//            //不是文件夹的文件没有对应树结点 存入路径 粘贴时只进行文件粘贴操作 不进行树节点的粘贴操作
//            clipBoardFilePaths.append(path);
//        }
//    }
//}

//void ProjectTree::pasteAction()
//{
//    //清空用于存粘贴的副本文件夹名字 用于高亮显示
//    pastedFolderNames.clear();
//    BaseTree::pasteAction();
//}

//void ProjectTree::pasteEnd()
//{
//    //粘贴结束后调用 粘贴不合法时 不会调用
//    //树重新排序
//    sortItems(0, Qt::AscendingOrder);
//    //处理剪贴板中的非文件夹文件 同时传副本文件夹的路径 用于高亮显示
//    QString destPath = getFilePath(getLevelPath(currentItem()));
//    pasteFile(clipBoardFilePaths, destPath.mid(0, destPath.length() - 1), pastedFolderNames);
//    //可能复制到当前显示的文件夹下了 更新文件列表 这个操作放在pasteFile里执行
//}

//void ProjectTree::pasteOne(QTreeWidgetItem *source, QTreeWidgetItem* transcript)
//{
//    //复制文件夹
//    QString sourcePath = getFilePath(getLevelPath(source));
//    sourcePath = sourcePath.mid(0, sourcePath.length() - 1);
//    QString transcriptPath = getFilePath(getLevelPath(transcript));
//    transcriptPath = transcriptPath.mid(0, transcriptPath.length() - 1);
//    copyDirectoryFiles(sourcePath, transcriptPath);
//    //复制对应meta文件夹
//    copyDirectoryFiles(FileList::getMetaFilePath(sourcePath), FileList::getMetaFilePath(transcriptPath));
//    //存副本文件夹名字 用于文件列表的高亮显示
//    pastedFolderNames.append(transcriptPath.mid(transcriptPath.lastIndexOf('/') + 1));
//}

//void ProjectTree::pasteItemWithoutPasteFile(QList<QString> clipBoardTranscriptFolderNames)
//{
//    //只粘贴树节点 不粘贴文件 粘贴文件的操作已经在FileList中执行了
//    //修改目的结点currentItem为FileList所指 下次在ProjectTree中粘贴 也使用这个目的结点
//    setCurrentItem(currentShowFolderContentItem);
//    //能执行到这个函数 粘贴操作一定合法
//    //去掉选中项的高亮
//    QList<QTreeWidgetItem*> selected = selectedItems();
//    int count = selected.count();
//    for (int i = 0; i < count; i++)
//    {
//        setItemSelected(selected.at(i), false);
//    }
//    //遍历剪贴板
//    count = clipBoardItems.count();
//    for (int i = 0; i < count; i++)
//    {
//        QTreeWidgetItem *item = clipBoardItems.at(i);
//        //拷贝结点
//        QTreeWidgetItem *transcript = item->clone();
//        //副本的名字可能重复 重命名
//        transcript->setText(0, clipBoardTranscriptFolderNames.at(i));
//        //在目的结点下接入拷贝的结点
//        if (currentShowFolderContentItem)
//        {
//            currentShowFolderContentItem->addChild(transcript);
//        }
//        else
//        {
//            addTopLevelItem(transcript);
//        }
//        //粘贴项高亮
//        setItemSelected(transcript, true);
//    }
//    //展开目的结点
//    if (currentShowFolderContentItem)
//        setItemExpanded(currentShowFolderContentItem, true);
//    //重新排序
//    sortItems(0, Qt::AscendingOrder);
//}

//void ProjectTree::removeClipBoardRenameItem(QString path)
//{
//    clipBoardItems.removeOne(getItemByPath(path));
//}



//void ProjectTree::loadMaterial(QString filePath)
//{
//    if (isLoadMaterial)
//    {
//        new Material(filePath);
//    }
//}

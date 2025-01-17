#include "BBFileListWidget.h"
#include <QWidgetAction>
#include <QMenu>
#include <QHBoxLayout>
#include <QLabel>
#include "Dialog/BBConfirmationDialog.h"
#include <QMimeData>
#include <QScrollBar>
#include <QDrag>
#include "BBFileSystemDataManager.h"
#include <QPainter>
#include <QFileInfo>


//---------------------------------------------------------------------------------------------------
//  BBPlainTextEdit
//---------------------------------------------------------------------------------------------------

BBPlainTextEdit::BBPlainTextEdit(QWidget *pParent)
    : QPlainTextEdit(pParent)
{

}

void BBPlainTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    editFinished();
}

void BBPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        editFinished();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}


//---------------------------------------------------------------------------------------------------
//  BBFileListWidget
//---------------------------------------------------------------------------------------------------

QSize BBFileListWidget::m_ItemSize = m_StandardItemSize;

QSize BBFileListWidget::m_StandardIconSize = QSize(43, 43);
QSize BBFileListWidget::m_StandardItemSize = QSize(180, 45);
//QSize BBFileListWidget::m_StandardIconSize = QSize(53, 53);
//QSize BBFileListWidget::m_StandardItemSize = QSize(55, 100);


BBFileListWidget::BBFileListWidget(QWidget *pParent)
    : QListWidget(pParent), m_eSenderTag(BBSignalSender::FileList)
{
    m_pEditingItem = NULL;
    m_pRenameEditor = NULL;
    m_pIndicatorItem = NULL;
    m_pFileData = NULL;

    setFlow(QListView::LeftToRight);
    // icon is at the top, and text is at the bottom
    // setViewMode(QListView::IconMode);
    // Dynamically adjust the layout position according to the size of the list container
    // From left to right and from top to bottom
    setResizeMode(QListView::Adjust);
    setIconSize(m_StandardIconSize);
    setSpacing(2);
    // Text can be showed in multiple lines
    setWordWrap(true);
    setWrapping(true);
    setAcceptDrops(true);
    setDragEnabled(true);
    // You can press shift and cmd to select multiple
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setMenu();

    QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(clickItem(QListWidgetItem*)));
    QObject::connect(this, SIGNAL(itemPressed(QListWidgetItem*)),
                     this, SLOT(pressItem(QListWidgetItem*)));
    QObject::connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(doubleClickItem(QListWidgetItem*)));
    QObject::connect(this, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                     this, SLOT(changeCurrentItem(QListWidgetItem*, QListWidgetItem*)));
}

BBFileListWidget::~BBFileListWidget()
{
    BB_SAFE_DELETE(m_pMenu);
}

void BBFileListWidget::loadItems(const QString &parentPath, BBFILE *pFileData, QListWidgetItem *pCurrentItem)
{
    // show the contents of the newly selected folder, the original list is cleared
    // just remove from the list, cannot delete the items, so cannot use clear();
    while (count() > 0)
    {
        takeItem(0);
    }

    for (BBFILE::Iterator it = pFileData->begin(); it != pFileData->end(); it++)
    {
        addItem(it.key());
    }
    m_pFileData = pFileData;

    m_ParentPath = parentPath;
    sortItems();
    setCurrentItem(pCurrentItem);
}

void BBFileListWidget::setSelectedItems(const QList<QListWidgetItem*> &items)
{
    for (int i = 0; i < items.count(); i++)
    {
        setItemSelected(items.at(i), true);
    }
}

void BBFileListWidget::clickItem(QListWidgetItem *pItem)
{
    emit clickItem(getPathByItem(pItem), m_pFileData->value(pItem)->m_eFileType);
}

void BBFileListWidget::pressItem(QListWidgetItem *pItem)
{
    emit pressItem(m_pFileData->value(pItem)->m_eFileType);
}

void BBFileListWidget::doubleClickItem(QListWidgetItem *pItem)
{
    emit openFile(getPathByItem(pItem), m_pFileData->value(pItem)->m_eFileType);
}

void BBFileListWidget::changeCurrentItem(QListWidgetItem *pCurrent, QListWidgetItem *pPrevious)
{
    BBFileType eCurrentType = BBFileType::Other;
    BBFileType ePreviousType = BBFileType::Other;
    BBFileInfo *pFileInfo = m_pFileData->value(pCurrent);
    if (pFileInfo)
    {
        eCurrentType = pFileInfo->m_eFileType;
    }
    pFileInfo = m_pFileData->value(pPrevious);
    if (pFileInfo)
    {
        ePreviousType = pFileInfo->m_eFileType;
    }
    emit changeCurrentItem(eCurrentType, ePreviousType);
}

void BBFileListWidget::changeItemSize(int factor)
{

}

void BBFileListWidget::newFolder()
{
    // Otherwise, after creating, several items will be selected
    QList<QListWidgetItem*> items = selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        setItemSelected(items.at(i), false);
    }
    emit newFolder(m_ParentPath, m_eSenderTag);
    openRenameEditor();
}

void BBFileListWidget::newSceneAction()
{
    newFile(0);
}

void BBFileListWidget::newMaterialAction()
{
    newFile(1);
}

void BBFileListWidget::newScript()
{

}

void BBFileListWidget::showInFolder()
{
    emit showInFolder(getPathByItem(currentItem()));
}

void BBFileListWidget::copyAction()
{

}

void BBFileListWidget::pasteAction()
{

}

void BBFileListWidget::openRenameEditor()
{
    QList<QListWidgetItem*> items = selectedItems();
    if (items.count() == 1)
    {
        m_pEditingItem = items.first();
        QWidget *pWidget = new QWidget(this);
        QVBoxLayout *pLayout = new QVBoxLayout(pWidget);
        pLayout->setContentsMargins(1, 2, 1, 2);
        pLayout->setSpacing(3);
        m_pRenameEditor = new BBPlainTextEdit(pWidget);
        m_pRenameEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // margin: 47px 1px 2px 1px;
        m_pRenameEditor->setStyleSheet("border: none; background: #d6dfeb; color: #191f28;"
                                       "selection-color: #d6dfeb; selection-background-color: #8193bc;"
                                       "font: 9pt \"Arial\"; padding: -2px;");
        m_pRenameEditor->setPlainText(m_pEditingItem->text());
        m_pRenameEditor->selectAll();
        QObject::connect(m_pRenameEditor, SIGNAL(editFinished()), this, SLOT(finishRename()));
        pLayout->addWidget(m_pRenameEditor, 1);
        setItemWidget(m_pEditingItem, pWidget);
        m_pRenameEditor->setFocus();
    }
}

void BBFileListWidget::finishRename()
{
    BB_PROCESS_ERROR_RETURN(m_pEditingItem);
    // without suffix
    QString newBaseName = m_pRenameEditor->toPlainText();
    QString oldName = m_pFileData->value(m_pEditingItem)->m_FileName;
    QString suffix = BBFileSystemDataManager::getFileSuffix(oldName);
    QString newName = newBaseName;
    if (suffix.count() > 0)
    {
        newName += "." + suffix;
    }
    if (oldName != newName)
    {
        emit rename(m_pEditingItem, m_ParentPath + "/" + oldName, m_ParentPath + "/" + newName);
    }
    removeItemWidget(m_pEditingItem);
    BB_SAFE_DELETE(m_pRenameEditor);
    m_pEditingItem = NULL;
    // Otherwise, the list has no focus and no longer triggers key events
    setFocus();
    sortItems();
}

void BBFileListWidget::deleteAction()
{
    QList<QListWidgetItem*> items = selectedItems();
    BB_PROCESS_ERROR_RETURN(items.count() > 0);

    BBConfirmationDialog dialog;
    dialog.setTitle("Delete selected?");
    if (items.count() == 1)
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete this?");
    }
    else
    {
        dialog.setMessage("You cannot undo this action.\n\nAre you sure to delete these "
                          + QString::number(items.count()) + " items?");
    }
    if (dialog.exec())
    {
        emit deleteFiles(items);

        setCurrentItem(NULL);
//        //清空属性栏 包括场景 层级视图选中
//        cancelHierarchyTreeSelectedItems();
//        itemClickedSlot(NULL);
    }
}

void BBFileListWidget::setMenu()
{
    // first level menu
    m_pMenu = new QMenu(this);
    QAction *pActionNewFolder = new QAction(tr("New Folder"));
    QAction *pLabelNewAsset = new QAction(tr("New Asset ..."));
    // As the title of a column, not clickable
    pLabelNewAsset->setEnabled(false);

    QWidgetAction *pActionNewScene = createWidgetAction(BB_PATH_RESOURCE_ICON(scene.png), tr("Scene"));
    QWidgetAction *pActionNewMaterial = createWidgetAction(BB_PATH_RESOURCE_ICON(material.png), tr("Material"));
    QWidgetAction *pActionNewScript = createWidgetAction(BB_PATH_RESOURCE_ICON(script.png), tr("Script"));

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
    QMenu *pMenuSort = new QMenu(tr("Sort By"), m_pMenu);
    QAction *pActionNameSort = new QAction(tr("Name"));
    QAction *pActionTypeSort = new QAction(tr("Type"));
    QAction *pActionCreateDateSort = new QAction(tr("Create Date"));
    QAction *pActionModifyDateSort = new QAction(tr("Modify Date"));
    pMenuSort->addAction(pActionNameSort);
    pMenuSort->addAction(pActionTypeSort);
    pMenuSort->addAction(pActionCreateDateSort);
    pMenuSort->addAction(pActionModifyDateSort);
    // slider
    QWidgetAction *pActionSlider = new QWidgetAction(m_pMenu);
    QWidget *pWidget = new QWidget(this);
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->setContentsMargins(6, 1, 6, 1);
    QLabel *pLabel = new QLabel("Size", pWidget);
    pLabel->setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\";");
    pLayout->addWidget(pLabel);
    QSlider *pSlider = new QSlider(Qt::Horizontal, pWidget);
    pSlider->setRange(-2, 2);
    pSlider->setValue(0);
    QObject::connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(changeItemSize(int)));
    pLayout->addWidget(pSlider);
    pActionSlider->setDefaultWidget(pWidget);

    m_pMenu->addAction(pActionNewFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pLabelNewAsset);
    m_pMenu->addAction(pActionNewScene);
    m_pMenu->addAction(pActionNewMaterial);
    m_pMenu->addAction(createWidgetAction(BB_PATH_RESOURCE_ICON(animation.png), tr("Animation")));
    m_pMenu->addAction(createWidgetAction(BB_PATH_RESOURCE_ICON(particle.png), tr("Particle")));
    m_pMenu->addAction(pActionNewScript);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionShowInFolder);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionCopy);
    m_pMenu->addAction(pActionPaste);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionRename);
    m_pMenu->addAction(pActionDelete);
    m_pMenu->addSeparator();
    m_pMenu->addMenu(pMenuSort);
    m_pMenu->addSeparator();
    m_pMenu->addAction(pActionSlider);

    QObject::connect(pActionNewFolder, SIGNAL(triggered()), this, SLOT(newFolder()));
    QObject::connect(pActionNewScene, SIGNAL(triggered()), this, SLOT(newSceneAction()));
    QObject::connect(pActionNewMaterial, SIGNAL(triggered()), this, SLOT(newMaterialAction()));
    QObject::connect(pActionNewScript, SIGNAL(triggered()), this, SLOT(newScript()));
    QObject::connect(pActionShowInFolder, SIGNAL(triggered()), this, SLOT(showInFolder()));
    QObject::connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copyAction()));
    QObject::connect(pActionPaste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    QObject::connect(pActionRename, SIGNAL(triggered()), this, SLOT(openRenameEditor()));
    QObject::connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

QWidgetAction* BBFileListWidget::createWidgetAction(const QString &iconPath, const QString &name)
{
    QWidgetAction *pAction = new QWidgetAction(m_pMenu);
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

void BBFileListWidget::newFile(int nType)
{
    // 0 scene
    emit newFile(m_ParentPath, nType);
    openRenameEditor();
}

void BBFileListWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);
    QListWidgetItem *pCenterItem = currentItem();
    QRect centerItemRect = visualItemRect(pCenterItem);
    // Used to calculate the absolute position of each item that needs to be dragged
    int hPos = horizontalScrollBar()->sliderPosition();
    int vPos = verticalScrollBar()->sliderPosition();
    // step of each item
    int hStep = centerItemRect.width() + spacing();
    int vStep = centerItemRect.height() + spacing();
    // The size of a single icon
    static int nSize = iconSize().width() * devicePixelRatio();
    // Record the row and column value and icon of the selected item
    struct Info
    {
        int nRow;
        int nColumn;
        QPixmap icon;
    };
    QList<Info> infos;
    // Used for the size of final icon
    int nMaxRow = 0;
    int nMaxColumn = 0;
    int nMinRow = INT_MAX;
    int nMinColumn = INT_MAX;
    // drag all selected items
    QList<QListWidgetItem*> items = selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        Info info;
        QListWidgetItem *pItem = items.at(i);
        QRect rect = visualItemRect(pItem);
        // rect is relative to the position of the scroll bar
        // translate and make it relative to 0
        rect.translate(hPos - spacing(), vPos - spacing());
        // Calculate the row and column value of the item based on the step
        info.nColumn = rect.left() / hStep;
        info.nRow = rect.top() / vStep;
        if (info.nColumn > nMaxColumn)
        {
            nMaxColumn = info.nColumn;
        }
        if (info.nColumn < nMinColumn)
        {
            nMinColumn = info.nColumn;
        }
        if (info.nRow > nMaxRow)
        {
            nMaxRow = info.nRow;
        }
        if (info.nRow < nMinRow)
        {
            nMinRow = info.nRow;
        }
        info.icon = pItem->icon().pixmap(nSize);
        info.icon.setDevicePixelRatio(devicePixelRatio());
        info.icon = info.icon.scaled(nSize, nSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        infos.append(info);
    }
    QPoint hotSpot;
    // final icon
    QPixmap pixmap((nSize + 6) * (nMaxColumn - nMinColumn + 1), (nSize + 6) * (nMaxRow - nMinRow + 1));
    pixmap.setDevicePixelRatio(devicePixelRatio());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    // paint the icon of each item to the final icon according to the row and column value of each item
    for (int i = 0; i < infos.count(); i++)
    {
        int x = (infos.at(i).nColumn - nMinColumn) * (nSize + 6) / devicePixelRatio();
        int y = (infos.at(i).nRow - nMinRow) * (nSize + 6) / devicePixelRatio();
        painter.drawPixmap(x, y, infos.at(i).icon);
        if (items.at(i) == pCenterItem)
        {
            // Take the center of pCenterItem as the mouse position
            hotSpot.setX(x + nSize / devicePixelRatio() / 2);
            hotSpot.setY(y + nSize / devicePixelRatio() / 2);
        }
    }
    painter.end();
    // Pack relevant data
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    // The pCenterItem is written in the first one, used for the situation that you just can read one item
    QString filePath = getPathByItem(pCenterItem);
    dataStream << filePath;
    // all selected items is written, used for the situation that you can read all selected items
    for (int i = 0; i < items.count(); i++)
    {
        filePath = getPathByItem(items.at(i));
        dataStream << filePath;
    }
    QMimeData *pMimeData = new QMimeData;
    // Give this data a unique identifying name
    pMimeData->setData(getMimeType(), itemData);
    QDrag drag(this);
    drag.setMimeData(pMimeData);
    drag.setHotSpot(hotSpot);
    drag.setPixmap(pixmap);
    drag.exec(Qt::MoveAction);
}

void BBFileListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->urls().isEmpty())
    {
        // the outside of the editor
        event->accept();
    }
    else if (event->mimeData()->hasFormat(getMimeType()))
    {
        // internal drag
        event->accept();
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void BBFileListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    // Scroll up and down when the mouse is at the top or bottom
    // QListWidget::dragMoveEvent(event); invalidates dropEvent
    int y = event->pos().y();
    if (y < 10)
    {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - 10);
    }
    else if (y >= height() - 10)
    {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() + 10);
    }
    // Drop position indicator
    QListWidgetItem *pItem = itemAt(event->pos());
    // Clear the last indicator position painted
    m_pIndicatorItem = NULL;
    if (pItem)
    {
        // only folder can show drop indicator
        QFileInfo fileInfo(getPathByItem(pItem));
        if (fileInfo.isDir())
        {
            // When dragging internally, the drop position cannot be the folder being dragged
            // It doesn't matter if it is dragged from the folder tree, since the item has been judged
            if ((event->mimeData()->hasFormat(getMimeType()) && !selectedItems().contains(pItem))
                    || event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
            {
                m_pIndicatorItem = pItem;
            }
        }
    }
    repaint();
}

void BBFileListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    // No need for indicator when dragLeave
    m_pIndicatorItem = NULL;
    repaint();
}

void BBFileListWidget::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->urls().isEmpty())
    {
        // from outside of the editor
        event->accept();

        QString parentPath = m_ParentPath;
        if (m_pIndicatorItem)
        {
            // Drag to the folder in the file list and import the asset into this folder
            parentPath += "/" + m_pFileData->value(m_pIndicatorItem)->m_FileName;
        }
        emit importAsset(parentPath, event->mimeData()->urls());
        // Open the asset import management dialog
        // BBAssetManager assetManager;
        // assetManager.exec();
    }
    else if (event->mimeData()->hasFormat(getMimeType()))
    {
        if (m_pIndicatorItem)
        {
            // there is a drop spot
            if (moveItem())
            {
                event->accept();
            }
            else
            {
                event->ignore();
            }
        }
        else
        {
            event->ignore();
        }
    }
    else if (event->mimeData()->hasFormat(BB_MIMETYPE_FOLDERTREEWIDGET))
    {
        if (moveItemFromFolderTree(event->mimeData()))
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }

    // The indicator is no longer needed after drop
    m_pIndicatorItem = NULL;
    repaint();
}

bool BBFileListWidget::moveItem()
{
    // move selected items into m_pIndicatorItem
    QList<QListWidgetItem*> items = selectedItems();
    emit moveFiles(items, m_ParentPath, m_ParentPath + "/" + m_pFileData->value(m_pIndicatorItem)->m_FileName, false);
    return true;
}

bool BBFileListWidget::moveItemFromFolderTree(const QMimeData *pMimeData)
{
    QString newParentPath = m_ParentPath;
    if (m_pIndicatorItem)
    {
        // Drag to the folder in the file list and move the asset into this folder
        newParentPath += "/" + m_pFileData->value(m_pIndicatorItem)->m_FileName;
    }

    QList<QString> oldFilePaths;
    QByteArray data = pMimeData->data(BB_MIMETYPE_FOLDERTREEWIDGET);
    QDataStream dataStream(&data, QIODevice::ReadOnly);
    QString levelPath;
    dataStream >> levelPath;
    while (!levelPath.isEmpty())
    {
        QString oldFilePath = BBConstant::BB_PATH_PROJECT_USER + "/" + levelPath;
        oldFilePaths.append(oldFilePath);
        dataStream >> levelPath;
    }

    emit moveFolders(oldFilePaths, newParentPath, false);

    return true;
}

void BBFileListWidget::paintEvent(QPaintEvent *event)
{
    QListWidget::paintEvent(event);

    // using "this" is wrong
    QPainter painter(viewport());
    // The file type logo color is painted in the top left corner
    for (int i = 0; i < count(); i++)
    {
        // Get the logo color of the corresponding file type of the item
        QColor color = BBFileSystemDataManager::getFileLogoColor(m_pFileData->value(item(i))->m_eFileType);
        if (color == nullptr)
            continue;
        QPen pen(color);
        painter.setPen(pen);
        // item position
        QRect rect = visualItemRect(item(i));
        for (int j = 8; j < 20; j++)
        {
            QPoint p1 = rect.topLeft() + QPoint(4, -1 + j);
            QPoint p2 = rect.topLeft() + QPoint(4 + j, -1);
            painter.drawLine(p1, p2);
        }
    }
    // paint drop indicator
    if (m_pIndicatorItem)
    {
        painter.setPen(QColor("#d6dfeb"));
        QRect rect = visualItemRect(m_pIndicatorItem);
        painter.drawRect(rect);
    }
    painter.end();
}

void BBFileListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        // There is no item at the mouse click position, remove the selection
        QListWidgetItem *pItem = itemAt(event->pos());
        if (!pItem)
        {
            setCurrentItem(NULL);
        }
    }
}

void BBFileListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    m_pMenu->exec(cursor().pos());
}

void BBFileListWidget::keyPressEvent(QKeyEvent *event)
{
    // Handling menu shortcut events
    int key;
#if defined(Q_OS_WIN32)
    key = Qt::Key_F2;
#elif defined(Q_OS_MAC)
    key = Qt::Key_Return;
#endif
    if (event->key() == key)
    {
        openRenameEditor();
    }
}

void BBFileListWidget::focusInEvent(QFocusEvent *event)
{
    // parent class, when the focus is obtained, the first item will show a blue box, which is ugly
    Q_UNUSED(event);
    setFocus();
    emit inFocus();
}

QString BBFileListWidget::getPathByItem(QListWidgetItem *pItem)
{
    if (pItem)
    {
        return m_ParentPath + "/" + m_pFileData->value(pItem)->m_FileName;
    }
    else
    {
        return m_ParentPath;
    }
}











//void FileList::changeItemSize(int factor)
//{
//    /*//修改图标大小item大小
//    setIconSize(standardIconSize + factor * 5 * QSize(1, 1));
//    itemSize = standardItemSize + factor * 5 * QSize(1, 1);
//    //刷新列表
//    showFolderContent(mFolderPath);*/
//}


//void FileList::updateMaterialFileIcon(QString filePath)
//{
//    QString onlyPath = filePath.mid(0, filePath.lastIndexOf('/'));
//    if (mFolderPath == onlyPath)
//    {
//        //被修改的材质文件当前正在文件列表中被显示
//        QString fileName = filePath.mid(filePath.lastIndexOf('/') + 1);
//        //findItems(fileName, Qt::MatchExactly);不行 因为text被换行了
//        //找该文件对应的item
//        QMap<QListWidgetItem*, FileInfo*>::Iterator itr;
//        for (itr = mMap.begin(); itr != mMap.end(); itr++)
//        {
//            FileInfo *fileInfo = itr.value();
//            if (fileInfo->mFileName == fileName)
//            {
//                //找该文件对应的item 修改图标
//                QListWidgetItem *item = itr.key();
//                Material *material = Material::mtlMap.value(filePath);
//                item->setIcon(QIcon(material->getPreview()));
//                break;
//            }
//        }
//    }
//}

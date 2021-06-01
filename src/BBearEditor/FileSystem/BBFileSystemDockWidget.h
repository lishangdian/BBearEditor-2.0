#ifndef BBFILESYSTEMDOCKWIDGET_H
#define BBFILESYSTEMDOCKWIDGET_H

#include <QDockWidget>


class QTreeWidgetItem;
class QListWidgetItem;
class BBOpenGLWidget;
class BBFileSystemData;


enum BBSignalSender
{
    FolderTree = 0,
    FileList = 1
};


namespace Ui {
class BBFileSystemDockWidget;
}

class BBFileSystemDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit BBFileSystemDockWidget(QWidget *pParent = 0);
    ~BBFileSystemDockWidget();

    void createProject();
    void openProject();

private slots:
    void clickItemInFolderTree(const QString &filePath, QTreeWidgetItem *pItem);
    void doubleClickItemInFileList(const QString &filePath);
    void clickItemInFolderPathBar(const QString &filePath);

    void newFolder(const QString &parentPath, const BBSignalSender &eSender);
    void showInFolder(const QString &filePath);

private:
    void setConnect();

    void updateFolderTree();
    void updateFolderTree(const QString &filePath);
    void updateFileList(const QString &parentPath, QTreeWidgetItem *pParentFolderItem, QListWidgetItem *pCurrentItem = 0);
    void updateFileList(const QString &parentPath, QListWidgetItem *pCurrentItem = 0);
    void updateFolderPathBar(const QString &filePath);

    Ui::BBFileSystemDockWidget *m_pUi;
    BBOpenGLWidget *m_pPreviewOpenGLWidget;
    BBFileSystemData *m_pData; 
};

#endif // BBFILESYSTEMDOCKWIDGET_H
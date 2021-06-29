#include "BBPreviewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Utils/BBUtils.h"
#include "3D/BBModel.h"
#include "IO/BBMaterialFileManager.h"
#include "Render/BBMaterial.h"


BBPreviewOpenGLWidget::BBPreviewOpenGLWidget(QWidget *pParent)
    : BBOpenGLWidget(pParent)
{
    BBMaterialFileManager::bindPreviewOpenGLWidget(this);
    // sphere for preview
    m_pSphere = NULL;
}

void BBPreviewOpenGLWidget::saveMaterialPreview(BBMaterial *pMaterial)
{
    if (!m_pSphere)
    {
        createSphere();
    }
    m_pSphere->setCurrentMaterial(pMaterial);
    m_pScene->enableFBO(true);
    update();
    QPixmap pix = m_pScene->getFBOPixmap();
    pMaterial->setOverviewMap(pix);
    m_pScene->enableFBO(false);
}

void BBPreviewOpenGLWidget::updateMaterialSphere(BBMaterial *pMaterial)
{
    BB_PROCESS_ERROR_RETURN(m_pSphere);
    BB_PROCESS_ERROR_RETURN(pMaterial);
    m_pSphere->setCurrentMaterial(pMaterial);
    update();
}

void BBPreviewOpenGLWidget::showMaterialPreview(const QString &filePath)
{
    if (!m_pSphere)
    {
        createSphere();
        m_pSphere->setCurrentMaterial(BBMaterialFileManager::loadMaterial(filePath));
    }
    else
    {
        m_pSphere->setActivity(true);
        m_pSphere->setVisibility(false);
    }
    update();
}

void BBPreviewOpenGLWidget::removeMaterialPreview()
{
    BB_PROCESS_ERROR_RETURN(m_pSphere);
    m_pSphere->setActivity(false);
    update();
}

void BBPreviewOpenGLWidget::createSphere()
{
    m_pSphere = m_pScene->createModelForPreview(BB_PATH_RESOURCE_MESH(sphere.obj));
}

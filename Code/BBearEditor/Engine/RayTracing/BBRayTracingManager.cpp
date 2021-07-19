#include "BBRayTracingManager.h"
#include "Utils/BBUtils.h"
#include "Scene/BBSceneManager.h"
#include "Render/BBEditViewOpenGLWidget.h"
#include "Scene/BBScene.h"
#include "Render/BBCamera.h"
#include "Render/BBMaterial.h"
#include "Scene/BBRendererManager.h"
#include "2D/BBFullScreenQuad.h"


BBRayTracingManager::BBRayTracingManager(BBScene *pScene)
{
    m_pScene = pScene;
    m_nCurrentRenderedPixel = 0;
    BBCamera *pCamera = m_pScene->getCamera();
    m_nTotalPixelCount = pCamera->getViewportWidth() * pCamera->getViewportHeight();
}

void BBRayTracingManager::open()
{
    // Stop refresh per frame
    BBSceneManager::getEditViewOpenGLWidget()->stopRenderThread();

    m_pScene->setRenderingFunc(&BBScene::rayTracingRender);

    render();
}

void BBRayTracingManager::close()
{
    BBSceneManager::getEditViewOpenGLWidget()->startRenderThread();
    m_pScene->setRenderingFunc(&BBScene::defaultRender);
}

void BBRayTracingManager::onWindowResize(int nWidth, int nHeight)
{
    m_nTotalPixelCount = nWidth * nHeight;
}

void BBRayTracingManager::updateWindow()
{
    BBSceneManager::getEditViewOpenGLWidget()->update();
}

void BBRayTracingManager::render()
{
    m_nCurrentRenderedPixel = 0;
    while (m_nCurrentRenderedPixel < m_nTotalPixelCount)
    {
        renderOnePixel(m_nCurrentRenderedPixel);
        m_nCurrentRenderedPixel++;
    }
    updateWindow();
}

void BBRayTracingManager::renderOnePixel(int nPixelIndex)
{

}
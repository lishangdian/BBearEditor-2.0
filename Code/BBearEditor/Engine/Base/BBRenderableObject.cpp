#include "BBRenderableObject.h"
#include "Render/BBDrawCall.h"
#include "Render/BBMaterial.h"
#include "BBUtils.h"
#include "Render/BBCamera.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBGLBuffers.h"
#include "Render/BBUniformUpdater.h"
#include "Render/BBRenderPass.h"


BBRenderableObject::BBRenderableObject()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBRenderableObject::BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_pDrawCalls = new BBDrawCall;
    m_bVisible = true;
    m_pMaterial = new BBMaterial;
    m_pVBO = NULL;
    m_pEBO = NULL;
    m_pIndexes = NULL;
    m_nIndexCount = 0;
    m_nVertexCount = 0;
    m_DefaultColor = QVector3D(1.0f, 1.0f, 1.0f);
}

BBRenderableObject::~BBRenderableObject()
{
    BB_SAFE_DELETE(m_pMaterial);
    BB_SAFE_DELETE(m_pVBO);
    BB_SAFE_DELETE(m_pEBO);
    BB_SAFE_DELETE(m_pIndexes);
}

void BBRenderableObject::init()
{
    BBDrawCall *pDrawCall = m_pDrawCalls;
    while (pDrawCall != nullptr)
    {
        pDrawCall->setMaterial(m_pMaterial);
        pDrawCall->setVBO(m_pVBO);
        pDrawCall->setEBO(m_pEBO);
        pDrawCall = pDrawCall->next<BBDrawCall>();
    }

}

void BBRenderableObject::render(BBCamera *pCamera)
{
    render(m_ModelMatrix, pCamera);
}

void BBRenderableObject::render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera)
{
    if (m_bVisible)
    {
        m_pMaterial->setMatrix4(NAME_MODELMATRIX, modelMatrix.data());
        m_pDrawCalls->draw(pCamera);
    }
}

void BBRenderableObject::draw()
{

}

void BBRenderableObject::appendDrawCall(BBDrawCall *pDrawCall)
{
    if (m_pDrawCalls == nullptr)
    {
        m_pDrawCalls = pDrawCall;
    }
    else
    {
        m_pDrawCalls->pushBack(pDrawCall);
    }
}

#include "BBSprite2D.h"
#include "Geometry/BBBoundingBox.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBTexture.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Scene/BBSceneManager.h"
#include "Render/BBCamera.h"


BBSprite2D::BBSprite2D()
    : BBSprite2D(0, 0, 100, 100)
{

}

BBSprite2D::BBSprite2D(int nCenterX, int nCenterY, int nWidth, int nHeight)
    : BBRenderableObject()
{
    m_nHalfWidth = nWidth / 2.0f;
    m_nHalfHeight = nHeight / 2.0f;
    m_pBoundingBox2D = new BBRectBoundingBox2D(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    m_pBoundingBox2D->init();

    setPosition(QVector3D(nCenterX, nCenterY, 0.0f));
    setScale(QVector3D(m_nHalfWidth, m_nHalfHeight, 0.0f));
}

BBSprite2D::~BBSprite2D()
{
    BB_SAFE_DELETE(m_pBoundingBox2D);
}

void BBSprite2D::init()
{
    m_pVBO = new BBVertexBufferObject(4);
    m_pVBO->setPosition(0, 1.0f, 1.0f, 0);
    m_pVBO->setPosition(1, -1.0f, 1.0f, 0);
    m_pVBO->setPosition(2, -1.0f, -1.0f, 0);
    m_pVBO->setPosition(3, 1.0f, -1.0f, 0);
    m_pVBO->setTexcoord(0, 1.0f, 1.0f);
    m_pVBO->setTexcoord(1, 0.0f, 1.0f);
    m_pVBO->setTexcoord(2, 0.0f, 0.0f);
    m_pVBO->setTexcoord(3, 1.0f, 0.0f);
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setColor(i, 1.0f, 1.0f, 1.0f);
    }

    m_pCurrentMaterial->init("UI",
                             BB_PATH_RESOURCE_SHADER(UI.vert),
                             BB_PATH_RESOURCE_SHADER(UI.frag));
    BBTexture texture;
    QString texturePath = BB_PATH_RESOURCE_ICON(empty2.png);
    m_pCurrentMaterial->getBaseRenderPass()->setSampler2D(LOCATION_TEXTURE(0), texture.createTexture2D(texturePath), texturePath);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 1.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_QUADS, 0, 4);
    appendDrawCall(pDrawCall);
}

void BBSprite2D::render(BBCamera *pCamera)
{
    BBRenderableObject::render(pCamera);
    m_pBoundingBox2D->render(pCamera);
}

void BBSprite2D::resize(float fWidth, float fHeight)
{
    m_pCurrentMaterial->setVector4(LOCATION_SCREEN_PARAMETERS, fWidth, fHeight, 0.0f, 0.0f);
}

void BBSprite2D::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBRenderableObject::setPosition(position, bUpdateLocalTransform);
    m_pBoundingBox2D->setPosition(position, bUpdateLocalTransform);
}

void BBSprite2D::setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform)
{
    BBRenderableObject::setRotation(nAngle, axis, bUpdateLocalTransform);
    m_pBoundingBox2D->setRotation(nAngle, axis, bUpdateLocalTransform);
}

void BBSprite2D::setRotation(const QVector3D &rotation, bool bUpdateLocalTransform)
{
    BBRenderableObject::setRotation(rotation, bUpdateLocalTransform);
    m_pBoundingBox2D->setRotation(rotation, bUpdateLocalTransform);
}

void BBSprite2D::setScale(const QVector3D &scale, bool bUpdateLocalTransform)
{
    BBRenderableObject::setScale(scale, bUpdateLocalTransform);
    m_pBoundingBox2D->setScale(scale, bUpdateLocalTransform);
}

void BBSprite2D::setActivity(bool bActive)
{
    BBRenderableObject::setActivity(bActive);
    m_pBoundingBox2D->setActivity(bActive);
    setVisibility(bActive);
}

void BBSprite2D::setVisibility(bool bVisible)
{
    m_pBoundingBox2D->setVisibility(bVisible);
}

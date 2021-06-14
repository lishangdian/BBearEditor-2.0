#ifndef BBRENDERABLEOBJECT_H
#define BBRENDERABLEOBJECT_H

#include "BBGameObject.h"
#include <QtOpenGL>

class BBGLShader;
class BBCamera;
class BBVertexBufferObject;
class BBRenderableObject : public BBGameObject
{
public:
    BBRenderableObject();
    BBRenderableObject(float px, float py, float pz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz);
    virtual ~BBRenderableObject();

    void render(BBCamera *pCamera) override;
    void render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setTexture(const QString &filePath, bool bInvertY = true);
    void setTexture(int nSize);
    void setTexture(const GLuint &nTexture);

//    void setAmbientMaterial(float r, float g, float b, float a);
//    void setAmbientMaterial(QColor color);
//    void setDiffuseMaterial(float r, float g, float b, float a);
//    void setDiffuseMaterial(QColor color);
//    void setSpecularMaterial(float r, float g, float b, float a);
//    void setSpecularMaterial(QColor color);

protected:
    virtual void draw();
    BBGLShader *m_pShader;
    BBVertexBufferObject *m_pVertexBuffer;
    unsigned short *m_pIndexes;
    int m_nIndexCount;
    int m_nVertexCount;
    QVector3D m_DefaultColor;
};

#endif // BBRENDERABLEOBJECT_H

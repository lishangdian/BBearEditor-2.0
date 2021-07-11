#ifndef BBMODEL_H
#define BBMODEL_H


#include "Base/BBGameObject.h"
#include "BBMesh.h"


class BBMesh;
class BBOBJMesh;
class BBBoundingBox3D;
class BBModel : public BBGameObject
{
public:
    BBModel(const BBMeshType &eType);
    BBModel(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale, const BBMeshType &eType);
    BBModel(float px, float py, float pz,
            float rx, float ry, float rz,
            float sx, float sy, float sz, const BBMeshType &eType);
    ~BBModel();

    inline BBMesh* getMesh() { return m_pMesh; }

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void insertInRenderQueue(BBRenderQueue *pQueue) override;
    void removeFromRenderQueue(BBRenderQueue *pQueue) override;

    void setCurrentMaterial(BBMaterial *pMaterial) override;
    void restoreMaterial() override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    void setScale(const QVector3D &scale, bool bUpdateLocalTransform = true) override;

    void setActivity(bool bActive) override;
    void setVisibility(bool bVisible) override;

    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const BBFrustum &frustum) override;
    void showCloseUp(QVector3D &outPosition, QVector3D &outViewCenter, float fDistFactor = 2.4) override;

private:
    BBMesh *m_pMesh;
    BBBoundingBox3D *m_pBoundingBox;
};

#endif // BBMODEL_H




//    void renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos);
//    void setDiffuseMaterial(float r, float g, float b, float a);
//    void setMaterial(Material *material);
//    Material *getMaterial();
//    void previewMaterial(Material *material);
//    void materialRemoveUser();
//    void removeMaterial();
//    void updateDirectionLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updateDirectionLightColor(QVector4D *lightColor, int count);
//    void updatePointLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updatePointLightColor(QVector4D *lightColor, int count);
//    void updatePointLightOption(QVector4D *option, int count);
//    void updateSpotLightPosition(QVector4D *lightPosition, QVector3D count);
//    void updateSpotLightDirection(QVector4D *lightDirection, int count);
//    void updateSpotLightColor(QVector4D *lightColor, int count);
//    void updateSpotLightOption(QVector4D *option, int count);
//    void updateSpotLightOption2(QVector4D *option, int count);
//    void switchFog(bool b);
//    void setFogColor(float r, float g, float b);
//    void setFogOption(float start, float end, int density, float power);
//    void setFogMode(int mode);
//    MeshType getMeshType();
//    FBX *getFbxMesh();
//    void play();
//    void stop();
//    void setFBO(GLuint texture);
//    void setShadowMap(GLuint map);
//    void changeTerrainHeightMap(QString path);
//    Terrain *getTerrainMesh();

//    void onLua();
//    void offLua();
//    void onUpdateLua();
//    void onKeyDownLua(QKeyEvent *e);
//    QList<ScriptManager*> mScriptManagers;
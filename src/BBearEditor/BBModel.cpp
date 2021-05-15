#include "BBModel.h"
#include "BBOBJMesh.h"
#include "BBUtils.h"

BBModel::BBModel(BBMeshType eType)
    : BBModel(0, 0, 0, 0, 0, 0, 1, 1, 1, eType)
{

}

BBModel::BBModel(const float px, const float py, const float pz,
                 const float rx, const float ry, const float rz,
                 const float sx, const float sy, const float sz, BBMeshType eType)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    setClassName(BB_CLASSNAME_MODEL);
    if (eType == BBMeshType::OBJ)
        m_pMesh = new BBOBJMesh(px, py, pz, rx, ry, rz, sx, sy, sz);
//    boundingBox = new AABBBoundingBox3D(px, py, pz, rx, ry, rz, sx, sy, sz);
//    mMaterial = NULL;
//    //暂时每个model有一个脚本
//    mScriptManagers.append(new ScriptManager(this));
}

void BBModel::init(const QString path)
{
    m_pMesh->init(path);
//    //必须在mesh init之后 position才有值
//    boundingBox->init(mMesh->mVertexPositions);
//    //释放
//    mMesh->mVertexPositions.clear();
}

//void Model::render(Camera camera)
//{
//    if (mIsActive)
//    {
//        mMesh->render(camera);
//        boundingBox->render(camera);
//    }
//}

//void Model::renderBuffer(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix, QVector3D cameraPos)
//{
//    mMesh->renderBuffer(viewMatrix, projectionMatrix, cameraPos);
//}

//void Model::resize(float width, float height)
//{
//    mMesh->resize(width, height);
//    boundingBox->resize(width, height);
//}

//void Model::setDiffuseMaterial(float r, float g, float b, float a)
//{
//    mMesh->setDiffuseMaterial(r, g, b, a);
//}

//void Model::setPosition(QVector3D position, bool isUpdateLocalTransform)
//{
//    GameObject::setPosition(position, isUpdateLocalTransform);
//    mMesh->setPosition(position, isUpdateLocalTransform);
//    boundingBox->setPosition(position, isUpdateLocalTransform);
//}

//void Model::setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform)
//{
//    GameObject::setRotation(angle, axis, isUpdateLocalTransform);
//    mMesh->setRotation(angle, axis, isUpdateLocalTransform);
//    boundingBox->setRotation(angle, axis, isUpdateLocalTransform);
//}

//void Model::setRotation(QVector3D rotation, bool isUpdateLocalTransform)
//{
//    GameObject::setRotation(rotation, isUpdateLocalTransform);
//    mMesh->setRotation(rotation, isUpdateLocalTransform);
//    boundingBox->setRotation(rotation, isUpdateLocalTransform);
//}

//void Model::setScale(QVector3D scale, bool isUpdateLocalTransform)
//{
//    GameObject::setScale(scale, isUpdateLocalTransform);
//    mMesh->setScale(scale, isUpdateLocalTransform);
//    boundingBox->setScale(scale, isUpdateLocalTransform);
//}

//bool Model::hit(Ray ray, float &distance)
//{
//    //先用包围盒进行粗略的碰撞检测
//    if (boundingBox->hitBoundingBox(ray, distance, boundingBox->getModelMatrix()))
//    {
//        //击中包围盒后 判断是否击中网格
//        if (mMesh->hit(ray, distance))
//        {
//            return true;
//        }
//    }
//    return false;
//}

//void Model::setActive(bool isActive)
//{
//    //可见性 不存在
//    GameObject::setActive(isActive);
//    mMesh->setActive(isActive);
//    boundingBox->setActive(isActive);
//    setVisible(isActive);
//}

//void Model::setVisible(bool isVisible)
//{
//    //可见性 还存在
//    GameObject::setVisible(isVisible);
//    boundingBox->setVisible(isVisible);
//}

//void Model::lookAtSelf(QVector3D &pos, QVector3D &viewCenter, float distFactor)
//{
//    //原始包围盒以零点为中心 需要变换
//    viewCenter = mModelMatrix * boundingBox->getCenter();
//    QVector3D boxRadius = boundingBox->getRadius() * mScale;
//    //取包围盒的最大半径 变为一个正方形包围盒
//    float size = max(abs(boxRadius.x()), abs(boxRadius.y()));
//    size = max(size, abs(boxRadius.z()));
//    pos = viewCenter + distFactor * size * QVector3D(1, 1, 1);
//}

//bool Model::belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                    QVector3D top1, QVector3D top2, QVector3D top3,
//                                    QVector3D right1, QVector3D right2, QVector3D right3,
//                                    QVector3D bottom1, QVector3D bottom2, QVector3D bottom3)
//{
//    //计算对象的包围盒是否位于选区的上下左右四个平面之中
//    //包围盒中心点在外侧的直接排除
//    QVector3D boundingBoxCenter = getModelMatrix() * boundingBox->getCenter();
//    if (boundingBoxCenter.distanceToPlane(left1, left2, left3) < 0)
//        return false;
//    if (boundingBoxCenter.distanceToPlane(top1, top2, top3) < 0)
//        return false;
//    if (boundingBoxCenter.distanceToPlane(right1, right2, right3) < 0)
//        return false;
//    if (boundingBoxCenter.distanceToPlane(bottom1, bottom2, bottom3) < 0)
//        return false;
//    //包围盒中心点在内测的 进一步计算包围盒的每一个顶点是否在内测
//    return boundingBox->belongToSelectionRegion(left1, left2, left3, top1, top2, top3,
//                                                right1, right2, right3, bottom1, bottom2, bottom3, getModelMatrix());
//}

//void Model::setMaterial(Material *material)
//{
//    //如果和原来的材质不一样 需要改变材质的使用者列表
//    if (mMaterial != material)
//    {
//        //从原来的材质的使用对象中移除
//        if (mMaterial)
//        {
//            mMaterial->removeUser(this);
//        }
//        //加入新材质的使用对象列表中
//        material->addUser(this);

//        mMaterial = material;
//    }
//    //设置新材质
//    mMesh->setMaterial(material);
//}

//Material *Model::getMaterial()
//{
//    return mMaterial;
//}

//void Model::previewMaterial(Material *material)
//{
//    //只给网格附上材质 不修改材质的使用者列表
//    mMesh->setMaterial(material);
//}

//void Model::materialRemoveUser()
//{
//    if (mMaterial)
//    {
//        mMaterial->removeUser(this);
//    }
//}

//void Model::removeMaterial()
//{
//    //材质被删除时调用 移除对象上的材质 不用修改材质的使用者列表 因为材质将会被删去
//    mMaterial = NULL;
//    //使用默认材质
//    mMesh->setDefaultMaterial();
//}

//void Model::updateDirectionLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mMesh->updateDirectionLightPosition(lightPosition, count);
//}

//void Model::updateDirectionLightColor(QVector4D *lightColor, int count)
//{
//    mMesh->updateDirectionLightColor(lightColor, count);
//}

//void Model::updatePointLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mMesh->updatePointLightPosition(lightPosition, count);
//}

//void Model::updatePointLightColor(QVector4D *lightColor, int count)
//{
//    mMesh->updatePointLightColor(lightColor, count);
//}

//void Model::updatePointLightOption(QVector4D *option, int count)
//{
//    mMesh->updatePointLightOption(option, count);
//}

//void Model::updateSpotLightPosition(QVector4D *lightPosition, QVector3D count)
//{
//    mMesh->updateSpotLightPosition(lightPosition, count);
//}

//void Model::updateSpotLightDirection(QVector4D *lightDirection, int count)
//{
//    mMesh->updateSpotLightDirection(lightDirection, count);
//}

//void Model::updateSpotLightColor(QVector4D *lightColor, int count)
//{
//    mMesh->updateSpotLightColor(lightColor, count);
//}

//void Model::updateSpotLightOption(QVector4D *option, int count)
//{
//    mMesh->updateSpotLightOption(option, count);
//}

//void Model::updateSpotLightOption2(QVector4D *option, int count)
//{
//    mMesh->updateSpotLightOption2(option, count);
//}

//void Model::switchFog(bool b)
//{
//    mMesh->switchFog(b);
//}

//void Model::setFogColor(float r, float g, float b)
//{
//    mMesh->setFogColor(r, g, b);
//}

//void Model::setFogOption(float start, float end, int density, float power)
//{
//    mMesh->setFogOption(start, end, density, power);
//}

//void Model::setFogMode(int mode)
//{
//    mMesh->setFogMode(mode);
//}

//MeshType Model::getMeshType()
//{
//    return mMesh->getMeshType();
//}

//FBX *Model::getFbxMesh()
//{
//    return mMesh->getFbx();
//}

//void Model::play()
//{
//    //点击开始运行按钮后 每个模型播放动画
//    if (getMeshType() == MeshType::fbx)
//    {
//        getFbxMesh()->startAnimation();
//    }
//}

//void Model::stop()
//{
//    if (getMeshType() == MeshType::fbx)
//    {
//        getFbxMesh()->stopAnimation();
//    }
//}

//void Model::setFBO(GLuint texture)
//{
//    mMesh->setTexture(texture);
//}

//void Model::setShadowMap(GLuint map)
//{
//    mMesh->setShadowMap(map);
//}

//void Model::changeTerrainHeightMap(QString path)
//{
//    mMesh->changeTerrainHeightMap(path);
//    //重新计算包围盒
//    boundingBox->init(mMesh->mVertexPositions);
//    //释放
//    mMesh->mVertexPositions.clear();
//}

//Terrain *Model::getTerrainMesh()
//{
//    return mMesh->getTerrain();
//}

//void Model::onLua()
//{
//    for (QList<ScriptManager*>::Iterator itr = mScriptManagers.begin(); itr != mScriptManagers.end(); itr++)
//    {
//        (*itr)->on();
//    }
//}

//void Model::offLua()
//{
//    for (QList<ScriptManager*>::Iterator itr = mScriptManagers.begin(); itr != mScriptManagers.end(); itr++)
//    {
//        (*itr)->off();
//    }
//}

//void Model::onUpdateLua()
//{
//    for (QList<ScriptManager*>::Iterator itr = mScriptManagers.begin(); itr != mScriptManagers.end(); itr++)
//    {
//        (*itr)->onUpdate();
//    }
//}

//void Model::onKeyDownLua(QKeyEvent *e)
//{
//    for (QList<ScriptManager*>::Iterator itr = mScriptManagers.begin(); itr != mScriptManagers.end(); itr++)
//    {
//        (*itr)->onKeyDown(e);
//    }
//}
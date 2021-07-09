#ifndef BBPOINTLIGHT_H
#define BBPOINTLIGHT_H


#include "BBLight.h"

class BBBoundingBox3D;

class BBPointLight : public BBLight
{
public:
    BBPointLight(BBScene *pScene);
    BBPointLight(BBScene *pScene, const QVector3D &position);
    BBPointLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation);
    ~BBPointLight();

    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;

    void setRadius(float fRadius);
    void setConstantFactor(float fValue) { m_Setting1[1] = fValue; }
    void setLinearFactor(float fValue) { m_Setting1[2] = fValue; }
    void setQuadricFactor(float fValue) { m_Setting1[3] = fValue; }

    inline float getRadius() { return m_Setting1[0]; }
    inline float getConstantFactor() { return m_Setting1[1]; }
    inline float getLinearFactor() { return m_Setting1[2]; }
    inline float getQuadricFactor() { return m_Setting1[3]; }

    bool cull(BBCamera *pCamera, const QRectF &displayBox) override;
    bool cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY) override;
};

#endif // BBPOINTLIGHT_H

#ifndef BBPARTICLE_H
#define BBPARTICLE_H


#include "Base/BBRenderableObject.h"

class BBShaderStorageBufferObject;
class BBShader;

class BBParticle : public BBRenderableObject
{
public:
    BBParticle(const QVector3D &position = QVector3D(0, 0, 0));
    ~BBParticle();

    void init() override;
    void render(BBCamera *pCamera) override;

private:
    void create0();
    void update0();

    void create1();
    void update1();

private:
    BBShaderStorageBufferObject *m_pSSBO;
    BBShader *m_pUpdateCShader;
};

#endif // BBPARTICLE_H

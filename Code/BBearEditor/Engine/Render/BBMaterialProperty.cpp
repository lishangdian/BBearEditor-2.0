#include "BBMaterialProperty.h"
#include "Utils/BBUtils.h"


/**
 * @brief BBMaterialProperty::BBMaterialProperty
 * @param eType
 */
BBMaterialProperty::BBMaterialProperty(const BBMaterialUniformPropertyType &eType, const char *name)
{
    m_eType = eType;
    memset(m_Name, 0, 64);
    strcpy(m_Name, name);
}

BBMaterialProperty::~BBMaterialProperty()
{

}


/**
 * @brief BBFloatMaterialProperty::BBFloatMaterialProperty
 * @param name
 */
BBFloatMaterialProperty::BBFloatMaterialProperty(const char *name)
    : BBMaterialProperty(Float, name)
{
    m_fPropertyValue = 0.0f;
}

BBFloatMaterialProperty::~BBFloatMaterialProperty()
{

}

BBMaterialProperty* BBFloatMaterialProperty::clone()
{
    BBFloatMaterialProperty *pRet = new BBFloatMaterialProperty(m_Name);
    pRet->setPropertyValue(m_fPropertyValue);
    return pRet;
}


/**
 * @brief BBMatrix4MaterialProperty::BBMatrix4MaterialProperty
 */
BBMatrix4MaterialProperty::BBMatrix4MaterialProperty(const char *name)
    : BBMaterialProperty(Matrix4, name)
{
    m_pPropertyValue = nullptr;
}

BBMatrix4MaterialProperty::~BBMatrix4MaterialProperty()
{
    BB_SAFE_DELETE(m_pPropertyValue);
}

BBMaterialProperty* BBMatrix4MaterialProperty::clone()
{
    BBMatrix4MaterialProperty *pRet = new BBMatrix4MaterialProperty(m_Name);
    pRet->setPropertyValue(m_pPropertyValue);
    return pRet;
}


/**
 * @brief BBVector4MaterialProperty::BBVector4MaterialProperty
 * @param eType
 */
BBVector4MaterialProperty::BBVector4MaterialProperty(const char *name)
    : BBMaterialProperty(Vector4, name)
{
    m_pPropertyValue = new float[4] {0, 0, 0, 1};
    if (strncmp(name, "Color_", 6) == 0)
    {
        m_eFactoryType = Color;
        m_NameInPropertyManager = QString(name).mid(6);
    }
    else
    {
        m_eFactoryType = Default;
    }
}

BBVector4MaterialProperty::~BBVector4MaterialProperty()
{
    BB_SAFE_DELETE(m_pPropertyValue);
}

BBMaterialProperty* BBVector4MaterialProperty::clone()
{
    BBVector4MaterialProperty *pRet = new BBVector4MaterialProperty(m_Name);
    pRet->setPropertyValue(m_pPropertyValue);
    return pRet;
}


/**
 * @brief BBSampler2DMaterialProperty::BBSampler2DMaterialProperty
 * @param name
 */
BBSampler2DMaterialProperty::BBSampler2DMaterialProperty(const char *name)
    : BBMaterialProperty(Sampler2D, name)
{
    m_TextureName = 0;
}

BBSampler2DMaterialProperty::~BBSampler2DMaterialProperty()
{

}

BBMaterialProperty* BBSampler2DMaterialProperty::clone()
{
    BBSampler2DMaterialProperty *pRet = new BBSampler2DMaterialProperty(m_Name);
    pRet->setTextureName(m_TextureName, m_ResourcePath);
    return pRet;
}

void BBSampler2DMaterialProperty::setTextureName(GLuint textureName, const QString &resourcePath)
{
    m_TextureName = textureName;
    m_ResourcePath = resourcePath;
}

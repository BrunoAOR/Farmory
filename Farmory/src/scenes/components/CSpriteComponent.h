#ifndef _H_C_SPRITE_COMPONENT_
#define _H_C_SPRITE_COMPONENT_

#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>
#include <render/CShaderProgram.h>
#include <render/CTexture2D.h>
#include <render/CMesh.h>


namespace maz
{

class CGameObject;


class CSpriteComponent
    : public CComponentBase<CSpriteComponent>
{
public:
    CSpriteComponent(CReference<CGameObject>& arOwner);
    ~CSpriteComponent();

    static constexpr EComponentType GetType()
    {
        return EComponentType::Sprite;
    }

public:
    CShaderProgram mShader;
    CTexture2D mTexture;
    CMesh mMesh;
};

} // maz

#endif // !_H_C_SPRITE_COMPONENT_

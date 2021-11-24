#include "CSpriteComponent.h"


namespace maz
{

CSpriteComponent::CSpriteComponent(CReference<CEntity>& arOwner)
    : CComponentBase(arOwner)
{
    //TODO: Should provide methods to set up Shader, texture (and mesh type?) from outside the Component
    mShader.Init(CFile("data/default.vs"), CFile("data/default.fs"));
    if (mShader.IsOk())
    {
        mShader.Use();
        mShader.SetUniform1f("aMixValue", 0.0f);
        mShader.SetUniform1i("aTexture0", 0);
        mShader.SetUniform1i("aTexture1", 1);
        mShader.StopUse();
    }
    bool lOk = mShader.IsOk();
    lOk = lOk && mTexture.Init();
    lOk = lOk && mTexture.SetImage(CFile("data/Smiley.png"), true);
    lOk = lOk && mMesh.InitRectangle(0, 0);
    MAZ_ASSERT(lOk, "Failed to properly set up.");
}


CSpriteComponent::~CSpriteComponent()
{
    mShader.End();
    mTexture.End();
    mMesh.End();
}

} // maz

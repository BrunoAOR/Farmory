#ifndef _H_C_TEST_SERVICE_
#define _H_C_TEST_SERVICE_

#include <services/IService.h>
#include <render/CShaderProgram.h>
#include <render/CTexture2D.h>
#include <render/CMesh.h>
#include <render/CCamera.h>


namespace maz
{

class CTestService
    : public CServiceBase<CTestService>
{
public:
    CTestService();
    ~CTestService();

    virtual bool Init() override;
    virtual void End() override;
    virtual bool IsOk() const override;

    virtual void Update() override;
    void TestRender();
    const CCamera& GetCamera() const { return mCamera; }

private:

    bool ShaderInit();
    bool TextureInit();
    bool MeshInit();
    void TestRender1();
    void TestRender2();
    void TestRender3();

private:
    CMesh mRect;
    CMesh mTris;
    CMesh mCube;

    CTexture2D mTexture0;
    CTexture2D mTexture1;

    float mMixValue;
    CShaderProgram mShader1;
    CShaderProgram mShader2;
    CShaderProgram mShader3;
    CShaderProgram mShader4;

    CCamera mCamera;
    float mCameraYaw;
    float mCameraPitch;
};

} // maz

#endif // !_H_C_TEST_SERVICE_

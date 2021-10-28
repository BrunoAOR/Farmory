#include <maz/globals.h>
#include "CTestService.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <maz/CGlfwHandler.h>
#include <services/CServicesManager.h>
#include <services/CInputService.h>
#include <services/CTimeService.h>

#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>
#include <external/glm/gtc/type_ptr.hpp>


namespace maz
{

float lCameraYaw = -90.0f;
float lCameraPitch = 0.0f;

CTestService::CTestService()
{ 
    MAZ_LOGGER("CTestService::CTestService called");
}


CTestService::~CTestService()
{
    MAZ_LOGGER("CTestService::~CTestService called");
}


bool CTestService::Init()
{
    bool lOk = true;

    mCameraYaw = -90.0f;
    mCameraPitch = 0.0f;
    lOk = lOk && ShaderInit();
    lOk = lOk && TextureInit();
    lOk = lOk && MeshInit();

    CGlfwHandler::SetCursorMode(ECursorMode::NORMAL);

    mCamera.Init();
    mCamera.SetPosition(TVec3(0.0f, 0.0f, 3.0f));
    mCamera.SetLookAt(TVec3(0.0f, 0.0f, -1.0f), TVec3(0.0f, 1.0f, 0.0f));
    mCamera.SetCameraMode(ECameraMode::ORTHOGRAPHIC);
    mCamera.SetClippingPlanes(0.1f, 100.0f);
    mCamera.SetFov(45.0f);

    return lOk;
}


void CTestService::End()
{
    mCamera.End();

    mShader1.End();
    mShader2.End();
    mShader3.End();
    mShader4.End();

    mTexture0.End();
    mTexture1.End();

    mRect.End();
    mTris.End();
    mCube.End();
}


bool CTestService::IsOk() const
{
    return false;
}


void CTestService::Update()
{
}


void CTestService::TestRender()
{
    TestRender3();
}



bool CTestService::ShaderInit()
{
    bool lOk = true;

    mShader1.Init(CFile("data/basic.vs"), CFile("data/basic.fs"));
    mShader2.Init(CFile("data/alt.vs"), CFile("data/alt.fs"));
    mShader3.Init(CFile("data/texture.vs"), CFile("data/texture.fs"));
    mShader4.Init(CFile("data/default.vs"), CFile("data/default.fs"));

    lOk = lOk && mShader1.IsOk() && mShader2.IsOk() && mShader3.IsOk() && mShader4.IsOk();
    return lOk;
}


bool CTestService::TextureInit()
{
    bool lOk = true;

    mMixValue = 0.5f;
    lOk = lOk && mTexture0.Init();
    lOk = lOk && mTexture0.SetImage(CFile("data/Bricks.jpg"), true);
    lOk = lOk && mTexture1.Init();
    lOk = lOk && mTexture1.SetImage(CFile("data/Smiley.png"), true);

    return lOk;
}


bool CTestService::MeshInit()
{
    bool lOk = true;
    lOk = lOk && mTris.InitTriangle(0, 0);
    lOk = lOk && mRect.InitRectangle(0, 0);
    lOk = lOk && mCube.InitCube();

    return lOk;
}


void CTestService::TestRender1()
{
    // Triangle
    mShader1.Use();
    mTris.Draw();

    // Rectangle - Color Changing
    mShader2.Use();
    float lTime = static_cast<float> (glfwGetTime());
    float lGreen = sinf(lTime) / 2.0f + 0.5f;
    mShader2.SetUniform4f("aUColor", 1.0f, lGreen, 0.0f, 1.0f);
    glm::mat4 lTransformS2(1.0f);
    lTransformS2 = glm::translate(lTransformS2, glm::vec3(-0.5, 0.5f, 0.0f));
    lTransformS2 = glm::scale(lTransformS2, glm::vec3(0.5f + 0.5f * std::sinf(static_cast<float>(glfwGetTime())), 0.5f, 1.0f));
    mShader2.SetUniformMat4("aTransform", glm::value_ptr(lTransformS2));
    mRect.Draw();

    CInputService* lInputService = Services::GetManager()->GetService< CInputService >();
    MAZ_ASSERT(nullptr != lInputService, "");
    // Rectangle - Keyboard controlled
    if (lInputService->IsKeyDown(GLFW_KEY_UP))
    {
        mMixValue += 0.01f;
        if (mMixValue > 1.0f) mMixValue = 1.0f;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_DOWN))
    {
        mMixValue -= 0.01f;
        if (mMixValue < 0.0f) mMixValue = 0.0f;
    }

    if (lInputService->IsKeyDown(GLFW_KEY_Q))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::MIRRORED_REPEAT);
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_W))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::REPEAT);
    }

    mShader3.Use();
    mShader3.SetUniform1f("aMixValue", mMixValue);
    mShader3.SetUniform1i("aTexture0", 0);
    mShader3.SetUniform1i("aTexture1", 1);

    glm::mat4 lTransform(1.0f);
    lTransform = glm::rotate(lTransform, glm::radians(90.0f) + static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
    lTransform = glm::translate(lTransform, glm::vec3(0.5f, 0.5f, 0.0f));
    lTransform = glm::scale(lTransform, glm::vec3(0.5f, 0.5f, 1.0f));
    mShader3.SetUniformMat4("aTransform", glm::value_ptr(lTransform));

    mTexture0.BindTexture(0);
    mTexture1.BindTexture(1);
    mRect.Draw();
    mTexture1.UnbindTexture();
    mTexture0.UnbindTexture();

    CShaderProgram::StopUse();
}


void CTestService::TestRender2()
{
    CInputService* lInputService = Services::GetManager()->GetService< CInputService >();
    MAZ_ASSERT(nullptr != lInputService, "");

    // Rectangle - Keyboard controlled
    if (lInputService->IsKeyDown(GLFW_KEY_UP))
    {
        mMixValue += 0.01f;
        if (mMixValue > 1.0f) mMixValue = 1.0f;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_DOWN))
    {
        mMixValue -= 0.01f;
        if (mMixValue < 0.0f) mMixValue = 0.0f;
    }

    if (lInputService->IsKeyDown(GLFW_KEY_Q))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::MIRRORED_REPEAT);
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_W))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::REPEAT);
    }

    mShader4.Use();
    mShader4.SetUniform1f("aMixValue", mMixValue);
    mShader4.SetUniform1i("aTexture0", 0);
    mShader4.SetUniform1i("aTexture1", 1);

    glm::mat4 lModelMatrix(1.0f);
    lModelMatrix = glm::rotate(lModelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mShader4.SetUniformMat4("aModelMatrix", glm::value_ptr(lModelMatrix));

    glm::mat4 lViewMatrix(1.0f);
    lViewMatrix = glm::translate(lViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    mShader4.SetUniformMat4("aViewMatrix", glm::value_ptr(lViewMatrix));

    glm::mat4 lProjectionMatrix(1.0f);
    lProjectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    mShader4.SetUniformMat4("aProjectionMatrix", glm::value_ptr(lProjectionMatrix));

    mTexture0.BindTexture(0);
    mTexture1.BindTexture(1);
    mRect.Draw();
    glm::mat4 lModelMatrixCube(1.0f);
    lModelMatrixCube = glm::rotate(lModelMatrixCube, static_cast<float>(glfwGetTime())* glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    //lModelMatrixCube = glm::rotate(lModelMatrixCube, static_cast< float >(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    mShader4.SetUniformMat4("aModelMatrix", glm::value_ptr(lModelMatrixCube));
    mCube.Draw();
    mTexture1.UnbindTexture();
    mTexture0.UnbindTexture();

    CShaderProgram::StopUse();
}


void CTestService::TestRender3()
{
    CInputService* lInputService = Services::GetManager()->GetService< CInputService >();
    MAZ_ASSERT(nullptr != lInputService, "");
    CTimeService* lTimeService = Services::GetManager()->GetService< CTimeService >();
    MAZ_ASSERT(nullptr != lTimeService, "");

    // Texture mixing
    if (lInputService->IsKeyDown(GLFW_KEY_UP))
    {
        mMixValue += 0.01f;
        if (mMixValue > 1.0f) mMixValue = 1.0f;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_DOWN))
    {
        mMixValue -= 0.01f;
        if (mMixValue < 0.0f) mMixValue = 0.0f;
    }

    // Texture wrap mode
    if (lInputService->IsKeyDown(GLFW_KEY_X))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::MIRRORED_REPEAT);
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_Z))
    {
        mTexture1.SetWrapMode(EAxis::X, EWrapMode::REPEAT);
    }

    // FOV
    if (lInputService->IsKeyDown(GLFW_KEY_RIGHT))
    {
        mCamera.SetFov(mCamera.GetFov() + 1.0f);
        //if (mFov > 89.0f) mFov = 89.0f;
        MAZ_LOGGER("FOV up   to %f", mCamera.GetFov());
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_LEFT))
    {
        mCamera.SetFov(mCamera.GetFov() - 1.0f);
        //if (mFov < 1.0f) mFov = 1.0f;
        MAZ_LOGGER("FOV down to %f", mCamera.GetFov());
    }

    glm::vec3 lCubePositions[] = {
      glm::vec3(0.0f,  0.0f,  -2.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    mShader4.Use();
    mShader4.SetUniform1f("aMixValue", mMixValue);
    mShader4.SetUniform1i("aTexture0", 0);
    mShader4.SetUniform1i("aTexture1", 1);

    // Camera
    // Mouse motion input
    const float kSensitivity = 0.05f;
    //lCameraYaw   += kSensitivity * lInputService->GetMousePosDelta(EMouseAxis::X_AXIS);
    //lCameraPitch -= kSensitivity * lInputService->GetMousePosDelta(EMouseAxis::Y_AXIS);
    //lCameraYaw   = (lCameraYaw < 0.0f) ? lCameraYaw + 360.0f : (lCameraYaw > 360.0f ? lCameraYaw - 360.0f : lCameraYaw);
    //lCameraPitch = (lCameraPitch < -89.0f) ? -89.0f : (lCameraPitch > 89.0f ? 89.0f : lCameraPitch);

    TVec3 lCameraFront;
    lCameraFront.x = cosf(glm::radians(lCameraYaw)) * cosf(glm::radians(lCameraPitch));
    lCameraFront.y = sinf(glm::radians(lCameraPitch));
    lCameraFront.z = sinf(glm::radians(lCameraYaw)) * cosf(glm::radians(lCameraPitch));
    lCameraFront = glm::normalize(lCameraFront);

    const float kFovSensitivity = 1.0f;
    float lFov = mCamera.GetFov() - kFovSensitivity * lInputService->GetMouseScrollDelta(EMouseAxis::Y_AXIS);
    lFov = (lFov < 1.0f) ? 1.0f : (lFov > 89.0f ? 89.0f : lFov);
    mCamera.SetFov(lFov);

    TVec3 lCamPos = mCamera.GetPosition();
    const float lCamSpeed = 2.5f;
    const float lCamDistance = lCamSpeed * lTimeService->GetDeltaTime();
    if (lInputService->IsKeyDown(GLFW_KEY_W))
    {
        lCamPos += lCamDistance * lCameraFront;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_S))
    {
        lCamPos -= lCamDistance * lCameraFront;
    }

    TVec3 lCameraUp(0.0f, 1.0f, 0.0f);
    if (lInputService->IsKeyDown(GLFW_KEY_A))
    {
        const glm::vec3 lCameraRight = glm::normalize(glm::cross(lCameraFront, lCameraUp));
        lCamPos -= lCamDistance * lCameraRight;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_D))
    {
        const glm::vec3 lCameraRight = glm::normalize(glm::cross(lCameraFront, lCameraUp));
        lCamPos += lCamDistance * lCameraRight;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_R))
    {
        const glm::vec3 lCameraRight = glm::normalize(glm::cross(lCameraFront, lCameraUp));
        lCamPos += lCamDistance * lCameraUp;
    }
    else if (lInputService->IsKeyDown(GLFW_KEY_F))
    {
        const glm::vec3 lCameraRight = glm::normalize(glm::cross(lCameraFront, lCameraUp));
        lCamPos -= lCamDistance * lCameraUp;
    }

    mCamera.SetPosition(lCamPos);
    mCamera.SetLookAt(lCameraFront, lCameraUp);
    
    mShader4.SetUniformMat4("aViewMatrix"      , mCamera.GetViewMatrixPtr());
    mShader4.SetUniformMat4("aProjectionMatrix", mCamera.GetProjMatrixPtr());

    mTexture0.BindTexture(0);
    mTexture1.BindTexture(1);

    for (uint32 i = 0; i < 10; ++i)
    {
        float lExtra = 0.0f;
        if (i % 3 == 0)
        {
            lExtra = 1.0f;
        }

        glm::mat4 lModelMatrix(1.0f);
        lModelMatrix = glm::translate(lModelMatrix, lCubePositions[i]);
        lModelMatrix = glm::rotate(lModelMatrix, glm::radians(20.0f * (i + lExtra * 5.0f * static_cast<float>(glfwGetTime()))), glm::vec3(1.0f, 0.3f, 0.5f));
        mShader4.SetUniformMat4("aModelMatrix", glm::value_ptr(lModelMatrix));
        mCube.Draw();
    }

    mTexture1.UnbindTexture();
    mTexture0.UnbindTexture();

    CShaderProgram::StopUse();
}

} // maz

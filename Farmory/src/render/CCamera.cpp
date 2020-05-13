#include <maz/globals.h>
#include "CCamera.h"
#include <maz/CGlfwHandler.h>

namespace maz
{

CCamera::CCamera()
    : mMode         (ECameraMode::ORTHOGRAPHIC)
    , mPos          (0.0f)
    , mFront        (0.0f, 0.0f, -1.0f)
    , mUp           (0.0f, 1.0f, 0.0f)
    , mViewMatrix   (1.0f)
    , mProjMatrix   (1.0f)
    , mFov          (45.0f)
    , mNear         (0.1f)
    , mFar          (100.0f)
    , mWidth        (0)
    , mHeight       (0)
{ ; }


CCamera::~CCamera() { ; }

bool CCamera::Init()
{
    bool lOk = true;

    lOk = lOk && CGlfwHandler::RegisterFramebufferResizedCallback(CGlfwHandler::FramebufferResizedCallback::Create< CCamera, &CCamera::OnFramebufferResized >(this));
    RebuildViewMatrix();
    RebuildProjMatrix(mMode);

    return lOk;
}

void CCamera::End()
{
    CGlfwHandler::UnregisterFramebufferResizedCallback(CGlfwHandler::FramebufferResizedCallback::Create< CCamera, &CCamera::OnFramebufferResized >(this));
}


void CCamera::SetCameraMode(ECameraMode aCameraMode)
{
    mMode = aCameraMode;
    RebuildProjMatrix(mMode);
}


void CCamera::SetPosition(const TVec3& aPosition)
{
    mPos = aPosition;
    RebuildViewMatrix();
}


bool CCamera::SetLookAt(const TVec3& aFront, const TVec3& aUp)
{
    bool lOk = true;
    MAZ_ASSERT(!IsZero(aFront), "[CCamera]::SetLookAt - Argument aFront is a zero vector. Operation will not proceed!");
    lOk = lOk && !IsZero(aFront);
    MAZ_ASSERT(!IsZero(aUp), "[CCamera]::SetLookAt - Argument aUp is a zero vector. Operation will not proceed!");
    lOk = lOk && !IsZero(aUp);
    MAZ_ASSERT(!lOk || (0 == glm::dot(aFront, aUp)), "[CCamera]::SetLookAt - Arguments aFront and aUp must be perpendicular to each other! Operation will not proceed!");
    lOk = lOk && (0 == glm::dot(aFront, aUp));

    if (lOk)
    {
        mFront = aFront;
        mUp = aUp;
        RebuildViewMatrix();
    }

    return lOk;
}


bool CCamera::SetClippingPlanes(float aNearDistance, float aFarDistance)
{
    bool lOk = true;
    MAZ_ASSERT((aNearDistance >= 0), "[CCamera]::SetClippingPlanes - Argument aNearDistance must be equal or larger than zero. Operation will not proceed!")
    lOk = lOk && (aNearDistance >= 0);
    MAZ_ASSERT((aNearDistance < aFarDistance), "[CCamera]::SetClippingPlanes - Argument aFarDistance must be larger than aNearDistance. Operation will not proceed!")
    lOk = lOk && (aNearDistance < aFarDistance);
    
    if (lOk)
    {
        mNear = aNearDistance;
        mFar  = aFarDistance;
        RebuildProjMatrix(mMode);
    }
    
    return lOk;
}


void CCamera::SetFov(float aFov)
{
    mFov = aFov;
    if (ECameraMode::PERSPECTIVE == mMode)
    {
        RebuildProjMatrix(ECameraMode::PERSPECTIVE);
    }
}


const float* CCamera::GetViewMatrixPtr() const
{
    return glm::value_ptr(mViewMatrix);
}


const float* CCamera::GetProjMatrixPtr() const
{
    return glm::value_ptr(mProjMatrix);
}



void CCamera::RebuildViewMatrix()
{
    mViewMatrix = glm::lookAt(mPos, mPos + mFront, mUp);
}


void CCamera::RebuildProjMatrix(ECameraMode aMode)
{
    MAZ_DBG_UNUSED_VAR(aMode);
    mProjMatrix = glm::perspective(glm::radians(mFov), static_cast< float >(mWidth) / mHeight, mNear, mFar);
}


void CCamera::OnFramebufferResized(int aWidth, int aHeight)
{
    mWidth = aWidth;
    mHeight = aHeight;
    RebuildProjMatrix(mMode);
}



} // maz

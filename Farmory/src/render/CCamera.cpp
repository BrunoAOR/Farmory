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
    , mWidth        (0.0f)
    , mHeight       (0.0f)
{ ; }


CCamera::~CCamera() { ; }


bool CCamera::Init()
{
    bool lOk = true;

    lOk = lOk && CGlfwHandler::RegisterFramebufferResizedCallback(CGlfwHandler::FramebufferResizedCallback::Create< CCamera, &CCamera::OnFramebufferResized >(this));
    int lWidth  = 0;
    int lHeight = 0;
    lOk = lOk && CGlfwHandler::GetFramebufferSize(lWidth, lHeight);
    if (lOk)
    {
        mWidth  = static_cast< float >(lWidth);
        mHeight = static_cast< float >(lHeight);
        RebuildViewMatrix();
        RebuildProjMatrix(mMode);
    }

    return lOk;
}


void CCamera::End()
{
    CGlfwHandler::UnregisterFramebufferResizedCallback(CGlfwHandler::FramebufferResizedCallback::Create< CCamera, &CCamera::OnFramebufferResized >(this));
}


ECameraMode CCamera::GetCameraMode() const
{
    return mMode;
}


const TVec3& CCamera::GetPosition() const
{
    return mPos;
}


const TVec3& CCamera::GetFront() const
{
    return mFront;
}


const TVec3& CCamera::GetUp() const
{
    return mUp;
}


float CCamera::GetNearPlaneDistance() const
{
    return mNear;
}


float CCamera::GetFarPlaneDistance() const
{
    return mFar;
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
    MAZ_ASSERT(!lOk || (!IsApproxEq(1, std::abs(glm::dot(aFront, aUp)))), "[CCamera]::SetLookAt - Arguments aFront and aUp shall not be parallel to each other! Operation will not proceed!");
    lOk = lOk && !IsApproxEq(1, std::abs(glm::dot(aFront, aUp)));

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


float CCamera::GetFov() const
{
    return mFov;
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
    if (ECameraMode::PERSPECTIVE == aMode)
    {
        mProjMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeight, mNear, mFar);
    }
    else if (ECameraMode::ORTHOGRAPHIC == aMode)
    {
        // Zooming-in means increasing the pixels per unit value
        const float lPixelsPerUnit = 200.0f;
        mProjMatrix = glm::ortho(0.0f, mWidth / lPixelsPerUnit, 0.0f, mHeight / lPixelsPerUnit, mNear, mFar);
    }
}


void CCamera::OnFramebufferResized(int aWidth, int aHeight)
{
    MAZ_LOGGER("CCamera::OnFramebufferResized - Old: %.0fx%.0f | New: %dx%d", mWidth, mHeight, aWidth, aHeight);
    mWidth  = static_cast< float >(aWidth);
    mHeight = static_cast< float >(aHeight);
    RebuildProjMatrix(mMode);
}

} // maz

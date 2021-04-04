#include <maz/globals.h>
#include "CInputService.h"
#include <maz/CGlfwHandler.h>
#include <GLFW/glfw3.h>
#include <maz/CDelegate.h>

#include <algorithm>

namespace maz
{

CInputService::TMouseInfo::TAxisInfo::TAxisInfo()
    : mLastPos          (MAX_FLOAT)
    , mNewPos           (MAX_FLOAT)
    , mPosOffset        (0.0f)
    , mLastScrollOffset (0.0f)
    , mNewScrollOffset  (0.0f)
{ ; }

CInputService::CInputService() { ; }
CInputService::~CInputService() { ; }


bool CInputService::Init()
{
    bool lOk = true;
    lOk = lOk && CGlfwHandler::RegisterKeyboardStateCallback(CGlfwHandler::KeyboardStateCallback::Create< CInputService, &CInputService::OnKeyboardState>(this));
    lOk = lOk && CGlfwHandler::RegisterMousePositionCallback(CGlfwHandler::MousePositionCallback::Create< CInputService, &CInputService::OnMousePosition>(this));
    lOk = lOk && CGlfwHandler::RegisterMouseScrollallback   (CGlfwHandler::MouseScrollCallback  ::Create< CInputService, &CInputService::OnMouseScroll  >(this));
    return lOk;
}


void CInputService::End()
{
    CGlfwHandler::UnregisterKeyboardStateCallback(CGlfwHandler::KeyboardStateCallback::Create< CInputService, &CInputService::OnKeyboardState>(this));
    CGlfwHandler::UnregisterMousePositionCallback(CGlfwHandler::MousePositionCallback::Create< CInputService, &CInputService::OnMousePosition>(this));
    CGlfwHandler::UnregisterMouseScrollCallback  (CGlfwHandler::MouseScrollCallback  ::Create< CInputService, &CInputService::OnMouseScroll  >(this));
}


bool CInputService::IsOk() const
{
    return true;
}


void CInputService::PreUpdate()
{
    UpdateMouseAxis(mMouseInfo.mXAxis);
    UpdateMouseAxis(mMouseInfo.mYAxis);
}


void CInputService::UpdateMouseAxis(TMouseInfo::TAxisInfo& aAxisInfo)
{
    // Pos
    if ((aAxisInfo.mLastPos != MAX_FLOAT) && (aAxisInfo.mNewPos != MAX_FLOAT))
    {
        aAxisInfo.mPosOffset = aAxisInfo.mNewPos - aAxisInfo.mLastPos;
    }
    aAxisInfo.mLastPos = aAxisInfo.mNewPos;
    
    // Scroll
    aAxisInfo.mLastScrollOffset = aAxisInfo.mNewScrollOffset;
    aAxisInfo.mNewScrollOffset = 0.0f;
}


bool CInputService::IsKeyDown(int aKey) const
{
    std::vector< int >::const_iterator lIt = std::find(mDownKeys.begin(), mDownKeys.end(), aKey);
    return lIt != mDownKeys.end();
}


float CInputService::GetMousePosDelta(EMouseAxis aMouseAxis) const
{
    float lDelta = 0.0f;
    switch (aMouseAxis)
    {
    case maz::EMouseAxis::X_AXIS:
        lDelta = mMouseInfo.mXAxis.mPosOffset;
        break;
    case maz::EMouseAxis::Y_AXIS:
        lDelta = mMouseInfo.mYAxis.mPosOffset;
        break;
    default:
        MAZ_ERROR("[CInputService]::GetMousePosDelta - Unsupported EMouseAxis value!");
        break;
    }
    return lDelta;
}


float CInputService::GetMouseScrollDelta(EMouseAxis aMouseAxis) const
{
    float lDelta = 0.0f;
    switch (aMouseAxis)
    {
    case maz::EMouseAxis::X_AXIS:
        lDelta = mMouseInfo.mXAxis.mLastScrollOffset;
        break;
    case maz::EMouseAxis::Y_AXIS:
        lDelta = mMouseInfo.mYAxis.mLastScrollOffset;
        break;
    default:
        MAZ_ERROR("[CInputService]::GetMouseScrollDelta - Unsupported EMouseAxis value!");
        break;
    }
    return lDelta;
}


void CInputService::OnKeyboardState(int aKey, int aScancode, int aAction, int aModifierBits)
{
    MAZ_UNUSED_VAR(aScancode);
    MAZ_UNUSED_VAR(aModifierBits);

    std::vector< int >::iterator lIt = std::find(mDownKeys.begin(), mDownKeys.end(), aKey);
    if (GLFW_PRESS == aAction)
    {
        if (lIt == mDownKeys.end())
        {
            mDownKeys.push_back(aKey);
        }
    }
    else if (GLFW_RELEASE == aAction)
    {
        MAZ_ASSERT(lIt != mDownKeys.end(), "TMP: Should not be here");
        mDownKeys.erase(lIt);
    }

    if (GLFW_KEY_ESCAPE == aKey && GLFW_RELEASE == aAction)
    {
        global::gShouldClose = true;
    }
}


void CInputService::OnMousePosition(double aXPos, double aYPos)
{
    mMouseInfo.mXAxis.mNewPos = static_cast< float >(aXPos);
    mMouseInfo.mYAxis.mNewPos = static_cast< float >(aYPos);
}


void CInputService::OnMouseScroll(double aXOffset, double aYOffset)
{
    mMouseInfo.mXAxis.mNewScrollOffset += static_cast< float >(aXOffset);
    mMouseInfo.mYAxis.mNewScrollOffset += static_cast< float >(aYOffset);
}

} // maz

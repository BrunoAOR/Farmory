#include <maz/globals.h>
#include "CTimeService.h"
#include <maz/CGlfwHandler.h>
#include <GLFW/glfw3.h>


namespace maz
{

CTimeService::CTimeService() 
    : mLastFrameTime(0.0f)
    , mDeltaTime(0.0f)
{ ; }


CTimeService::~CTimeService() { ; }


bool CTimeService::Init()
{
    bool lOk = CGlfwHandler::IsOk();

    if (lOk)
    {
        mLastFrameTime = static_cast< float >(glfwGetTime());
    }
    
    return lOk;
}


void CTimeService::End()
{
}


bool CTimeService::IsOk() const
{
    return CGlfwHandler::IsOk();
}


void CTimeService::Update()
{
    const float lCurrentFrameTime = static_cast< float >(glfwGetTime());
    mDeltaTime = lCurrentFrameTime - mLastFrameTime;
    mLastFrameTime = lCurrentFrameTime;
}


float CTimeService::GetDeltaTime() const
{
    return mDeltaTime;
}

} // maz

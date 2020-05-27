#include "CEntityHandle.h"
#include <ecs/CEcsManager.h>

namespace maz
{
namespace ecs
{

void CEntityHandle::Destroy()
{
    const bool lOk = mEcsManager->DestroyEntity(mId);
    MAZ_UNUSED_VAR(lOk);
    MAZ_ASSERT(lOk, "[CEntityHandle]::Destroy - Destrution failed!");
    mId = kInvalidId;
    mEcsManager = nullptr;
}   

} // ecs
} // maz

#ifndef _H_C_SYSTEMS_MANAGER_
#define _H_C_SYSTEMS_MANAGER_

#include <maz/globals.h>
#include <scenes/ISystem.h>
#include <array>


namespace maz
{

class CSystemsManager
{
public:
    CSystemsManager();
    ~CSystemsManager();

    void Shutdown();
    void RefreshSystems();
    template<typename SYSTEM>
    bool RegisterSystem();


private:
    std::array<ISystem*, kMaxSystemsCount> mSystems;
    uint16 mSystemsCount;
};


template<typename SYSTEM>
inline bool CSystemsManager::RegisterSystem()
{
    MAZ_ASSERT(mSystemsCount < kMaxSystemsCount - 1, "CSystemsManager::RegisterSystem - Attempting to Register a system but the maximum amount of systems (%u) has been reached", kMaxSystemsCount);
    mSystems[mSystemsCount] = MAZ_NEW(SYSTEM);
    ++mSystemsCount;
    return true;
}

} // maz

#endif // !_H_C_SYSTEMS_MANAGER_

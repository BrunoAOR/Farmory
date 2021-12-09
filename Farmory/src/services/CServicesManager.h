#ifndef H_C_SERVICES_MANAGER_
#define H_C_SERVICES_MANAGER_

#include <services/IService.h>
#include <vector>


namespace maz
{

class CServicesManager;

namespace Services
{
    
bool StartUp();
void Shutdown();
CServicesManager* GetManager();

} // Services


class CServicesManager
{
public:
    CServicesManager();
    ~CServicesManager();

    bool Init();
    void End();

    bool IsOk();
    
    void PreUpdate();
    void Update();
    void PostUpdate();

    template< typename T >
    T* GetService();

private:
    IService* GetService(const uint16 aServiceId);

private:
    std::vector< IService* > mServices;
};


template< typename T >
T* CServicesManager::GetService()
{
    T* lpService = static_cast<T*>(GetService(T::GetSystemId()));
    return lpService;
}

} // maz

#endif // !H_C_SERVICES_MANAGER_

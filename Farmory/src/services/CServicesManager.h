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
    std::vector< IService* > mServices;
};


template< typename T >
T* CServicesManager::GetService()
{
    T* lService = nullptr;

    for (size_t i = 0, iCount = mServices.size(); (i < iCount) && (nullptr == lService); ++i)
    {
        lService = dynamic_cast< T* >(mServices[i]);
    }

    return lService;
}

} // maz

#endif // !H_C_SERVICES_MANAGER_

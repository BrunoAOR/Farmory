#ifndef _H_C_SCENES_SERVICE_
#define _H_C_SCENES_SERVICE_

#include <services/IService.h>
#include <scenes/CComponentsManager.h>
#include <scenes/CGameObjectsManager.h>
#include <scenes/CSystemsManager.h>


namespace maz
{

class CScenesService
    : public IService
{
public:
    CScenesService();
    virtual ~CScenesService();

    virtual bool Init() override;
    virtual void End() override;
    virtual bool IsOk() const override;

    virtual void Update() override;

private:
    bool LoadTestScenes();

private:
    CComponentsManager mComponentsManager;
    CGameObjectsManager mGameObjectsManager;
    CSystemsManager mSystemsManager;
};

} // maz

#endif // !_H_C_SCENES_SERVICE_

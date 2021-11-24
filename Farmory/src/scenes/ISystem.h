#ifndef _H_I_SYSTEM_
#define _H_I_SYSTEM_

#include <maz/globals.h>
#include <scenes/SComponentsSignature.h>
#include <scenes/CEntity.h>
#include <maz/CReference.h>
#include <array>


namespace maz
{

class ISystem
{
    friend class CSystemsManager;

public:
    ISystem();
    virtual ~ISystem() = 0 {};

    const SComponentsSignature& GetSignature() const { return mSignature; }
    virtual void Update() = 0;
    virtual void Shutdown() {}

protected:
    void AddComponentToSignature(EComponentType aComponentType);
    void RemoveComponentFromSignature(EComponentType aComponentType);

protected:
    std::array<CReference<CEntity>, kMaxEntitiesCount> mEntities;

private:
    SComponentsSignature mSignature;
};

} // maz

#endif // !_H_I_SYSTEM_

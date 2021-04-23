#ifndef _H_S_COMPONENTS_SIGNATURE_
#define _H_S_COMPONENTS_SIGNATURE_

#include <maz/globals.h>
#include <scenes/EComponentType.h>

namespace maz
{

struct SComponentsSignature
{
public:
    SComponentsSignature();

    void AddComponent(EComponentType aComponentType);
    void RemoveComponent(EComponentType aComponentType);

    bool IsSignatureContainedInSelf(const SComponentsSignature& aOtherSginature);

private:
    static constexpr uint8 sBitsPerSubSignature = 32u;
    static constexpr uint8 sSubSignaturesCount = (MAZ_ENUM_COUNT(EComponentType) - 1) / sBitsPerSubSignature + 1;
    uint32 mSubSignatures[sSubSignaturesCount];
};

} // maz

#endif // !_H_S_COMPONENTS_SIGNATURE_

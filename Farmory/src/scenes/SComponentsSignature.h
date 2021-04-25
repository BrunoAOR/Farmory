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

    bool IsSupersetOf(const SComponentsSignature& aOtherSignature) const;
    bool IsSubsetOf(const SComponentsSignature& aOtherSignature) const;

private:
    static constexpr uint8 sBitsPerSubSignature = 32u;
    static constexpr uint8 sSubSignaturesCount = (MAZ_ENUM_COUNT(EComponentType) - 1) / sBitsPerSubSignature + 1;
    uint32 mSubSignatures[sSubSignaturesCount];
};

} // maz

#endif // !_H_S_COMPONENTS_SIGNATURE_

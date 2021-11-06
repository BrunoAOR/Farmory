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

    bool IsSupersetOf(const SComponentsSignature& arOtherSignature) const;
    bool IsSubsetOf(const SComponentsSignature& arOtherSignature) const;

private:
    static constexpr uint8 kBitsPerSubSignature = 32u;
    static constexpr uint8 kSubSignaturesCount = (EnumCount<EComponentType>() - 1) / kBitsPerSubSignature + 1;
    uint32 mSubSignatures[kSubSignaturesCount];
};

} // maz

#endif // !_H_S_COMPONENTS_SIGNATURE_

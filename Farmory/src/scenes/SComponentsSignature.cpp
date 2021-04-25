#include "SComponentsSignature.h"


namespace maz
{

SComponentsSignature::SComponentsSignature()
{
    for (uint8 i = 0; i < sSubSignaturesCount; ++i)
    {
        mSubSignatures[i] = 0;
    }
}


void SComponentsSignature::AddComponent(EComponentType aComponentType)
{
    const uint8 componentTypeNumber = EnumToNumber(aComponentType);
    const uint8 componentTypeSubSignatureIndex = componentTypeNumber / sBitsPerSubSignature;
    const uint8 componentTypeBitIndex = componentTypeNumber % sBitsPerSubSignature;

    mSubSignatures[componentTypeSubSignatureIndex] = (mSubSignatures[componentTypeSubSignatureIndex] | (1 << componentTypeBitIndex));
}


void SComponentsSignature::RemoveComponent(EComponentType aComponentType)
{
    const uint8 componentTypeNumber = EnumToNumber(aComponentType);
    const uint8 componentTypeSubSignatureIndex = componentTypeNumber / sBitsPerSubSignature;
    const uint8 componentTypeBitIndex = componentTypeNumber % sBitsPerSubSignature;

    mSubSignatures[componentTypeSubSignatureIndex] = (mSubSignatures[componentTypeSubSignatureIndex] & ~(1 << componentTypeBitIndex));
}


bool SComponentsSignature::IsSupersetOf(const SComponentsSignature& aOtherSignature) const
{
    bool isContainedWithin = true;

    for (uint8 i = 0; (i < sSubSignaturesCount) && isContainedWithin; ++i)
    {
        isContainedWithin = ((mSubSignatures[i] & aOtherSignature.mSubSignatures[i]) == aOtherSignature.mSubSignatures[i]);
    }

    return isContainedWithin;
}


bool SComponentsSignature::IsSubsetOf(const SComponentsSignature& aOtherSignature) const
{
    return aOtherSignature.IsSupersetOf(*this);
}

} // maz

#include "SComponentsSignature.h"


namespace maz
{

SComponentsSignature::SComponentsSignature()
{
    for (uint8 i = 0; i < kSubSignaturesCount; ++i)
    {
        mSubSignatures[i] = 0;
    }
}


void SComponentsSignature::AddComponent(EComponentType aComponentType)
{
    const uint8 lComponentTypeNumber = EnumToNumber(aComponentType);
    const uint8 lComponentTypeSubSignatureIndex = lComponentTypeNumber / kBitsPerSubSignature;
    const uint8 lComponentTypeBitIndex = lComponentTypeNumber % kBitsPerSubSignature;

    mSubSignatures[lComponentTypeSubSignatureIndex] = (mSubSignatures[lComponentTypeSubSignatureIndex] | (1 << lComponentTypeBitIndex));
}


void SComponentsSignature::RemoveComponent(EComponentType aComponentType)
{
    const uint8 lComponentTypeNumber = EnumToNumber(aComponentType);
    const uint8 lComponentTypeSubSignatureIndex = lComponentTypeNumber / kBitsPerSubSignature;
    const uint8 lComponentTypeBitIndex = lComponentTypeNumber % kBitsPerSubSignature;

    mSubSignatures[lComponentTypeSubSignatureIndex] = (mSubSignatures[lComponentTypeSubSignatureIndex] & ~(1 << lComponentTypeBitIndex));
}


bool SComponentsSignature::IsSupersetOf(const SComponentsSignature& arOtherSignature) const
{
    bool lIsContainedWithin = true;

    for (uint8 i = 0; (i < kSubSignaturesCount) && lIsContainedWithin; ++i)
    {
        lIsContainedWithin = ((mSubSignatures[i] & arOtherSignature.mSubSignatures[i]) == arOtherSignature.mSubSignatures[i]);
    }

    return lIsContainedWithin;
}


bool SComponentsSignature::IsSubsetOf(const SComponentsSignature& arOtherSignature) const
{
    return arOtherSignature.IsSupersetOf(*this);
}

} // maz

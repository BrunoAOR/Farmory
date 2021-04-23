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

bool SComponentsSignature::IsSignatureContainedInSelf(const SComponentsSignature& aOtherSginature)
{
    bool isContainedWithin = true;
    
    for (uint8 i = 0; (i < sSubSignaturesCount) && isContainedWithin; ++i)
    {
        isContainedWithin = ((mSubSignatures[i] & aOtherSginature.mSubSignatures[i]) == aOtherSginature.mSubSignatures[i]);
    }

    return isContainedWithin;
}

} // maz

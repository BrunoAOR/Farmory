#ifndef _H_S_COMPONENTS_SIGNATURE_
#define _H_S_COMPONENTS_SIGNATURE_

#include <maz/globals.h>
#include <scenes/EComponentType.h>


namespace maz
{

struct SComponentsSignature
{
public:
    constexpr SComponentsSignature() 
        : mSubSignatures{ 0 }
    {}
    
    template<typename... COMPONENT_CLASSES>
    static constexpr SComponentsSignature Create();

    void AddComponent(EComponentType aComponentType);
    void RemoveComponent(EComponentType aComponentType);

    bool IsSupersetOf(const SComponentsSignature& arOtherSignature) const;
    bool IsSubsetOf(const SComponentsSignature& arOtherSignature) const;

private:
    using subSignatureSizeType = uint32;
    static constexpr uint8 kBitsPerSubSignature = 8u * sizeof(subSignatureSizeType);
    static constexpr uint8 kSubSignaturesCount = ( (EnumCount<EComponentType>() - 1u) / kBitsPerSubSignature ) + 1u;
    subSignatureSizeType mSubSignatures[kSubSignaturesCount];

    struct SNullType {};

    template<typename COMPONENT_CLASS>
    static constexpr void AddTypeToSignature(subSignatureSizeType apSubSignature[kSubSignaturesCount]);

    template<typename COMPONENT_CLASS, typename ... COMPONENT_CLASSES>
    static constexpr void AddTypesTosignature(subSignatureSizeType apSubSignature[kSubSignaturesCount]);
};


template<typename... COMPONENT_CLASSES>
constexpr SComponentsSignature SComponentsSignature::Create()
{
    SComponentsSignature lComponentsSignature;
    AddTypesTosignature<COMPONENT_CLASSES..., SNullType>(lComponentsSignature.mSubSignatures);
    return lComponentsSignature;
}


template<typename COMPONENT_CLASS>
constexpr void SComponentsSignature::AddTypeToSignature(subSignatureSizeType apSubSignature[kSubSignaturesCount])
{
    static_assert(std::is_same< EComponentType, decltype(std::declval<COMPONENT_CLASS>().GetType())>(), "Types provided must be Components!");
    constexpr uint8 lComponentTypeNumber = EnumToNumber(COMPONENT_CLASS::GetType());
    constexpr uint8 lComponentTypeSubSignatureIndex = lComponentTypeNumber / kBitsPerSubSignature;
    constexpr uint8 lComponentTypeBitIndex = lComponentTypeNumber % kBitsPerSubSignature;

    apSubSignature[lComponentTypeSubSignatureIndex] = (apSubSignature[lComponentTypeSubSignatureIndex] | (1 << lComponentTypeBitIndex));
}


template<typename COMPONENT_CLASS, typename... COMPONENT_CLASSES>
constexpr void SComponentsSignature::AddTypesTosignature(subSignatureSizeType apSubSignature[kSubSignaturesCount])
{
    AddTypeToSignature<COMPONENT_CLASS>(apSubSignature);
    AddTypesTosignature<COMPONENT_CLASSES...>(apSubSignature);
}


template<>
constexpr void SComponentsSignature::AddTypesTosignature<SComponentsSignature::SNullType>(subSignatureSizeType apSubSignature[kSubSignaturesCount])
{
    MAZ_UNUSED_VAR(apSubSignature);
}

} // maz

#endif // !_H_S_COMPONENTS_SIGNATURE_

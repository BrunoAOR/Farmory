#ifndef _H_C_COMPONENTS_MANAGER_
#define _H_C_COMPONENTS_MANAGER_

#include <maz/globals.h>
#include <scenes/EComponentType.h>
#include <array>
#include <maz/CReferenceOwner.h>

namespace maz
{

class CGameObject;
class CComponentsManager
{
private:
    class CComponentManagerBase
    {
    public:
        CComponentManagerBase(EComponentType aComponentType) : mComponentType(aComponentType) {}
        EComponentType GetType() const { return mComponentType; }

    private:
        const EComponentType mComponentType;
    };

    template<typename COMPONENT_CLASS>
    class CComponentManager : public CComponentManagerBase
    {
    public:
        CComponentManager(EComponentType aComponentType)
            : CComponentManagerBase(aComponentType)
        {
            for (size_t i = 0, iCount = mComponents.size(); i < iCount; ++i)
            {
                mComponents[i] = std::move(CReferenceOwner<COMPONENT_CLASS>());
            }
            std::memset(mComponentsBuffer, 0, sizeof(COMPONENT_CLASS) * kMaxComponentsPerType);
            mComponentsBufferUseFlag.fill(false);
        }

        uint16 AddComponent(CGameObject& aOwner)
        {
            CReference<COMPONENT_CLASS> component;
            uint16 index = kInvalidComponentIndex;
            for (size_t i = 0, iCount = mComponentsBufferUseFlag.size(); (i < iCount) && (index == kInvalidComponentIndex); ++i)
            {
                if (!mComponentsBufferUseFlag[i])
                {
                    mComponentsBufferUseFlag[i] = true;
                    index = static_cast<uint16>(i);
                }
            }
            MAZ_ASSERT(index != kInvalidComponentIndex, "[CComponentManager]::AddComponent - Failed to find an available slot for component!");
            if (index != kInvalidComponentIndex)
            {
                mComponents[index] = CReferenceOwner(MAZ_PLACEMENT_NEW(&(mComponentsBuffer[sizeof(COMPONENT_CLASS) * index]), COMPONENT_CLASS, aOwner));
            }

            return index;
        }

        CReference<COMPONENT_CLASS> GetComponent(uint16 aIndex)
        {
            MAZ_ASSERT(mComponentsBufferUseFlag[aIndex], "[CComponentManager]::GetComponent - Attempting to retrieve uninitialized component at index %hu!", aIndex);
            return mComponents[aIndex].getReference();
        }

    private:
        std::array<CReferenceOwner<COMPONENT_CLASS>, kMaxComponentsPerType> mComponents;
        uint8 mComponentsBuffer[sizeof(COMPONENT_CLASS) * kMaxComponentsPerType];
        std::array<bool, kMaxComponentsPerType> mComponentsBufferUseFlag;
    };

public:
    CComponentsManager()
    {
        mComponentManagers.fill(nullptr);
    }

    template<typename COMPONENT_CLASS>
    bool RegisterComponent()
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] == nullptr
            , "[CComponentsManager]::RegisterComponent - Component of the desired type has already been registered!");
        mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] = MAZ_NEW(CComponentManager<COMPONENT_CLASS>, COMPONENT_CLASS::GetType());
        return true;
    }

    template<typename COMPONENT_CLASS>
    uint16 AddComponent(CGameObject& aOwner)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::AddComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->AddComponent(aOwner);
    }

    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent(const uint16 aComponentIndex)
    {
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->GetComponent(aComponentIndex);
    }

private:
    std::array<CComponentManagerBase*, MAZ_ENUM_COUNT(EComponentType)> mComponentManagers;
};

} // maz

#endif // !_H_C_COMPONENTS_MANAGER_

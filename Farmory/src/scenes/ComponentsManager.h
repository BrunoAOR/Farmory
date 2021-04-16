#ifndef _H_C_COMPONENTS_MANAGER_
#define _H_C_COMPONENTS_MANAGER_

#include <maz/globals.h>
#include <scenes/EComponentType.h>
#include <array>
#include <maz/CReferenceMaster.h>

namespace maz
{

class CGameObject;
class CComponentsManager
{
private:
    template<typename COMPONENT_CLASS>
    friend class CComponentManager;
    void updateComponentIdForGameObject(CReference<CGameObject> aGameObject, EComponentType aComponentType, uint16 aId);

    class CComponentManagerBase
    {
    public:
        CComponentManagerBase(EComponentType aComponentType) : mComponentType(aComponentType) {}
        virtual ~CComponentManagerBase() {}
        EComponentType GetType() const { return mComponentType; }
        virtual void RefreshComponents(CComponentsManager* aComponentsManager) = 0;
            
    private:
        const EComponentType mComponentType;
    };

    template<typename COMPONENT_CLASS>
    class CComponentManager : public CComponentManagerBase
    {
    private:
        enum class EComponentUseFlags : uint8
        {
            NONE        = 0,
            IN_USE      = 1 << 0,
            JUST_ADDED  = 1 << 1,
            TO_REMOVE   = 1 << 2
        };

    public:
        CComponentManager(EComponentType aComponentType)
            : CComponentManagerBase(aComponentType)
        {
            for (size_t i = 0, iCount = mComponents.size(); i < iCount; ++i)
            {
                mComponents[i] = std::move(CReferenceHolder<COMPONENT_CLASS>());
            }
            std::memset(mComponentsBuffer, 0, sizeof(COMPONENT_CLASS) * kMaxComponentsPerType);
            mComponentsBufferUseFlags.fill(EComponentUseFlags::NONE);
        }


        virtual void RefreshComponents(CComponentsManager* aComponentsManager) final
        {
            for (uint16 i = 0, iCount = static_cast<uint16>(mComponents.size()); i < iCount; ++i)
            {
                if (IsFlagSet(mComponentsBufferUseFlags[i], EComponentUseFlags::IN_USE))
                {
                    if (IsFlagSet(mComponentsBufferUseFlags[i], EComponentUseFlags::JUST_ADDED))
                    {
                        ResetFlag(mComponentsBufferUseFlags[i], EComponentUseFlags::JUST_ADDED);
                        MAZ_ASSERT(!IsFlagSet(mComponentsBufferUseFlags[i], EComponentUseFlags::TO_REMOVE), "[CComponentManager]::RefreshComponents - A component has both the JUST_ADDED and the TO_REMOVE flags. This should not happen!");
                        aComponentsManager->updateComponentIdForGameObject(mComponents[i]->GetOwner(), COMPONENT_CLASS::GetType(), i);
                    }
                    else if (IsFlagSet(mComponentsBufferUseFlags[i], EComponentUseFlags::TO_REMOVE))
                    {
                        ResetFlag(mComponentsBufferUseFlags[i], EComponentUseFlags::TO_REMOVE);
                        aComponentsManager->updateComponentIdForGameObject(mComponents[i]->GetOwner(), COMPONENT_CLASS::GetType(), kInvalidComponentIndex);
                        mComponents[i].~CReferenceHolder<COMPONENT_CLASS>();
                        reinterpret_cast<COMPONENT_CLASS*>(&(mComponentsBuffer[sizeof(COMPONENT_CLASS) * i]))->~COMPONENT_CLASS();
                        ResetFlag(mComponentsBufferUseFlags[i], EComponentUseFlags::IN_USE);
                    }
                }
            }
        }


        CReference<COMPONENT_CLASS> AddComponent(CReference<CGameObject>& aOwner)
        {
            CReference<COMPONENT_CLASS> component;
            uint16 index = kInvalidComponentIndex;
            for (size_t i = 0, iCount = mComponentsBufferUseFlags.size(); (i < iCount) && (index == kInvalidComponentIndex); ++i)
            {
                if (!IsFlagSet(mComponentsBufferUseFlags[i], EComponentUseFlags::IN_USE))
                {
                    SetFlag(mComponentsBufferUseFlags[i], EComponentUseFlags::IN_USE);
                    SetFlag(mComponentsBufferUseFlags[i], EComponentUseFlags::JUST_ADDED);
                    index = static_cast<uint16>(i);
                }
            }
            MAZ_ASSERT(index != kInvalidComponentIndex, "[CComponentManager]::AddComponent - Failed to find an available slot for component!");
            if (index != kInvalidComponentIndex)
            {
                mComponents[index] = CReferenceHolder<COMPONENT_CLASS>(MAZ_PLACEMENT_NEW(&(mComponentsBuffer[sizeof(COMPONENT_CLASS) * index]), COMPONENT_CLASS, aOwner));
                component = mComponents[index].GetReference();
            }

            return component;
        }


        bool RemoveComponent(uint16 aComponentIndex)
        {
            bool lOk = IsFlagSet(mComponentsBufferUseFlags[aComponentIndex], EComponentUseFlags::IN_USE);

            if (lOk)
            {
                MAZ_ASSERT(!IsFlagSet(mComponentsBufferUseFlags[aComponentIndex], EComponentUseFlags::JUST_ADDED), "[CComponentManager]::RemoveComponent - Attempting to remove a component that has been added in this frame and not yet commited!");
                SetFlag(mComponentsBufferUseFlags[aComponentIndex], EComponentUseFlags::TO_REMOVE);
            }

            return lOk;
        }


        CReference<COMPONENT_CLASS> GetComponent(uint16 aIndex)
        {
            MAZ_ASSERT(IsFlagSet(mComponentsBufferUseFlags[aIndex], EComponentUseFlags::IN_USE), "[CComponentManager]::GetComponent - Attempting to retrieve uninitialized component at index %hu!", aIndex);
            return mComponents[aIndex].GetReference();
        }

    private:
        std::array<CReferenceHolder<COMPONENT_CLASS>, kMaxComponentsPerType> mComponents;
        uint8 mComponentsBuffer[sizeof(COMPONENT_CLASS) * kMaxComponentsPerType];
        std::array<EComponentUseFlags, kMaxComponentsPerType> mComponentsBufferUseFlags;
    };

public:
    CComponentsManager();
    void RefreshComponents();


    template<typename COMPONENT_CLASS>
    bool RegisterComponent()
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] == nullptr
            , "[CComponentsManager]::RegisterComponent - Component of the desired type has already been registered!");
        mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] = MAZ_NEW(CComponentManager<COMPONENT_CLASS>, COMPONENT_CLASS::GetType());
        return true;
    }


    template<typename COMPONENT_CLASS>
    CReference< COMPONENT_CLASS> AddComponent(CReference<CGameObject>& aOwner)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::AddComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->AddComponent(aOwner);
    }


    template<typename COMPONENT_CLASS>
    bool RemoveComponent(const uint16 aComponentIndex)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::RemoveComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->RemoveComponent(aComponentIndex);
    }


    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent(const uint16 aComponentIndex)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::GetComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->GetComponent(aComponentIndex);
    }

private:
    std::array<CComponentManagerBase*, MAZ_ENUM_COUNT(EComponentType)> mComponentManagers;
};

} // maz

#endif // !_H_C_COMPONENTS_MANAGER_

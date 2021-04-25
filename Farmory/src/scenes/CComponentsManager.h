#ifndef _H_C_COMPONENTS_MANAGER_
#define _H_C_COMPONENTS_MANAGER_
#define MAZ_LOG_VERBOSE

#include <maz/globals.h>
#include <scenes/EComponentType.h>
#include <array>
#include <maz/CReferenceMasterBuffer.h>

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
        CComponentManagerBase(EComponentType aComponentType) : mComponentType(aComponentType) { MAZ_LOGGER_VERBOSE("CComponentManagerBase::CComponentManagerBase - called"); }
        virtual ~CComponentManagerBase() { MAZ_LOGGER_VERBOSE("CComponentManagerBase::~CComponentManagerBase - called"); }
        EComponentType GetType() const { return mComponentType; }
        
        virtual void Shutdown(CComponentsManager* aComponentsManager) = 0;
        virtual void RefreshComponents(CComponentsManager* aComponentsManager) = 0;
        virtual bool RemoveComponent(const uint16 aComponentIndex) = 0;

    private:
        const EComponentType mComponentType;
    };

    template<typename COMPONENT_CLASS>
    class CComponentManager : public CComponentManagerBase
    {
        using CComponentBuffer = CReferenceHolderBuffer<COMPONENT_CLASS, kMaxComponentsPerType>;

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
            MAZ_LOGGER_VERBOSE("ComponentManager::ComponentManager - called | ComponentType: %hhu", EnumToNumber(GetType()));
        }


        ~CComponentManager()
        {
            MAZ_LOGGER_VERBOSE("ComponentManager::~ComponentManager - called | ComponentType: %hhu", EnumToNumber(GetType()));
        }


        void Shutdown(CComponentsManager* aComponentsManager)
        {
            typename CComponentBuffer::CBufferIterator iterator = mComponentsBuffer.GetIterator(CComponentBuffer::EIteratorFlags::ANY);

            while (iterator)
            {
                aComponentsManager->updateComponentIdForGameObject(iterator.Get()->GetOwner(), COMPONENT_CLASS::GetType(), kInvalidComponentId);
                ++iterator;
            }
            mComponentsBuffer.Clear();
        }


        virtual void RefreshComponents(CComponentsManager* aComponentsManager) final
        {
            typename CComponentBuffer::CBufferIterator iterator = mComponentsBuffer.GetIterator(static_cast<CComponentBuffer::EIteratorFlags>(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING | CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING));

            while (iterator)
            {
                if (iterator.HasIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING))
                {
                    aComponentsManager->updateComponentIdForGameObject(iterator.Get()->GetOwner(), COMPONENT_CLASS::GetType(), iterator.GetId());
                    iterator.ClearIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING);
                }
                else if (iterator.HasIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING))
                {
                    aComponentsManager->updateComponentIdForGameObject(iterator.Get()->GetOwner(), COMPONENT_CLASS::GetType(), kInvalidComponentId);
                    bool removed = iterator.RemoveElement();
                    MAZ_ASSERT(removed, "CComponentManager::RefreshComponents - Failed to remove component of type %hhu with id %hhu that was flagged for removal!", EnumToNumber(COMPONENT_CLASS::GetType()), iterator.GetId());
                    iterator.ClearIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
                }
                ++iterator;
            }
        }


        uint16 AddComponent(CReference<CGameObject>& aOwner)
        {
            return mComponentsBuffer.AddElement(aOwner);
        }


        bool RemoveComponent(const uint16 aComponentId)
        {
            return mComponentsBuffer.FlagElementForRemoval(aComponentId);
        }


        CReference<COMPONENT_CLASS> GetComponent(const uint16 aComponentId)
        {
            return mComponentsBuffer.GetElement(aComponentId);
        }

    private:
        CComponentBuffer mComponentsBuffer;
        //std::array<CReferenceHolder<COMPONENT_CLASS>, kMaxComponentsPerType> mComponents;
        //uint8 mComponentsBuffer[sizeof(COMPONENT_CLASS) * kMaxComponentsPerType];
        //std::array<EComponentUseFlags, kMaxComponentsPerType> mComponentsBufferUseFlags;
    };

public:
    CComponentsManager();
    void Shutdown();
    void RefreshComponents();
    bool RemoveComponent(EComponentType aComponentType, const uint16 aComponentIndex);

    template<typename COMPONENT_CLASS>
    bool RegisterComponent()
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] == nullptr
            , "[CComponentsManager]::RegisterComponent - Component of the desired type has already been registered!");
        mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] = MAZ_NEW(CComponentManager<COMPONENT_CLASS>, COMPONENT_CLASS::GetType());
        return true;
    }


    template<typename COMPONENT_CLASS>
    uint16 AddComponent(CReference<CGameObject>& aOwner)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::AddComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->AddComponent(aOwner);
    }


    template<typename COMPONENT_CLASS>
    bool RemoveComponent(const uint16 aComponentId)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "[CComponentsManager]::RemoveComponent - Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->RemoveComponent(aComponentId);
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

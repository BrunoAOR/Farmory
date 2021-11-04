#ifndef _H_C_COMPONENTS_MANAGER_
#define _H_C_COMPONENTS_MANAGER_
#define MAZ_LOG_VERBOSE

#include <maz/globals.h>
#include <scenes/EComponentType.h>
#include <array>
#include <maz/CReferenceBuffer.h>


namespace maz
{

class CGameObject;
class CComponentsManager
{
private:
    template<typename COMPONENT_CLASS>
    friend class CComponentManager;
    
    void updateComponentIdForGameObject(CReference<CGameObject> aGameObject, EComponentType aComponentType, uint16 aId);
    
    
    template<typename COMPONENT_CLASS>
    void setComponentThisReference(CReference<COMPONENT_CLASS>& aComponent)
    {
        aComponent->mThis = aComponent;
    }


    class CComponentManagerBase
    {
    public:
        CComponentManagerBase(EComponentType aComponentType) : mComponentType(aComponentType) { MAZ_LOGGER_VERBOSE("Called"); }
        virtual ~CComponentManagerBase() { MAZ_LOGGER_VERBOSE("Called"); }
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
        using CComponentBuffer = CReferenceBuffer<COMPONENT_CLASS, kMaxComponentsPerType>;

    public:
        CComponentManager(EComponentType aComponentType)
            : CComponentManagerBase(aComponentType)
        {
            MAZ_LOGGER_VERBOSE("Called | ComponentType: %hhu", EnumToNumber(GetType()));
        }


        ~CComponentManager()
        {
            MAZ_LOGGER_VERBOSE("Called | ComponentType: %hhu", EnumToNumber(GetType()));
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
                    MAZ_ASSERT(removed, "Failed to remove component of type %hhu with id %hhu that was flagged for removal!", EnumToNumber(COMPONENT_CLASS::GetType()), iterator.GetId());
                    iterator.ClearIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
                }
                ++iterator;
            }
        }


        uint16 AddComponent(CComponentsManager* aComponentsManager, CReference<CGameObject>& aOwner)
        {
            const uint16 componentId = mComponentsBuffer.AddElement(aOwner);
            CReference<COMPONENT_CLASS> component = mComponentsBuffer.GetElement(componentId);
            aComponentsManager->setComponentThisReference(component);

            return componentId;
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
            , "Component of the desired type has already been registered!");
        mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] = MAZ_NEW(CComponentManager<COMPONENT_CLASS>, COMPONENT_CLASS::GetType());
        return true;
    }


    template<typename COMPONENT_CLASS>
    uint16 AddComponent(CReference<CGameObject>& aOwner)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->AddComponent(this, aOwner);
    }


    template<typename COMPONENT_CLASS>
    bool RemoveComponent(const uint16 aComponentId)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->RemoveComponent(aComponentId);
    }


    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent(const uint16 aComponentIndex)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->GetComponent(aComponentIndex);
    }

private:
    //TODO: Consider determining the full size required for all managers and put them all one after the other in memory on a buffer.
    // Then, just have the array of pointers like here, but we would have all components in contiguous memory.
    // (Still separated by the fact that not all capacity in each manager will be used)
    std::array<CComponentManagerBase*, EnumCount<EComponentType>()> mComponentManagers;
};

} // maz

#ifdef MAZ_LOG_VERBOSE
#undef MAZ_LOG_VERBOSE
#endif

#endif // !_H_C_COMPONENTS_MANAGER_

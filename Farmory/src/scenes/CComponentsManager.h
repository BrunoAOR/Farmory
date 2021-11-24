#ifndef _H_C_COMPONENTS_MANAGER_
#define _H_C_COMPONENTS_MANAGER_
#define MAZ_LOG_VERBOSE

#include <maz/globals.h>
#include <scenes/EComponentType.h>
#include <array>
#include <maz/CReferenceBuffer.h>


namespace maz
{

class CEntity;
class CComponentsManager
{
private:
    template<typename COMPONENT_CLASS>
    friend class CComponentManager;
    
    void updateComponentIdForEntity(CReference<CEntity> aEntity, EComponentType aComponentType, uint16 aId);
    
    
    template<typename COMPONENT_CLASS>
    void setComponentThisReference(CReference<COMPONENT_CLASS>& arComponent)
    {
        arComponent->mThis = arComponent;
    }


    class CComponentManagerBase
    {
    public:
        CComponentManagerBase(EComponentType aComponentType) : mComponentType(aComponentType) { MAZ_LOGGER_VERBOSE("Called"); }
        virtual ~CComponentManagerBase() { MAZ_LOGGER_VERBOSE("Called"); }
        EComponentType GetType() const { return mComponentType; }
        
        virtual void Shutdown(CComponentsManager* apComponentsManager) = 0;
        virtual void RefreshComponents(CComponentsManager* apComponentsManager) = 0;
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


        void Shutdown(CComponentsManager* apComponentsManager)
        {
            typename CComponentBuffer::CBufferIterator iterator = mComponentsBuffer.GetIterator(CComponentBuffer::EIteratorFlags::ANY);

            while (iterator)
            {
                apComponentsManager->updateComponentIdForEntity(iterator.Get()->GetOwner(), COMPONENT_CLASS::GetType(), kInvalidComponentId);
                ++iterator;
            }
            mComponentsBuffer.Clear();
        }


        virtual void RefreshComponents(CComponentsManager* apComponentsManager) final
        {
            typename CComponentBuffer::CBufferIterator lIt = mComponentsBuffer.GetIterator(static_cast<CComponentBuffer::EIteratorFlags>(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING | CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING));

            while (lIt)
            {
                if (lIt.HasIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING))
                {
                    apComponentsManager->updateComponentIdForEntity(lIt.Get()->GetOwner(), COMPONENT_CLASS::GetType(), lIt.GetId());
                    lIt.ClearIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_ADD_PENDING);
                }
                else if (lIt.HasIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING))
                {
                    apComponentsManager->updateComponentIdForEntity(lIt.Get()->GetOwner(), COMPONENT_CLASS::GetType(), kInvalidComponentId);
                    bool lRemoved = lIt.RemoveElement();
                    MAZ_ASSERT(lRemoved, "Failed to remove component of type %hhu with id %hhu that was flagged for removal!", EnumToNumber(COMPONENT_CLASS::GetType()), lIt.GetId());
                    lIt.ClearIteratorFlag(CComponentBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
                }
                ++lIt;
            }
        }


        uint16 AddComponent(CComponentsManager* apComponentsManager, CReference<CEntity>& arOwner)
        {
            const uint16 lComponentId = mComponentsBuffer.AddElement(arOwner);
            CReference<COMPONENT_CLASS> lComponent = mComponentsBuffer.GetElement(lComponentId);
            apComponentsManager->setComponentThisReference(lComponent);

            return lComponentId;
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
    uint16 AddComponent(CReference<CEntity>& arOwner)
    {
        MAZ_ASSERT(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())] != nullptr
            , "Component of the desired type have not been registered!");
        return static_cast<CComponentManager<COMPONENT_CLASS>*>(mComponentManagers[EnumToNumber(COMPONENT_CLASS::GetType())])->AddComponent(this, arOwner);
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

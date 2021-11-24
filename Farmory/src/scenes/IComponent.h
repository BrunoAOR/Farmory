#ifndef _H_I_COMPONENT_
#define _H_I_COMPONENT_

#include<maz/CReference.h>
#include<scenes/CGameObject.h>


namespace maz
{

class CEntity;
class IComponent
{
public:
    IComponent(CReference<CEntity>& arOwner);
    virtual ~IComponent();

    const CReference<CEntity> GetOwner() const;
    CReference<CEntity> GetOwner();

private:
    CReference<CEntity> mOwner;
};


template<typename COMPONENT_CLASS>
class CComponentBase
    : public IComponent
{
    friend class CComponentsManager;
    template<typename COMPONENT_CLASS>
    friend class CComponentManager;

public:
    CComponentBase(CReference<CEntity>& arOwner);
    virtual ~CComponentBase();

protected:
    CReference<COMPONENT_CLASS>& GetThis() { return mThis; }

private:
    CReference<COMPONENT_CLASS> mThis;
};


template<typename COMPONENT_CLASS>
inline CComponentBase<COMPONENT_CLASS>::CComponentBase(CReference<CEntity>& arOwner)
    : IComponent(arOwner)
{}


template<typename COMPONENT_CLASS>
inline CComponentBase<COMPONENT_CLASS>::~CComponentBase()
{}

} // maz

#endif // !_H_I_COMPONENT_

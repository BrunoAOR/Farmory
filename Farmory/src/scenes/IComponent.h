#ifndef _H_I_COMPONENT_
#define _H_I_COMPONENT_

#include<maz/CReference.h>


namespace maz
{

class CGameObject;
class IComponent
{
public:
    IComponent(CReference<CGameObject>& aOwner);
    virtual ~IComponent();

    const CReference<CGameObject> GetOwner() const;
    CReference<CGameObject> GetOwner();

private:
    CReference<CGameObject> mOwner;
};

template<typename COMPONENT_CLASS>
class CComponentBase
    : public IComponent
{
    friend class CComponentsManager;
    template<typename COMPONENT_CLASS>
    friend class CComponentManager;

public:
    CComponentBase(CReference<CGameObject>& aOwner);
    virtual ~CComponentBase();

protected:
    CReference<COMPONENT_CLASS>& GetThis() { return mThis; }

private:
    CReference<COMPONENT_CLASS> mThis;
};

template<typename COMPONENT_CLASS>
inline CComponentBase<COMPONENT_CLASS>::CComponentBase(CReference<CGameObject>& aOwner)
    : IComponent(aOwner)
{}

template<typename COMPONENT_CLASS>
inline CComponentBase<COMPONENT_CLASS>::~CComponentBase()
{}

} // maz

#endif // !_H_I_COMPONENT_
